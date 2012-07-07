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
#include "NodeShader.h"

using namespace exRay;

NodeShader::NodeShader(const std::string &name, Node *parent) : Node(name, parent)
{
	addAttrib(new VShader("cgfx_shader"));
	addAttrib(new VNode("env_light", true));
}

NodeShader::~NodeShader(void)
{
}

void NodeShader::cacheVariables(void)
{
	Shader	*theShader;
	getAttrib("cgfx_shader")->getValue(theShader);
	if(theShader)
		theShader->cacheVariables(this);

	Node::cacheVariables();
}

bool NodeShader::acceptsConnection(Variable *cvariable) const
{
	if(cvariable->getType() == SHADER)
		return true;
	return false;
}

void NodeShader::evaluate(void)
{
	if(evaluated) return;
	if(hasOutput())
	{
		Shader	*funcShader;
		getAttrib("cgfx_shader")->getValue(funcShader);
		setOutput(funcShader);
	}

	Node::evaluate();
}