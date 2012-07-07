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

#ifndef __VARIABLE_H
#define __VARIABLE_H

#include "../types/Mesh.h"
#include "../types/Image.h"

namespace exRay {

/// Typy wartoœci atrybutów wêz³ów.
enum VARTYPE
{
	NONE,
	INTEGER,
	FLOAT,
	FLOAT2,
	FLOAT3,
	FLOAT4,
	MATRIX4,
	STRING,
	MESH,
	SHADER,
	NODE,
	COMPOUND,
};

class Node;
class Shader;
class VCompound;

/// Generyczny atrybut (zmienna) wêz³a.
/** Bazowa klasa dla wszystkich klas atrybutów wêz³ów. */
class Variable
{
protected:
	std::string		name;
	VCompound		*parent;
	Node			*vnode;
	bool			dirty;
	bool			array;
public:
	Variable(const std::string &varName, bool isArray=false);
	virtual ~Variable();

	virtual VARTYPE	getType() const				{ return NONE;		}

	const std::string&		getName() const		{ return name;			}
	VCompound*				getParent() const	{ return parent;		}
	bool					isDirty() const		{ return dirty;			}
	bool					isArray() const		{ return array;			}
	bool					isConnected() const	{ return !(vnode==NULL);}
	void					setClean()			{ dirty = false;		}

	void					setOutputNode(Node *pnode) { vnode = pnode; }
	Node*					getOutputNode(void)		   { return vnode;  }

	virtual unsigned int	size() const			{ return 0;		}

	virtual bool	getValue(int&,int i=0)			{ return false;	}
	virtual bool	getValue(float&,int i=0)		{ return false;	}
	virtual bool	getValue(vector2&,int i=0)		{ return false;	}
	virtual bool	getValue(vector3&,int i=0)		{ return false;	}
	virtual bool	getValue(vector4&,int i=0)		{ return false;	}
	virtual bool	getValue(matrix4&,int i=0)		{ return false; }
	virtual bool	getValue(std::string&,int i=0)	{ return false; }
	virtual bool	getValue(Mesh*&,int i=0)		{ return false; }
	virtual bool	getValue(Shader*&,int i=0)		{ return false; }
	virtual bool	getValue(Node*&, int i=0)		{ return false; }

	virtual bool	setValue(int,int i=0)			{ return false; }
	virtual bool	setValue(float,int i=0)			{ return false; }
	virtual bool	setValue(vector2&,int i=0)		{ return false; }
	virtual bool	setValue(vector3&,int i=0)		{ return false; }
	virtual bool	setValue(vector4&,int i=0)		{ return false; }
	virtual bool	setValue(matrix4&,int i=0)		{ return false; }
	virtual bool	setValue(std::string&,int i=0)	{ return false; }
	virtual bool	setValue(Mesh*&, int i=0)		{ return false; }
	virtual bool	setValue(Shader*&, int i=0)		{ return false; }
	virtual bool	setValue(Node*&, int i=0)		{ return false; }

	virtual bool	pushValue(int)					{ return false; }
	virtual bool	pushValue(float)				{ return false; }
	virtual bool	pushValue(vector2&)				{ return false; }
	virtual bool	pushValue(vector3&)				{ return false; }
	virtual bool	pushValue(vector4&)				{ return false; }
	virtual bool	pushValue(matrix4&)				{ return false; }
	virtual bool	pushValue(std::string&)			{ return false; }
	virtual bool	pushValue(Mesh*&)				{ return false; }
	virtual bool	pushValue(Shader*&)				{ return false; }
	virtual bool	pushValue(Node*&)				{ return false; }

	virtual bool	popValue(void)					{ return false; }
	virtual bool	popValue(int&)					{ return false;	}
	virtual bool	popValue(float&)				{ return false;	}
	virtual bool	popValue(vector2&)				{ return false;	}
	virtual bool	popValue(vector3&)				{ return false;	}
	virtual bool	popValue(vector4&)				{ return false;	}
	virtual bool	popValue(matrix4&)				{ return false; }
	virtual bool	popValue(std::string&)			{ return false; }
	virtual bool	popValue(Mesh*&)				{ return false; }
	virtual bool	popValue(Shader*&)				{ return false; }
	virtual bool	popValue(Node*&)				{ return false; }

	virtual bool	removeValue(int i)						{ return false; }
	virtual bool	copyValue(Variable*, int i=0, int j=-1)	{ return false; }
};

/// Atrybut typu Integer.
class VInteger : public Variable
{
protected:
	std::vector<int>	value;
public:
	VInteger(const std::string &varName, bool isArray=false);
	virtual ~VInteger();

	virtual VARTYPE	getType() const					{ return INTEGER;	}

	virtual bool	getValue(int&out,int i=0);
	virtual bool	setValue(int in,int i=0);
	virtual bool	pushValue(int in);
	virtual bool	popValue(void);		
	virtual bool	popValue(int& out);	
	virtual bool	removeValue(int i);
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const				{ return (unsigned int)value.size(); }
};

/// Atrybut typu Float.
class VFloat : public Variable
{
protected:
	std::vector<float>	value;
public:
	VFloat(const std::string &varName, bool isArray=false);
	virtual ~VFloat();

	virtual VARTYPE	getType() const					{ return FLOAT;		}

