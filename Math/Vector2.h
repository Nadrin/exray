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

#ifndef __VECTOR2_H
#define __VECTOR2_H

#define	MATH_VECTOR2

namespace exRay {

class vector3;
class vector4;

/// Wektor przestrzeni dwuwymiarowej.
class vector2
{
public:
	// Konstruktory.
	vector2(void)				{ x=0.0f;	y=0.0f;		}
	vector2(float newVal)		{ x=newVal;	y=newVal;	}
	vector2(float nX, float nY)	{ x=nX;		y=nY;		}

	// Destruktor.
	virtual ~vector2(void)		{ }

	// Operatory:
	// Przypisanie (cast to float)
	operator float* () const		{ return (float*)&cell;			}
	operator const float* () const	{ return (const float*)&cell;	}

	// Dodawanie.
	vector2 operator + (const vector2 &vec) const
	{ return vector2(x+vec.x, y+vec.y);		}
	// Odejmowanie.
	vector2 operator - (const vector2 &vec) const
	{ return vector2(x-vec.x, y-vec.y);		}
	// Mno¿enie.
	vector2 operator * (const vector2 &vec) const
	{ return vector2(x*vec.x, y*vec.y);		}
	// Dzielenie.
	vector2 operator / (const vector2 &vec) const
	{	
		if(vec.x == 0.0f || vec.y == 0.0f) return vector2();
		return vector2(x/vec.x, y/vec.y);
	}

	void operator += (const vector2 &vec)
	{ x+=vec.x; y+=vec.y; }
	void operator += (const float &value)
	{ x+=value; y+=value; }

	void operator -= (const vector2 &vec)
	{ x-=vec.x; y-=vec.y; }
	void operator -= (const float &value)
	{ x-=value; y-=value; }

	void operator *= (const vector2 &vec)
	{ x*=vec.x; y*=vec.y; }
	void operator *= (const float &value)
	{ x*=value; y*=value; }

	void operator /= (const vector2 &vec)
	{
		if(vec.x == 0.0f || vec.y == 0.0f)
			return;
		x/=vec.x; y/=vec.y;
	}
	void operator /= (const float &value)
	{
		if(value == 0.0f)
			return;
		x/=value; y/=value;
	}

	// Dodawanie liczby float.
	friend vector2 operator + (float value, const vector2 &vec)
	{ return vector2(vec.x+value, vec.y+value);		}
	// Odejmowanie liczby float.
	friend vector2 operator - (float value, const vector2 &vec)
	{ return vector2(vec.y-value, vec.y-value);		}
	// Mno¿enie przez liczbê float.
	friend vector2 operator * (float value, const vector2 &vec)
	{ return vector2(vec.x*value, vec.y*value);		}
	// Dzielenie przez liczbê float.
	friend vector2 operator / (float value, const vector2 &vec)
	{
		if(value == 0.0f) return vector2();
		return vector2(vec.x/value, vec.y/value);
	}

	vector2 operator + (void) const
	{ return *this; }

	vector2 operator - (void) const
	{ return vector2(-x, -y); }

	// Porównywanie dwóch wektorów.
	bool operator == (const vector2 &vec) const
	{
		return	(((x-MCMP_BIAS)<=vec.x) && ((x+MCMP_BIAS)>= vec.x)) &&
				(((y-MCMP_BIAS)<=vec.y) && ((y+MCMP_BIAS)>= vec.y));
	}
	bool operator != (const vector2 &vec) const
	{ return !(*this == vec);	}


	// £aduje wektor zerami.
	void	loadZero()
	{ x=0.0f; y=0.0f; }
	// £aduje wektor jedynkami.
	void	loadOne()
	{ x=1.0f; y=1.0f; }

	// Zwraca d³ugoœc wektora.
	float	length() const
	{ return sqrtf(SQR(x)+SQR(y));	}
	// Zwraca kwadrat d³ugoœci wektora.
	float	sqLength() const
	{ return (SQR(x) + SQR(y));		}
	// Zwraca odwrotnoœc d³ugoœci wektora.
	float	invLength() const
	{ return INV(length());			}

	// Normalizuje wektor.
	void	normalize()
	{
		float	len = length();
		if(len==0.0f || len==1.0f)	return;
		x /= len;
		y /= len;
	}
	// Zwraca wektor znormalizowany.
	vector2	getNormalized() const
	{
		vector2	result = *this;
		result.normalize();
		return result;
	}

	// Iloczyn skalarny.
	float	dot(const vector2 &vec) const
	{ return (x*vec.x+y*vec.y); }

	// Liniowa interpolacja.
	vector2	lerp(const vector2 &vec, const float factor) const
	{ return (*this)*(1.0f-factor) + vec*factor; }

	// IO
	bool read(std::ifstream &file)
	{
		if(!file.is_open())
			return false;
		file.read((char*)cell, sizeof(cell));
		return true;
	}
	bool write(std::ofstream &file)
	{
		if(!file.is_open())
			return false;
		file.write((char*)cell, sizeof(cell));
		return true;
	}

	union
	{
		struct	{ float x; float y; };
		float	cell[2];
	};
};

} // exRay

#endif 