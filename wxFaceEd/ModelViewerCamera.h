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
// Original code taken and adopted/modified from http://www.nigels.com/glt/gltzpr/ Nigel Stewart
#pragma once
#include <wx/glcanvas.h>
#include <math.h>
#include <XMath/Vector3.h>

class ModelViewerCamera
{
	double m_left;
	double m_right;
	double m_bottom;
	double m_top;
	double m_zNear;
	double m_zFar;

	double m_matrix[16];
	double m_matrixInverse[16];

	int  m_mouseX;
	int  m_mouseY;
	bool m_mouseLeft;
	bool m_mouseMiddle;
	bool m_mouseRight;

	double m_dragPosX;
	double m_dragPosY;
	double m_dragPosZ;

	double vlen(double x, double y, double z) const {return sqrt(x*x+y*y+z*z);}
	void pos(double *px,double *py,double *pz,const int x,const int y,const int *viewport);
	void getMatrix();
	void invertMatrix(const GLdouble *m, GLdouble *out );
	// Configurable center point for zooming and rotation 
	XMath::Vector3 m_referencePoint;
	wxGLCanvas* m_parentWnd;

public:
	void reset();
	void setReferencePoint(const XMath::Vector3& ref) {m_referencePoint = ref;}
	const XMath::Vector3& getReferencePoint() const {return m_referencePoint;}
	void mouse(int button, int state, int x, int y);
	void move(int x, int y);
	void init(wxGLCanvas* parent);
	void resize(int w, int h, double left = 205, double top = 200, double zNear = 300, double zFar = 800);

	ModelViewerCamera(void);
	~ModelViewerCamera(void);
};
