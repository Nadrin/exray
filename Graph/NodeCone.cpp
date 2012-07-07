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
#include "NodeCone.h"

using namespace exRay;

NodeCone::NodeCone(const std::string &name, Node *parent) : Object(name, parent)
{
}

NodeCone::~NodeCone(void)
{
}

void NodeCone::cacheVariables(void)
{
}

vector3 NodeCone::getNormal(const vector3 &intPoint, const int intersectFlag)
{
	return vector3();
}

Node* NodeCone::intersect(const vector3 &rayOrigin, const vector3 &rayDirection, float &rayDistance, int &factor, int &flag)
{
	factor	= 0;
	flag	= 0;
	return NULL;
}
