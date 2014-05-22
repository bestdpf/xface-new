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
 * The Original Code is Xface Core Library.
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
	\file	IRenderer.h
	\brief	Interface for renderer.
*/
#ifndef IRENDERER_H_
#define IRENDERER_H_

#include <boost/shared_ptr.hpp>

namespace XEngine{
class Drawable;
class Transform;
class IndexedFaceSet;

/*!
	\brief Interface for renderer.
	\ingroup XEngine
	\author Koray Balci
	\version 1.0
	\date   May 2003

	Abstract base class for all type of renderers. This class ensures the future 
	development paths for different API's like OpenGL, DirectX, software, etc..
*/
class IRenderer
{
public:
	//! Pure virtual method for rendering a Drawable object.
	virtual void render(boost::shared_ptr<Drawable> pDrawable) const = 0;
//	virtual void setRotation(float, float, float) = 0;
	virtual void setTransform(const Transform& tr) = 0;
	virtual void prepareBufferedMesh(IndexedFaceSet* pMesh) = 0;
	IRenderer(void){};
	virtual ~IRenderer(void){};
};
}

#endif
