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

#ifndef RUBBERBANDBOX_H_
#define RUBBERBANDBOX_H_

#include <XMath/Rectangle.h>
#include <XEngine/VertexStruct.h>

using XMath::Rectangle2;
using namespace XEngine;

class RubberBandBox
{
	Rectangle2 m_rect;
	ColorRGBA m_color;
	bool m_bVisible;

public:
	const Rectangle2& getRectangle() const {return m_rect;};
	void clear(){m_bVisible = false;};
	void setColor(float r, float g, float b){m_color = ColorRGBA(r, g, b);}
	void setColor(const ColorRGBA& color) {m_color = color;};
	void selectArea(long wndx, long wndy, bool start = false);
	static void set2D(long wndWidth, long wndHeight);
	static void set3D();
	void draw(long wndWidth, long wndHeight);
	RubberBandBox();		
	~RubberBandBox(void);
};

#endif // RUBBERBANDBOX_H_