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

#ifndef __MATH_H
#define __MATH_H

#define MATHLIB
#define	MCMP_BIAS	0.001f

// Marka.
#ifndef SQR
#define	SQR(x)		((x)*(x))
#endif

#ifndef INV
#define INV(x)		((1.0f)/(x))
#endif

// Sta³e.
#ifndef MPI_FLOAT
#define MPI_FLOAT	3.141593f
#endif

#ifndef MPI180_FLOAT
#define MPI180_FLOAT 0.017453f
#endif

#ifndef MPI
#define MPI			MPI_FLOAT
#endif

#ifndef MPI180
#define MPI180		MPI180_FLOAT
#endif

#ifndef MEPSILON
#define MEPSILON	0.001f
#endif

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#endif // __TYPESLIB_H