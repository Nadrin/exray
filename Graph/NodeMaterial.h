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

#ifndef __NODE_MATERIAL_H
#define __NODE_MATERIAL_H

namespace exRay {

class ShaderUniforms;

/// Standardowy materia³ obiektu.
/** Wêze³ sceny bêd¹cy definicj¹ standardowego materia³u */
class NodeMaterial : public Node
{
private: // cache
	vector3	cColor;
	float	cDiffuse;
	float	cSpecular;
	float	cSpecularExponent;
	float	cReflectance;
	float	cRefraction;
	float	cDensity;
public:
	NodeMaterial(const std::string &name, Node *parent);
	virtual ~NodeMaterial(void);

	virtual void	cacheVariables(void);
	virtual bool	acceptsConnection(Variable *cvariable) const;
	virtual void	evaluate(void);

	bool			isReflective(void) const;
	bool			isRefractive(const float rindex) const;

	// It shouldn't be done that way. :( Duh!
	float			getRefractionIndex(void) const;
	float			getDensity(void) const;

	void			updateShader(ShaderUniforms *su);

	virtual const std::string getType(void) const
	{ return std::string("material"); }
};

/// Kreator klasy NodeMaterial.
class NodeMaterialCreator : public NodeCreator
{
public:
	virtual Node* operator()(const std::string &name, Node *parent=NULL) const
	{ return new NodeMaterial(name, parent); }
};

} // exRay

#endif