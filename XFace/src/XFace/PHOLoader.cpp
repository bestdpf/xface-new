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
 * The Original Code is Xface Core Library; XFace.
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

#include <XFace/PHOLoader.h>

namespace XFace{

std::list<std::pair<std::string, unsigned int> > PHOLoader::load(const std::string& filename)
{
	std::ifstream input(filename.c_str());
	std::list<std::pair<std::string, unsigned int> > retVal = load(input);
	input.close();

	return retVal;
}

std::list<std::pair<std::string, unsigned int> > PHOLoader::load(std::istream& input)
{
	std::list<std::pair<std::string, unsigned int> > phoList;
	std::string alias;
	float duration, prev_duration = 0;
	m_total_duration = 0;

	while ( !input.eof() ) // input have a last empty line, but it is parsed as 0 duration silence anyways, so no problem
	{
		input >> alias >> m_total_duration;
		duration = m_total_duration - prev_duration;
		if( duration > 0.000001f )
		{
			phoList.push_back(std::make_pair(alias, (unsigned int)(duration*1000.0f))); // duration is in ms
		}
		prev_duration = m_total_duration;
	}
	return phoList;
}

}// namespace XFace