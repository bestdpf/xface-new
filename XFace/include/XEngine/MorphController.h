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
	\file	MorphController.h
	\brief	Morph targets controller.
*/

#pragma once
#ifndef MORPHCONTROLLER_H_
#define MORPHCONTROLLER_H_

#include "ITimedController.h"
#include "KeyframeInterpolator.h"
#include "MorphTarget.h"
#include <XMath/Perlin.h>
#include <XFace/FDP.h>

#include <string>
#include <list>
#include <algorithm>
#include "MorphChannel.h"
#include "MorphBlender.h"
namespace XEngine{

/*!
	\brief Morph targets controller
*/
class MorphController :
	public ITimedController
{
    typedef boost::shared_ptr<MorphChannel> ChannelItem;
	typedef std::list<MorphTarget*> MorphTargetList;
	typedef std::map<std::string, std::map<std::string, Entity> > MorphDictionary;
		
	//! next functor
	struct next_functor
	{
		void operator()(const ChannelItem& p) const
		{ 
			p->next();
		}
	};
	
	//!	rewinds channels
	struct rewind_functor
	{
		void operator()(const ChannelItem& p) const
		{ 
			p->rewind();
		}
	};
	
	//!	clears sequence
	struct clear_seq_functor
	{
		void operator()(const ChannelItem& p) const
		{ 
			p->clear();
		}
	};

	//!	dumps MorphTarget objects
	struct dump_functor
	{
		void operator()(const ChannelItem& p) const
		{ 
			p->dumpDebugInfo();
		}
	};

	// this is a singleton class
	static MorphController* m_pThis;
	~MorphController(void);
	MorphController();

	MorphBlender m_blender;
	Transform m_transform;
	MorphDictionary m_dictionaries;
	Entity m_result;
	std::list<boost::shared_ptr<MorphChannel> > m_channels;
public:
	void clearDictionary();
	void clearAllChannels();
	MorphBlender* const getBlender() {return &m_blender;}
	void dumpDebugInfo() const;
	void setFDP(boost::shared_ptr<XFace::FDP> pFDP) {m_blender.setFDP(pFDP); reset();}	
	const Transform& getTransform() const {return m_transform;}
	void setTransform(const Transform& tr) {m_transform = tr;}
	void processDictionary();
	void addChannel(const std::string& name);
	void addChannel(boost::shared_ptr<MorphChannel> ch);
	void next();
	void rewind();
	void reset();
	const Entity& getResult() const {return m_result;}
	const Entity& getDictionaryItem(const std::string& key);
	MorphDictionary getDictionary() const;
	void removeDictionaryTarget(const std::string& key);
	void addDictionaryTarget(const std::string& al, const std::string& cat, const Entity& ent);

	void clearChannel(const std::string& channel);
	void pushTarget(MorphTarget* pTrg, const std::string& channel);
	void pushTargets(const MorphTargetList& targets, const std::string& channel);
	void update(unsigned int elapsed_time);
	ChannelItem findChannel(const std::string& name);
	//! singleton access
	static MorphController* getInstance();
	static void releaseInstance();
};

} // namespace XEngine

#endif // MORPHCONTROLLER_H_