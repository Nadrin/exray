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
#include "NodeLight.h"

using namespace exRay;

NodeLight::NodeLight(const std::string &name, Node *parent) : Object(name, parent)
{
	addAttrib(new VFloat3("direction"));
	link(this, "position", "direction");

	addAttrib(new VFloat3("color"));
	//addAttrib(new VFloat("ambient"));
	//addAttrib(new VFloat("diffuse"));
	//addAttrib(new VFloat("specular"));
}

NodeLight::~NodeLight(void)
{
}

void NodeLight::cacheVariables(void)
{
	vector4	worldPosition(cPosition, 1.0f);
	worldPosition	= worldTM * vector4(0.0f, 0.0f, 0.0f, 1.0f);
	cPosition		= vector3(worldPosition.x, worldPosition.y, worldPosition.z);

	Object::cacheVariables();
}

bool NodeLight::map(Node *pnode, unsigned int index)
{
	Variable	*envLight = pnode->getAttrib("env_light");
	if(envLight == NULL)
		return false;
	Node	*outReference = (Node*)this;
	envLight->pushValue(outReference);
	return true;
}

NodeAreaLight::NodeAreaLight(const std::string &name, Node *parent) : NodeLight(name, parent)
{
	addAttrib(new VFloat2("size"));
}

NodeAreaLight::~NodeAreaLight(void)
{
}

void NodeAreaLight::cacheVariables(void)
{
	NodeLight::cacheVariables();

	vector2	cArea;
	getAttrib("size")->getValue(cArea);
	cSize	= vector3(cArea.x, 0.0f, cArea.y);

	cDim[0] = cPosition;
	cDim[1] = cPosition + cSize;

	//Object::cacheVariables();
}

Node* NodeAreaLight::intersect(const vector3 &rayOrigin, const vector3 &rayDirection, float &rayDistance, int &factor, int &flag)
{
	float	dist[6];
	vector3	intPoint[6];

	factor			= 0;
	flag			= 0;
	Node	*result = NULL;

	for(int i=0; i<6; i++)
		dist[i] = -1.0f;

	if(rayDirection.x != 0.0f)
	{
		dist[0] = (cDim[0].x - rayOrigin.x) / rayDirection.x;
		dist[3] = (cDim[1].x - rayOrigin.x) / rayDirection.x;
	}
	if(rayDirection.y != 0.0f)
	{
		dist[1] = (cDim[0].y - rayOrigin.y) / rayDirection.y;
		dist[4] = (cDim[1].y - rayOrigin.y) / rayDirection.y;
	}
	if(rayDirection.z != 0.0f)
	{
		dist[2] = (cDim[0].z - rayOrigin.z) / rayDirection.z;
		dist[5] = (cDim[1].z - rayOrigin.z) / rayDirection.z;
	}

	for(int i=0; i<6; i++) if(dist[i] > 0.0f)
	{
		intPoint[i] = rayOrigin + dist[i]*rayDirection;
		if((intPoint[i].x > (cDim[0].x - MEPSILON)) && (intPoint[i].x < (cDim[1].x + MEPSILON)) &&
		   (intPoint[i].y > (cDim[0].y - MEPSILON)) && (intPoint[i].y < (cDim[1].y + MEPSILON)) &&
		   (intPoint[i].z > (cDim[0].z - MEPSILON)) && (intPoint[i].z < (cDim[1].z + MEPSILON)))
		{
			if(dist[i] < rayDistance)
			{
				rayDistance = dist[i];
				result		= this;
				factor		= 1;
			}
		}
	}
	return result;
}