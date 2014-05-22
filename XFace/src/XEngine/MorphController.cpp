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
#include <XEngine/MorphController.h>
#include <iostream>


namespace XEngine{

/*!
	Singleton class accessor for the one and only instance of this class.
*/
MorphController* MorphController::m_pThis = 0;
MorphController* MorphController::getInstance()
{
	if (!m_pThis)
		m_pThis = new MorphController();
	
	return m_pThis;
}

/*!
    Releases the one and only instance.
*/
void MorphController::releaseInstance()
{
    delete m_pThis;
    m_pThis = 0;
}

MorphController::MorphController() 
	: ITimedController()
{
}

MorphController::~MorphController(void)
{
	clearAllChannels();
}

MorphController::ChannelItem MorphController::findChannel(const std::string& name)
{
	std::list<ChannelItem >::const_iterator it = m_channels.begin();
	while(it != m_channels.end())
	{
		if((*it)->getName() == name)
			return *it;
		++it;
	}
	return ChannelItem();
}

void MorphController::reset()
{
	clearAllChannels();
	m_result.release(true);
	m_result.copyFrom(getDictionaryItem("Rest"), true);
}

void MorphController::clearDictionary() 
{
	m_dictionaries.clear();
}

void MorphController::clearAllChannels() 
{
	std::for_each(m_channels.begin(), m_channels.end(), clear_seq_functor());
	m_channels.clear();
}

void MorphController::clearChannel(const std::string& channel) 
{
	ChannelItem ch = findChannel(channel);
	if(ch)
		ch->clear();
}

void MorphController::pushTarget(MorphTarget* pTrg, const std::string& channel) 
{
	ChannelItem ch = findChannel(channel);
	if(ch)
		ch->pushTarget(pTrg);
}

void MorphController::pushTargets(const MorphTargetList& targets, const std::string& channel) 
{
	ChannelItem ch = findChannel(channel);
	if(ch)
		ch->pushTargets(targets);
} 

const Entity& MorphController::getDictionaryItem(const std::string& key)
{
	MorphDictionary::const_iterator it_dict = m_dictionaries.begin();
	while(it_dict != m_dictionaries.end())
	{
		std::map<std::string, Entity>::const_iterator it_ent = it_dict->second.find(key);
		if(it_ent != it_dict->second.end()) // if we found
			return it_ent->second;
		else
			++it_dict;
	}
	static Entity entInvalid;
	return entInvalid;
}

void MorphController::rewind()
{
	std::for_each(m_channels.begin(), m_channels.end(), rewind_functor());
}

void MorphController::next()
{
	std::for_each(m_channels.begin(), m_channels.end(), next_functor());
}

void MorphController::update(unsigned int elapsed_time)
{
	if(!elapsed_time)
		return;
	Entity rest = getDictionaryItem("Rest");
	
	std::list<ChannelItem >::iterator it = m_channels.begin();
	while (it != m_channels.end())
	{	
		(*it)->update(m_result, rest, elapsed_time);
		++it;
	}
}

void MorphController::addDictionaryTarget(const std::string& al, const std::string& cat, const Entity& ent) 
{
	MorphDictionary::iterator it_dict = m_dictionaries.find(cat);
	if(it_dict == m_dictionaries.end())
		it_dict = m_dictionaries.insert(m_dictionaries.end(), 
			std::make_pair(cat, std::map<std::string, Entity>()));

	it_dict->second.insert(std::make_pair(al, ent));
}

void MorphController::removeDictionaryTarget(const std::string& key) 
{
	MorphDictionary::iterator it = m_dictionaries.begin();
	while (it != m_dictionaries.end())
	{
		std::map<std::string, Entity>::iterator it_key = it->second.find(key);
		if(it_key != it->second.end())
			it->second.erase(it_key);		
		++it;
	}	
}

MorphController::MorphDictionary MorphController::getDictionary() const 
{
	return m_dictionaries;
}

// Dict processing methods
///////
void MorphController::processDictionary()
{
	MorphDictionary::const_iterator it_dict = m_dictionaries.find("Viseme");
	if(it_dict != m_dictionaries.end())
		m_blender.prioritizeChannel(it_dict->second, getDictionaryItem("Rest"));
}

void MorphController::addChannel(const std::string& name)
{
	ChannelItem channel = findChannel(name);
	if(!channel)
		m_channels.push_back(boost::shared_ptr<MorphChannel>(new MorphChannel(name)));
}

void MorphController::addChannel(boost::shared_ptr<MorphChannel> ch)
{
	ChannelItem channel = findChannel(ch->getName());
	if(!channel)
		m_channels.push_back(ch);
	else
		ch.reset();
}

void MorphController::dumpDebugInfo() const
{
	std::for_each(m_channels.begin(), m_channels.end(), dump_functor());
}
} // namespace XEngine
