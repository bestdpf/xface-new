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

#include "VertexSelector.h"

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#include <XEngine/MeshManager.h>
#include <XMath/XMath.h>
using namespace XMath;
using namespace XEngine;

// Screen coordinates are passed and checked if we collide with the face, etc..
bool VertexSelector::selectVertex(long wndx, long wndy, const IndexedFaceSet* pMesh, const Transform& globalTr, unsigned short& index) 
{
	if(!pMesh)
		return false;
	return intersectMeshRay3(pMesh, createRay(wndx, wndy, globalTr), index);
}

const Ray3 VertexSelector::createRay(long wndx, long wndy, const Transform& tr) const
{
	Ray3 retRay;
	// holds the 3D coordinates
	GLdouble x, y, z;		
	
	// arrays to hold matrix information	
	Matrix4d m1;
	glGetDoublev(GL_MODELVIEW_MATRIX, m1);
	
	Matrix4 m2 = tr.getLocalTransform();
	Matrix4d model_view = m1*m2;
	
	Matrix4d projection;
	glGetDoublev(GL_PROJECTION_MATRIX, projection);	
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);		
	
	// correct y
	wndy = viewport[3] - (GLint) wndy;
/*	
	float wndz = -1;
	glReadPixels(wndx, wndy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wndz);   
*/	
	// get 3D coordinates based on window coordinates	
	gluUnProject(wndx , wndy , 1,		
				model_view  , projection, viewport,
				&x     , &y   , &z);  
	Vector3 r2((float)x, (float)y, (float)z);

	// get 3D coordinates based on window coordinates	
	gluUnProject(wndx , wndy , 0,		
				model_view  , projection, viewport,
				&x     , &y   , &z);  
	Vector3 r1((float)x, (float)y, (float)z);
	
	retRay.setOrigin(r1);
	retRay.setDirection((r2-r1).normalize());

	return retRay;
}
	
bool VertexSelector::screenToFace(long wndx, long wndy, const IndexedFaceSet* pMesh, const Transform& globalTr, Vector3& impactPnt) const
{
	if(!pMesh)
		return false;
	
	return intersectMeshRay3(pMesh, createRay(wndx, wndy, globalTr), impactPnt);
/*	// holds the 3D coordinates
	GLdouble x, y, z;		
	
	// arrays to hold matrix information	
	GLdouble model_view[16];	
	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);	
	
	GLdouble projection[16];	
	glGetDoublev(GL_PROJECTION_MATRIX, projection);	
	
	GLint viewport[4];	
	glGetIntegerv(GL_VIEWPORT, viewport);		
	// correct y
	wndy = viewport[3] - (GLint) wndy;
	
	float wndz = -1;
	glReadPixels(wndx, wndy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wndz);   
	if(wndz == 1)	// no collision happened
		return false;

	// get 3D coordinates based on window coordinates	
	gluUnProject(wndx , wndy , wndz,		
				model_view  , projection, viewport,
				&x     , &y   , &z);  
	impactPnt = Vector3((float)x, (float)y, (float)z);
	//impactPnt = objView.getInverseTransform() * impactPnt;

	return true;
*/
}

/*!
	Given a rectangular area, calculates the vertices inside of it..
*/
std::set<unsigned short> VertexSelector::selectRect(const IndexedFaceSet* pMesh, const XMath::Rectangle2& rect, const Matrix4& objView)
{
	std::set<unsigned short> selected;
	// arrays to hold matrix information	
	Matrix4d m1;
	glGetDoublev(GL_MODELVIEW_MATRIX, m1);
	
	Matrix4d model_view = m1*objView;
	Vector3 view_vect((float)-model_view[2], (float)-model_view[6], (float)-model_view[10]);
	
	GLdouble projection[16];	
	glGetDoublev(GL_PROJECTION_MATRIX, projection);	
	
	GLint viewport[4];	
	glGetIntegerv(GL_VIEWPORT, viewport);		
	
	GLdouble wndx, wndy, wndz;
	size_t nVert = pMesh->getVertexCount();
	const std::vector<Vector3>& pVert = pMesh->getVertices();
	
	// for eliminating back facing vertices
	const std::vector<Vector3>& pNorm = pMesh->getNormals();
	for (size_t i = 0; i < nVert; ++i)
	{
		// get 3D coordinates based on window coordinates	
		gluProject(pVert[i].x, pVert[i].y, pVert[i].z,	
				model_view , projection, viewport,
				&wndx , &wndy , &wndz);  
//		wndy = viewport[3] - (GLint) wndy;

		if(rect.isPointInRect((float)wndx, (float)wndy))
		{
			Vector3 pnt2(objView*pNorm[i]);
			pnt2.x -= objView[12];
			pnt2.y -= objView[13];
			pnt2.z -= objView[14];

			if(pnt2.dot(view_vect) < 0)
				selected.insert((unsigned short)i);
		}
	}

	return selected;
}
