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
#include "../Core/Engine.h"
#include "../Graph/Variable.h"
#include "../Graph/Node.h"
#include "../Graph/Object.h"
#include "../Graph/NodeLight.h"
#include "../Graph/NodeMaterial.h"
#include "../Graph/NodePlane.h"
#include "../Graph/NodeShader.h"
#include "../Graph/NodeSphere.h"
#include "../Graph/NodeBox.h"
#include "../Graph/NodeCone.h"
#include "../Graph/NodeCylinder.h"
#include "../Graph/NodeTorus.h"
#include "Shader.h"
#include "../Shaders/ShaderPhong.h"
#include "Scene.h"

using namespace exRay;

/// Wewnêtrzna klasa rozszerzaj¹ca funkcjonalnóœc std::vector<std::string>.
class exRay::StringVector : public std::vector<std::string>
{
public:
	std::vector<std::string>::iterator find(const std::string &value)
	{
		std::vector<std::string>::iterator it = begin();
		for(;it<end(); it++) if((*it) == value) return it;
		return end();
	}
	bool has(const std::string &value)
	{ return find(value) != end(); }
};

Scene::Scene(void)
{
	errorCount		= 0;
	scriptOpened	= false;

	frameX = frameY = 0;

	createErrorMap();
	createParameterMap();
	registerBuiltInShaders();
	registerBuiltInCreators();
}

Scene::Scene(const std::string &filename)
{
	errorCount		= 0;
	scriptOpened	= false;

	frameX = frameY = 0;

	createErrorMap();
	createParameterMap();
	registerBuiltInShaders();
	registerBuiltInCreators();

	if(filename.length() == 0)
		return;

	std::ifstream file(filename.c_str());
	if(!file.is_open())
		return;

	parseFile(file);
	file.close();
}

Scene::~Scene(void)
{
	for(CreatorMap::iterator it=creatorMap.begin(); it!=creatorMap.end(); it++)
		delete it->second;
	for(ShaderMap::iterator it=shaderMap.begin(); it!=shaderMap.end(); it++)
		delete it->second;
}

void Scene::createErrorMap(void)
{	
	errorMap[NoError]				= "No error.";
	errorMap[ParseError]			= "Parse error. Term or constant expected.";
	errorMap[ParenthesisExpected]	= "Opening or closing parenthesis expected.";
	errorMap[DotExpected]			= "Dot property operator expected.";
	errorMap[UnexpectedEOL]			= "Unexpected end of line found.";
	errorMap[ConstructorInvalid]	= "Invalid constructor call.";
	errorMap[AssignmentInvalid]		= "Invalid property assignment.";
	errorMap[ConstantInvalid]		= "Invalid constant in assignment expression.";
	errorMap[CallInvalid]			= "Invalid function call.";
	errorMap[UnknownNodeType]		= "Unknown node type specified in constructor call.";
	errorMap[UnknownParameter]		= "Unknown parameter.";
	errorMap[InvalidParameter]		= "Invalid parameter value.";
	errorMap[UndeclaredIdentifier]	= "Undeclared identifier.";
	errorMap[RedeclaredIdentifier]	= "Identifier already declared and cannot be constructed.";
	errorMap[ArgumentMismatch]		= "Wrong number of arguments passed to a function.";
	errorMap[UnknownFunction]		= "No such function.";
	errorMap[UnknownShader]			= "Unknown shader object.";
	errorMap[UnknownError]			= "An unknown error occured.";

	errorMap[UnknownInstruction]	= "Unknown instruction code. Ignoring.";
	errorMap[UnknownAttribute]		= "Property does not exist.";
	errorMap[InvalidValue]			= "Property value type mismatch.";
	errorMap[ShaderSetFailed]		= "Shader assignment failed.";
	errorMap[ConnectionFailed]		= "Connection to foreign property failed.";
	errorMap[MappingFailed]			= "Property mapping between nodes failed.";

	errorMap[NoShaderAssigned]		= "has no shader connected to.";
	errorMap[NoMaterialAssigned]	= "has no material connected to."; 
}

