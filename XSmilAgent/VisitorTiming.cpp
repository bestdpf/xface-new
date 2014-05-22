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
 * The Original Code is XSmilAgent
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include "VisitorTiming.h"
#include <iostream>
#include <vector>
#include <strstream>
#include <boost/tokenizer.hpp>

std::stack<boost::shared_ptr<SGGroup> > VisitorTiming::m_parentStack;
std::map<std::string, SMILSpeech> VisitorTiming::m_speechMap;

/*!
	SMILSpeech visitor method
*/
void VisitorTiming::operator()(SMILSpeech& node) const
{
	// apply delay (begin/end/dur tag) for itself
	applyOwnDelay(node);
	// apply any propagated delay
	applyParentDelay(node);
	// update the parent beginVal/endVal
	updateParent(node);

	// correct mark time stamps (were 0 until here)
	std::vector<SMILSpeechContent>::iterator it_prev = node.content.end(), it_c = node.content.begin();
	float begin = node.synch.beginVal;
	while (it_c != node.content.end())
	{
		if (it_c->type == SMILSpeechContent::kMark)
		{
			begin = resolveMark(node.performance.id + it_c->value) + node.synch.beginVal;
		}
		else if (it_c->type == SMILSpeechContent::kText)
		{
			it_c->beginVal = begin;
			if(it_prev != node.content.end())
				it_prev->endVal = begin;
			it_prev = it_c;
		}
		
		++it_c;
	}
	if(it_prev != node.content.end())
		it_prev->endVal = node.synch.endVal;

	// if there is a delay (or this is not the first speech in the script)
	if(node.synch.beginVal > 0)
	{
		// propagate the delay to mark values as well
		SpeechMarkMap marks = static_cast<SpeechMarkMap>(m_speechMarks);
		SpeechMarkMap::iterator it = marks.begin();
		while(it != marks.end())
		{
			it->second += node.synch.beginVal;
			++it;
		}
	}
	// add this speech to the map for use with SMIL nodes that referenced this speech node
	m_speechMap.insert(std::make_pair(node.performance.id, node));
}

/*!
	SMILReferenced visitor method
*/
void VisitorTiming::operator()(SMILReferencedBlock& node) const
{
	// if there is a reference, use it
	if(!node.content_id.empty())
		applyReference(node);
	else
	{
		// treat it as a normal node
		applyOwnDelay(node);
		applyParentDelay(node);
		updateParent(node);
	}
}

void VisitorTiming::operator()(SMILBlock& node) const
{
	// treat it as a normal node
	applyOwnDelay(node);
	applyParentDelay(node);
	updateParent(node);
}

/*!
	SGGroup visitor method
*/
void VisitorTiming::operator()(boost::shared_ptr<SGGroup>& node) const
{
	// apply the delays from own and parent
	applyOwnDelay(*node);
	applyParentDelay(*node);
	
	// this node is the current parent now
	m_parentStack.push(node);
	
	//process children
	std::for_each(node->m_children.begin(), node->m_children.end()
		,boost::apply_visitor(*this));
	
	// go back to previous parent block content
	m_parentStack.pop();

	// update that parent
	updateParent(*node);
}

/*!
	Apply any referenced timing 
*/
void VisitorTiming::applyReference(SMILReferencedBlock& node) const
{
	// check if there is a reference to some SMILSpeech content
	// if any, then discard previous beginVal/endVal and update accordingly
	if(!node.content_id.empty())
	{
		// update beginVal
		if(!node.content_begin.empty())
			node.synch.beginVal = resolveMark(node.content_id + node.content_begin);
		else
		{
			// set the referenced speech begin value as begin for this node.
			std::map<std::string, SMILSpeech>::const_iterator it = 
						m_speechMap.find(node.content_id);
			if(it != m_speechMap.end())
				node.synch.beginVal = it->second.synch.beginVal;
		}
		
		// update endVal
		if(!node.content_end.empty())
			node.synch.endVal = resolveMark(node.content_id + node.content_end);
		else
		{
			// set the referenced speech end value as end for this node.
			std::map<std::string, SMILSpeech>::const_iterator it = 
						m_speechMap.find(node.content_id);
			if(it != m_speechMap.end())
				node.synch.endVal = it->second.synch.endVal;
		}
	}
}

