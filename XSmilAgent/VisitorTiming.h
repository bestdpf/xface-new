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
/*!
	\file VisitorTiming.h
	\brief visitor for timing calculations.
*/
#pragma once

#include "SGNode.h"
#include <map>
#include <stack>

/*!
	\brief visitor for timing calculations.
	\author Koray Balci

	This class is a visitor class for the node tree parsed from the script.

	The job of this class is to visit the relevant nodes in the tree and update 
	the timing information. It checks the parent/children relationships, delays,
	references. One of the most complicated part of the whole SMIL-Agent 
	processing is implemented here. 
*/
class VisitorTiming
    : public boost::static_visitor<>
{
	typedef std::map<std::string, float> SpeechMarkMap;
    SpeechMarkMap m_speechMarks;
	static std::stack<boost::shared_ptr<SGGroup> > m_parentStack;
	static std::map<std::string, SMILSpeech> m_speechMap;
	
	float decodeTime(const std::string& str) const;
	float resolveMark(const std::string& mark) const;

	void applyReference(SMILReferencedBlock& node) const;
	void updateParent(SMILBlock& node) const;
	void applyParentDelay(SMILBlock& node) const;
	void applyOwnDelay(SMILBlock& node) const;
public:
	static void reset();
	VisitorTiming(const SpeechMarkMap& speechMarks) : m_speechMarks(speechMarks){}
	void operator()(SMILBlock& node) const;
	void operator()(SMILSpeech& node) const;
	void operator()(SMILReferencedBlock& node) const;
    void operator()(boost::shared_ptr<SGGroup>& node) const;
};