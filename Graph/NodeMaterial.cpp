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
#include "NodeMaterial.h"
#include "../Types/Shader.h"

using namespace exRay;

NodeMaterial::NodeMaterial(const std::string &name, Node *parent) : Node(name, parent)
{
	addAttrib(new VFloat3("color"));
	addAttrib(new VFloat("diffuse"));
	addAttrib(new VFloat("specular"));
	addAttrib(new VFloat("specular_exponent"));
	addAttrib(new VFloat("reflectance"));
	addAttrib(new VFloat("refraction"));
	addAttrib(new VFloat("density"));
}

NodeMaterial::~NodeMaterial(void)
{
}

bool NodeMaterial::acceptsConnection(Variable *cvariable) const
{
	if(cvariable->getType() != NODE)
		return false;
	return true;
}

void NodeMaterial::cacheVariables(void)
{
	getAttrib("color")->getValue(cColor);
	getAttrib("diffuse")->getValue(cDiffuse);
	getAttrib("specular")->getValue(cSpecular);
	getAttrib("specular_exponent")->getValue(cSpecularExponent);
	getAttrib("reflectance")->getValue(cReflectance);
	getAttrib("refraction")->getValue(cRefraction);
	getAttrib("density")->getValue(cDensity);
	
	Node::cacheVariables();
}

void NodeMaterial::updateShader(ShaderUniforms *su)
{
	su->MaterialColor			 = &cColor;
	su->MaterialDiffuse			 = cDiffuse;
	su->MaterialSpecular		 = cSpecular;
	su->MaterialSpecularExponent = cSpecularExponent;
	su->MaterialReflectance		 = cReflectance;
}

void NodeMaterial::evaluate(void)
{
	if(evaluated) return;
	if(hasOutput())
		setOutput(this);

	Node::evaluate();
}

bool NodeMaterial::isReflective(void) const
{
	return (cReflectance > 0.0f);
}

bool NodeMaterial::isRefractive(const float rindex) const
{
	return (cRefraction != 0.0f && cRefraction != rindex);
}

float NodeMaterial::getRefractionIndex(void) const
{ return cRefraction; }

float NodeMaterial::getDensity(void) const
{ return cDensity; }
