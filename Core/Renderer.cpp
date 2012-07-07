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
#include "../Types/Image.h"
#include "Renderer.h"

#include "../Graph/Variable.h"
#include "../Graph/Node.h"
#include "../Graph/Object.h"
#include "../Graph/NodeMaterial.h"
#include "../Graph/NodeLight.h"
#include "../Types/Shader.h"

using namespace exRay;

int Renderer::instanceCount = 0;

Renderer::Renderer(Image *newBuffer, Node *newRoot, unsigned int depth)
{
	frameBuffer			= newBuffer;
	rootNode			= newRoot;

	lastHitNode[0]		= NULL;
	lastHitNode[1]		= NULL;
	lastRenderedNode	= NULL;

	cacheNodes			= NULL;
	cacheSize			= 0;
	cacheLights			= NULL;
	cacheLightsSize		= 0;

	primaryRays			= 0;
	secondaryRays		= 0;

	cacheShader			= new ShaderUniforms*[depth+1];
	cacheLightVisibility= new float*[depth+1];
	for(unsigned int i=0; i<=depth; i++)
	{
		cacheLightVisibility[i] = NULL;
		cacheShader[i]			= new ShaderUniforms;
	}

	params[TraceDepth]	= depth;

	NodeCamera	defaultCamera("", NULL);
	applyCamera(&defaultCamera);

	id = ++instanceCount;
	setDefaultParameters();
}

Renderer::~Renderer(void)
{
	if(cacheShader)
	{
		for(unsigned int i=0; i<(unsigned int)params[TraceDepth]; i++)
			delete cacheShader[i];
		delete[] cacheShader;
	}
	if(cacheNodes)
		delete[] cacheNodes;
	if(cacheLights)
		delete[] cacheLights;
	if(cacheLightVisibility)
	{
		for(unsigned int i=0; i<(unsigned int)params[TraceDepth]; i++)
		{
			if(cacheLightVisibility[i])
				delete[] cacheLightVisibility[i];
		}
		delete[] cacheLightVisibility;
	}
}

void Renderer::setDefaultParameters(void)
{
	params[Supersampling]	=	Full;
	params[ShadowSampling]	=	MonteCarlo;
	params[RenderSamples]	=	1;
	params[ShadowSamples]	=	8;

	paramsf[EnvRIndex]		= 1.0f;
}

Node* Renderer::getRootNode(void) const
{ return rootNode; }

Node* Renderer::getLastHitNode(void) const
{ return lastHitNode[0]; }

Image* Renderer::getFramebuffer(void) const
{ return frameBuffer; }

void Renderer::setFramebuffer(Image *buffer)
{ 
	frameBuffer		= buffer;
	camDelta[0]		= (camPosition[1] - camPosition[0]) / float(frameBuffer->getWidth());
	camDelta[1]		= (camPosition[3] - camPosition[0]) / float(frameBuffer->getHeight());
}

void Renderer::setParameter(const int id, const int value)
{ params[id] = value; }

void Renderer::setParameter(const int id, const float value)
{ paramsf[id] = value; }

float Renderer::getParameterf(const int id) const
{ return paramsf[id]; }

int Renderer::getParameter(const int id) const
{ return params[id]; }

volatile unsigned int Renderer::getPrimaryRays(void) const
{ return primaryRays; }

volatile unsigned int Renderer::getSecondaryRays(void) const
{ return secondaryRays; }

void Renderer::resetRayCounters(void)
{
	primaryRays		= 0;
	secondaryRays	= 0;
}

