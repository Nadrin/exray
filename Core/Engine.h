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

#ifndef __ENGINE_H
#define __ENGINE_H

/// G³ówna przestrzeñ nazw. Do niej nale¿¹ wszystkie klasy silnika, klasy matematyczne, oraz klasa Application.
namespace exRay {

class Renderer;
class Image;
class Node;

/// Klasa - containter. Bierze udzia³ w komunikacji miêdzy w¹tkami.
/** Podstawowy "pakiet" komunikacyjny ITC. Ka¿demu w¹tkowi renderuj¹cemu
	przekazywany jest adres do instancji tej klasy.
*/
class ThreadData
{
public:
	Renderer				*tracer;
	volatile unsigned int	*frameStatus;
	volatile unsigned int	*threadStatus;
	volatile unsigned int	*linesRendered;
public:
	ThreadData(void)
	{ tracer = NULL; frameStatus = NULL; threadStatus = NULL; linesRendered = NULL; }
	ThreadData(Renderer *newTracer, volatile unsigned int *newFrameStatus, 
		volatile unsigned int *newThreadStatus, volatile unsigned int *newLinesRendered)
	{ 
		tracer			= newTracer;
		frameStatus		= newFrameStatus;
		threadStatus	= newThreadStatus;
		linesRendered	= newLinesRendered;
	}
	virtual ~ThreadData(void)
	{ }
};

/// G³ówna klasa, "kernel" silnika.
/**	Zadaniami tej klasy jest inicjalizacja procesu renderowania, tworzenie/niszczenie oraz zarz¹dzanie
	wieloma renderererami, a tak¿e kontrolowanie asynchronicznych w¹tków renderuj¹cych oraz zapewnianie
	komunikacji miêdzy nimi (interthread communication). Udostêpnia ona równie¿ interfejs pozwalaj¹cy
	pobieraæ i ustawiaæ parametry rendererów. Przechowuje aktualny bufor klatki oraz graf sceny.
	Na systemach Windows XP SP3 oraz Windows Vista pozwala tak¿e wykryæ iloœæ rdzeni procesora.
*/
class Engine
{
private:
	unsigned int			cpuCores;
	unsigned int			traceDepth;
	std::vector<Renderer*>	renderThread;
	std::vector<ThreadData>	renderITC;	// Inter-Thread Communication
	std::vector<void*>		renderTH;

	Image*					frameBuffer;
	Node*					rootNode;

	volatile unsigned int*	frameStatus;
	volatile unsigned int*	threadStatus;
	volatile unsigned int	linesRendered;
private:
	static int						getLogicalCPUsCount(void);
	static unsigned long __stdcall	threadProc(void *pITC);
public:
	Engine(Image *outBuffer, const int depth, const int threads);
	virtual ~Engine(void);

	unsigned int	getCPUs(void) const;
	unsigned int	getTraceDepth(void) const;
	Node*			getRootNode(void) const;
	Image*			getFramebuffer(void) const;
	Renderer*		getTracer(const unsigned int id) const;

	bool			setFramebuffer(Image *buffer);
	bool			resetTracers(void);
	bool			resetThreads(void);

	bool			createRenderingThreads(void);
	void			terminateRenderingThreads(void);
	void			pauseRendering(void);
	void			resumeRendering(void);

	bool			threadsFinished(void) const;

	unsigned int	getProgressI(void) const;
	float			getProgressF(void) const;

	void			setParameter(const int id, const int value);
	void			setParameter(const int id, const float value);
	int				getParameter(const int id) const;
	float			getParameterf(const int id) const;

	enum
	{
		Autodetect = 0,

		ThreadIdle,
		ThreadInProgress,
		ThreadFinished,
	};
};

} // exRay

#endif