/*!
	Update the parent node using values from the current node.
*/
void VisitorTiming::updateParent(SMILBlock& node) const
{
	// if we have a parent
	if(!m_parentStack.empty())
	{
		if (m_parentStack.top()->type == SMILBlock::kSeq)		// if it's a sequence node
		{
			// only update the end value
			m_parentStack.top()->synch.endVal = node.synch.endVal;
		}
		else if (m_parentStack.top()->type == SMILBlock::kPar)	// if it's a parallel node
		{
			// get the minimum of the current value and this node for beginVal
			m_parentStack.top()->synch.beginVal = std::min(m_parentStack.top()->synch.beginVal, node.synch.beginVal);
			// get the maximum of the current value and this node for endVal
			m_parentStack.top()->synch.endVal = std::max(m_parentStack.top()->synch.endVal, node.synch.endVal);
		}
	}
}

/*!
	If there is a delay from the parent, apply.
*/
void VisitorTiming::applyParentDelay(SMILBlock& node) const
{
	// if we have a parent
	if(!m_parentStack.empty())
	{
		// update the beginVal/endVal to parent beginVal delay
		node.synch.beginVal += m_parentStack.top()->synch.beginVal;
		node.synch.endVal	+= m_parentStack.top()->synch.beginVal;
		// calculate current duration of the parent
		float dur = m_parentStack.top()->synch.endVal - m_parentStack.top()->synch.beginVal;
		if (m_parentStack.top()->type == SMILBlock::kSeq)
		{
			// if it's a sequence, we should shift both beginVal/endVal
			node.synch.beginVal += dur;
			node.synch.endVal	+= dur;
		}
		else if (m_parentStack.top()->type == SMILBlock::kPar)
		{
			/*	if it's a parallel, we should only move the endVal
			 // consider the case of a speech and speech-animation in a par:
			 // we update the beginVal from parent, but endVal should match to the speech endVal.
			 // speech node already should have updated parent endVal (and thus the duration).
			 // so we use this duration to update correctly this node's (speech-animation) duration/endVal.
			*/
			node.synch.endVal	+= dur;
		}
	}
}

void VisitorTiming::applyOwnDelay(SMILBlock& node) const
{
	// get the begin-end-dur content (returns 0 if there is none)
	float begin = decodeTime(node.synch.begin);
	float end	= decodeTime(node.synch.end);
	float dur	= decodeTime(node.synch.dur);

	// update beginVal
	if(begin > 0)
		node.synch.beginVal += begin;

	// update endVal
	if(end > 0 && dur > 0)
		node.synch.endVal += std::min(end, begin + dur);
	else if (dur > 0) // needed for action only scripts
		node.synch.endVal += dur;
	else if (end > 0)
		node.synch.endVal += end;
	else if(begin > 0)
		node.synch.endVal += begin;
}

/*!
	\note resolves only seconds and miliseconds, do not use minutes, etc..
	This function resolves the timing values for begin/end/dur attributes. 
	It is not in a good shape, not robust, have to be worked out again.
*/
float VisitorTiming::decodeTime(const std::string& str) const
{
	if(str.empty())
		return 0;

	float val = -1;
	if( !(((str[0] >= '0') && (str[0] <= '9')) || (str[0] == '.')))
		return -1;

	// tokenize time, example "2:33ms" 
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(":");
	tokenizer tokens(str, sep);
	std::vector<std::string> slots;
	for (tokenizer::iterator tok_iter = tokens.begin();
		tok_iter != tokens.end(); ++tok_iter)
		slots.push_back(*tok_iter);

	std::vector<std::string>::reverse_iterator it = slots.rbegin();
	std::strstream t, v;
	bool first = true;
	while(it != slots.rend())
	{
		if(first)
		{
			std::string::iterator it_str = it->begin();
			while(it_str != it->end())
			{
				if ( ((*it_str >= '0') && (*it_str <= '9')) || (*it_str == '.') )
					v << *it_str;
				else
					t << *it_str;
	
				++it_str;
			}
			t << '\0';
			std::string tt = t.str();
			val = (float)atof(v.str());
			if(tt == "s" || tt.empty())
				val *= 1000;
			else // it is in ms (only other option for now!
			{
				if (val /10.0f < 1.0f) // xx:1ms is xx100
					val *= 100;
				else if ((val / 100.0f) < 1.0f) // xx:17ms is xx170
					val *= 10;
				// xx:1712ms is error but not handled!
			}
			first = false;
		}
		else
		{
			val += (float)atof(it->c_str())*1000;
		}
		++it;
	}
	
	return val;
}

float VisitorTiming::resolveMark(const std::string& mark) const
{
	if(mark.empty())
		return 0;
		
	// find the coresponding begin or end timestamp.
	SpeechMarkMap::const_iterator it = m_speechMarks.find(mark);
	if(it != m_speechMarks.end())
		return it->second;
	
	return 0;
}

void VisitorTiming::reset()
{
	while(!m_parentStack.empty())
		m_parentStack.pop();
	
	m_speechMap.clear();
}
