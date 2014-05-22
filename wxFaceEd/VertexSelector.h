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
 * The Original Code is wxFaceEd
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#ifndef VERTEX_SELECTOR_H_
#define VERTEX_SELECTOR_H_


#include <XMath/Matrix4.h>
#include <XMath/Ray3.h>
#include <XMath/Rectangle.h>
#include <XEngine/IndexedFaceSet.h>
#include <XEngine/Transform.h>
#include <set>

using namespace XEngine;
using namespace XMath;

class VertexSelector
{
	const Ray3 createRay(long wndx, long wndy, const Transform& tr) const;
public:
	VertexSelector(void){};
	bool screenToFace(long wndx, long wndy, const IndexedFaceSet* pMesh, const Transform& globalTr, Vector3& impactPnt) const;	
	static std::set<unsigned short> selectRect(const IndexedFaceSet* pMesh, const XMath::Rectangle2& rect, const Matrix4& objView);
	bool selectVertex(long wndx, long wndy, const IndexedFaceSet* pMesh, const Transform& globalTr, unsigned short& index); 	
};

#endif //VERTEX_SELECTOR_H_