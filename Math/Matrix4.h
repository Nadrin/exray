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

#ifndef __MATRIX4_H
#define __MATRIX4_H

#define	MATH_MATRIX4

namespace exRay {

class vector2;
class vector3;
class vector4;

/// Macierz kwadratowa 4x4.
class matrix4
{
public:
	// Konstruktory.
	matrix4(bool identity=false)	{ if(identity) loadIdentity(); else memset(cell, 0, 64); }
	matrix4(float *data)			{ memcpy(cell, data, 64);						}
	matrix4(float value)			{ for(int i=0; i<16; i++) { cell[i] = value; }  }
	matrix4(vector4 &row1, vector4 &row2, vector4 &row3, vector4 &row4)
	{	memcpy(&cell[0], row1.cell, 16); memcpy(&cell[4], row2.cell, 16);
		memcpy(&cell[8], row3.cell, 16); memcpy(&cell[12], row4.cell, 16);
	}
	matrix4(float *row1, float *row2, float *row3, float *row4)
	{	memcpy(&cell[0], row1, 16); memcpy(&cell[4], row2, 16);
		memcpy(&cell[8], row3, 16); memcpy(&cell[12], row4, 16);
	}
	matrix4(float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8,
			float c9, float c10, float c11, float c12, float c13, float c14, float c15, float c16)
	{
		cell[0]=c1; cell[1]=c2; cell[2]=c3; cell[3]=c4; cell[4]=c5; cell[5]=c6;
		cell[6]=c7; cell[7]=c8; cell[8]=c9; cell[9]=c10; cell[10]=c11; cell[11]=c12;
		cell[12]=c13; cell[13]=c14; cell[14]=c15; cell[15]=c16;
	}

	// Destruktor.
	virtual ~matrix4(void)			{ }

	// Operatory:
	// przypisanie (cast to float)
	operator float* () const		{ return (float*)&cell;			}
	operator const float* () const	{ return (const float*)&cell;	}

	// £aduje macierz zerami.
	void	loadZero(void)
	{ memset(cell, 0, 64); }
	// Ustawia macierz In.
	void	loadIdentity(void)
	{
		memset(cell, 0, 64);
		cell[0] = 1.0f; cell[5] = 1.0f; cell[10] = 1.0f; cell[15] = 1.0f;
	}

	// Dodawanie.
	matrix4 operator + (const matrix4 &m) const
	{ 
		matrix4 result;
		for(int i=0; i<16; i++) { result.cell[i] = cell[i] + m.cell[i]; }
		return result;
	}

	// Odejmowanie.
	matrix4 operator - (const matrix4 &m) const
	{
		matrix4 result;
		for(int i=0; i<16; i++) { result.cell[i] = cell[i] - m.cell[i]; }
		return result;
	}

	// Mno¿enie (TODO: zoptymalizowaæ!)
	matrix4 operator * (const matrix4 &m) const
	{
		matrix4 r;

		r.cell[0] = cell[0]*m.cell[0] + cell[1]*m.cell[4] + cell[2]*m.cell[8] + cell[3]*m.cell[12];
		r.cell[1] = cell[0]*m.cell[1] + cell[1]*m.cell[5] + cell[2]*m.cell[9] + cell[3]*m.cell[13];
		r.cell[2] = cell[0]*m.cell[2] + cell[1]*m.cell[6] + cell[2]*m.cell[10] + cell[3]*m.cell[14];
		r.cell[3] = cell[0]*m.cell[3] + cell[1]*m.cell[7] + cell[2]*m.cell[11] + cell[3]*m.cell[15];

		r.cell[4] = cell[4]*m.cell[0] + cell[5]*m.cell[4] + cell[6]*m.cell[8] + cell[7]*m.cell[12];
		r.cell[5] = cell[4]*m.cell[1] + cell[5]*m.cell[5] + cell[6]*m.cell[9] + cell[7]*m.cell[13];
		r.cell[6] = cell[4]*m.cell[2] + cell[5]*m.cell[6] + cell[6]*m.cell[10] + cell[7]*m.cell[14];
		r.cell[7] = cell[4]*m.cell[3] + cell[5]*m.cell[7] + cell[6]*m.cell[11] + cell[7]*m.cell[15];

		r.cell[8] = cell[8]*m.cell[0] + cell[9]*m.cell[4] + cell[10]*m.cell[8] + cell[11]*m.cell[12];
		r.cell[9] = cell[8]*m.cell[1] + cell[9]*m.cell[5] + cell[10]*m.cell[9] + cell[11]*m.cell[13];
		r.cell[10] = cell[8]*m.cell[2] + cell[9]*m.cell[6] + cell[10]*m.cell[10] + cell[11]*m.cell[14];
		r.cell[11] = cell[8]*m.cell[3] + cell[9]*m.cell[7] + cell[10]*m.cell[11] + cell[11]*m.cell[15];

		r.cell[12] = cell[12]*m.cell[0] + cell[13]*m.cell[4] + cell[14]*m.cell[8] + cell[15]*m.cell[12];
		r.cell[13] = cell[12]*m.cell[1] + cell[13]*m.cell[5] + cell[14]*m.cell[9] + cell[15]*m.cell[13];
		r.cell[14] = cell[12]*m.cell[2] + cell[13]*m.cell[6] + cell[14]*m.cell[10] + cell[15]*m.cell[14];
		r.cell[15] = cell[12]*m.cell[3] + cell[13]*m.cell[7] + cell[14]*m.cell[11] + cell[15]*m.cell[15];

		return r;
	}

