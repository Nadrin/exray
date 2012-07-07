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

#ifndef __VECTOR4_H
#define __VECTOR4_H

#define	MATH_VECTOR4

namespace exRay {

class vector2;
class vector3;

/// Wektor przestrzeni czterowymiarowej.
class vector4
{
public:
	// Konstruktory.
	vector4(void)									{ x=0.0f;	y=0.0f;		z=0.0f;		w=0.0f;		}
	vector4(float newVal)							{ x=newVal;	y=newVal;	z=newVal;	w=newVal;	}
	vector4(float nX, float nY)						{ x=nX;		y=nY;		z=0.0f;		w=0.0f;		}
	vector4(const vector2 &vec)					 	{ x=vec.x;	y=vec.y;	z=0.0f;		w=0.0f;		}
	vector4(float nX, float nY, float nZ)			{ x=nX;		y=nY;		z=nZ;		w=0.0f;		}
	vector4(const vector2 &vec, float nZ)			{ x=vec.x;	y=vec.y;	z=nZ;		w=0.0f;		}
	vector4(const vector3 &vec)						{ x=vec.x;	y=vec.y;	z=vec.z;	w=0.0f;		}
	vector4(float nX, float nY, float nZ, float nW)	{ x=nX;		y=nY;		z=nZ;		w=nW;		}
	vector4(const vector3 &vec, float nW)			{ x=vec.x;	y=vec.y;	z=vec.z;	w=nW;		}
	vector4(const vector2 &v1, const vector2 &v2)	{ x=v1.x;	y=v1.y;		z=v2.x;		w=v2.y;		}

	// Destruktor.
	virtual ~vector4(void)		{ }

	// Operatory:
	// Przypisanie (cast to float)
	operator float* () const		{ return (float*)&cell;			}
	operator const float* () const	{ return (const float*)&cell;	}

	// Dodawanie.
	vector4 operator + (const vector4 &vec) const
	{ return vector4(x+vec.x, y+vec.y, z+vec.z, w+vec.w);		}
	// Odejmowanie.
	vector4 operator - (const vector4 &vec) const
	{ return vector4(x-vec.x, y-vec.y, y-vec.z, w-vec.w);		}
	// Mno¿enie.
	vector4 operator * (const vector4 &vec) const
	{ return vector4(x*vec.x, y*vec.y, z*vec.z, w*vec.w);		}
	// Dzielenie.
	vector4 operator / (const vector4 &vec) const
	{	
		if(vec.x == 0.0f || vec.y == 0.0f || vec.z == 0.0f || vec.w == 0.0f) return vector4();
		return vector4(x/vec.x, y/vec.y, z/vec.z, w/vec.w);
	}

	void operator += (const vector4 &vec)
	{ x+=vec.x; y+=vec.y; z+=vec.z; w+=vec.w; }
	void operator += (const float &value)
	{ x+=value; y+=value; z+=value; w+=value; }

	void operator -= (const vector4 &vec)
	{ x-=vec.x; y-=vec.y; z-=vec.z; w-=vec.w; }
	void operator -= (const float &value)
	{ x-=value; y-=value; z-=value; w-=value; }

	void operator *= (const vector4 &vec)
	{ x*=vec.x; y*=vec.y; z*=vec.z; w*=vec.w; }
	void operator *= (const float &value)
	{ x*=value; y*=value; z*=value; w*=value; }

	void operator /= (const vector4 &vec)
	{
		if(vec.x == 0.0f || vec.y == 0.0f || vec.z == 0.0f || vec.w == 0.0f)
			return;
		x/=vec.x; y/=vec.y; z/=vec.z; w/=vec.w;
	}
	void operator /= (const float &value)
	{
		if(value == 0.0f)
			return;
		x/=value; y/=value; z/=value; w/=value;
	}

	// Dodawanie liczby float.
	friend vector4 operator + (float value, const vector4 &vec)
	{ return vector4(vec.x+value, vec.y+value, vec.z+value, vec.w+value);	}
	// Odejmowanie liczby float.
	friend vector4 operator - (float value, const vector4 &vec)
	{ return vector4(vec.y-value, vec.y-value, vec.z-value, vec.w-value);	}
	// Mno¿enie liczby float.
	friend vector4 operator * (float value, const vector4 &vec)
	{ return vector4(vec.x*value, vec.y*value, vec.z*value, vec.w*value);	}
	// Dzielenie liczby float.
	friend vector4 operator / (float value, const vector4 &vec)
	{
		if(value == 0.0f) return vector4();
		return vector4(vec.x/value, vec.y/value, vec.z/value, vec.w/value);
	}

	vector4 operator + (void) const
	{ return *this; }
	vector4 operator - (void) const
	{ return vector4(-x, -y, -z, -w); }

	// Porównanie dwóch wektorów.
	bool operator == (const vector4 &vec) const
	{
		return	(((x-MCMP_BIAS)<=vec.x) && ((x+MCMP_BIAS)>= vec.x)) &&
				(((y-MCMP_BIAS)<=vec.y) && ((y+MCMP_BIAS)>= vec.y)) &&
				(((z-MCMP_BIAS)<=vec.z) && ((z+MCMP_BIAS)>= vec.z)) &&
				(((w-MCMP_BIAS)<=vec.w) && ((w+MCMP_BIAS)>= vec.w));
	}
	bool operator != (const vector4 &vec) const
	{ return !(*this == vec);	}


	// £aduje wektor zerami.
	void	loadZero()
	{ x=0.0f; y=0.0f; z=0.0f; w=0.0f; }
	// £aduje wektor jedynkami.
	void	loadOne()
	{ x=1.0f; y=1.0f; z=1.0f; w=1.0f; }

	// Zwraca d³ugoœæ wektora.
	float	length() const
	{ return sqrtf(SQR(x)+SQR(y)+SQR(z)+SQR(w)); }
	// Zwraca kwadrat d³ugoœci wektora.
	float	sqLength() const
	{ return (SQR(x)+SQR(y)+SQR(z)+SQR(w));		}
	// Zwraca odwrotnoœæ d³ugoœci wektora.
	float	invLength() const
	{ return INV(length());						}

	// Normalizuje wektor.
	void	normalize()
	{
		float	len = length();
		if(len==0.0f || len==1.0f)	return;
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}
	// Zwraca wektor znormalizowany.
	vector4	getNormalized() const
	{
		vector4	result = *this;
		result.normalize();
		return result;
	}

	// Iloczyn skalarny.
	float	dot3(const vector3 &vec) const
	{ return (x*vec.x+y*vec.y+z*vec.z);			}
	float	dot3(const vector4 &vec) const
	{ return (x*vec.x+y*vec.y+z*vec.z);			}
	float	dot(const vector4 &vec) const
	{ return (x*vec.x+y*vec.y+z*vec.z+w*vec.w); }

	// Iloczyn wektorowy (tylko w R3).
	vector4	cross(const vector3 &vec) const
	{ return vector4(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - vec.y*x); }
	vector4	cross(const vector4 &vec) const
	{ return vector4(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - vec.y*x); }

	// Liniowa interpolacja.
	vector4	lerp(const vector4 &vec, const float factor) const
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
		struct	{ float x; float y; float z; float w; };
		struct	{ float r; float g; float b; float a; };
		float	cell[4];
	};
};

} // exRay

#endif