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
#include "Image.h"

using namespace exRay;

#pragma pack(push, 2)
/// Wewnêtrzna struktura okreœlaj¹ca nag³ówek pliku obrazu BMP.
struct BitmapHeader
{
	short	id;
	long	fileSize;
	short	reserved[2];
	long	offset;

	long	infoSize;
	long	width;
	long	height;
	short	bitPlanes;
	short	bitDepth;
	long	flags[6];
};
#pragma pack(pop)

#pragma pack(push, 1)
/// Wewnêtrzna struktura okreœlaj¹ca nag³ówek pliku obrazu TGA.
struct TargaHeader
{
	char	offset;
	char	cmapType;
	char	imgType;
	char	cmapData[5];

	short	xStart;
	short	yStart;
	short	width;
	short	height;
	char	bitDepth;
	char	flags;
};
#pragma pack(pop)


Image::Image(void)
{
	imageData	= NULL;
	width		= 0;
	height		= 0;
	colorDepth	= 0;
}

Image::Image(unsigned int x, unsigned int y, unsigned int bpp)
{
	imageData	= new unsigned char[x*y*bpp];
	width		= x;
	height		= y;
	colorDepth	= bpp;

	memset(imageData, 0, x*y*bpp);
}

Image::~Image(void)
{
	if(imageData)
		delete[] imageData;
}

void Image::free(void)
{
	if(imageData)
		delete[] imageData;
	imageData	= NULL;
	width		= 0;
	height		= 0;
	colorDepth	= 0;
}

bool Image::getRawLine(const unsigned int y, unsigned char *line)
{
	if(y > height)
		return false;
	memcpy(line, &imageData[y*width*colorDepth], width*colorDepth);
	return true;
}

int Image::getFormat(const std::string &filename)
{
	std::string	 extension;
	char		 *buffer;
	unsigned int dotpos = (unsigned int)filename.find_last_of('.');
	
	if(dotpos == std::string::npos)
		return Image::FormatBMP;
	extension = filename.substr(dotpos);

	buffer = new char[extension.length()+1];
	strcpy(buffer, extension.c_str());
	_strlwr(buffer);
	extension = std::string(buffer);
	delete[] buffer;

	if(extension == ".bmp" )
		return Image::FormatBMP;
	if(extension == ".tga")
		return Image::FormatTGA;
	return Image::FormatBMP;
}

bool Image::writeToFile(const std::string &filename)
{
	if(width == 0 || height == 0)
		return false;
	if(colorDepth != 3) // only 24bpp supported
		return false;

	switch(getFormat(filename))
	{
	case Image::FormatBMP:
		return writeBMP(filename);
	case Image::FormatTGA:
		return writeTGA(filename);
	}
	return false;
}

bool Image::writeBMP(const std::string &filename)
{
	std::ofstream file(filename.c_str(), std::ios_base::out | std::ios_base::binary);
	if(!file.is_open())
		return false;

	BitmapHeader	bmpHeader;
	memset(&bmpHeader, 0, sizeof(BitmapHeader));
	memcpy(&bmpHeader.id, "BM", 2);

	unsigned int imageDataSize = width*height*colorDepth;

	bmpHeader.fileSize	= sizeof(BitmapHeader) + imageDataSize;
	bmpHeader.offset	= sizeof(BitmapHeader);
	bmpHeader.infoSize	= sizeof(BitmapHeader) - 14;
	bmpHeader.width		= width;
	bmpHeader.height	= height;
	bmpHeader.bitPlanes	= 1;
	bmpHeader.bitDepth	= colorDepth*8;

	file.write((char*)&bmpHeader, sizeof(BitmapHeader));

	unsigned char	*line = new unsigned char[width*colorDepth];
	unsigned char	swapValue;
	for(unsigned int y=0; y<height; y++)
	{
		getRawLine(y, line);
		for(unsigned int x=0; x<width; x++)
		{
			swapValue           = line[x*colorDepth];
			line[x*colorDepth]  = line[x*colorDepth+2];
			line[x*colorDepth+2]= swapValue;
		}
		file.write((char*)line, width*colorDepth);
	}
	delete[] line;
	file.close();
	return true;
}

bool Image::writeTGA(const std::string &filename)
{
	std::ofstream file(filename.c_str(), std::ios_base::out | std::ios_base::binary);
	if(!file.is_open())
		return false;

	TargaHeader	tgaHeader;
	memset(&tgaHeader, 0, sizeof(TargaHeader));

	tgaHeader.imgType	= 2; // RGB
	tgaHeader.width		= width;
	tgaHeader.height	= height;
	tgaHeader.bitDepth	= colorDepth*8;

	file.write((char*)&tgaHeader, sizeof(TargaHeader));

	unsigned char	*line = new unsigned char[width*colorDepth];
	unsigned char	swapValue;
	for(unsigned int y=0; y<height; y++)
	{
		getRawLine(y, line);
		for(unsigned int x=0; x<width; x++)
		{
			swapValue           = line[x*colorDepth];
			line[x*colorDepth]  = line[x*colorDepth+2];
			line[x*colorDepth+2]= swapValue;
		}
		file.write((char*)line, width*colorDepth);
	}
	delete[] line;
	file.close();
	return true;
}

bool Image::putPixel(const int x, const int y, const vector3 &color)
{
	if(colorDepth < 3)
		return false;
	unsigned int loc = (y*width+x)*colorDepth;
	imageData[loc]	 = unsigned char(color.r*255.0f);
	imageData[loc+1] = unsigned char(color.g*255.0f);
	imageData[loc+2] = unsigned char(color.b*255.0f);
	return true;
}

bool Image::getPixel(const int x, const int y, vector3 &color)
{
	if(colorDepth < 3)
		return false;
	unsigned int loc = (y*width+x)*colorDepth;
	color.r			 = float(imageData[loc])/255.0f;
	color.g			 = float(imageData[loc+1])/255.0f;
	color.b			 = float(imageData[loc+2])/255.0f;
	return true;
}

bool Image::putPixel(const int x, const int y, const vector4 &color)
{
	if(colorDepth < 4)
		return false;
	unsigned int loc = (y*width+x)*colorDepth;
	imageData[loc]	 = unsigned char(color.r*255.0f);
	imageData[loc+1] = unsigned char(color.g*255.0f);
	imageData[loc+2] = unsigned char(color.b*255.0f);
	imageData[loc+3] = unsigned char(color.a*255.0f);
	return true;
}

bool Image::getPixel(const int x, const int y, vector4 &color)
{
	if(colorDepth < 4)
		return false;
	unsigned int loc = (y*width+x)*colorDepth;
	color.r			 = float(imageData[loc])/255.0f;
	color.g			 = float(imageData[loc+1])/255.0f;
	color.b			 = float(imageData[loc+2])/255.0f;
	color.a			 = float(imageData[loc+3])/255.0f;
	return true;
}