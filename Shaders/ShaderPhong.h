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

#ifndef __SHADER_PHONG_H
#define __SHADER_PHONG_H

namespace exRay {

/// Shader: Oœwietlenie Lambert/Phong.
/** Oblicza oœwietlenie rozproszone zgodnie z prawem Lamberta oraz oœwietlenie odbite
	zgodnie z aproksymacj¹ Phonga. Przy obliczeniach uwzglêdnia ca³kowite odbicie
	oraz refrakcjê.
*/
class ShaderPhong : public Shader
{
private: // cache
	vector3			*cLightColor;
	vector3			*cLightPosition;
	//float			*cLightDiffuse;
	//float			*cLightSpecular;
	unsigned int	cLightCount;
public:
	ShaderPhong(void);
	virtual ~ShaderPhong(void);

	virtual void	cacheVariables(Node *pnode);
	virtual vector3	computeShading(ShaderUniforms *su, const float *visibility=NULL);
};

} // exRay

#endif