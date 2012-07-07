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

Object::Object(const std::string &name, Node *parent) : Node(name, parent)
{
	addAttrib(new VFloat3("position"));
	addAttrib(new VFloat3("rotation"));
	addAttrib(new VFloat3("scale"))->setValue(vector3(1.0f, 1.0f, 1.0f));

	addAttrib(new VShader("cgfx_shader"));
	addAttrib(new VNode("material"));

	cShader		= NULL;
	cMaterial	= NULL;
	localTM.loadIdentity();
	worldTM.loadIdentity();
	inverseTM.loadIdentity();
}

Object::~Object()
{
}

void Object::cacheVariables(void)
{
	getAttrib("cgfx_shader")->getValue(cShader);
	getAttrib("material")->getValue(cMaterial);
	Node::cacheVariables();
}

vector3 Object::getNormal(const vector3& intPoint, const int intersectFlag)
{
	return vector3();
}

void Object::updateTransformation(const matrix4 *multiplyBy)
{
	Variable	*position = getAttrib("position");
	Variable	*rotation = getAttrib("rotation");
	Variable	*scale    = getAttrib("scale");

	if(position->isDirty() || rotation->isDirty() || scale->isDirty())
	{
		vector3	vecPosition, vecRotation, vecScale;
		position->getValue(vecPosition);
		rotation->getValue(vecRotation);
		scale->getValue(vecScale);

		// stopnie na radiany!
		vecRotation *= MPI180;

		localTM.loadIdentity();
		localTM = localTM.scale(vecScale) * localTM.rotate(vecRotation) * localTM.translate(vecPosition);
		inverseTM.loadIdentity();
		inverseTM = inverseTM.scale(vecScale) * inverseTM.translate(-vecPosition) * inverseTM.rotate(-vecRotation);
	}

	worldTM = localTM;
	if(nodeParent)
	{
		if(nodeParent->isObject())
		{
			worldTM		= worldTM * ((Object*)nodeParent)->worldTM;
			inverseTM	= inverseTM * ((Object*)nodeParent)->inverseTM;
		}
	}

	if(multiplyBy)
		worldTM = worldTM * (*multiplyBy);

	Node::updateTransformation(multiplyBy);

	position->setClean();
	rotation->setClean();
	scale->setClean();
}

NodeCamera::NodeCamera(const std::string &name, Node *parent) : Object(name, parent)
{
	addAttrib(new VFloat2("screen"))->setValue(vector2(8.0f, 6.0f));
	addAttrib(new VFloat3("origin"))->setValue(vector3(0.0f, 0.0f, -5.0f));
}

NodeCamera::~NodeCamera(void)
{
}
