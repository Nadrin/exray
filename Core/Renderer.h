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

#ifndef __RENDERER_H
#define __RENDERER_H

namespace exRay {

class Image;
class Node;
class ShaderUniforms;

/// Definicje parametrów renderera.
enum Parameters
{
	// IDs
	TraceDepth = 0,
	Supersampling,
	ShadowSampling,
	RenderSamples,
	ShadowSamples,
	// Reserved
	FrameWidth	= 254,
	FrameHeight	= 255,

	EnvRIndex = 0,

	// Values
	Full = 0,
	Adaptive,
	RegularGrid,
	MonteCarlo,

	// Param-array size
	ParamsCount		= 5,
	FParamsCount	= 1,
};

/// Klasa renderera (raytracera).
/** Podstawowy element silnika. Raytracer wykonuj¹cy proces rekurencyjnego, wstecznego
	œledzenia promieni i syntezy obrazu.
	Klasa renderuje pojedyncze linie obrazu piksel po pikselu. Znajduje przeciêcia z prymitywami na scenie
	oraz zarz¹dza cachem renderowania. W razie potrzeby generuje promienie wtórne, wylicza
	odbicie, refrakcjê oraz aproksymuje cienie. W ostatnim etapie renderowania przekazuje
	informacje do aktualnego shadera, który wylicza ostateczny kolor piksela.
*/
class Renderer
{
private:
	static int				instanceCount;
	int						id;
	volatile unsigned int	primaryRays;
	volatile unsigned int	secondaryRays;

	// Parameters
	int					params[ParamsCount];
	float				paramsf[FParamsCount];

	Image*				frameBuffer;
	Node*				rootNode;
	Node*				lastHitNode[2];
	Node*				lastRenderedNode;

	Node**				cacheNodes;
	unsigned int		cacheSize;

	Node**				cacheLights;
	unsigned int		cacheLightsSize;
	float**				cacheLightVisibility;

	ShaderUniforms**	cacheShader;

	vector3				camDelta[2];
	vector3				camPosition[4];
	vector3				camOrigin;
private:
	Node*			findNearestIntersection(vector3 &rayOrigin, vector3 &rayDirection, float &rayDistance,
											int &hitFactor, int &hitFlag, Node *force=NULL);
	Node*			findAnyIntersection(vector3 &rayOrigin, vector3 &rayDirection, float &rayDistance,
										int &hitFactor, int &hitFlag, Node *force=NULL);
	static float	getRandomNumber(const float fmax);
	void			setDefaultParameters(void);
	Node*			renderRay(const vector3 rayStart, vector3 &pixel);
	void			applyCamera(Node *camNode);
public:
	Renderer(Image *newBuffer, Node *newRoot, unsigned int depth);
	~Renderer(void);

	bool	cacheSceneGraph(void);
	void	renderScanline(const unsigned int y);
	Node*	raytrace(vector3 &rayOrigin, vector3 &rayDirection, vector3 &pixel, float &outDistance, 
					 const float rindex, unsigned int depth);

	Node*	getRootNode(void) const;
	Image*	getFramebuffer(void) const;
	void	setFramebuffer(Image *buffer);

	Node*	getLastHitNode(void) const;

	void	setParameter(const int id, const int value);
	void	setParameter(const int id, const float value);
	int		getParameter(const int id) const;
	float	getParameterf(const int id) const;

	volatile unsigned int	getPrimaryRays(void) const;
	volatile unsigned int	getSecondaryRays(void) const;
	void					resetRayCounters(void);

	int		getID(void) const
	{ return id; }
};

} // exRay

#endif