void Scene::createParameterMap(void)
{
	// Parameters
	parameterMap.first["Supersampling"]		= exRay::Supersampling;
	parameterMap.first["ShadowSampling"]	= exRay::ShadowSampling;
	parameterMap.first["RenderSamples"]		= exRay::RenderSamples;
	parameterMap.first["ShadowSamples"]		= exRay::ShadowSamples;
	parameterMap.first["RefractionIndex"]	= exRay::EnvRIndex;

	parameterMap.first["Width"]				= exRay::FrameWidth;
	parameterMap.first["Height"]			= exRay::FrameHeight;

	// Values
	parameterMap.second["Adaptive"]			= exRay::Adaptive;
	parameterMap.second["Full"]				= exRay::Full;
	parameterMap.second["RegularGrid"]		= exRay::RegularGrid;
	parameterMap.second["MonteCarlo"]		= exRay::MonteCarlo;
}

void Scene::registerBuiltInCreators(void)
{
	creatorMap["Node"]		= new NodeCreator;
	creatorMap["Object"]	= new ObjectCreator;
	creatorMap["Group"]		= new GroupCreator;
	creatorMap["Camera"]	= new NodeCameraCreator;
	creatorMap["Light"]		= new NodeLightCreator;
	creatorMap["AreaLight"]	= new NodeAreaLightCreator;
	creatorMap["Material"]	= new NodeMaterialCreator;
	creatorMap["Shader"]	= new NodeShaderCreator;
	creatorMap["Plane"]		= new NodePlaneCreator;
	creatorMap["Sphere"]	= new NodeSphereCreator;
	creatorMap["Box"]		= new NodeBoxCreator;

	// Jeszcze nie obs³ugiwane!
	//creatorMap["Cone"]		= new NodeConeCreator;
	//creatorMap["Cylinder"]	= new NodeCylinderCreator;
	//creatorMap["Torus"]		= new NodeTorusCreator;
}

void Scene::registerBuiltInShaders(void)
{
	shaderMap["Generic"]	= new Shader;
	shaderMap["Phong"]		= new ShaderPhong;
}

std::string Scene::getScriptLog(void) const
{
	return logStream.str();
}

void Scene::clear()
{
	errorCount		= 0;
	scriptOpened	= false;

	lines.clear();
	logStream.str().clear();
}

bool Scene::registerCreator(const std::string &name, NodeCreator *creator)
{
	if(creator == NULL)
		return false;

	CreatorMap::iterator location = creatorMap.find(name);
	if(location != creatorMap.end())
		return false;
	creatorMap[name] = creator;
	return true;
}

bool Scene::registerShader(const std::string &name, Shader *shader)
{
	if(shader == NULL)
		return false;

	ShaderMap::iterator location = shaderMap.find(name);
	if(location != shaderMap.end())
		return false;
	shaderMap[name] = shader;
	return true;
}

bool Scene::isValidValue(const std::string &value)
{
	float	test;
	return sscanf(value.c_str(), "%f", &test) > 0;
}

void Scene::stripString(std::string &string)
{
	unsigned int	offset=0, loffset=0;
	std::string		result;
	while(offset < string.length() && offset != std::string::npos)
	{
		offset	= (unsigned int)string.find_first_not_of(" \t", offset);
		loffset	= (unsigned int)string.find_first_of(" \t", offset);
		if(loffset == std::string::npos)
			loffset = (unsigned int)string.length()+1;

		result += string.substr(offset, loffset-offset+1);
		offset = loffset+1;

		if(result.at(result.length()-1) == '\t')
			result[result.length()-1] = ' ';
	}

	offset = (unsigned int)result.find_first_of(";#", 0);
	string = result.substr(0, offset);
}

bool Scene::reportError(unsigned int line, int errorID, const std::string &info)
{
	logStream << "(" << line << ") ";
	if(info.length() > 0)
		logStream << "'" << info << "': ";
	logStream << errorMap[errorID] << std::endl;
	return false;
}

bool Scene::reportError(unsigned int line, int errorID, const char info)
{
	logStream << "(" << line << ") ";
	if(info != 0)
		logStream << "'" << info << "': ";
	logStream << errorMap[errorID] << std::endl;
	return false;
}

