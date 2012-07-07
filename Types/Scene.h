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

#ifndef __SCENE_H
#define __SCENE_H

namespace exRay {

class Node;
class Engine;
class Shader;
class NodeCreator;
class StringVector;

typedef std::vector<std::string>				TokenString;
typedef std::pair<unsigned int, TokenString>	TokenSet;
typedef std::map<int, std::string>				ErrorMap;
typedef std::map<std::string, NodeCreator*>		CreatorMap;
typedef std::map<std::string, Shader*>			ShaderMap;
typedef std::map<std::string, int>				ParameterMap;
typedef std::pair<ParameterMap, ParameterMap>	ParameterSet;

/// Klasa sceny.
/** Wczytuje skrypt konfiguracji sceny z pliku tekstowego i na jego podstawie
	automatycznie konstruuje odpowiedni graf sceny oraz ustawia parametry silnika.
*/
class Scene
{
private:
	std::stringstream		logStream;
	std::vector<TokenSet>	lines;
	ErrorMap				errorMap;
	CreatorMap				creatorMap;
	ShaderMap				shaderMap;
	ParameterSet			parameterMap;

	int						errorCount;
	bool					scriptOpened;
	
	unsigned int			frameX;
	unsigned int			frameY;
private:
	void		createErrorMap(void);
	void		createParameterMap(void);
	void		registerBuiltInShaders(void);
	void		registerBuiltInCreators(void);

	bool		reportError(unsigned int line, int errorID, const std::string &info="");
	bool		reportError(unsigned int line, int errorID, const char info);
	void		reportWarning(int errorID, const std::string &ifno="");
	void		reportValidationError(int errorID, const std::string &info="");

	bool		getTokens(TokenSet &tokens, std::string &string);
	bool		getTerm(std::string &result, const std::string &string, unsigned int &pos, unsigned int line=0);
	bool		interpretLine(TokenSet &line, StringVector &identifiers);
	bool		functionCall(TokenSet &line, unsigned int offset, unsigned int argc, StringVector &args);

	static void		stripString(std::string &string);
	static bool		isValidValue(const std::string &value);
public:
	Scene(void);
	Scene(const std::string &filename);
	virtual ~Scene(void);

	std::string	getScriptLog(void) const;
	void		clear(void);

	bool		registerCreator(const std::string &name, NodeCreator *creator);
	bool		registerShader(const std::string &name, Shader *shader);
	int			parseFile(const std::string &filename);
	int			parseFile(std::ifstream &file);
	int			createGraph(Node *root);
	bool		setupEngine(Engine *engine);
	int			validateGraph(Node *root);

	bool	isOpened(void) const
	{ return scriptOpened; }

	int		getErrorCount(void) const
	{ return errorCount; }

	unsigned int	getFrameX(void) const
	{
		if(frameX == 0) return Scene::DefaultX;
		return frameX;
	}
	unsigned int	getFrameY(void) const
	{ 
		if(frameY == 0) return Scene::DefaultY;
		return frameY;
	}

	enum
	{
		MaxLine = 65536,
		MinWH   = 16,
		DefaultX= 640,
		DefaultY= 480,

		// Typy instrukcji.
		Constructor = 0,
		Parameter,
		Assignment,
		Connection,
		Mapping,
		ShaderSet,

		// B³êdy.
		NoError	= 0,
		ParseError,
		ParenthesisExpected,
		DotExpected,
		UnexpectedEOL,
		ConstructorInvalid,
		AssignmentInvalid,
		ConstantInvalid,
		CallInvalid,
		UnknownNodeType,
		UnknownShader,
		UnknownParameter,
		InvalidParameter,
		UndeclaredIdentifier,
		RedeclaredIdentifier,
		ArgumentMismatch,
		UnknownFunction,
		UnknownError,

		// Ostrze¿enia.
		UnknownInstruction,
		UnknownAttribute,
		InvalidValue,
		ShaderSetFailed,
		ConnectionFailed,
		MappingFailed,

		// B³êdy walidacji grafu.
		NoShaderAssigned,
		NoMaterialAssigned,
		TooManyCameras,
	};
};

} // exRay

#endif