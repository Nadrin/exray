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
#include "NodeBox.h"

using namespace exRay;

NodeBox::NodeBox(const std::string &name, Node *parent) : Object(name, parent)
{
	addAttrib(new VFloat3("size"));
}

NodeBox::~NodeBox(void)
{
}

void NodeBox::cacheVariables(void)
{
	vector4	worldPosition(cPosition, 1.0f);
	worldPosition	= worldTM * vector4(0.0f, 0.0f, 0.0f, 1.0f);
	cPosition		= vector3(worldPosition.x, worldPosition.y, worldPosition.z);

	getAttrib("size")->getValue(cSize);

	cDim[0] = cPosition;
	cDim[1] = cPosition + cSize;

	Object::cacheVariables();
}

vector3 NodeBox::getNormal(const vector3 &intPoint, const int intersectFlag)
{
	switch(intersectFlag)
	{
	case NodeBox::BoxIntNegativeX:
		return vector3(-1.0f, 0.0f, 0.0f);
	case NodeBox::BoxIntNegativeY:
		return vector3(0.0f, -1.0f, 0.0f);
	case NodeBox::BoxIntNegativeZ:
		return vector3(0.0f, 0.0f, -1.0f);
	case NodeBox::BoxIntPositiveX:
		return vector3(1.0f, 0.0f, 0.0f);
	case NodeBox::BoxIntPositiveY:
		return vector3(0.0f, 1.0f, 0.0f);
	case NodeBox::BoxIntPositiveZ:
		return vector3(0.0f, 0.0f, 1.0f);
	}
	return vector3();
}

Node* NodeBox::intersect(const vector3 &rayOrigin, const vector3 &rayDirection, float &rayDistance, int &factor, int &flag)
{
	float	dist[6];
	vector3	intPoint[6];

	Node	*result = NULL;

	for(int i=0; i<6; i++)
		dist[i] = -1.0f;

	// Przeciêcie z pierwsz¹ "p³yt¹"
	if(rayDirection.x != 0.0f)
	{
		dist[0] = (cDim[0].x - rayOrigin.x) / rayDirection.x;
		dist[3] = (cDim[1].x - rayOrigin.x) / rayDirection.x;
	}
	// Przeciêcie z drug¹ "p³yt¹"
	if(rayDirection.y != 0.0f)
	{
		dist[1] = (cDim[0].y - rayOrigin.y) / rayDirection.y;
		dist[4] = (cDim[1].y - rayOrigin.y) / rayDirection.y;
	}
	// Przeciêcie z trzeci¹ "p³yt¹"
	if(rayDirection.z != 0.0f)
	{
		dist[2] = (cDim[0].z - rayOrigin.z) / rayDirection.z;
		dist[5] = (cDim[1].z - rayOrigin.z) / rayDirection.z;
	}

	for(int i=0; i<6; i++) if(dist[i] > 0.0f)
	{
		// Obliczanie punktu przeciêcia dla i-tej powierchni p³yt (ka¿da p³yta to 2 równoleg³e powierzchnie)
		intPoint[i] = rayOrigin + dist[i]*rayDirection;
		// Ograniczanie do wymiarów boxa.
		if((intPoint[i].x > (cDim[0].x - MEPSILON)) && (intPoint[i].x < (cDim[1].x + MEPSILON)) &&
		   (intPoint[i].y > (cDim[0].y - MEPSILON)) && (intPoint[i].y < (cDim[1].y + MEPSILON)) &&
		   (intPoint[i].z > (cDim[0].z - MEPSILON)) && (intPoint[i].z < (cDim[1].z + MEPSILON)))
		{
			// Trafiliœmy w box.
			if(dist[i] < rayDistance)
			{
				// W któr¹ stronê wskazuje normal?
				switch(i)
				{
				case 0: flag = NodeBox::BoxIntNegativeX; break; // -X
				case 3: flag = NodeBox::BoxIntPositiveX; break; // X
				case 1: flag = NodeBox::BoxIntNegativeY; break; // -Y
				case 4: flag = NodeBox::BoxIntPositiveY; break; // Y
				case 2: flag = NodeBox::BoxIntNegativeZ; break; // -Z
				case 5: flag = NodeBox::BoxIntPositiveZ; break; // Z
				}
				rayDistance = dist[i];
				result		= this;
				factor		= 1;
			}
		}
	}
	return result;
}
