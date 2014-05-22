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
 * The Original Code is Xface Core Library; Some Structures
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
	\file	VertexStruct.h
	\brief	Vertex structures are here
*/

#ifndef VERTEXSTRUCT_H_
#define VERTEXSTRUCT_H_

namespace XEngine{

/*!
	3D Vertex used for mesh coordinates and normals in 3D space.

class Vertex3D
{
public:
	float x, y, z;
};
*/
/*!
    \ingroup XEngine
	2D Vertex used for texture coordinates
*/
class Vertex2D
{
public:
	union{
		float s;
		float x;
	};
	union{
		float t;
		float y;
	};
};

/*!
    \ingroup XEngine
	RGBA color structure
*/
class ColorRGBA
{
public:
	operator float*() {return &r;};
	operator const float*() const {return &r;};
	ColorRGBA(){}
	ColorRGBA(float _r, float _g, float _b, float _a = 1)
		: r(_r), g(_g), b(_b), a(_a){}
	float r, g, b, a;
};
}
#endif //VERTEXSTRUCT_H_
