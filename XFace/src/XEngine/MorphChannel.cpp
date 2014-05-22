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
#include <iostream>
#include <XEngine/MorphChannel.h>
#include <XEngine/MorphController.h>


namespace XEngine{

MorphChannel::MorphChannel() : m_totalTimePassed(0), m_unfinishedFrame(0)
{
}

MorphChannel::MorphChannel(const std::string& name) : m_name(name), m_totalTimePassed(0), m_unfinishedFrame(0)
{
}

MorphChannel::~MorphChannel(void)
{
	clear();
}

/*!
*/
void MorphChannel::clear() 
{
	std::for_each(m_targetSequence.begin(), m_targetSequence.end(), destroy_functor());
	m_targetSequence.clear();
	m_current = m_next = m_targetSequence.begin();
	m_result.release(true);
	m_totalTimePassed = 0;
	m_unfinishedFrame = 0;
}

bool MorphChannel::updateSelf(unsigned int elapsed_time)
{
	// if it's really early, do nothing
	if(elapsed_time < 1)
		return false;

	// update the time
	m_totalTimePassed += elapsed_time;
		
	// if the sequence is empty or we are at the end, return the last/empty entity
	if(m_next == m_targetSequence.end() || m_targetSequence.empty())
		return borderFix();
	
	if(m_totalTimePassed < (*m_current)->getStart())	// if it's not time yet
		return borderFix();
	
	if(m_totalTimePassed >= (*m_current)->getStart())	
	{
		while(m_totalTimePassed > (*m_current)->getEnd())	// time's up, go to next
		{
			m_current = m_next++;
			if(m_next == m_targetSequence.end())
				return borderFix();
		}
	}
	if(m_totalTimePassed < (*m_current)->getStart())	// if we are in a void space
		return borderFix();
	
	// do the interpolation
	float weight = ((float)m_totalTimePassed - (float)(*m_current)->getStart()) / (float)(*m_current)->getDuration();
	
	KeyframeInterpolator::interpolate((*m_current)->getEntity(), (*m_next)->getEntity(), m_result, weight);
	m_unfinishedFrame = &(*m_next)->getEntity();

	return true;
}

bool MorphChannel::borderFix()
{
	if(m_unfinishedFrame)
	{
		m_result.copyFrom(*m_unfinishedFrame, true);
		m_unfinishedFrame = 0;
	}
	return false;
}

bool MorphChannel::updateResult(Entity& result, const Entity& rest)
{
	if(m_result.getDrawableCount())
		result = m_result; // do not try copyFrom, creates problems..
	else
		return false;
	return true;
}

// sequence operations
bool MorphChannel::update(Entity& result, const Entity& rest, unsigned int elapsed_time)
{
	if(updateSelf(elapsed_time))
		return updateResult(result, rest);
	
	return false;
}

void MorphChannel::next()
{
	if(m_next != m_targetSequence.end())
		m_current = m_next++;
	else
		rewind();
}

void MorphChannel::rewind()
{
	m_current = m_next = m_targetSequence.begin();
	if(!m_targetSequence.empty())
	{
		++m_next;
		m_result.release(true);
		//m_result.copyFrom((*m_current)->getEntity(), true);
	}
	m_totalTimePassed = 0;
}

MorphTarget* MorphChannel::smoothTarget(MorphTarget* pTrg)
{
	if(pTrg->getAlias() == "Rest")
		return pTrg;
	Entity toInsert;
	MorphController* controller = MorphController::getInstance();
	if(pTrg->getWeight() < 1.0f)
	{
		Entity restEnt = controller->getDictionaryItem("Rest");
		if(controller->getDictionaryItem(pTrg->getAlias()).getDrawableCount() == 0)
			return pTrg;
	
		if(pTrg->getEntity().getDrawableCount() == 0)
			pTrg->setEntity(controller->getDictionaryItem(pTrg->getAlias()));
	
		toInsert.copyFrom(controller->getDictionaryItem(pTrg->getAlias()), true);
		KeyframeInterpolator::interpolate(restEnt, pTrg->getEntity(), toInsert, pTrg->getWeight());
		pTrg->setEntity(toInsert);
	}
	else
	{
		// if not yet initialized
		if(pTrg->getEntity().getDrawableCount() == 0)
		{
			toInsert.copyFrom(controller->getDictionaryItem(pTrg->getAlias()), true);
			pTrg->setEntity(toInsert);
		}
	}

	return pTrg;
}

void MorphChannel::pushTarget(MorphTarget* pTrg) 
{
	// if the target sequence empty, we should initialize m_current to the beginning as well
	bool rew = m_targetSequence.empty();

	m_targetSequence.push_back(smoothTarget(pTrg));
	
	if(rew)
		rewind();
}

void MorphChannel::pushTargets(const MorphTargetList& targets) 
{
	// if the target sequence empty, we should initialize m_current to the beginning as well
	bool rew = m_targetSequence.empty();
	
	MorphTargetList::const_iterator it = targets.begin();
	while (it != targets.end())
	{
		smoothTarget(*it);
		m_targetSequence.push_back(*it);
		++it;
	}
//	m_targetSequence.insert(m_targetSequence.end(), targets.begin(), targets.end());
		
	if(rew)
		rewind();
} 

void MorphChannel::dumpDebugInfo() const
{
	std::cerr << "Channel Name: " << m_name << std::endl;
	std::for_each(m_targetSequence.begin(), m_targetSequence.end(), dump_functor());
	if(m_current != m_targetSequence.end())
		std::cerr << "\tcurrent: " << (*m_current)->getAlias();
	if(m_next != m_targetSequence.end())
		std::cerr << " next: " << (*m_next)->getAlias();
	std::cerr << std::endl;
}
} // namespace XEngine
