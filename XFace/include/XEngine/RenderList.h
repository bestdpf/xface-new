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
	\file	RenderList.h
	\brief	Render list storing Drawables and calling proper IRenderer methods.
*/
#ifndef RENDERLIST_H_
#define RENDERLIST_H_

#include <list>
#include <boost/shared_ptr.hpp>
#include "Drawable.h"
#include "IRenderer.h"


namespace XEngine{
/*!
	\brief Render list storing Drawables and calling proper IRenderer methods.
	\ingroup XEngine
	\author Koray Balci
	\version 1.1
	\date   May 2003
	modified : added boost::shared_ptr support + functors

	This class keeps a list of pointers to Drawable instances. Once renderList() 
	method is called, contents of the list are sent to IRenderer sequentially.
*/
class RenderList
{
	struct render_functor
	{
		boost::shared_ptr<IRenderer> m_pRenderer;
		render_functor(boost::shared_ptr<IRenderer> pRend) : m_pRenderer(pRend) {}
		void operator()(boost::shared_ptr<Drawable> item) const{
			m_pRenderer->render(item/*.get()*/);
		}
	};
	typedef std::list<boost::shared_ptr<Drawable> > RLIST;
	RLIST m_List;
public:
	//! Adds a Drawable pointer to the list.
	void addDrawable(boost::shared_ptr<Drawable> item){m_List.push_back(item);}
	//! Adds a Drawable list to the list.
	void addDrawables(const std::list<boost::shared_ptr<Drawable> >& dr){m_List.insert(m_List.end(), dr.begin(), dr.end());}
	//! Removes a Drawable with given name.
	void removeDrawable(const std::string& name);
	
	//! Clears the list.
	void clearList(){m_List.clear();};
	//! Sends the contents of the list to IRenderer sequentially.
	void renderList(boost::shared_ptr<IRenderer> pRenderer) const;

	~RenderList(void);
};
}
#endif
