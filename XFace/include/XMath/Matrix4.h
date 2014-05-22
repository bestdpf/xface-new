/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Xface Core Library; Math
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * For info, contact: xface-info@fbk.eu or http://xface.fbk.eu
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

/*! 
	\file	Matrix4.h
	\brief	4x4 Matrix class
*/


#ifndef MATRIX4_H_
#define MATRIX4_H_

#include "Vector3.h"

namespace XMath{
	
	class Matrix4d;
/*!
	\brief	4x4 Matrix class for float
	\ingroup XMath
	\author	Koray Balci
	\version 1.0 (April 2002)
		1.1
	\date   April 2002

	Class which defines 4x4 matrix and related/necessary mathematical operations.

	Matrix data are stored in a one dimensional array in OpenGL format. Check the
	additional constructor and () operator overload implementation for accessing
	data as if it is a two dimensional array. 
	\note No inlining has been done yet. All the inlines and other optimizations are to be 
	added in a later stage when we have a better understanding of bottlenecks.
*/
class Matrix4 
{
public:
	//! Negates
	void negate(void);
	//! Switches to inverse of this matrix.
	Matrix4 getInverseTransform() const;
	//! Switches to Identity matrix
	void loadIdentity();
	//! returns Transpose
	Matrix4 getTranspose() const;
	//! float* cast overloaded
	operator float*() {return &m_data[0];};
	//! float* cast overloaded
	operator const float*() const {return &m_data[0];}
	//! * operator overloaded (matrix)
	Matrix4 operator *(const Matrix4& rhs);
	//! * operator overloaded (matrix double)
	Matrix4d operator *(const Matrix4d& rhs);
	//! * operator overloaded (scalar)
	Matrix4 operator *(const float scalar) const;
	//! () operator overloaded
	float& operator() (unsigned int i, unsigned int j){return m_data[i*4 + j];};
	//! [] operator overloaded
	float& operator[](int i){return m_data[i];};
	//! [] operator overloaded (const version)
	float operator[](int i) const {return m_data[i];};
	//! = operator overloaded
	Matrix4& operator=(const Matrix4 & rhs);
	//! * operator overloaded (vector)
	Vector3 operator *(const Vector3& rhs) const;

	//! Accessor function for the data member
	const float* getMatrix(){return &m_data[0];};
	//! Copy Constructor
	Matrix4(const Matrix4& pCopy);
	//! Default Constructor
	Matrix4();
	//! Additional constructor for easy initialization
	Matrix4 (float f00, float f01, float f02, float f03,
		float f10, float f11, float f12, float f13, 
		float f20, float f21, float f22, float f23,
		float f30, float f31, float f32, float f33);
	//! Additional constructor for easy initialization
	Matrix4(const float mat[16]);
	//! Destructor
	~Matrix4(){};

private:
	//! Matrix data stored here
	float m_data[16];
};


class Matrix4d
{
public:
	//! Negates
	void negate(void);
	//! Switches to inverse of this matrix.
	Matrix4d getInverseTransform() const;
	//! Switches to Identity matrix
	void loadIdentity();
	//! returns Transpose
	Matrix4d getTranspose() const;
	//! double* cast overloaded
	operator double*() {return &m_data[0];};
	//! double* cast overloaded
	operator const double*() const {return &m_data[0];}
	//! * operator overloaded (matrix)
	Matrix4d operator *(const Matrix4d& rhs);
	//! * operator overloaded (float matrix)
	Matrix4d operator *(const Matrix4& rhs);
	//! * operator overloaded (scalar)
	Matrix4d operator *(const double scalar) const;
	//! () operator overloaded
	double& operator() (unsigned int i, unsigned int j){return m_data[i*4 + j];};
	//! [] operator overloaded
	double& operator[](int i){return m_data[i];};
	//! [] operator overloaded (const version)
	double operator[](int i) const {return m_data[i];};
	//! = operator overloaded
	Matrix4d& operator=(const Matrix4d & rhs);
	//! * operator overloaded (vector)
	Vector3 operator *(const Vector3& rhs) const;

	//! Accessor function for the data member
	const double* getMatrix(){return &m_data[0];};
	//! Copy Constructor
	Matrix4d(const Matrix4d& pCopy);
	//! Default Constructor
	Matrix4d();
	//! Additional constructor for easy initialization
	Matrix4d (double f00, double f01, double f02, double f03,
		double f10, double f11, double f12, double f13, 
		double f20, double f21, double f22, double f23,
		double f30, double f31, double f32, double f33);
	//! Additional constructor for easy initialization
	Matrix4d(const double mat[16]);
	//! Destructor
	~Matrix4d(){};

private:
	//! Matrix data stored here
	double m_data[16];
};
}
#endif // MATRIX4_H_
