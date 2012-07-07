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

#ifndef __IMAGE_H
#define __IMAGE_H

namespace exRay {

/// Klasa bufora klatki.
/** Klasa przechowuj¹ca informacje o wyrenderowanym obrazie. Umo¿liwia zapisywanie/odczytywanie
	pikseli z wczeœniej zaalokowanego bufora klatki obrazu oraz zapisywanie go do pliku w dwóch
	formatach (zale¿nie od podanego rozszerzenia nazwy pliku): Windows Bitmap (.bmp) lub Truevision Targa (.tga).
	Zapisywanie klatki do pliku obs³ugiwane jest tylko dla 24 bitów na piksel.
	Przy operacjach get/put pixel konwersja z formatu wejœciowego (96 bitów na piksel, IEEE floating-point)
	na format wyjœciowy (24 lub 32 bity na piksel, fixed point) wykonywana jest "w locie".
*/
class Image
{
private:
	unsigned char*	imageData;
	unsigned int	width;
	unsigned int	height;
	unsigned int	colorDepth;
private:
	bool			writeBMP(const std::string &filename);
	bool			writeTGA(const std::string &filename);
	bool			getRawLine(const unsigned int y, unsigned char *line);
public:
	Image();
	Image(unsigned int x, unsigned int y, unsigned int bpp);
	virtual ~Image(void);
	void	free(void);

	unsigned char	*getData() const		{ return imageData; }
	unsigned int	getWidth() const		{ return width;		}
	unsigned int	getHeight() const		{ return height;	}
	unsigned int	getColorDepth() const	{ return colorDepth;}

	static int		getFormat(const std::string &filename);
	bool			writeToFile(const std::string &filename);

	bool			putPixel(const int x, const int y, const vector3 &color);
	bool			getPixel(const int x, const int y, vector3 &color);
	bool			putPixel(const int x, const int y, const vector4 &color);
	bool			getPixel(const int x, const int y, vector4 &color);

	enum
	{
		FormatBMP,
		FormatTGA,
	};
};

} // exRay

#endif