	// Mno¿enie wektora przez macierz.
	vector4 operator * (const vector4 &v) const
	{
		vector4	r;

		r.x = cell[0]*v.x + cell[1]*v.y + cell[2]*v.z + cell[3]*v.w;
		r.y = cell[4]*v.x + cell[5]*v.y + cell[6]*v.z + cell[7]*v.w;
		r.z = cell[8]*v.x + cell[9]*v.y + cell[10]*v.z + cell[11]*v.w;
		r.w = cell[12]*v.x + cell[13]*v.y + cell[14]*v.z + cell[15]*v.w;

		return r;
	}
	vector3 operator * (const vector3 &v) const
	{
		vector4	r4, v4(v, 1.0f);
		r4 = *this * v4;
		return vector3(r4.x, r4.y, r4.z);
	}

	
	void operator += (const matrix4 &m)
	{ *this = m + *this; }
	
	void operator -= (const matrix4 &m)
	{ *this = m - *this; }
	
	void operator *= (const matrix4 &m)
	{ *this = m * *this; }

	
	matrix4 operator + (void) const 
	{ return *this; }
	
	matrix4 operator - (void) const
	{
		return matrix4(	-cell[0], -cell[1], -cell[2], -cell[3], -cell[4], -cell[5],
						-cell[6], -cell[7], -cell[8], -cell[9], -cell[10], -cell[11],
						-cell[12], -cell[13], -cell[14], -cell[15]);
	}

	// Zamienia komórkê [a] z komórk¹ [b].
	bool swap(const unsigned int a, const unsigned int b)
	{
		if(a > 15 || b > 15)
			return false;
		float temp	= cell[a];
		cell[a]		= cell[b];
		cell[b]		= temp;
		return true;
	}

	// Transpozycja.
	void transpose(void)
	{
		swap(1,4); swap(2,8); swap(3,12);
		swap(4,1); swap(6,9); swap(7,13);
		swap(8,2); swap(9,6); swap(11,14);
		swap(12,3); swap(13,7); swap(14,11);
	}
	matrix4 getTransposed(void) const
	{
		return matrix4(
			cell[0], cell[4], cell[8], cell[12],
			cell[1], cell[5], cell[9], cell[13],
			cell[2], cell[6], cell[10], cell[14],
			cell[3], cell[7], cell[11], cell[15]);
	}

	// Odwracanie macierzy.
	void	invert(void)
	{
		// TODO: Zaimplementowaæ!
		return;
	}
	matrix4 getInverted(void) const
	{
		matrix4	result = *this;
		result.invert();
		return result;
	}
	// Transformacje przestrzenne:

	// Translacja
	void setTranslation(const vector3 &vec)
	{ cell[3] = vec.x; cell[7] = vec.y; cell[11] = vec.z; }
	void setTranslation(const float x, const float y, const float z)
	{ cell[3] = x; cell[7] = y; cell[11] = z; }
	matrix4 translate(const vector3 &vec) const
	{
		matrix4 result(true);
		result.setTranslation(vec);
		return result;
	}
	matrix4 translate(const float x, const float y, const float z) const
	{
		matrix4 result(true);
		result.setTranslation(x, y, z);
		return result;
	}

	// Skalowanie.
	void setScale(const float s)
	{ cell[0] = s; cell[5] = s; cell[10] = s; cell[15] = s; }
	void setScale(const vector3 &vec)
	{ cell[0] = vec.x; cell[5] = vec.y; cell[10] = vec.z; cell[15] = 1.0f; }
	void setScale(const float x, const float y, const float z)
	{ cell[0] = x; cell[5] = y; cell[10] = z; cell[15] = 1.0f; }
	matrix4 scale(const float s) const
	{
		matrix4 result(true);
		result.setScale(s);
		return result;
	}
	matrix4 scale(const vector3 &vec) const
	{
		matrix4 result(true);
		result.setScale(vec);
		return result;
	}
	matrix4 scale(const float x, const float y, const float z) const
	{
		matrix4 result(true);
		result.setScale(x, y, z);
		return result;
	}

	// Obrót (oœ X)
	void setRotationX(const float x)
	{
		cell[5]  = cosf(x); 
		cell[6]  = -sinf(x);
		cell[9]  = sinf(x);
		cell[10] = cosf(x);
	}
	// Obrót (oœ Y)
	void setRotationY(const float y)
	{
		cell[0]  = cosf(y);
		cell[2]  = sinf(y);
		cell[8]  = -sinf(y);
		cell[10] = cosf(y);
	}
	// Obrót (oœ Z)
	void setRotationZ(const float z)
	{
		cell[0]  = cosf(z);
		cell[1]  = -sinf(z);
		cell[4]  = sinf(z);
		cell[5]  = cosf(z);
	}

	// Obrót na wszystkich 3 osiach.
	matrix4 rotate(const vector3 &vec) const
	{
		matrix4 mx(true), my(true), mz(true);
		mx.setRotationX(vec.x);
		my.setRotationY(vec.y);
		mz.setRotationZ(vec.z);
		return mx * my * mz;
	}
	matrix4 rotate(const float x, const float y, const float z) const
	{
		matrix4 mx(true), my(true), mz(true);
		mx.setRotationX(x);
		my.setRotationY(y);
		mz.setRotationZ(z);
		return mx * my * mz;
	}

	// Dane (zmienne z wartoœciami komórek macierzy)
	union
	{
		float	matrix[4][4];
		float	cell[16];
	};
};

} // exRay

#endif