void Renderer::applyCamera(Node *camNode)
{
	if(!camNode->isObject())
		return;
	matrix4	viewMatrix = ((Object*)camNode)->getTM();

	vector2	screenPlane;
	camNode->getAttrib("origin")->getValue(camOrigin);
	camNode->getAttrib("screen")->getValue(screenPlane);

	camPosition[0]	= vector3(-screenPlane.x*0.5f, -screenPlane.y*0.5f, 0.0f);
	camPosition[1]	= vector3(screenPlane.x*0.5f, -screenPlane.y*0.5f, 0.0f);
	camPosition[2]	= vector3(screenPlane.x*0.5f, screenPlane.y*0.5f, 0.0f);
	camPosition[3]	= vector3(-screenPlane.x*0.5f, screenPlane.y*0.5f, 0.0f);

	camOrigin		= viewMatrix * camOrigin;
	camPosition[0]	= viewMatrix * camPosition[0];
	camPosition[1]	= viewMatrix * camPosition[1];
	camPosition[2]	= viewMatrix * camPosition[2];
	camPosition[3]	= viewMatrix * camPosition[3];

	if(frameBuffer)
	{
		camDelta[0]		= (camPosition[1] - camPosition[0]) / float(frameBuffer->getWidth());
		camDelta[1]		= (camPosition[3] - camPosition[0]) / float(frameBuffer->getHeight());
	}
}

bool Renderer::cacheSceneGraph(void)
{
	if(cacheNodes)
	{
		delete[] cacheNodes;
		cacheNodes = NULL;
		cacheSize  = 0;
	}
	if(cacheLights)
	{
		delete[] cacheLights;
		for(unsigned int i=0; i<=(unsigned int)params[TraceDepth]; i++) if(cacheLightVisibility[i])
			delete[] cacheLightVisibility[i];
		cacheLights		= NULL;
		cacheLightsSize = 0;
	}

	NodeList	*STLNodes	= new NodeList;
	NodeList	*STLLights	= new NodeList;
	NodeList	*STLCamera	= new NodeList;

	rootNode->getObjectsArray(STLNodes);
	rootNode->getNodesArray("light", STLLights);
	rootNode->getNodesArray("camera", STLCamera);

	if(STLCamera->size() > 0)
		applyCamera(STLCamera->at(0));

	if(STLNodes->size() == 0)
	{
		delete STLNodes;
		return false;
	}

	cacheSize		= (unsigned int)STLNodes->size();
	cacheLightsSize	= (unsigned int)STLLights->size();

	cacheNodes = new Node*[cacheSize];
	if(cacheLightsSize > 0)
	{
		cacheLights			= new Node*[cacheLightsSize];
		for(unsigned int i=0; i<=(unsigned int)params[TraceDepth]; i++)
			cacheLightVisibility[i] = new float[cacheLightsSize];
	}

	for(unsigned int i=0; i<cacheSize; i++)
		cacheNodes[i] = STLNodes->at(i);
	for(unsigned int i=0; i<cacheLightsSize; i++)
		cacheLights[i] = STLLights->at(i);

	delete STLNodes;
	delete STLLights;
	delete STLCamera;
	return true;
}

Node* Renderer::renderRay(const vector3 rayStart, vector3 &pixel)
{
	float	rayDistance;
	vector3	rayDirection	= rayStart - camOrigin;
	rayDirection.normalize();

	return raytrace(camOrigin, rayDirection, pixel, rayDistance, paramsf[EnvRIndex], 0); 
}

void Renderer::renderScanline(const unsigned int y)
{
	vector3  rayStart	 = camPosition[0] + float(y) * camDelta[1];
	float	 sampleDelta = 1.0f / float(params[RenderSamples]);
	Node*	 currentNode = NULL;

	vector3	 pixelColor, pixelSample;
	
	for(unsigned int x=0; x<frameBuffer->getWidth(); x++)
	{
		pixelColor  = vector3();
		currentNode = renderRay(rayStart, pixelColor);

		if(params[Supersampling] == Full || currentNode != lastRenderedNode)
		{
			for(int sx=0; sx<params[RenderSamples]; sx++)
			{
				int skipLast = 0;
				if(params[RenderSamples] == sx+1) skipLast = 1;

				for(int sy=0; sy<params[RenderSamples]-skipLast; sy++)
				{
					renderRay(rayStart + camDelta[0]*float(sx)*sampleDelta + camDelta[1]*float(sy)*sampleDelta,
						pixelSample);
					pixelColor += pixelSample;
				}
			}
			lastRenderedNode = currentNode;
			pixelColor      /= float(SQR(params[RenderSamples]));
		}

		rayStart   += camDelta[0];

		if(pixelColor.r > 1.0f) pixelColor.r = 1.0f;
		if(pixelColor.g > 1.0f) pixelColor.g = 1.0f;
		if(pixelColor.b > 1.0f) pixelColor.b = 1.0f;
		frameBuffer->putPixel(x, y, pixelColor);
	}
}

