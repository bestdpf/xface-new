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

// Original code taken and adopted from http://www.nigels.com/glt/gltzpr/ Nigel Stewart
#include "ModelViewerCamera.h"
#include <stdio.h>
#include <GL/glu.h>
#include <wx/event.h>

ModelViewerCamera::ModelViewerCamera(void)
{
	m_parentWnd = 0;

	m_left   = 0.0;
	m_right  = 0.0;
	m_bottom = 0.0;
	m_top    = 0.0;
	m_zNear  = 30;
	m_zFar   = 1200.0;

	m_mouseX      = 0;
	m_mouseY      = 0;
	m_mouseLeft   = false;
	m_mouseMiddle = false;
	m_mouseRight  = false;

	m_dragPosX  = 0.0;
	m_dragPosY  = 0.0;
	m_dragPosZ  = 0.0;

	m_referencePoint.x = 0;
	m_referencePoint.y = -1;
	m_referencePoint.z = -659;
}

ModelViewerCamera::~ModelViewerCamera(void)
{
}

void ModelViewerCamera::init(wxGLCanvas* parent)
{
	m_parentWnd = parent;
	getMatrix();
}

void ModelViewerCamera::resize(int w, int h, double left, double top, double zNear, double zFar)
{
	glViewport(0,0,w,h);

    m_top    =  top;
    m_bottom = -top;
    m_left   = top*(-(double)w/(double)h); // made left->top and had a better perspective (12.10.2007)
    m_right  = -m_left;
	m_zNear	 = zNear;
	m_zFar   = zFar;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(m_left, m_right, m_bottom, m_top, m_zNear, m_zFar);
	//gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, m_zNear, m_zFar);
	
    glMatrixMode(GL_MODELVIEW);
}

void ModelViewerCamera::getMatrix()
{
    glGetDoublev(GL_MODELVIEW_MATRIX, m_matrix);
    invertMatrix(m_matrix, m_matrixInverse);
}

/*
 * From Mesa-2.2\src\glu\project.c
 *
 * Compute the inverse of a 4x4 matrix.  Contributed by scotter@lafn.org
 */

