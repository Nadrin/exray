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

#ifndef __NODE_H
#define __NODE_H

namespace exRay {

class Node;
class Variable;
class Renderer;
class Mesh;
class Shader;

typedef std::pair<Variable*, unsigned int>	VReference;
typedef std::vector<VReference*>			VReferenceList;

typedef std::vector<Node*>					NodeList;
typedef std::vector<Variable*>				VList;
typedef std::map<std::string, Node*>		NodeMap;
typedef std::map<std::string, Variable*>	VMap;

/// Wêze³ sceny.
/** Klasa bazowa dla wszystkich elementów grafu sceny. Udostêpnia metody operuj¹ce na grafie.
	Zarz¹dza atrybutami i podwêz³ami, tworzy po³aczenia miêdzy wêz³ami zale¿nymi, a tak¿e linki i
	mapowania atrybutów. Zarz¹dza zale¿noœciami pomiêdzy poszczególnymi wêz³ami podgrafu, którego
	jest korzeniem, a tak¿e przelicza taki podgraf w razie koniecznoœci. 
*/
class Node
{
protected:
	std::string			nodeName;
	Node*				nodeParent;

	VMap				nodeVars;
	NodeList			childList;
	NodeList			affectedBy;
	NodeMap				childMap;

	VReferenceList		outputVars;
	NodeList			outputNodes;
	VReferenceList		linkDestVars;
	VReferenceList		linkSourceVars;
	NodeList			linkNodes;
	 
	bool				evaluated;
protected:
	bool			addDependencyNode(Node *pnode);
	void			updateAttrib(Variable *attrib);
protected:
	bool			setOutput(int);
	bool			setOutput(float);
	bool			setOutput(vector2&);
	bool			setOutput(vector3&);
	bool			setOutput(vector4&);
	bool			setOutput(matrix4&);
	bool			setOutput(std::string&);
	bool			setOutput(Mesh*);
	bool			setOutput(Shader*);
	bool			setOutput(Node*);
public:
	Node(const std::string &name, Node *parent);
	virtual ~Node();

	virtual const std::string getType(void) const
	{ return std::string("node"); }

	virtual bool isObject(void) const
	{ return false; }

	virtual bool acceptsConnection(Variable *cvariable) const
	{ return false; }

	virtual bool ignoreIntersection(void) const
	{ return false; }

	bool isEvaluated(void) const
	{ return evaluated; }

	const std::string&	getName(void) const		{ return nodeName;		}
	Node*				getParent(void) const	{ return nodeParent;	}

	Node*				getChild(unsigned int index);
	Node*				getChild(const std::string &cname);
	unsigned int		getChildCount(void) const;

	bool				deleteChild(unsigned int index);
	bool				deleteChild(const std::string &cname);

	bool				addChild(Node* child);

	Variable*			getAttrib(unsigned int index, bool update=false);
	Variable*			getAttrib(const std::string &name, bool update=false);	
	unsigned int		getAttribCount(void) const;
	Variable*			addAttrib(Variable *value);

	bool				connect(Node *pnode, const std::string &attrName, unsigned int index=0);
	bool				link(Node *pnode, const std::string &src, const std::string &dst="", unsigned int srci=0, unsigned int dsti=0);
	void				clearEvaluated(bool recursive);
	bool				hasOutput(void);
	void				getObjectsArray(NodeList *referenceList);
	void				getNodesArray(const std::string &type, NodeList *referenceList);

	// Virtual methods
	virtual bool		map(Node *pnode, unsigned int index=0);
	virtual void		updateTransformation(const matrix4 *multiplyBy=NULL);
	virtual Node*		intersect(const vector3 &rayOrigin, const vector3 &rayDirection, float &rayDistance, int &factor, int &flag);
	virtual void		evaluate(void);
	virtual void		cacheVariables(void);
};

/// Kreator klasy Node.
/** Klasa bazowa dla wszystkich kreatorów wêz³ów sceny. Kreator wêz³a jest funktorem zwracaj¹cym adres
	do nowej instancji wêz³a.
*/
class NodeCreator
{
public:
	virtual Node* operator()(const std::string &name, Node *parent=NULL) const
	{ return new Node(name, parent); }
};

} // exRay

#endif