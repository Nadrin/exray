/*
	This file is part of EX-Ray Raytracing Engine.
	(C)2007 - 2008 Micha³ Siejak.

    EX-Ray is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    EX-Ray is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with EX-Ray.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../Config.h"
#include "Variable.h"
#include "Node.h"
#include "Object.h"

using namespace exRay;

Node::Node(const std::string &name, Node *parent)
{
	nodeName	= name;
	evaluated	= false;

	if(parent)
	{ if(!parent->addChild(this)) nodeParent = NULL;
	} else nodeParent = NULL;

}

Node::~Node()
{
	for(unsigned int i=0; i<childList.size(); i++)
		deleteChild(i);
	for(VMap::iterator vi=nodeVars.begin(); vi!=nodeVars.end(); vi++)
		delete vi->second;
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
		delete (*vi);
	for(VReferenceList::iterator vi=linkSourceVars.begin(); vi!=linkSourceVars.end(); vi++)
		delete (*vi);
	for(VReferenceList::iterator vi=linkDestVars.begin(); vi!=linkDestVars.end(); vi++)
		delete (*vi);
}

Node* Node::getChild(unsigned int index)
{
	if(index >= childList.size())
		return NULL;
	return childList.at(index);
}

Node* Node::getChild(const std::string &cname)
{
	NodeMap::iterator	result = childMap.find(cname);
	if(result == childMap.end())
		return NULL;
	return result->second;
}

bool Node::addChild(Node *child)
{
	if(!child)
		return false;
	if(getChild(child->getName()))
		return false;

	child->nodeParent = this;
	childList.push_back(child);
	childMap[child->getName()] = child;
	return true;
}

bool Node::deleteChild(unsigned int index)
{
	if(index >= childList.size())
		return false;

	NodeMap::iterator it = childMap.find(childList.at(index)->getName());

	delete it->second;
	childList.erase(childList.begin()+index);
	childMap.erase(it);
	return true;
}

bool Node::deleteChild(const std::string &cname)
{
	NodeMap::iterator it = childMap.find(cname);
	if(it == childMap.end())
		return false;

	delete it->second;
	for(NodeList::iterator i=childList.begin(); i<childList.end(); i++)
	{
		if((*i)->getName() == cname)
		{
			childList.erase(i);
			break;
		}
	}
	childMap.erase(it);
	return true;
}

Variable* Node::getAttrib(unsigned int index, bool update)
{
	if(index >= nodeVars.size())
		return NULL;
	VMap::iterator it = nodeVars.begin();
	for(unsigned int i=0; i<index; i++) it++;
	if(update)
		updateAttrib(it->second);
	return it->second;
}

Variable* Node::getAttrib(const std::string &name, bool update)
{
	VMap::iterator it = nodeVars.find(name);
	if(it == nodeVars.end())
		return NULL;
	if(update)
		updateAttrib(it->second);
	return it->second;
}

Variable* Node::addAttrib(Variable *value)
{
	if(!value)	return NULL;

	VMap::iterator it = nodeVars.find(value->getName());
	if(it != nodeVars.end())
		return false;
	nodeVars[value->getName()] = value;
	return value;
}

unsigned int Node::getChildCount(void) const
{
	return (unsigned int)childList.size();
}

unsigned int Node::getAttribCount(void) const
{
	return (unsigned int)nodeVars.size();
}

bool Node::hasOutput(void)
{
	return (outputNodes.size() > 0);
}

bool Node::addDependencyNode(Node *pnode)
{
	for(NodeList::iterator i=affectedBy.begin(); i!=affectedBy.end(); i++)
	{
		if((*i) == pnode)
			return false;
	}
	affectedBy.push_back(pnode);
	return true;
}

bool Node::connect(Node *pnode, const std::string &attrName, unsigned int index)
{
//	if(outputNode)
//		return false;
	Variable *attrib = pnode->getAttrib(attrName);
	if(!attrib)
		return false;
	if(attrib->isConnected())
		return false;
	if(attrib->size() <= index)
		return false;
	if(!acceptsConnection(attrib))
		return false;

	attrib->setOutputNode(this);
	pnode->addDependencyNode(this);

	outputNodes.push_back(pnode);
	outputVars.push_back(new VReference(attrib, index));
	
//	outputNode	= pnode;
//	outputIndex	= index;
//	output		= attrib;
	return true;
}

bool Node::link(Node *pnode, const std::string &src, const std::string &dst, unsigned int srci, unsigned int dsti)
{
	Variable	*vsrc = getAttrib(src);
	Variable	*vdst;

	if(dst == std::string(""))
		vdst = pnode->getAttrib(src);
	else
		vdst = pnode->getAttrib(dst);

	if(!vsrc || !vdst)
		return false;
	if(vdst->isConnected())
		return false;
	if(vsrc->getType() != vdst->getType())
		return false;
	if(vsrc->size() <= srci || vdst->size() <= dsti)
		return false;

	vdst->setOutputNode(this);
	pnode->addDependencyNode(this);

	linkNodes.push_back(pnode);
	linkSourceVars.push_back(new VReference(vsrc, srci));
	linkDestVars.push_back(new VReference(vdst, dsti));
	return true;
}

bool Node::map(Node *pnode, unsigned int index)
{
	return false;
}

void Node::clearEvaluated(bool recursive)
{
	evaluated = false;
	if(!recursive)
		return;

	for(NodeList::iterator i=childList.begin(); i<childList.end(); i++)
	{ (*i)->clearEvaluated(true); }
}

void Node::updateAttrib(Variable *attrib)
{
	if(!attrib->isConnected())
		return;
	if(!attrib->getOutputNode()->isEvaluated())
		attrib->getOutputNode()->evaluate();
}

void Node::getObjectsArray(NodeList *referenceList)
{
	if(isObject())
		referenceList->push_back(this);
	for(NodeList::iterator i=childList.begin(); i<childList.end(); i++)
		(*i)->getObjectsArray(referenceList);
}

void Node::getNodesArray(const std::string &type, NodeList *referenceList)
{
	if(getType() == type)
		referenceList->push_back(this);
	for(NodeList::iterator i=childList.begin(); i<childList.end(); i++)
	{
		if((*i)->getType() == type)
			(*i)->getNodesArray(type, referenceList);
	}
}

// Virtuals
void Node::updateTransformation(const matrix4 *multiplyBy)
{
	for(NodeList::iterator i=childList.begin(); i<childList.end(); i++)
			(*i)->updateTransformation(multiplyBy);
}

void Node::cacheVariables(void)
{
	for(NodeList::iterator i=childList.begin(); i<childList.end(); i++)
		(*i)->cacheVariables();
}

Node* Node::intersect(const vector3 &rayOrigin, const vector3 &rayDirection, float &rayDistance, int &factor, int &flag)
{
	factor	= 0;
	flag	= 0;
	return NULL;
}

void Node::evaluate(void)
{
	if(evaluated) return;

	VReferenceList::iterator srci = linkSourceVars.begin();
	VReferenceList::iterator dsti = linkDestVars.begin();

	while((srci != linkSourceVars.end()) && (dsti != linkDestVars.end()))
	{
		(*srci)->first->copyValue((*dsti)->first, (*srci)->second, (*dsti)->second);
		srci++;
		dsti++;
	}
	evaluated = true;

//	for(VMap::iterator i=nodeVars.begin(); i!=nodeVars.end(); i++)
//		(*i).second->setClean();
	for(NodeList::iterator i=childList.begin(); i<childList.end(); i++)
		(*i)->evaluate();

	//cacheVariables();
}

// output set
bool Node::setOutput(int value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}

bool Node::setOutput(float value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}

bool Node::setOutput(vector2 &value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}

bool Node::setOutput(vector3 &value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}

bool Node::setOutput(vector4 &value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}

bool Node::setOutput(matrix4 &value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}

bool Node::setOutput(std::string &value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}

bool Node::setOutput(Mesh *value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}

bool Node::setOutput(Shader *value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}

bool Node::setOutput(Node *value)
{
	for(VReferenceList::iterator vi=outputVars.begin(); vi!=outputVars.end(); vi++)
	{
		if(!(*vi)->first->setValue(value, (*vi)->second))
			return false;
	}
	return true;
}