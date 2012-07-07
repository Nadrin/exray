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
#include "NodeSphere.h"

using namespace exRay;

NodeSphere::NodeSphere(const std::string &name, Node *parent) : Object(name, parent)
{
	addAttrib(new VFloat("radius"));
}

NodeSphere::~NodeSphere(void)
{	
}

void NodeSphere::cacheVariables(void)
{
	getAttrib("radius")->getValue(cRadius);
	cSqRadius  = SQR(cRadius);
	cInvRadius = INV(cRadius);

	vector4	worldCentre(0.0f, 0.0f, 0.0f, 1.0f);
	worldCentre = worldTM * worldCentre;
	cCentre = vector3(worldCentre.x, worldCentre.y, worldCentre.z);

	Object::cacheVariables();
}

vector3 NodeSphere::getNormal(const vector3 &intPoint, const int intersectFlag)
{
	return ((intPoint - cCentre)).getNormalized();
}

// Geometryczna metoda (czasem szybsza od podejœcia algebraicznego)
Node* NodeSphere::intersect(const vector3 &rayOrigin, const vector3 &rayDirection, float &rayDistance, int &factor, int &flag)
{
	factor	   = 0;
	flag	   = 0;
	vector3	oc = cCentre - rayOrigin; // Wektor od pocz¹tku promienia do œrodka kuli.
	
	float	ocSqLen	= oc.dot(oc);
	if(ocSqLen < cSqRadius) // Kwadrat (oc) mniejszy ni¿ r^2. Promieñ ma swój pocz¹tek w œrodku kuli.
	{
		float	t		 = oc.dot(rayDirection);
		float	halfcord = cSqRadius - ocSqLen + SQR(t);
		rayDistance		 = t + sqrtf(halfcord);
		factor			 = -1;
	}
	else // Promieñ ma swój pocz¹tek na zewn¹trz kuli.
	{
		float	t = oc.dot(rayDirection);
		if(t < 0.0f) return NULL; // Promieñ nie trafia w kulê.
		float	halfcord = cSqRadius - ocSqLen + SQR(t);
		if(halfcord < 0.0f) return NULL;
		rayDistance = t - sqrtf(halfcord);
		factor		= 1;
	}
	return this;
}
