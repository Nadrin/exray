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

#ifndef __NODE_LIGHT_H
#define __NODE_LIGHT_H

namespace exRay {

/// Standardowe œwiat³o punktowe.
class NodeLight : public Object
{
protected:
	vector3	cPosition;
public:
	NodeLight(const std::string &name, Node *parent);
	virtual ~NodeLight(void);

	virtual bool map(Node *pnode, unsigned int index=0);
	virtual const std::string getType(void) const
	{ return std::string("light"); }

	virtual void	cacheVariables(void);
	virtual bool	isAreaLight(void) const
	{ return false; }

	virtual bool ignoreIntersection(void) const
	{ return true; }

	virtual vector3	getCachedPosition(void) const
	{ return cPosition; }
	virtual vector2	getCachedArea(void) const
	{ return vector2(); }
};

/// Kreator klasy NodeLight.
class NodeLightCreator : public NodeCreator
{
public:
	virtual Node* operator()(const std::string &name, Node *parent=NULL) const
	{ return new NodeLight(name, parent); }
};

/// Œwiat³o powierzchniowe.
class NodeAreaLight : public NodeLight
{
protected:
	vector3	cSize;
	vector3	cDim[2];
public:
	NodeAreaLight(const std::string &name, Node *parent);
	virtual ~NodeAreaLight(void);

	virtual bool	isAreaLight(void) const
	{ return true; }

	virtual const std::string getType(void) const
	{ return std::string("light"); }

	virtual bool ignoreIntersection(void) const
	{ return true; }

	virtual void	cacheVariables(void);
	virtual Node*	intersect(const vector3 &rayOrigin, const vector3 &rayDirection, float &rayDistance, int &factor, int &flag);

	virtual vector2	getCachedArea(void) const
	{ return vector2(cSize.x, cSize.z); }
};

/// Kreator klasy NodeAreaLight.
class NodeAreaLightCreator : public NodeCreator
{
public:
	virtual Node* operator()(const std::string &name, Node *parent=NULL) const
	{ return new NodeAreaLight(name, parent); }
};

} // exRay

#endif