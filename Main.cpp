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

#include "Config.h"
#include "Types/Image.h"
#include "Core/Application.h"
#include "Core/Engine.h"
#include "Graph/Node.h"
#include "Types/Scene.h"

#ifdef WIN32 // WIN32 PLATFORM SPECIFIC
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace exRay;

int main(int argc, char *argv[])
{
	Application		theApp;
	if(!theApp.parseArguments(argc, argv))
		return 1;

	theApp.setTitle(std::string("exRay ")+std::string(EXRAY_VERSION_STRING));
	theApp.printStatus("Loading input scene script.");
	Scene *scene = new Scene(theApp.getInput());
	if(scene->getErrorCount() > 0)
	{
		theApp.printError("There were errors. Aborting.");
		theApp.showLog("Parsing Log", scene->getScriptLog());
		delete scene;
		return 1;
	}

	theApp.printStatus("Initializing the engine.");
	Engine	*rayTracer		= new Engine(NULL, theApp.getDepth(), theApp.getCores());
	scene->setupEngine(rayTracer);
	Image	*frameBuffer	= new Image(scene->getFrameX(), scene->getFrameY(), 3);
	rayTracer->setFramebuffer(frameBuffer);

	int	warnings, errors;
	theApp.printStatus("Creating scene graph.");
	warnings = scene->createGraph(rayTracer->getRootNode());
	rayTracer->getRootNode()->clearEvaluated(true);
	rayTracer->getRootNode()->evaluate();
	rayTracer->getRootNode()->updateTransformation();

	errors = scene->validateGraph(rayTracer->getRootNode());
	if(errors > 0)
	{
		theApp.printError("There were errors. Aborting.");
		theApp.showLog("Graph Status Log", scene->getScriptLog());
		delete scene; delete rayTracer; delete frameBuffer;
		return 1;
	}

	rayTracer->getRootNode()->cacheVariables();
	theApp.printStatus("Spawning rendering threads.");
	if(!rayTracer->createRenderingThreads())
	{
		delete scene; delete rayTracer; delete frameBuffer;
		theApp.printError("Failed to initialize rendering threads. Aborting.");
		return 1;
	}
	
	theApp.printStatus("Rendering in progress...");

	if(warnings > 0)
		theApp.showLog("Graph Status Log", scene->getScriptLog());

	printf("\n[ Scene Information ]\n");
	theApp.showSceneInfo(rayTracer);
	printf("\n[ Rendering Status ]\n");
	theApp.showProgress(rayTracer);

#ifdef WIN32
	unsigned int	startTime = GetTickCount();
	char			timeString[256];
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
#endif

	rayTracer->resumeRendering();
	do
	{
#ifdef WIN32
		Sleep(Application::MainThreadIdle);
#else
		_sleep(Application::MainThreadIdle);
#endif
		theApp.showProgress(rayTracer);
	} while(!rayTracer->threadsFinished());

	theApp.showProgress(rayTracer);
	for(unsigned int i=0; i<rayTracer->getCPUs()+1; i++)
		fputc('\n', stdout);

#ifdef WIN32
	theApp.getTime(GetTickCount()-startTime, timeString);
	printf("  Total rendering time: %s.\n\n", timeString);
#endif

	theApp.printStatus("Writing framebuffer to file.");
	if(!frameBuffer->writeToFile(theApp.getOutput()))
	{
		delete scene; delete rayTracer; delete frameBuffer;
		theApp.printError("Operation failed!");
		return 1;
	}
	theApp.printStatus("Rendering completed. Shutting down.");

	delete scene;
	delete rayTracer;
	delete frameBuffer;
	return 0;
}