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

using namespace exRay;

Variable::Variable(const std::string &varName, bool isArray)
{
	name	= varName;
	parent	= NULL;
	dirty	= true;
	array	= isArray;
	vnode	= NULL;
}

Variable::~Variable()
{ }

VInteger::VInteger(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(0);
}

VInteger::~VInteger()
{ }

bool VInteger::getValue(int&out,int i)
{ 
	out = value.at(i);
	return true;
}

bool VInteger::setValue(int in,int i)
{ 
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VInteger::pushValue(int in)
{ 
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VInteger::popValue(void)
{ 
	value.pop_back();
	dirty	= true;
	return true;
}

bool VInteger::popValue(int& out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VInteger::removeValue(int i)
{
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VInteger::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != INTEGER)
		return false;
	if(j == -1) j=i;

	int	tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}

VFloat::VFloat(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(0.0f);
}

VFloat::~VFloat()
{ }

bool VFloat::getValue(float&out,int i)
{
	out = value.at(i);
	return true;
}

bool VFloat::setValue(float in,int i)
{
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VFloat::pushValue(float in)
{
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VFloat::popValue(void)
{
	value.pop_back();
	dirty	= true;
	return true;
}

bool VFloat::popValue(float& out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VFloat::removeValue(int i)
{ 
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VFloat::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != FLOAT)
		return false;
	if(j == -1) j=i;

	float	tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}

VFloat2::VFloat2(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(vector2());
}

VFloat2::~VFloat2()
{ }

bool VFloat2::getValue(vector2 &out,int i)
{
	out = value.at(i);
	return true;
}

bool VFloat2::setValue(vector2 &in,int i)
{
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VFloat2::pushValue(vector2 &in)
{
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VFloat2::popValue(void)
{
	value.pop_back();
	dirty	= true;
	return true;
}

bool VFloat2::popValue(vector2 &out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VFloat2::removeValue(int i)
{ 
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VFloat2::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != FLOAT2)
		return false;
	if(j == -1) j=i;

	vector2	tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}

VFloat3::VFloat3(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(vector3());
}

VFloat3::~VFloat3()
{ }

bool VFloat3::getValue(vector3 &out,int i)
{
	out = value.at(i);
	return true;
}

bool VFloat3::setValue(vector3 &in,int i)
{
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VFloat3::pushValue(vector3 &in)
{
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VFloat3::popValue(void)
{
	value.pop_back();
	dirty	= true;
	return true;
}

bool VFloat3::popValue(vector3 &out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VFloat3::removeValue(int i)
{ 
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VFloat3::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != FLOAT3)
		return false;
	if(j == -1) j=i;

	vector3	tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}

VFloat4::VFloat4(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(vector4());
}

VFloat4::~VFloat4()
{ }

bool VFloat4::getValue(vector4 &out,int i)
{
	out = value.at(i);
	return true;
}

bool VFloat4::setValue(vector4 &in,int i)
{
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VFloat4::pushValue(vector4 &in)
{
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VFloat4::popValue(void)
{
	value.pop_back();
	dirty	= true;
	return true;
}

bool VFloat4::popValue(vector4 &out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VFloat4::removeValue(int i)
{ 
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VFloat4::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != FLOAT4)
		return false;
	if(j == -1) j=i;

	vector4	tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}

VMatrix4::VMatrix4(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(matrix4());
}

VMatrix4::~VMatrix4()
{ }

bool VMatrix4::getValue(matrix4 &out,int i)
{
	out = value.at(i);
	return true;
}

bool VMatrix4::setValue(matrix4 &in,int i)
{
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VMatrix4::pushValue(matrix4 &in)
{
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VMatrix4::popValue(void)
{
	value.pop_back();
	dirty	= true;
	return true;
}

bool VMatrix4::popValue(matrix4 &out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VMatrix4::removeValue(int i)
{ 
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VMatrix4::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != MATRIX4)
		return false;
	if(j == -1) j=i;

	matrix4	tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}

VString::VString(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(std::string(""));
}

VString::~VString()
{ }

bool VString::getValue(std::string &out,int i)
{
	out = value.at(i);
	return true;
}

bool VString::setValue(std::string &in,int i)
{
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VString::pushValue(std::string &in)
{
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VString::popValue(void)
{
	value.pop_back();
	dirty	= true;
	return true;
}

bool VString::popValue(std::string &out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VString::removeValue(int i)
{ 
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VString::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != STRING)
		return false;
	if(j == -1) j=i;

	std::string	tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}

VMesh::VMesh(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(NULL);
}

VMesh::~VMesh()
{ }

bool VMesh::getValue(Mesh *&out,int i)
{
	out = value.at(i);
	return true;
}

bool VMesh::setValue(Mesh *&in,int i)
{
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VMesh::pushValue(Mesh *&in)
{
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VMesh::popValue(void)
{
	value.pop_back();
	dirty	= true;
	return true;
}

bool VMesh::popValue(Mesh *&out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VMesh::removeValue(int i)
{ 
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VMesh::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != MESH)
		return false;
	if(j == -1) j=i;

	Mesh	*tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}

VShader::VShader(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(NULL);
}

VShader::~VShader()
{ }

bool VShader::getValue(Shader *&out,int i)
{
	out = value.at(i);
	return true;
}

bool VShader::setValue(Shader *&in,int i)
{
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VShader::pushValue(Shader *&in)
{
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VShader::popValue(void)
{
	value.pop_back();
	dirty	= true;
	return true;
}

bool VShader::popValue(Shader *&out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VShader::removeValue(int i)
{ 
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VShader::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != SHADER)
		return false;
	if(j == -1) j=i;

	Shader	*tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}

VNode::VNode(const std::string &varName, bool isArray) : Variable(varName, isArray)
{
	if(!isArray)
		value.push_back(NULL);
}

VNode::~VNode()
{ }

bool VNode::getValue(Node *&out,int i)
{
	out = value.at(i);
	return true;
}

bool VNode::setValue(Node *&in,int i)
{
	value[i] = in;
	dirty	 = true;
	return true;
}

bool VNode::pushValue(Node *&in)
{
	value.push_back(in);
	array	= true;
	dirty	= true;
	return true;
}

bool VNode::popValue(void)
{
	value.pop_back();
	dirty	= true;
	return true;
}

bool VNode::popValue(Node *&out)
{
	out = value.at(value.size()-1);
	value.pop_back();
	dirty	= true;
	return true;
}

bool VNode::removeValue(int i)
{ 
	value.erase(value.begin()+i);
	dirty	= true;
	return true;
}

bool VNode::copyValue(Variable *v, int i, int j)
{
	if(v->getType() != NODE)
		return false;
	if(j == -1) j=i;

	Node	*tmpValue;
	if(!getValue(tmpValue, i))
		return false;
	if(!v->setValue(tmpValue, j))
		return false;
	return true;
}