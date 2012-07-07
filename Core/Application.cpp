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
#include "Application.h"
#include "Engine.h"
#include "Renderer.h"
#include "../Types/Image.h"

#ifdef WIN32 // WIN32 PLATFORM SPECIFIC
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace exRay;

Application::Application(void)
{
	printAbout();

	argCores	= 0; // Engine::Autodetect
	argDepth	= 8;

	errorMap[Application::WrongArgCount] = "Wrong number of arguments. Use '-h' for help";
	errorMap[Application::UnknownOption] = "Unknown option";
	errorMap[Application::InvalidValue]	 = "Invalid value";
	errorMap[Application::DupeOption]	 = "Duplcated option";
	errorMap[Application::InputNotFound] = "Input file does not exist";
	errorMap[Application::OutputNotKnown]= "No output file specified";
	errorMap[Application::TooManyCores]	 = "Maximum allowed logical CPUs number is 8";
}

Application::~Application(void)
{
}

void Application::printHelp(void)
{
	printf("Usage: exray [options] -o <output> <input>\n\n");
	printf("Possible command line switches are:\n");
	
	printf(" --help  -h\tPrints this message. Must be the first option.\n");

	printf(" --cores -c\tSpecifies number of logical CPUs (rendering threads)\n");
	printf("\t\tto use. If not specified the engine will try to autodetect\n\t\tand match the number of CPU cores.\n");

	printf(" --depth -d\tSpecifies the recursive's raytracer trace depth value.\n");
	printf("\t\tIf not specified the default value of 8 will be used.\n");

	printf(" --output -o\tSpecifiers the output image filename. Supported output formats\n");
	printf("\t\tare BMP and TGA and are determined by passed file extension.\n");
	printf("\t\tIf an unknown extension is passed the engine uses BMP\n\t\tby default.\n");

	printf(" <input>\tThe input scene configuration script to be rendered.\n");
}

void Application::printAbout(void)
{
	printf("EX-RAY Raytracing Engine. Version %s\n", EXRAY_VERSION_STRING);
	printf("(C) Copyright 2007-2008 Michal Siejak.\n\n");
}

bool Application::isArgument(const char *argv, const char *shortForm, const char *longForm)
{
	return (strcmp(argv, shortForm)==0 || strcmp(argv, longForm)==0);
}

bool Application::isValue(const char *string)
{
	int	test;
	return sscanf(string, "%i", &test) > 0;
}

bool Application::reportError(const int errorID, const char *info)
{
	printf("%s", errorMap[errorID].c_str());
	if(strlen(info) > 0)
		printf(": %s", info);
	else printf(".");

	return false;
}

bool Application::parseArguments(int argc, char **argv)
{
	if(argc < 2)
		return reportError(Application::WrongArgCount);

	if(isArgument(argv[1], "-h", "--help"))
	{
		printHelp();
		return false;
	}
	if(argc < 4 || argc > 8)
		return reportError(Application::WrongArgCount);
	if(argc % 2 != 0)
		return reportError(Application::WrongArgCount);

	bool	cores=false, depth=false, output=false;
	for(int i=1; i<argc-1; i+=2)
	{
		if(isArgument(argv[i], "-c", "--cores"))
		{
			if(cores)
				return reportError(Application::DupeOption, argv[i]);
			if(!isValue(argv[i+1]))
				return reportError(Application::InvalidValue, argv[i+1]);
			sscanf(argv[i+1], "%i", &argCores);
			if(argCores <= 0)
				return reportError(Application::InvalidValue, argv[i+1]);
			if(argCores > 8)
				return reportError(Application::TooManyCores);
			cores = true;
		}
		else if(isArgument(argv[i], "-d", "--depth"))
		{
			if(depth)
				return reportError(Application::DupeOption, argv[i]);
			if(!isValue(argv[i+1]))
				return reportError(Application::InvalidValue, argv[i+1]);
			sscanf(argv[i+1], "%i", &argDepth);
			if(argDepth <= 0)
				return reportError(Application::InvalidValue, argv[i+1]);
			depth = true;
		}
		else if(isArgument(argv[i], "-o", "--output"))
		{
			if(output)
				return reportError(Application::DupeOption, argv[i]);
			argOutput = std::string(argv[i+1]);
			output    = true;
		}
		else return reportError(Application::UnknownOption, argv[i]);
	}

	std::ifstream	testFile(argv[argc-1]);
	if(!testFile.is_open())
		return reportError(Application::InputNotFound, argv[argc-1]);
	testFile.close();

	if(!output)
		return reportError(Application::OutputNotKnown);

	argInput = std::string(argv[argc-1]);
	return true;
}