void Scene::reportWarning(int errorID, const std::string &info)
{
	logStream << "Warning. ";
	if(info.length() > 0)
		logStream << "'" << info << "': ";
	logStream << errorMap[errorID] << std::endl;
}

void Scene::reportValidationError(int errorID, const std::string &info)
{
	logStream << "Error: Node ";
	if(info.length() > 0)
		logStream << "'" << info << "' ";
	logStream << errorMap[errorID] << std::endl;
}

bool Scene::getTerm(std::string &result, const std::string &string, unsigned int &pos, unsigned int line)
{
	unsigned int	delim;
	bool			number = false;
	if(string.at(pos) == ' ') pos++;

	if(string.find_first_of("-0123456789", pos) == pos)
		number = true;

	delim = (unsigned int)string.find_first_of(" .(),\"'", pos);
	if(delim == std::string::npos)
	{
		result	= string.substr(pos, std::string::npos);
		pos		= (unsigned int)string.length();
		return true;
	}
	else if(delim == 0)
		return reportError(line, Scene::ParseError, string.at(0));
	else
	{
		result = string.substr(pos, delim-pos);
		pos   += delim-pos;
		if(number && string.at(pos) == '.')
		{
			if(string.find_first_of("0123456789", pos) == pos+1)
			{
				result += string.at(pos++);
				delim = (unsigned int)string.find_first_of(" .(),\"'", pos+1);
				result += string.substr(pos, delim-pos);
				pos    += delim-pos;
			}
		}
	}
	return true;
}

bool Scene::getTokens(TokenSet &tokens, std::string &string)
{
	std::string		term;
	char			singleton[2];
	unsigned int	position = 0;

	singleton[1] = 0;
	if(string.at(0) == '#' || string.at(0) == ';')
		return true;

	while(position < string.length())
	{
		if(!getTerm(term, string, position, tokens.first)) return false;
		if(term.length() > 0)
			tokens.second.push_back(term);
		if(position >= string.length())
			break;
		singleton[0] = string.at(position++);
		if(singleton[0] != ' ')
			tokens.second.push_back(std::string(singleton));
	}
	return true;
}

bool Scene::functionCall(TokenSet &line, unsigned int offset, unsigned int argc, StringVector &args)
{
	if(line.second.size() != offset + 2*argc + 1)
		return reportError(line.first, Scene::ArgumentMismatch, line.second.at(offset-1));
	if(line.second.at(offset) != "(")
		return reportError(line.first, Scene::ParenthesisExpected, line.second.at(offset));

	for(unsigned int i=offset+1; i<line.second.size()-1; i++)
	{
		if((i-offset) % 2 != 0)
			args.push_back(line.second.at(i));
		else if(line.second.at(i) != ",")
				return reportError(line.first, Scene::CallInvalid);
	}
	if(line.second.at(line.second.size()-1) != ")")
		return reportError(line.first, Scene::ParenthesisExpected, line.second.at(line.second.size()-1));
	return true;
}

