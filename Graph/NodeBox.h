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

#ifndef __NODE_BOX_H
#define __NODE_BOX_H

namespace exRay {

/// Obiekt sceny: Prostopad³oœcian.
class NodeBox : public Object
{
private: // cache
	vector3	cPosition;
	vector3	cSize;
	vector3	cDim[2];
public:
	NodeBox(const std::string &name, Node *parent);
	virtual ~NodeBox(void);

	virtual void	cacheVariables(void);
	virtual vector3	getNormal(const vector3 &intPoint, const int intersectFlag);
	virtual Node*	intersect(const vector3 &rayOrigin, const vector3 &rayDirection, float &rayDistance, int &factor, int &flag);

	virtual const std::string getType(void) const
	{ return std::string("box"); }

	enum
	{
		BoxIntPositiveX,
		BoxIntNegativeX,
		BoxIntPositiveY,
		BoxIntNegativeY,
		BoxIntPositiveZ,
		BoxIntNegativeZ,
	};
};

/// Kreator klasy NodeBox.
class NodeBoxCreator : public NodeCreator
{
public:
	virtual Node* operator()(const std::string &name, Node *parent=NULL) const
	{ return new NodeBox(name, parent); }
};

} // exRay

#endif