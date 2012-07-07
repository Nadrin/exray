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

#ifndef __SHADER_H
#define __SHADER_H

namespace exRay {

class Node;
class ShaderUniforms;

/// Generyczna funkcja cieniuj¹ca.
/** Bazowa klasa, której funkcjonalnoœc implementuj¹ wszystkie shadery. */
class Shader
{
public:
	Shader();
	virtual ~Shader(void);

	virtual void	cacheVariables(Node *pnode);
	virtual vector3	computeShading(ShaderUniforms *su, const float *visibility=NULL);
	virtual ShaderUniforms* createRenderingCache(void) const;
};

/// Cache renderowania.
class ShaderUniforms
{
public:
	ShaderUniforms(void)
	{
		ReflectFlag			= false;
		RefractFlag			= false;
	}
	virtual ~ShaderUniforms(void)
	{ }

public:
	vector3	Normal;
	vector3	*IntPoint;
	vector3	*RayDirection;
	vector3	*RayOrigin;

	bool	ReflectFlag;
	bool	RefractFlag;
	vector3	ReflectComponent;
	vector3	RefractComponent;

	vector3	*MaterialColor;
	float	MaterialDiffuse;
	float	MaterialSpecular;
	float	MaterialSpecularExponent;
	float	MaterialReflectance;
	float	MaterialAbsorbance;
};

} // exRay

#endif