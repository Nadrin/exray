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
#include "Renderer.h"
#include "../Graph/Node.h"
#include "../Types/Image.h"

#ifdef WIN32 // WIN32 PLATFORM SPECIFIC
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "Engine.h"

using namespace exRay;

Engine::Engine(Image *outBuffer, const int depth, const int threads)
{
	rootNode		= new Node("root", NULL);
	frameBuffer		= outBuffer;
	traceDepth		= depth;

	if(threads == Engine::Autodetect)
		cpuCores	= getLogicalCPUsCount();
	else cpuCores	= threads;
	if(cpuCores == 0)
		cpuCores = 1;

	threadStatus	= new volatile unsigned int[cpuCores];
	for(unsigned int i=0; i<cpuCores; i++)
	{
		renderThread.push_back(new Renderer(frameBuffer, rootNode, traceDepth));
		renderITC.push_back(ThreadData());
		renderTH.push_back(NULL);
		threadStatus[i] = Engine::ThreadIdle;
	}
	linesRendered = 0;

	if(frameBuffer)
	{
		frameStatus		= new unsigned int[frameBuffer->getHeight()];
		memset(const_cast<unsigned int*>(frameStatus), 0, frameBuffer->getHeight()*sizeof(unsigned int));
	}
}

Engine::~Engine(void)
{
	terminateRenderingThreads();
	for(std::vector<Renderer*>::iterator i=renderThread.begin(); i<renderThread.end(); i++)
		delete (*i);

	delete[]	threadStatus;
	delete[]	frameStatus;
	delete		rootNode;
}

unsigned int Engine::getTraceDepth(void) const
{ return traceDepth; }

unsigned int Engine::getCPUs(void) const
{ return cpuCores; }

Node* Engine::getRootNode(void) const
{ return rootNode; }

Image* Engine::getFramebuffer(void) const
{ return frameBuffer; }

bool Engine::setFramebuffer(Image *buffer)
{ 
	if(!buffer)
		return false;

	if(!frameBuffer)
		frameStatus = new volatile unsigned int[buffer->getHeight()];
	else if(buffer->getHeight() != frameBuffer->getHeight())
	{
		delete[] frameStatus;
		frameStatus = new volatile unsigned int[buffer->getHeight()];
	}
	memset(const_cast<unsigned int*>(frameStatus), 0, buffer->getHeight()*sizeof(unsigned int));
	frameBuffer = buffer;
	for(unsigned int i=0; i<cpuCores; i++)
		renderThread[i]->setFramebuffer(frameBuffer);
	return true;
}

void Engine::setParameter(const int id, const int value)
{
	for(std::vector<Renderer*>::iterator i=renderThread.begin(); i<renderThread.end(); i++)
		(*i)->setParameter(id, value);
}

void Engine::setParameter(const int id, const float value)
{
	for(std::vector<Renderer*>::iterator i=renderThread.begin(); i<renderThread.end(); i++)
		(*i)->setParameter(id, value);
}

int Engine::getParameter(const int id) const
{
	return renderThread.at(0)->getParameter(id);
}

float Engine::getParameterf(const int id) const
{
	return renderThread.at(0)->getParameterf(id);
}

Renderer* Engine::getTracer(const unsigned int id) const
{
	if(id >= renderThread.size())
		return NULL;
	return renderThread.at(id);
}