Node* Renderer::findNearestIntersection(vector3 &rayOrigin, vector3 &rayDirection, float &rayDistance,
										int &hitFactor, int &hitFlag, Node *force)
{
	Node	*hitNode	= NULL;
	Node	*testNode	= NULL;
	float	minDistance	= 10000.0f;
	int		testFactor, testFlag;

	for(unsigned int i=0; i<cacheSize; i++)
	{
		if(cacheNodes[i]->ignoreIntersection() && cacheNodes[i] != force)
			continue;
		testNode = cacheNodes[i]->intersect(rayOrigin, rayDirection, minDistance, testFactor, testFlag);
		if(testNode)
		{
			if(minDistance < rayDistance)
			{
				hitNode		= testNode;
				hitFactor	= testFactor;
				hitFlag		= testFlag;
				rayDistance	= minDistance;
			}
		}
	}
	return hitNode;
}

Node* Renderer::findAnyIntersection(vector3 &rayOrigin, vector3 &rayDirection, float &rayDistance,
									int &hitFactor, int &hitFlag, Node *force)
{
	Node	*hitNode	= NULL;
	Node	*testNode	= NULL;
	int		testFactor, testFlag;

	for(unsigned int i=0; i<cacheSize; i++)
	{
		if(cacheNodes[i]->ignoreIntersection() && cacheNodes[i] != force)
			continue;
		testNode = cacheNodes[i]->intersect(rayOrigin, rayDirection, rayDistance, testFactor, testFlag);
		if(testNode)
		{
			hitNode		= testNode;
			hitFactor	= testFactor;
			hitFlag		= testFlag;
			break;
		}
	}
	return hitNode;	
}

float Renderer::getRandomNumber(const float fmax)
{
	return fmax * (float(rand()%1000) / 1000.0f);
}