bool Scene::interpretLine(TokenSet &line, StringVector &identifiers)
{
	TokenSet	instruction;
	if(line.second.at(0) == "new")
	{
		instruction.first = Scene::Constructor;
		if(line.second.size() < 5 || line.second.size() > 7)
			return reportError(line.first, Scene::ConstructorInvalid); 
		CreatorMap::iterator creatorLocation = creatorMap.find(line.second.at(1));
		if(creatorLocation == creatorMap.end())
			return reportError(line.first, Scene::UnknownNodeType, line.second.at(1));
		instruction.second.push_back(line.second.at(1));
		if(line.second.at(2) != "(")
			return reportError(line.first, Scene::ParenthesisExpected, line.second.at(2));
		if(identifiers.has(line.second.at(3)))
			return reportError(line.first, Scene::RedeclaredIdentifier, line.second.at(3));
		instruction.second.push_back(line.second.at(3));

		if(line.second.at(4) == ")")
		{
			if(line.second.size() > 5)
				return reportError(line.first, Scene::ConstructorInvalid);
			//identifiers.push_back(line.second.at(3));
			//return true;
		}

		else if(line.second.at(4) == ",")
		{
			if(line.second.size() < 7)
				return reportError(line.first, Scene::ConstructorInvalid);
			if(!identifiers.has(line.second.at(5)))
				return reportError(line.first, Scene::UndeclaredIdentifier, line.second.at(5));
			instruction.second.push_back(line.second.at(5));
			if(line.second.at(6) != ")")
				return reportError(line.first, Scene::ParenthesisExpected, line.second.at(6));
		}

		else return reportError(line.first, Scene::ConstructorInvalid);
		identifiers.push_back(line.second.at(3));
	}
	else if(line.second.at(0) == "parameter" || line.second.at(0) == "set")
	{
		instruction.first = Scene::Parameter;
		if(line.second.size() < 3)
			return reportError(line.first, Scene::UnexpectedEOL);
		ParameterMap::iterator paramLocation = parameterMap.first.find(line.second.at(1));
		if(paramLocation == parameterMap.first.end())
			return reportError(line.first, Scene::UnknownParameter, line.second.at(1));
		instruction.second.push_back(line.second.at(1));

		ParameterMap::iterator valueLocation = parameterMap.second.find(line.second.at(2));
		switch(paramLocation->second)
		{
		case exRay::Supersampling:
			if(valueLocation->second != exRay::Full && valueLocation->second != exRay::Adaptive)
				return reportError(line.first, Scene::InvalidParameter, line.second.at(2));
			break;
		case exRay::ShadowSampling:
			if(valueLocation->second != exRay::RegularGrid && valueLocation->second != exRay::MonteCarlo)
				return reportError(line.first, Scene::InvalidParameter, line.second.at(2));
			break;
		default:
			if(!isValidValue(line.second.at(2)))
				return reportError(line.first, Scene::InvalidParameter, line.second.at(2));
		}
		instruction.second.push_back(line.second.at(2));
	}
	else
	{
		instruction.first = Scene::Assignment;
		if(line.second.size() < 5)
			return reportError(line.first, Scene::AssignmentInvalid);
		if(!identifiers.has(line.second.at(0)))
			return reportError(line.first, Scene::UndeclaredIdentifier, line.second.at(0));
		instruction.second.push_back(line.second.at(0));
		if(line.second.at(1) != ".")
			return reportError(line.first, Scene::DotExpected, line.second.at(1));
		
		if(line.second.at(3) == "(")
		{
			StringVector	fargs;
			if(line.second.at(2) == "shader")
			{
				instruction.first = Scene::ShaderSet;
				if(!functionCall(line, 3, 1, fargs))
					return false;
				ShaderMap::iterator shaderLocation = shaderMap.find(fargs.at(0));
				if(shaderLocation == shaderMap.end())
					return reportError(line.first, Scene::UnknownShader, fargs.at(0));
				instruction.second.push_back(fargs.at(0));
			}
			else if(line.second.at(2) == "connect")
			{
				instruction.first = Scene::Connection;
				if(!functionCall(line, 3, 2, fargs))
					return false;
				if(!identifiers.has(fargs.at(0)))
					return reportError(line.first, Scene::UndeclaredIdentifier, fargs.at(0));
				instruction.second.push_back(fargs.at(0));
				instruction.second.push_back(fargs.at(1));
			}
			else if(line.second.at(2) == "map")
			{
				instruction.first = Scene::Mapping;
				if(!functionCall(line, 3, 1, fargs))
					return false;
				if(!identifiers.has(fargs.at(0)))
					return reportError(line.first, Scene::UndeclaredIdentifier, fargs.at(0));
				instruction.second.push_back(fargs.at(0));
			}
			else return reportError(line.first, Scene::UnknownFunction, line.second.at(2));
		}

		else if(line.second.at(3) != "=")
			return reportError(line.first, Scene::AssignmentInvalid);
		else if(line.second.at(4) == "(")
		{
			instruction.second.push_back(line.second.at(2));

			unsigned int i=4;
			bool		 vclosed=false;
			while(++i < line.second.size())
			{
				if(i % 2 == 0)
				{
					if(line.second.at(i) == ")")
						vclosed = true;
					else if(line.second.at(i) != ",")
						return reportError(line.first, Scene::AssignmentInvalid);
					continue;
				}
				if(!isValidValue(line.second.at(i)))
					return reportError(line.first, Scene::ConstantInvalid, line.second.at(i));
				instruction.second.push_back(line.second.at(i));
			}
			if(!vclosed)
				return reportError(line.first, Scene::ParenthesisExpected, line.second.at(i-1));
		}
		else
		{
			instruction.second.push_back(line.second.at(2));
			if(!isValidValue(line.second.at(4)))
				return reportError(line.first, Scene::ConstantInvalid, line.second.at(4));
			instruction.second.push_back(line.second.at(4));
		}
	}
	lines.push_back(instruction);
	return true;
}

