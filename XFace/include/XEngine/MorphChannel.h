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
 * The Original Code is XEngine
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */
#pragma once

#include <string>
#include <map>
#include <algorithm>
#include "Entity.h"
#include <XEngine/MorphChannel.h>
#include <XEngine/MorphTarget.h>
#include <XEngine/KeyframeInterpolator.h>

namespace XEngine{

class MorphChannel
{
protected:
	/*!
		dumps MorphTarget objects
	*/
	struct dump_functor
	{
		void operator()(MorphTarget* pTrg) const
		{ 
			std::cerr << "\t" << pTrg->getAlias() << " start: " << pTrg->getStart() << "\tend: " << pTrg->getEnd()<< std::endl;
		}
	};

	/*!
		destroys MorphTarget objects (pointers)
	*/
	struct destroy_functor
	{
		void operator()(MorphTarget* pTrg)
		{ 
			pTrg->getEntity().release(true);
			delete pTrg;
		}
	};

	/*!
		smooths MorphTarget objects with the "Rest" keyframe
	
	struct smooth_functor
	{
		smooth_functor(const Entity& rest_) 
			: restEnt(rest_){}
		void operator()(MorphTarget* pTrg) const
		{
			if(pTrg->getWeight() < 1.0f && pTrg->getAlias() != "Rest")
			{
				Entity toInsert;
				toInsert.copyFrom(restEnt, true);
				interpolator.interpolate(restEnt, pTrg->getEntity(), toInsert, pTrg->getWeight());
				pTrg->setEntity(toInsert);
			}
		}
	private:
		KeyframeInterpolator interpolator;
		Entity restEnt;
	};
*/
	
	std::string m_name;
	unsigned int m_totalTimePassed;

	// sequence member vars
	typedef std::list<MorphTarget*> MorphTargetList;
	MorphTargetList m_targetSequence;
	MorphTargetList::iterator m_current, m_next;
	Entity m_result;

	MorphTarget* smoothTarget(MorphTarget* pTrg);
	Entity* m_unfinishedFrame;
	bool borderFix();
	virtual bool updateSelf(unsigned int elapsed_time);
	virtual bool updateResult(Entity& result, const Entity& rest);
public:
	const MorphTarget* const getCurrentTarget() const {return *m_current;}
	void dumpDebugInfo() const;
	size_t getTargetCount() const {return m_targetSequence.size();}
	const std::string& getName() const {return m_name;}

	bool update(Entity& result, const Entity& rest, unsigned int elapsed_time);
	void next();
	void rewind();
	void pushTarget(MorphTarget* pTrg);
	void pushTargets(const MorphTargetList& targets);
	virtual void clear();

	MorphChannel(const std::string& name);
	MorphChannel();
	virtual ~MorphChannel(void);
};

} // namespace XEngine