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
#include "../Core/Renderer.h"
#include "../Graph/Variable.h"
#include "Shader.h"

using namespace exRay;

Shader::Shader()
{
}

Shader::~Shader(void)
{
	
}

vector3 Shader::computeShading(ShaderUniforms *su, const float *visibility)
{
	return vector3(1.0f, 1.0f, 1.0f);
}

void Shader::cacheVariables(Node *pnode)
{
}

ShaderUniforms* Shader::createRenderingCache(void) const
{
	return new ShaderUniforms();
}