int Scene::parseFile(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if(!file.is_open())
	{
		scriptOpened = false;
		errorCount	 = 0;
		return -1;
	}

	int errors = parseFile(file);
	file.close();
	return errors;
}

int Scene::parseFile(std::ifstream &file)
{
	unsigned int	lineCount = 1;
	int				errors	  = 0;
	char*			lineBuffer = new char[Scene::MaxLine];
	std::string		currentLine;

	TokenSet		tokenLine;
	StringVector	identifiers;

	clear();
	while(!file.eof())
	{
		file.getline(lineBuffer, Scene::MaxLine);
		currentLine = std::string(lineBuffer);
		if(currentLine.length() == 0)
		{ lineCount++; continue; }

		stripString(currentLine);
		if(currentLine.length() == 0)
		{ lineCount++; continue; }

		tokenLine.first = lineCount;
		tokenLine.second.clear();
		if(!getTokens(tokenLine, currentLine))
		{ errors++; lineCount++; continue; }
		if(!interpretLine(tokenLine, identifiers))
		{ errors++; lineCount++; continue; }

		lineCount++;
	}
	delete[] lineBuffer;

	scriptOpened = errors == 0;
	errorCount	 = errors;
	return errors;
}

bool Scene::setupEngine(Engine *engine)
{
	if(!scriptOpened || errorCount > 0 || !engine)
		return false;

	int	paramName, paramIValue;
	float		   paramFValue;

	for(std::vector<TokenSet>::iterator i=lines.begin(); i<lines.end(); i++)
	{
		if(i->first != Scene::Parameter)
			continue;
		paramName  = parameterMap.first.find(i->second.at(0))->second;

		if(paramName == exRay::Supersampling || paramName == exRay::ShadowSampling)
		{
			paramIValue = parameterMap.second.find(i->second.at(1))->second;
			engine->setParameter(paramName, paramIValue);
		}
		else if(paramName == exRay::EnvRIndex)
		{
			paramFValue = fabsf((float)atof(i->second.at(1).c_str()));
			engine->setParameter(paramName, paramFValue);
		}
		else if(paramName == exRay::FrameWidth || paramName == exRay::FrameHeight)
		{
			if(paramName == exRay::FrameWidth)
			{
				frameX = (int)abs(atoi(i->second.at(1).c_str()));
				if(frameX < Scene::MinWH) frameX = Scene::MinWH;
			}
			if(paramName == exRay::FrameHeight)
			{
				frameY = (int)abs(atoi(i->second.at(1).c_str()));
				if(frameY < Scene::MinWH) frameY = Scene::MinWH;
			}
		}
		else
		{
			paramIValue = (int)abs(atoi(i->second.at(1).c_str()));
			engine->setParameter(paramName, paramIValue);
		}
	}
	return true;
}