Node* Renderer::raytrace(vector3 &rayOrigin, vector3 &rayDirection, vector3 &pixel, float &outDistance,
						 const float rindex, unsigned int depth)
{	
	float	rayDistance = 10000.0f;
	Node	*hitNode	= NULL;
	Node	*testNode	= NULL;

	vector3	intPoint;
	int		intFactor, intFlag;
	int		testFactor, testFlag;

	if(depth == 0) primaryRays++;
	else		   secondaryRays++;
	hitNode = findNearestIntersection(rayOrigin, rayDirection, rayDistance, intFactor, intFlag);
	if(hitNode == NULL)
	{
		pixel = vector3(0.0f, 0.0f, 0.0f);
		return NULL;
	}
	
	Object			*hitObject	= (Object*)hitNode;
	Shader			*hitShader	= hitObject->getCachedShader();
	NodeMaterial	*hitMaterial= (NodeMaterial*)hitObject->getCachedMaterial();

	if(hitNode != lastHitNode[0] || hitNode != lastHitNode[1])
	{
		delete cacheShader[depth];
		cacheShader[depth] = hitShader->createRenderingCache();
		if(depth == 0)
			lastHitNode[0] = hitNode;
		lastHitNode[1]	   = hitNode;
	}

	intPoint	= rayOrigin + rayDirection*rayDistance;
	outDistance	= rayDistance;

	cacheShader[depth]->IntPoint		= &intPoint;
	cacheShader[depth]->RayDirection	= &rayDirection;
	cacheShader[depth]->RayOrigin		= &rayOrigin;
	cacheShader[depth]->Normal			= hitObject->getNormal(intPoint, intFlag);

	for(unsigned int i=0; i<cacheLightsSize; i++)
	{
		NodeLight	*light	= (NodeLight*)cacheLights[i];
		int			isamples= params[ShadowSamples];

		if(isamples > 2)
		{ isamples -= depth; if(isamples < 2) isamples = 2; }

		if(light->isAreaLight())
		{
			cacheLightVisibility[depth][i] = 0.0f;

			vector3	lightPos		= light->getCachedPosition();
			vector2	lightArea		= light->getCachedArea();

			vector2	gridDelta(lightArea.x/float(isamples), lightArea.y/float(isamples));
			float	increment		= 1.0f / float(SQR(isamples));

			for(int x=0; x<isamples; x++) for(int y=0; y<isamples; y++)
			{
				vector3	samplePos(lightPos.x + float(x)*gridDelta.x, lightPos.y, lightPos.z + float(y)*gridDelta.y);
				if(params[ShadowSampling] == MonteCarlo)
				{
					samplePos.x += getRandomNumber(gridDelta.x);
					samplePos.z += getRandomNumber(gridDelta.y);
				}

				vector3	lightVec	= samplePos - intPoint;
				rayDistance			= lightVec.length();
				lightVec			/= rayDistance;

				if(depth == 0) primaryRays++;
				else		   secondaryRays++;
				testNode = findNearestIntersection(intPoint + lightVec * MEPSILON, lightVec, rayDistance,
												testFactor, testFlag, cacheLights[i]);
				if(testNode == cacheLights[i])
					cacheLightVisibility[depth][i] += increment;
			}
		}
		else
		{
			cacheLightVisibility[depth][i] = 0.0f;

			vector3	lightVec		= light->getCachedPosition() - intPoint;
			rayDistance				= lightVec.length();
			lightVec			   /= rayDistance;

			if(depth == 0) primaryRays++;
			else		   secondaryRays++;
			testNode = findNearestIntersection(intPoint + lightVec * MEPSILON, lightVec, rayDistance, testFactor, testFlag);
			if(testNode == NULL)
			{
				//if(testFactor == 1)
					cacheLightVisibility[depth][i] = 1.0f;
			}
		}
	}

	if(hitMaterial->isReflective() && depth < (unsigned int)params[TraceDepth])
	{
		vector3	reflectVec = rayDirection - 2.0f * rayDirection.dot(cacheShader[depth]->Normal) * cacheShader[depth]->Normal;
		raytrace(intPoint + reflectVec*MEPSILON, reflectVec, cacheShader[depth]->ReflectComponent, rayDistance, rindex, depth+1);
		cacheShader[depth]->ReflectFlag = true;
	}
	
	if(hitMaterial->getRefractionIndex() > 0.0f && depth < (unsigned int)params[TraceDepth])
	{
		float	rn		= rindex / hitMaterial->getRefractionIndex();
		vector3	rnormal = cacheShader[depth]->Normal * (float)intFactor;

		float	cosI	= rayDirection.dot(rnormal); // dotND
		float	cos2T	= 1.0f - SQR(rn) * (1.0f - SQR(cosI)); // cos2T = 1 - sin2T
		if(cos2T > 0.0f) // Dla sin2T > 1 (cos2T < 0) zachodzi ca³kowite wewnêtrzne odbicie.
		{
			vector3	refractVec = rn * rayDirection - (rn * cosI + sqrtf(cos2T)) * cacheShader[depth]->Normal;
			raytrace(intPoint + refractVec*0.001f, refractVec, cacheShader[depth]->RefractComponent, rayDistance,
				hitMaterial->getRefractionIndex(), depth+1);
			cacheShader[depth]->RefractFlag = true;
			cacheShader[depth]->MaterialAbsorbance = hitMaterial->getDensity() * -rayDistance;
		}
	}

	hitMaterial->updateShader(cacheShader[depth]);
	pixel = hitShader->computeShading(cacheShader[depth], cacheLightVisibility[depth]);
	return hitNode; 
}