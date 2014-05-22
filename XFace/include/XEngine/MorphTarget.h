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
 * Portions creat
 ed by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

/*! 
	\file	MorphTarget.h
	\brief	Morph target definition.
*/

#pragma once
#ifndef MORPHTARGET_H_
#define MORPHTARGET_H_

#include "Entity.h"
#include <string>

namespace XEngine{

class MorphTarget
{
protected:
	Entity m_entity;
	float m_weight;
	unsigned int m_duration;
	unsigned int m_start, m_end;
	std::string m_alias;
public:
	//@{ Accessor
	void setEntity(const Entity& ent) {m_entity = ent;}

	unsigned int getStart() const {return m_start;}
	unsigned int getEnd() const {return m_end;}
	const std::string& getAlias() const {return m_alias;}
	float getWeight() const {return m_weight;}
	unsigned int getDuration() const {return m_duration;}
	Entity& getEntity() {return m_entity;}
	//@}
	MorphTarget(const Entity& entity, const std::string& alias, unsigned int start, unsigned int end, float weight/* = 1.0f*/) 
		: m_entity(entity), m_alias(alias), m_weight(weight), m_duration(end - start), m_start(start), m_end(end){}

	MorphTarget(){}
	virtual ~MorphTarget(void){}
};

} // namespace XEngine

#endif // MORPHTARGET_H_