// AFAIK nie ma eleganckiego sposobu na querowanie informacji o CPU.
// To bêdzie trochê brudnego C. :-) (Oczywiœcie móg³bym u¿yæ __cpuid intrinsic assembly,
// ale to by by³o jeszcze gorsze).
int Engine::getLogicalCPUsCount(void)
{
#ifdef WIN32
#ifdef EXRAY_WINNT6
	_SYSTEM_LOGICAL_PROCESSOR_INFORMATION	*CPUInfo = NULL, *currentCPU = NULL;
	DWORD	bufferSize, byteOffset;
	int		coresFound = 0;

	bufferSize = sizeof(_SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
	byteOffset = 0;
	CPUInfo	   = (_SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(bufferSize);
	if(CPUInfo == NULL)
		return 0;

	if(!GetLogicalProcessorInformation(CPUInfo, &bufferSize) && GetLastError() != ERROR_SUCCESS)
	{
		free(CPUInfo);
		if(GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			return 0;
		
		CPUInfo = (_SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(bufferSize);
		if(CPUInfo == NULL)
			return 0;
	
		if(!GetLogicalProcessorInformation(CPUInfo, &bufferSize))
		{
			free(CPUInfo);
			return 0;
		}
	}

	currentCPU = CPUInfo;
	while(byteOffset < bufferSize)
	{
		if(currentCPU->Relationship == RelationProcessorCore)
			coresFound++;
		byteOffset += sizeof(_SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
		currentCPU++;
	}
	free(CPUInfo);
	return coresFound;
#else
	return 1;
#endif
#else
	return 1;
#endif
}

bool Engine::resetTracers(void)
{
	for(unsigned int i=0; i<renderThread.size(); i++)
	{
		delete renderThread[i];
		renderThread[i] = new Renderer(frameBuffer, rootNode, traceDepth);
		renderITC[i]	= ThreadData();
		renderTH[i]		= NULL;
	}
	if(frameBuffer)
		memset(const_cast<unsigned int*>(frameStatus), 0, frameBuffer->getHeight()*sizeof(unsigned int));
	linesRendered = 0;
	return true;
}

bool Engine::resetThreads(void)
{
	terminateRenderingThreads();
	return createRenderingThreads();
}

unsigned int Engine::getProgressI(void) const
{ return linesRendered; }

float Engine::getProgressF(void) const
{
	return float(linesRendered)/float(frameBuffer->getHeight());
}

bool Engine::createRenderingThreads(void)
{
	for(unsigned int i=0; i<renderThread.size(); i++)
	{
		renderThread[i]->cacheSceneGraph();
		renderITC[i]	= ThreadData(renderThread[i], frameStatus, &threadStatus[i], &linesRendered);
#ifdef WIN32
		renderTH[i]		= CreateThread(NULL, 0, Engine::threadProc, &renderITC[i], CREATE_SUSPENDED, NULL);
#endif
		threadStatus[i] = Engine::ThreadIdle;
		if(renderTH[i] == NULL)
			return false;
	}
	return true;
}

void Engine::resumeRendering(void)
{
	for(std::vector<void*>::iterator i=renderTH.begin(); i<renderTH.end(); i++)
	{ 
		if(*i) 
		{
#ifdef WIN32
			ResumeThread((*i));
#endif
		}
	}
}

void Engine::pauseRendering(void)
{
	for(std::vector<void*>::iterator i=renderTH.begin(); i<renderTH.end(); i++)
	{ 
		if(*i)
		{
#ifdef WIN32
			SuspendThread((*i));
#endif
		}
	}
}

bool Engine::threadsFinished(void) const
{
	for(unsigned int i=0; i<renderThread.size(); i++)
	{
		if(threadStatus[i] != Engine::ThreadFinished)
			return false;
	}
	return true;
}

void Engine::terminateRenderingThreads(void)
{
	for(std::vector<void*>::iterator i=renderTH.begin(); i<renderTH.end(); i++)
	{
		if((*i))
		{
#ifdef WIN32
			TerminateThread((*i), 0);
			CloseHandle((*i));
#endif
			(*i) = NULL;
		}
	}
}

unsigned long __stdcall Engine::threadProc(void *pITC)
{
	ThreadData	*ITC	= (ThreadData*)pITC;
	*ITC->threadStatus	= Engine::ThreadInProgress;

	unsigned int	lockedLine = 0;
	while(lockedLine < ITC->tracer->getFramebuffer()->getHeight())
	{
		// Szukaj pierwszej linii do zablokowania.
		if(ITC->frameStatus[lockedLine] == 0)
			ITC->frameStatus[lockedLine] = ITC->tracer->getID();
		else
		{
			lockedLine++;
			continue;
		}

		// Renderuj zablokowan¹ liniê.
		ITC->tracer->renderScanline(lockedLine);
		(*ITC->linesRendered)++;
		lockedLine++;
	}
	*ITC->threadStatus = Engine::ThreadFinished;
	return 0;
}