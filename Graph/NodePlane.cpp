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
#include "NodePlane.h"

using namespace exRay;

NodePlane::NodePlane(const std::string &name, Node *parent) : Object(name, parent)
{
	addAttrib(new VFloat3("normal"));
	addAttrib(new VFloat("distance"));
}

NodePlane::~NodePlane(void)
{
}

void NodePlane::cacheVariables(void)
{
	getAttrib("normal")->getValue(cNormal);
	getAttrib("distance")->getValue(cDistance);
	cNormal.normalize();

	vector4	worldPosition(cNormal*-cDistance, 1.0f);
	vector4	worldNormal(cNormal, 1.0f);

	worldPosition	= worldTM * worldPosition;
	worldNormal		= inverseTM.getTransposed() * worldNormal;

	cNormal		= vector3(worldNormal.x, worldNormal.y, worldNormal.z).getNormalized();
	cDistance	= vector3(worldPosition.x, worldPosition.y, worldPosition.z).length();

	Object::cacheVariables();
}

vector3 NodePlane::getNormal(const vector3 &intPoint, const int intersectFlag)
{
	return cNormal;
}

Node* NodePlane::intersect(const vector3 &rayOrigin, const vector3 &rayDirection, float &rayDistance, int &factor, int &flag)
{
	factor		  = 0;
	flag		  = 0;
	float	dotND = cNormal.dot(rayDirection);
	if(dotND != 0.0f)
	{
		float	testDistance = -(cNormal.dot(rayOrigin) + cDistance) / dotND;
		if(testDistance > 0.0f)
		{
			rayDistance = testDistance;
			factor		= 1;
			return this;
		}
	}
	return NULL;
}