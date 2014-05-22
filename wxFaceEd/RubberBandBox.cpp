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

#include "RubberBandBox.h"
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>

RubberBandBox::RubberBandBox()
: m_color(0, 0, 0.5f), m_bVisible(false)
{
}

RubberBandBox::~RubberBandBox(void)
{
}

void RubberBandBox::set2D(long wndWidth, long wndHeight)
{
	glMatrixMode(GL_PROJECTION);				
	glPushMatrix();								
	glLoadIdentity();							
	glOrtho(0, wndWidth, 0, wndHeight, -1, 1);	
	glMatrixMode(GL_MODELVIEW);					
}

void RubberBandBox::set3D()
{
	glMatrixMode(GL_PROJECTION);	
	glPopMatrix();					
	glMatrixMode(GL_MODELVIEW);		
}

void RubberBandBox::draw(long wndWidth, long wndHeight)
{
	if (!m_bVisible)
		return;
	set2D(wndWidth, wndHeight);

	ColorRGBA oldColor;
	glGetFloatv(GL_CURRENT_COLOR, (float*)&oldColor);
	glColor3fv((float*)&m_color);
	glPolygonMode(GL_FRONT, GL_LINE);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glLoadIdentity();
	glTranslated(m_rect.getX0(), m_rect.getY0(), 0.0f);	

	glBegin(GL_LINE_STRIP);
		glVertex2f(0, 0);
		glVertex2f(m_rect.getWidth(), 0);
		glVertex2f(m_rect.getWidth(), m_rect.getHeight());
		glVertex2f(0, m_rect.getHeight());
		glVertex2f(0, 0);
	glEnd();

	glPopMatrix();
	
	glFlush();	
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3fv((float*)&oldColor);	

	set3D();

}
void RubberBandBox::selectArea(long wndx, long wndy, bool start)
{
	if(start)
	{
		m_rect.setX0((float)wndx);
		m_rect.setY0((float)wndy);
		m_rect.setX1((float)wndx);
		m_rect.setY1((float)wndy);

		m_bVisible = true;
	}
	else
	{
		m_rect.setX1((float)wndx);
		m_rect.setY1((float)wndy);
	}
}