	virtual bool	getValue(float&out,int i=0);
	virtual bool	setValue(float in,int i=0);
	virtual bool	pushValue(float in);
	virtual bool	popValue(void);	
	virtual bool	popValue(float& out);
	virtual bool	removeValue(int i);	
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const				{ return (unsigned int)value.size(); }
};

/// Atrybut typu Vector2.
class VFloat2 : public Variable
{
protected:
	std::vector<vector2>	value;
public:
	VFloat2(const std::string &varName, bool isArray=false);
	virtual ~VFloat2();

	virtual VARTYPE	getType() const					{ return FLOAT2;	}

	virtual bool	getValue(vector2&out,int i=0);
	virtual bool	setValue(vector2&in,int i=0);
	virtual bool	pushValue(vector2& in);
	virtual bool	popValue(void);		
	virtual bool	popValue(vector2& out);
	virtual bool	removeValue(int i);
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const				{ return (unsigned int)value.size(); }
};

/// Atrybut typu Vector3.
class VFloat3 : public Variable
{
protected:
	std::vector<vector3>	value;
public:
	VFloat3(const std::string &varName, bool isArray=false);
	virtual ~VFloat3();

	virtual VARTYPE	getType() const					{ return FLOAT3;	}

	virtual bool	getValue(vector3&out,int i=0);
	virtual bool	setValue(vector3&in,int i=0);
	virtual bool	pushValue(vector3&in);
	virtual bool	popValue(void);
	virtual bool	popValue(vector3& out);
	virtual bool	removeValue(int i);		
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const				{ return (unsigned int)value.size(); }
};

/// Atrybut typu Vector4.
class VFloat4 : public Variable
{
protected:
	std::vector<vector4>	value;
public:
	VFloat4(const std::string &varName, bool isArray=false);
	virtual ~VFloat4();
	
	virtual VARTYPE	getType() const					{ return FLOAT4;	}

	virtual bool	getValue(vector4&out,int i=0);
	virtual bool	setValue(vector4&in,int i=0);
	virtual bool	pushValue(vector4&in);
	virtual bool	popValue(void);
	virtual bool	popValue(vector4& out);
	virtual bool	removeValue(int i);
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const				{ return (unsigned int)value.size(); }
};

/// Atrybut typu Matrix4.
class VMatrix4 : public Variable
{
protected:
	std::vector<matrix4>	value;
public:
	VMatrix4(const std::string &varName, bool isArray=false);
	virtual ~VMatrix4();

	virtual VARTYPE	getType() const					{ return MATRIX4;	}

	virtual bool	getValue(matrix4&out,int i=0);
	virtual bool	setValue(matrix4&in,int i=0);
	virtual bool	pushValue(matrix4&in);
	virtual bool	popValue(void);
	virtual bool	popValue(matrix4& out);
	virtual bool	removeValue(int i);
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const				{ return (unsigned int)value.size(); }
};

/// Atrybut typu String.
class VString : public Variable
{
protected:
	std::vector<std::string> value;
public:
	VString(const std::string &varName, bool isArray=false);
	virtual ~VString();

	virtual VARTYPE	getType() const						{ return STRING;	}

	virtual bool	getValue(std::string&out,int i=0);
	virtual bool	setValue(std::string&in,int i=0);
	virtual bool	pushValue(std::string&in);
	virtual bool	popValue(void);
	virtual bool	popValue(std::string& out);
	virtual bool	removeValue(int i);
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const					{ return (unsigned int)value.size(); }
};	

/// Atrybut typu Mesh.
class VMesh : public Variable
{
protected:
	std::vector<Mesh*> value;
public:
	VMesh(const std::string &varName, bool isArray=false);
	virtual ~VMesh();

	virtual VARTYPE	getType() const						{ return MESH;	}

	virtual bool	getValue(Mesh *&out,int i=0);
	virtual bool	setValue(Mesh *&in,int i=0);
	virtual bool	pushValue(Mesh *&in);
	virtual bool	popValue(void);
	virtual bool	popValue(Mesh *&out);
	virtual bool	removeValue(int i);
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const					{ return (unsigned int)value.size(); }
};	

/// Atrybut typu Shader.
class VShader : public Variable
{
protected:
	std::vector<Shader*> value;
public:
	VShader(const std::string &varName, bool isArray=false);
	virtual ~VShader();

	virtual VARTYPE	getType() const						{ return SHADER;	}

	virtual bool	getValue(Shader *&out,int i=0);
	virtual bool	setValue(Shader *&in,int i=0);
	virtual bool	pushValue(Shader *&in);
	virtual bool	popValue(void);
	virtual bool	popValue(Shader *&out);
	virtual bool	removeValue(int i);
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const					{ return (unsigned int)value.size(); }
};

/// Atrybut typu Node Reference.
class VNode : public Variable
{
protected:
	std::vector<Node*> value;
public:
	VNode(const std::string &varName, bool isArray=false);
	virtual ~VNode();

	virtual VARTYPE	getType() const						{ return NODE;	}

	virtual bool	getValue(Node *&out,int i=0);
	virtual bool	setValue(Node *&in,int i=0);
	virtual bool	pushValue(Node *&in);
	virtual bool	popValue(void);
	virtual bool	popValue(Node *&out);
	virtual bool	removeValue(int i);
	virtual bool	copyValue(Variable*, int i=0, int j=-1);

	virtual unsigned int size() const					{ return (unsigned int)value.size(); }
};

} // exRay

#include "../types/Shader.h"

#endif