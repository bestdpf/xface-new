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
	\file RendererManager.h
	\brief	Manages creation of renderer, storage of drawables to be rendered and rendering them.
*/
#ifndef RENDERERMANAGER_H_
#define RENDERERMANAGER_H_
#pragma once

#include <boost/shared_ptr.hpp>
#include "IRenderer.h"
#include "Transform.h"
#include "RenderList.h"

namespace XEngine{
/*!
	\brief	Manages creation of renderer, storage of drawables to be rendered and rendering them.
*/
class RenderManager
{
	Transform m_globalTransform;
	RenderList m_renderList;
	boost::shared_ptr<IRenderer> m_pRenderer;
public:
	void resetDrawables() {m_renderList.clearList();}
	void addDrawables(const std::list<boost::shared_ptr<Drawable> >& dr) {m_renderList.addDrawables(dr);}
	void setRenderer(boost::shared_ptr<IRenderer> pRend) {m_pRenderer = pRend;}
	void setGlobalTransform(const Transform& tr) {m_globalTransform = tr;}
	void render() const;
	void update();
};

} // namespace XEngine

#endif // RENDERMANAGER_H_