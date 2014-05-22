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
 * For info, contact: xface-info@fbk.eu or http://xface.fbk.eu
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include "VisitorSpeech.h"
#include <strstream>

std::string VisitorSpeech::m_outFilename;
std::string VisitorSpeech::m_script;
float VisitorSpeech::m_time = 0;

/*!
	Clear the script and reset the global time to 0.
*/
void VisitorSpeech::reset()
{
	m_outFilename.clear();
	m_script.clear();
	m_time = 0;
}


/*!
	This method exists for traversing the children of the node properly,
	traversing is propagated here.
*/
void VisitorSpeech::operator()(const boost::shared_ptr<SGGroup>& node) const
{
	std::for_each(node->m_children.begin(), node->m_children.end(),
		boost::apply_visitor(*this));
}

std::string VisitorSpeech::getExtlessFilename(const std::string & filename)
{
	std::string extlessName;
	std::string::size_type index = filename.find_last_of(".", filename.size());
	if ( index != std::string::npos )
		std::copy(filename.begin(), filename.begin() + index , std::back_inserter(extlessName));
	
	return extlessName;
}

/*!
	Basic (base class) implemntation of SMILSpeech visitor only concatenates
	the contents of all SMILSpeech (speech tag) nodes. If there are markers, 
	they are ignored.
*/
void VisitorSpeech::operator()(const SMILSpeech& obj) const
{
	// if we have a filename reference and not a text to synthesize
	if(!obj.filename.empty())
	{
		m_time += obj.synch.endVal - obj.synch.beginVal;
		m_outFilename = getExtlessFilename(obj.filename);
	}
	else
		m_outFilename = obj.performance.id;


	// create the text to be synthesized
	std::vector<SMILSpeechContent>::const_iterator it = obj.content.begin();
	while (it != obj.content.end())
	{
		if (it->type == SMILSpeechContent::kText)
		{
			std::copy(it->value.begin(), it->value.end(), std::back_inserter(m_script));
			m_script.append(" ");
		}
		++it;
	}
	
	m_time += obj.synch.endVal - obj.synch.beginVal;
}
