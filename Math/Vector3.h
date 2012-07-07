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

#ifndef __VECTOR3_H
#define __VECTOR3_H

#define	MATH_VECTOR3

namespace exRay {

class vector2;
class vector4;

/// Wektor przestrzeni trójwymiarowej.
class vector3
{
public:
	// Konstruktory.
	vector3(void)							{ x=0.0f;	y=0.0f;		z=0.0f;		}
	vector3(float newVal)					{ x=newVal;	y=newVal;	z=newVal;	}
	vector3(float nX, float nY)				{ x=nX;		y=nY;		z=0.0f;		}
	vector3(const vector2 &vec)				{ x=vec.x;	y=vec.y;	z=0.0f;		}
	vector3(float nX, float nY, float nZ)	{ x=nX;		y=nY;		z=nZ;		}
	vector3(const vector2 &vec, float nZ)	{ x=vec.x;	y=vec.y;	z=nZ;		}

	// Destruktor.
	virtual ~vector3(void)		{ }

	// Operatory:
	// Przypisanie (cast to float)
	operator float* () const		{ return (float*)&cell;			}
	operator const float* () const	{ return (const float*)&cell;	}

	// Dodawanie.
	vector3 operator + (const vector3 &vec) const
	{ return vector3(x+vec.x, y+vec.y, z+vec.z);		}
	// Odejmowanie.
	vector3 operator - (const vector3 &vec) const
	{ return vector3(x-vec.x, y-vec.y, z-vec.z);		}
	// Mno¿enie.
	vector3 operator * (const vector3 &vec) const
	{ return vector3(x*vec.x, y*vec.y, z*vec.z);		}
	// Dzielenie.
	vector3 operator / (const vector3 &vec) const
	{	
		if(vec.x == 0.0f || vec.y == 0.0f || vec.z == 0.0f) return vector3();
		return vector3(x/vec.x, y/vec.y, z/vec.z);
	}

	void operator += (const vector3 &vec)
	{ x+=vec.x; y+=vec.y; z+=vec.z; }
	void operator += (const float &value)
	{ x+=value; y+=value; z+=value; }

	void operator -= (const vector3 &vec)
	{ x-=vec.x; y-=vec.y; z-=vec.z; }
	void operator -= (const float &value)
	{ x-=value; y-=value; z-=value; }

	void operator *= (const vector3 &vec)
	{ x*=vec.x; y*=vec.y; z*=vec.z; }
	void operator *= (const float &value)
	{ x*=value; y*=value; z*=value; }

	void operator /= (const vector3 &vec)
	{
		if(vec.x == 0.0f || vec.y == 0.0f || vec.z == 0.0f)
			return;
		x/=vec.x; y/=vec.y; z/=vec.z;
	}
	void operator /= (const float &value)
	{
		if(value == 0.0f)
			return;
		x/=value; y/=value; z/=value;
	}

	// Dodawanie liczby float.
	friend vector3 operator + (float value, const vector3 &vec)
	{ return vector3(vec.x+value, vec.y+value, vec.z+value);		}
	// Odejmowanie liczby float.
	friend vector3 operator - (float value, const vector3 &vec)
	{ return vector3(vec.y-value, vec.y-value, vec.z-value);		}
	// Mno¿enie przez liczbê float.
	friend vector3 operator * (float value, const vector3 &vec)
	{ return vector3(vec.x*value, vec.y*value, vec.z*value);		}
	// Dzielenie przez liczbê float.
	friend vector3 operator / (float value, const vector3 &vec)
	{
		if(value == 0.0f) return vector3();
		return vector3(vec.x/value, vec.y/value, vec.z/value);
	}

	vector3 operator + (void) const
	{ return *this; }
	vector3 operator - (void) const
	{ return vector3(-x, -y, -z); }

	// Porównanie dwóch wektorów.
	bool operator == (const vector3 &vec) const
	{
		return	(((x-MCMP_BIAS)<=vec.x) && ((x+MCMP_BIAS)>= vec.x)) &&
				(((y-MCMP_BIAS)<=vec.y) && ((y+MCMP_BIAS)>= vec.y)) &&
				(((z-MCMP_BIAS)<=vec.z) && ((z+MCMP_BIAS)>= vec.z));
	}
	bool operator != (const vector3 &vec) const
	{ return !(*this == vec);	}


	// £aduje wektor zerami.
	void	loadZero()
	{ x=0.0f; y=0.0f; z=0.0f; }
	// £aduje wektor jedynkami.
	void	loadOne()
	{ x=1.0f; y=1.0f; z=1.0f; }

	// Zwraca d³ugoœc wektora.
	float	length() const
	{ return sqrtf(SQR(x)+SQR(y)+SQR(z)); }
	// Zwraca kwadrat d³ugoœci wektora.
	float	sqLength() const
	{ return (SQR(x) + SQR(y) + SQR(z));	}
	// Zwraca odwrotnoœæ d³ugoœci wektora.
	float	invLength() const
	{ return INV(length());					}

	// Normalizuje wektor.
	void	normalize()
	{
		float	len = length();
		if(len==0.0f || len==1.0f)	return;
		x /= len;
		y /= len;
		z /= len;
	}
	// Zwraca wektor znormalizowany.
	vector3	getNormalized() const
	{
		vector3	result = *this;
		result.normalize();
		return result;
	}

	// Iloczyn skalarny.
	float	dot(const vector3 &vec) const
	{ return (x*vec.x+y*vec.y+z*vec.z); }

	// Iloczyn wektorowy.
	vector3	cross(const vector3 &vec) const
	{ return vector3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x); }

	// Liniowa interpolacja.
	vector3	lerp(const vector3 &vec, const float factor) const
	{ return (*this)*(1.0f-factor) + vec*factor; }

	// IO
	bool read(std::ifstream &file)
	{
		if(!file.is_open())
			return false;
		file.read((char*)&cell, sizeof(cell));
		return true;
	}
	bool write(std::ofstream &file)
	{
		if(!file.is_open())
			return false;
		file.write((char*)&cell, sizeof(cell));
		return true;
	}

	union
	{
		struct	{ float x; float y; float z; };
		struct	{ float r; float g; float b; };
		float	cell[3];
	};
};

} // exRay

#endif