void ModelViewerCamera::invertMatrix(const GLdouble *m, GLdouble *out )
{

	/* NB. OpenGL Matrices are COLUMN major. */
	#define MAT(m,r,c) (m)[(c)*4+(r)]

	/* Here's some shorthand converting standard (row,column) to index. */
	#define m11 MAT(m,0,0)
	#define m12 MAT(m,0,1)
	#define m13 MAT(m,0,2)
	#define m14 MAT(m,0,3)
	#define m21 MAT(m,1,0)
	#define m22 MAT(m,1,1)
	#define m23 MAT(m,1,2)
	#define m24 MAT(m,1,3)
	#define m31 MAT(m,2,0)
	#define m32 MAT(m,2,1)
	#define m33 MAT(m,2,2)
	#define m34 MAT(m,2,3)
	#define m41 MAT(m,3,0)
	#define m42 MAT(m,3,1)
	#define m43 MAT(m,3,2)
	#define m44 MAT(m,3,3)

	GLdouble det;
	GLdouble d12, d13, d23, d24, d34, d41;
	GLdouble tmp[16]; /* Allow out == in. */

	/* Inverse = adjoint / det. (See linear algebra texts.)*/

	/* pre-compute 2x2 dets for last two rows when computing */
	/* cofactors of first two rows. */
	d12 = (m31*m42-m41*m32);
	d13 = (m31*m43-m41*m33);
	d23 = (m32*m43-m42*m33);
	d24 = (m32*m44-m42*m34);
	d34 = (m33*m44-m43*m34);
	d41 = (m34*m41-m44*m31);

	tmp[0] =  (m22 * d34 - m23 * d24 + m24 * d23);
	tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
	tmp[2] =  (m21 * d24 + m22 * d41 + m24 * d12);
	tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

	/* Compute determinant as early as possible using these cofactors. */
	det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

   /* Run singularity test. */
   if (det == 0.0) {
      /* printf("invert_matrix: Warning: Singular matrix.\n"); */
/*    memcpy(out,_identity,16*sizeof(double)); */
   }
   else {
      GLdouble invDet = 1.0 / det;
      /* Compute rest of inverse. */
      tmp[0] *= invDet;
      tmp[1] *= invDet;
      tmp[2] *= invDet;
      tmp[3] *= invDet;

      tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
      tmp[5] =  (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
      tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
      tmp[7] =  (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

      /* Pre-compute 2x2 dets for first two rows when computing */
      /* cofactors of last two rows. */
      d12 = m11*m22-m21*m12;
      d13 = m11*m23-m21*m13;
      d23 = m12*m23-m22*m13;
      d24 = m12*m24-m22*m14;
      d34 = m13*m24-m23*m14;
      d41 = m14*m21-m24*m11;

      tmp[8] =  (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
      tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
      tmp[10] =  (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
      tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
      tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
      tmp[13] =  (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
      tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
      tmp[15] =  (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

      memcpy(out, tmp, 16*sizeof(GLdouble));
   }

	#undef m11
	#undef m12
	#undef m13
	#undef m14
	#undef m21
	#undef m22
	#undef m23
	#undef m24
	#undef m31
	#undef m32
	#undef m33
	#undef m34
	#undef m41
	#undef m42
	#undef m43
	#undef m44
	#undef MAT
}

void ModelViewerCamera::pos(double *px,double *py,double *pz,const int x,const int y,const int *viewport)
{
    /*
      Use the ortho projection and viewport information
      to map from mouse co-ordinates back into world
      co-ordinates
    */

    *px = (double)(x-viewport[0])/(double)(viewport[2]);
    *py = (double)(y-viewport[1])/(double)(viewport[3]);

    *px = m_left + (*px)*(m_right - m_left);
    *py = m_top  + (*py)*(m_bottom - m_top);
    *pz = m_zNear;
}

void ModelViewerCamera::move(int x, int y)
{
    bool changed = false;

    const int dx = x - m_mouseX;
    const int dy = y - m_mouseY;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);

    if (dx==0 && dy==0)
        return;

    if (m_mouseMiddle || (m_mouseLeft && m_mouseRight))
    {
        double s = exp((double)dy*0.01);

        glTranslatef( m_referencePoint.x, m_referencePoint.y, m_referencePoint.z);
        glScalef(s,s,s);
		glTranslatef(-m_referencePoint.x,-m_referencePoint.y,-m_referencePoint.z);

        changed = true;
    }
    else
        if (m_mouseLeft)
        {
            double ax,ay,az;
            double bx,by,bz;
            double angle;

            ax = dy;
            ay = dx;
            az = 0.0;
            angle = vlen(ax,ay,az)/(double)(viewport[2]+1)*180.0;

            /* Use inverse matrix to determine local axis of rotation */

            bx = m_matrixInverse[0]*ax + m_matrixInverse[4]*ay + m_matrixInverse[8] *az;
            by = m_matrixInverse[1]*ax + m_matrixInverse[5]*ay + m_matrixInverse[9] *az;
            bz = m_matrixInverse[2]*ax + m_matrixInverse[6]*ay + m_matrixInverse[10]*az;

            glTranslatef( m_referencePoint.x, m_referencePoint.y, m_referencePoint.z);
            glRotatef(angle,bx,by,bz);
            glTranslatef(-m_referencePoint.x, -m_referencePoint.y, -m_referencePoint.z);

            changed = true;
        }
        else
            if (m_mouseRight)
            {
                double px,py,pz;

                pos(&px,&py,&pz,x,y,viewport);

                glLoadIdentity();
                glTranslatef(px - m_dragPosX, py - m_dragPosY, pz - m_dragPosZ);
                glMultMatrixd(m_matrix);

                m_dragPosX = px;
                m_dragPosY = py;
                m_dragPosZ = pz;

                changed = true;
            }

    m_mouseX = x;
    m_mouseY = y;

    if (changed)
    {
        getMatrix();
        m_parentWnd->Refresh();
    }
}

void ModelViewerCamera::mouse(int button, int state, int x, int y)
{
   GLint viewport[4];

    m_mouseX = x;
    m_mouseY = y;

    if (state == 0)
        switch (button)
        {
            case wxMOUSE_BTN_LEFT:   m_mouseLeft   = false; break;
            case wxMOUSE_BTN_MIDDLE: m_mouseMiddle = false; break;
            case wxMOUSE_BTN_RIGHT:  m_mouseRight  = false; break;
        }
    else
        switch (button)
        {
            case wxMOUSE_BTN_LEFT:   m_mouseLeft   = true; break;
            case wxMOUSE_BTN_MIDDLE: m_mouseMiddle = true; break;
            case wxMOUSE_BTN_RIGHT:  m_mouseRight  = true; break;
        }

    glGetIntegerv(GL_VIEWPORT,viewport);
    pos(&m_dragPosX, &m_dragPosY, &m_dragPosZ, x, y, viewport);
}

void ModelViewerCamera::reset()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	getMatrix();
}