int Scene::createGraph(Node *root)
{
	if(!scriptOpened || errorCount > 0 || !root)
		return -1;

	int			warnings = 0;
	char		buffer[32];

	Node		*node, *conNode;
	Variable	*operand;

	float v1; vector2 v2; vector3 v3; vector4 v4;
	for(std::vector<TokenSet>::iterator i=lines.begin(); i<lines.end(); i++)
	{
		switch(i->first)
		{
		case Scene::Constructor:
			if(i->second.size() == 2)
				node = root;
			else node = root->getChild(i->second.at(2));
			creatorMap[i->second.at(0)]->operator ()(i->second.at(1), node);
			break;
		case Scene::Parameter:
			break;
		case Scene::Assignment:
			node	= root->getChild(i->second.at(0));
			operand	= node->getAttrib(i->second.at(1));
			if(!operand)
			{
				reportWarning(UnknownAttribute, i->second.at(0)+"."+i->second.at(1));
				warnings++;
				break;
			}
			switch(i->second.size())
			{
			case 3: // float
				if(operand->getType() != FLOAT)
				{
					reportWarning(InvalidValue);
					warnings++; break;
				}
				v1 = (float)atof(i->second.at(2).c_str());
				operand->setValue(v1);
				break;
			case 4: // vector2
				if(operand->getType() != FLOAT2)
				{
					reportWarning(InvalidValue);
					warnings++; break;
				}
				v2 = vector2((float)atof(i->second.at(2).c_str()),
							 (float)atof(i->second.at(3).c_str()));
				operand->setValue(v2);
				break;
			case 5: // vector3
				if(operand->getType() != FLOAT3)
				{
					reportWarning(InvalidValue);
					warnings++; break;
				}
				v3 = vector3((float)atof(i->second.at(2).c_str()),
							 (float)atof(i->second.at(3).c_str()),
							 (float)atof(i->second.at(4).c_str()));
				operand->setValue(v3);
				break;
			case 6: // vector4
				if(operand->getType() != FLOAT4)
				{
					reportWarning(InvalidValue);
					warnings++; break;
				}
				v4 = vector4((float)atof(i->second.at(2).c_str()),
							 (float)atof(i->second.at(3).c_str()),
							 (float)atof(i->second.at(4).c_str()),
							 (float)atof(i->second.at(5).c_str()));
				operand->setValue(v4);
				break;
			default:
				reportWarning(InvalidValue);
				warnings++;
			}
			break;
		case Scene::ShaderSet:
			node		= root->getChild(i->second.at(0));
			operand		= node->getAttrib("cgfx_shader");
			if(!operand)
			{
				reportWarning(Scene::ShaderSetFailed, i->second.at(0));
				warnings++;
			} else operand->setValue(shaderMap[i->second.at(1)]);
			break;
		case Scene::Connection:
			node		= root->getChild(i->second.at(0));
			conNode		= root->getChild(i->second.at(1));

			if(!node->connect(conNode, i->second.at(2)))
			{
				reportWarning(Scene::ConnectionFailed,
					i->second.at(0)+"<->"+i->second.at(1)+"."+i->second.at(2));
				warnings++;
			}
			break;
		case Scene::Mapping:
			node		= root->getChild(i->second.at(0));
			conNode		= root->getChild(i->second.at(1));

			if(!node->map(conNode))
			{
				reportWarning(Scene::MappingFailed, i->second.at(0)+"<->"+i->second.at(1));
				warnings++;
			}
			break;
		default:
			sprintf(buffer, "%d", i->first);
			reportWarning(UnknownInstruction, buffer);
			warnings++;
		}
	}
	return warnings;
}

int Scene::validateGraph(Node *root)
{
	int			errors = 0;
	NodeList	graphObjects;
	Shader		*testShader;
	Node		*testNode;

	if(root == NULL)
		return -1;

	root->getObjectsArray(&graphObjects);
	for(NodeList::iterator i=graphObjects.begin(); i<graphObjects.end(); i++)
	{
		if((*i)->ignoreIntersection())
			continue;

		(*i)->getAttrib("cgfx_shader")->getValue(testShader);
		if(testShader == NULL)
		{
			reportValidationError(Scene::NoShaderAssigned, (*i)->getName());
			errors++;
		}
		(*i)->getAttrib("material")->getValue(testNode);
		if(testNode == NULL)
		{
			reportValidationError(Scene::NoMaterialAssigned, (*i)->getName());
			errors++;
		}
	}
	return errors;
}
