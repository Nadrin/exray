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

#ifndef __APPLICATION_H
#define __APPLICATION_H

namespace exRay {

typedef std::map<int, std::string>	ErrorMap;

class Engine;

/// Klasa aplikacji konsolowej.
/** G³owna i jedyna klasa warstwy prezentacji. Odpowiedzialna jest za interakcjê z u¿ytkownikiem
	(przetwarzanie argumentów programu), zbieranie danych z silnika oraz wyœwietlanie komunikatów 
	w oknie konsoli. 
*/
class Application
{
private:	
	ErrorMap		errorMap;
	unsigned int	argCores;
	unsigned int	argDepth;
	std::string		argInput;
	std::string		argOutput;
private:
	static bool	isArgument(const char *argv, const char *shortForm, const char *longForm);
	static bool	isValue(const char *string);
	bool		reportError(const int errorID, const char *info="");
public:
	Application(void);
	~Application(void);

	bool		parseArguments(int argc, char **argv);

	static void	printHelp(void);
	static void	printAbout(void);

	static void	showLog(const std::string &title, std::string &slog);
	static void printStatus(const std::string &string);
	static void printError(const std::string &string);
	static void getTime(const unsigned int timestamp, char *buffer);
	
	void		showProgress(Engine *engine) const;
	void		showSceneInfo(Engine *engine) const;

	static void gotoxy(const int x, const int y);
	static void getxy(int &x, int &y);
	static void setTitle(const std::string &string);

	unsigned int	getCores(void) const	{ return argCores; }
	unsigned int	getDepth(void) const	{ return argDepth; }
	std::string		getInput(void) const	{ return argInput; }
	std::string		getOutput(void) const	{ return argOutput; }

	enum
	{
		WrongArgCount,
		UnknownOption,
		InvalidValue,
		DupeOption,
		InputNotFound,
		OutputNotKnown,
		TooManyCores,

		ProgressBarWidth = 60,
		MainThreadIdle   = 200,
	};
};

} // exRay

#endif