void Application::showLog(const std::string &title, std::string &slog)
{
	printf("\n[ %s ]\n", title.c_str());
	printf("%s", slog.c_str());
}

void Application::printStatus(const std::string &string)
{
	printf("-> %s\n", string.c_str());
}

void Application::printError(const std::string &string)
{
	printf("!> %s\n", string.c_str());
}

void Application::gotoxy(const int x, const int y)
{
#ifdef WIN32
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
#endif
}

void Application::getxy(int &x, int &y)
{
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO	consoleInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
	x = consoleInfo.dwCursorPosition.X;
	y = consoleInfo.dwCursorPosition.Y;
#endif
}

void Application::setTitle(const std::string &string)
{
	size_t	converted = 0;
	wchar_t	unistring[256];

	mbstowcs_s(&converted, unistring, string.length()+1, string.c_str(), _TRUNCATE);

#ifdef WIN32
	SetConsoleTitle(unistring);
#endif
}

void Application::getTime(const unsigned int timestamp, char *buffer)
{
	unsigned int	seconds = timestamp/1000;
	unsigned int	minutes = seconds/60;
	unsigned int	hours	= minutes/60;
	seconds	= seconds % 60;
	minutes = minutes % 60;

	if(hours > 0)
		sprintf(buffer, "%02u:", hours);
	sprintf(buffer, "%02u:%02u", minutes, seconds);
}

void Application::showSceneInfo(Engine *engine) const
{
	std::string	superSampling, shadowSampling;
	int			samples[2];

	if(engine->getParameter(exRay::Supersampling) == exRay::Full)
		superSampling	= "Full";
	else superSampling	= "Adaptive";
	if(engine->getParameter(exRay::ShadowSampling) == exRay::RegularGrid)
		shadowSampling	= "Regular Grid";
	else shadowSampling = "Monte Carlo";

	samples[0] = engine->getParameter(exRay::RenderSamples);
	samples[1] = engine->getParameter(exRay::ShadowSamples);

	printf("  Input script : \"%s\"\n", argInput.c_str());
	printf("  Output image : \"%s\"\n", argOutput.c_str());
	printf("  Logical CPUs : %u assigned.\n\n", engine->getCPUs());

	printf("  Frame Width  : %u\t\t", engine->getFramebuffer()->getWidth());
	printf("  Supersampling  : %s (%ux%u)\n", superSampling.c_str(), samples[0], samples[0]);

	printf("  Frame Height : %u\t\t", engine->getFramebuffer()->getHeight());
	printf("  Shadow Approx. : %s (%ux%u)\n", shadowSampling.c_str(), samples[1], samples[1]);
}

void Application::showProgress(Engine *engine) const
{
	int						cursor[2];
	volatile unsigned int	tracerStats[2];
	int	fullFactor = int(engine->getProgressF() * Application::ProgressBarWidth);

	for(unsigned int i=0; i<engine->getCPUs(); i++)
	{
		tracerStats[0] = engine->getTracer(i)->getPrimaryRays() * (1000 / MainThreadIdle);
		tracerStats[1] = engine->getTracer(i)->getSecondaryRays() * (1000 / MainThreadIdle);

		printf("\r  CPU%u:    Primary : %8u rays/sec. ", i, tracerStats[0]);
		printf("Secondary+ : %8u rays/sec.\n", tracerStats[1]);
		engine->getTracer(i)->resetRayCounters();
	}

	printf("  [%3.0f %%] [", engine->getProgressF()*100.0f);
	for(int i=0; i<fullFactor; i++) putc('#', stdout);
	for(int i=0; i<Application::ProgressBarWidth-fullFactor; i++) putc('.', stdout);
	printf("]\r");
	
	getxy(cursor[0], cursor[1]);
	gotoxy(0, cursor[1]-engine->getCPUs());
}