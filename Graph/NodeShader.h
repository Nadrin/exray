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

#ifndef __NODE_SHADER_H
#define __NODE_SHADER_H

namespace exRay {

/// Wêze³ shader'a.
/** Wêze³ sceny bêd¹cy refernecj¹ do funkcji cieniuj¹cej. Dana funkcja cieniuj¹ca (shader) oblicza oœwietlenie
	dla wszystkich wêz³ów, do których ten wêze³ jest pod³¹czony metod¹ Node::connect().
*/
class NodeShader : public Node
{
public:
	NodeShader(const std::string &name, Node *parent);
	virtual ~NodeShader(void);

	virtual void	cacheVariables(void);
	virtual bool	acceptsConnection(Variable *cvariable) const;
	virtual void	evaluate(void);

	virtual const std::string getType(void) const
	{ return std::string("shader"); }
};

/// Kreator klasy NodeShader.
class NodeShaderCreator : public NodeCreator
{
public:
	virtual Node* operator()(const std::string &name, Node *parent=NULL) const
	{ return new NodeShader(name, parent); }
};

} // exRay

#endif