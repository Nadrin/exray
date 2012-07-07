/*
	This file is part of EX-Ray Raytracing Engine.
	(C)2007 - 2008 Micha� Siejak.

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

#ifndef __OBJECT_H
#define __OBJECT_H

namespace exRay {

/// Obiekt sceny.
/** Klasa bazowa dla w�z��w sceny b�d�cych obiektami. Obiekt to w�ze� (node) posiadaj�cy
	reprezentacj� w przestrzeni t.j. pozycj�, obr�t, skal� oraz lokaln� macierz transformacji.
	Na przyk�ad: Kula (NodeSphere) lub �wiat�o (NodeLight) s� zar�wno w�z�ami sceny jak i 
	obiektami (dziedzicz� po klasie bazowej Object), natomiast materia� (NodeMaterial)
	jest wy�acznie w�z�em (dziedziczy po klasie bazowej Node).
*/
class Object : public Node
{
private:
	Shader	*cShader;
	Node	*cMaterial;
protected:
	matrix4	worldTM;
	matrix4	inverseTM;
	matrix4	localTM;
public:
	Object(const std::string &name, Node *parent);
	virtual ~Object();

	virtual const std::string getType(void) const
	{ return std::string("object"); }

	virtual bool isObject(void) const
	{ return true; }

	virtual void	cacheVariables(void);
	virtual void	updateTransformation(const matrix4 *multiplyBy=NULL);
	virtual vector3	getNormal(const vector3 &intPoint, const int intersectFlag=0);

	matrix4			getTM(void) const
	{ return worldTM; }
	matrix4			getLocalTM(void) const
	{ return localTM; }
	matrix4			getInverseTM(void) const
	{ return inverseTM; }

	Shader*			getCachedShader(void) const
	{ return cShader; }
	Node*			getCachedMaterial(void) const
	{ return cMaterial; }
};

/// Kreator klasy Object.
class ObjectCreator : public NodeCreator
{
public:
	virtual Node* operator()(const std::string &name, Node *parent=NULL) const
	{ return new Object(name, parent); }
};

/// Grupa obiekt�w na scenie.
class Group : public Object
{
public:
	Group(const std::string &name, Node *parent) : Object(name, parent) { }
	virtual ~Group() { }

	virtual const std::string getType(void) const
	{ return std::string("group"); }
};

/// Kreator klasy Group.
class GroupCreator : public NodeCreator
{
public:
	virtual Node* operator()(const std::string &name, Node *parent=NULL) const
	{ return new Group(name, parent); }
};

/// Kamera.
/**	W�ze� standardowej kamery perspektywicznej. Ka�da scena mo�e posiada� tylko jedn� kamer�
	(dalsze w�z�y typu "camera" s� ignorowane). Wzgl�dem jej macierzy trasnformacji mno�one
	s� koordynaty p�aszczyzny "ekranu" (w kt�r� wystrzliwane s� primary rays).
*/
class NodeCamera : public Object
{
public:
	NodeCamera(const std::string &name, Node *parent);
	virtual ~NodeCamera();

	virtual bool ignoreIntersection(void) const
	{ return true; }

	virtual const std::string getType(void) const
	{ return std::string("camera"); }
};

/// Kreator klasy NodeCamera.
class NodeCameraCreator : public NodeCreator
{
public:
	virtual Node* operator()(const std::string &name, Node *parent=NULL) const
	{ return new NodeCamera(name, parent); }
};

} // exRay

#endif