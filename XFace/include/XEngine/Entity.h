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
 * The Original Code is Xface Core Library;.
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

#pragma once
#ifndef ENTITY_H_
#define ENTITY_H_

#include <list>
#include <string>

#include "Drawable.h"
#include "Transform.h"

#include <boost/shared_ptr.hpp>

/*! 
	\file Entity.h
	\brief Represents an entity visible in the scene, includes drawables, ai, etc..
*/

namespace XEngine{

/*!
	\brief Represents an entity visible in the scene, includes drawables, ai, etc..
*/
class Entity
{
protected:	
	std::list<boost::shared_ptr<Drawable> > m_drawables;
	Transform m_transform;
public:
	void resetDeformedVertices();
	virtual Entity& copyFrom(const Entity& rhs, bool duplicateData = false);
	void addDrawables(std::list<boost::shared_ptr<Drawable> >& dr) {m_drawables.insert(m_drawables.end(), dr.begin(), dr.end());}
	void addDrawable(boost::shared_ptr<Drawable>& dr) {m_drawables.push_back(dr);}
	const std::list<boost::shared_ptr<Drawable> >& getDrawables() const {return m_drawables;}
	size_t getDrawableCount() const { return m_drawables.size();}
	const Transform& getTransform() const {return m_transform;}
	void setTransform(const Transform& tr) {m_transform = tr;}
	virtual void release(bool destroyData = false);
	Entity() {}
	virtual ~Entity(void);
};

} // namespace XEngine
#endif // ENTITY_H_