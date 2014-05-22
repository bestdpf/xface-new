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
#pragma once
#ifndef MORPHBLENDER_H_
#define MORPHBLENDER_H_

#include <boost/shared_ptr.hpp>
#include <XFace/FDP.h>
#include <XEngine/Entity.h>


namespace XEngine{
class MorphBlender
{
	std::map<std::string, std::set<unsigned short> > m_lipAreas;
	boost::shared_ptr<XFace::FDP> m_pFDP;
	Entity m_restEntity;
	Entity m_priorityChannel;
	void resetEntity(Entity& ent) const;
	void computeEntityDifference(const Entity& rest, Entity& dst, const Entity& ent) const;
	void normalizeEntity(Entity& dst) const;
	Entity prepareChannelPriority(const Entity& channel);
public:
	const std::set<unsigned short>& getLipArea(const std::string& meshName);
	void setFDP(boost::shared_ptr<XFace::FDP> pFDP) {m_pFDP = pFDP;}
	bool blend(Entity& dst, const Entity& exp);
	Entity prioritizeChannel(const std::map<std::string, Entity>& dictionary, const Entity& rest);
};

} // namespace XEngine

#endif // MORPHBLENDER_H_