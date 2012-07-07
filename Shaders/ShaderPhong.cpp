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
#include "../Types/Shader.h"
#include "../Graph/Variable.h"
#include "../Graph/Node.h"
#include "ShaderPhong.h"

using namespace exRay;

ShaderPhong::ShaderPhong(void) : Shader()
{
	cLightCount		= 0;
	cLightColor		= NULL;
	cLightPosition	= NULL;
}

ShaderPhong::~ShaderPhong(void)
{
	if(cLightColor)	delete[] cLightColor;
	if(cLightPosition) delete[] cLightPosition;
}

void ShaderPhong::cacheVariables(Node *pnode)
{
	VNode	*lights = (VNode*)pnode->getAttrib("env_light");

	if(!lights) return;
	if(lights->size() == 0) return;

	cLightCount		= lights->size();
	cLightColor		= new vector3[cLightCount];
	cLightPosition	= new vector3[cLightCount];

	Node	*lightNode;
	for(unsigned int i=0; i<cLightCount; i++)
	{
		lights->getValue(lightNode, i);
		lightNode->getAttrib("color")->getValue(cLightColor[i]);
		lightNode->getAttrib("position")->getValue(cLightPosition[i]);
	}
}

vector3 ShaderPhong::computeShading(ShaderUniforms *su, const float *visibility)
{
	vector3	pixel, cLightDirection, cLightReflect;
	float	diffuse, specular;

	for(unsigned int i=0; i<cLightCount; i++)
	{
		diffuse		= 0.0f;
		specular	= 0.0f;

		cLightDirection = (cLightPosition[i] - (*su->IntPoint)).getNormalized();
		float dotNL = su->Normal.dot(cLightDirection);
		if(dotNL > 0.0f)
		{
			diffuse = su->MaterialDiffuse * dotNL * visibility[i];
			cLightReflect = 2.0f * su->Normal * cLightDirection.dot(su->Normal) - cLightDirection;
			float dotNR = su->Normal.dot(cLightReflect);
			if(dotNR > 0.0f)
				specular = powf(dotNR, su->MaterialSpecularExponent) * su->MaterialSpecular * visibility[i];
		}

		pixel += diffuse * cLightColor[i] * (*su->MaterialColor);
		pixel += specular * cLightColor[i];
		//pixel += (*su->MaterialColor) * cLightDiffuse[i] * diffuse;
		//pixel += cLightSpecular[i] * specular;
	}

	if(su->ReflectFlag)
	{
		if(su->MaterialReflectance > 0.0f)
			pixel += su->MaterialReflectance * su->ReflectComponent * (*su->MaterialColor);
	}
	if(su->RefractFlag)
	{
		vector3	rAbsorbance = (*su->MaterialColor) * su->MaterialAbsorbance;
		pixel += su->RefractComponent * vector3(expf(rAbsorbance.x), expf(rAbsorbance.y), expf(rAbsorbance.y));
	}
	return pixel;
}
