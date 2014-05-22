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
 * The Original Code is XFace Core
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

#include <iostream>
#include <fstream>
#include <cassert>

#include <XFace/PhonemeDictionary.h>

namespace XFace{

std::string PhonemeDictionary::loadDictionary(const std::string& filename)
{
	std::string retVal;
	std::ifstream input(filename.c_str());
	if(!input.is_open())
		return retVal;
	
	retVal = loadDictionary(input);
	input.close();

	return retVal;
}

std::string PhonemeDictionary::loadDictionary(std::istream& input)
{
	std::map<std::string, std::string> dict;
	std::string src, trg, lang;
	
	input >> lang;
	while ( !input.eof() ) // input have a last empty line, but it is parsed as 0 duration silence anyways, so no problem
	{
		input >> src >> trg;
		dict.insert(std::make_pair(src, trg));
	}
	m_dictionaries.insert(std::make_pair(lang, dict));
	
	return lang;
}
	
const PhonemeDictionary::DictionaryContent& PhonemeDictionary::getDictionary(const std::string& lang) const
{
	static DictionaryContent invalidVal;
	assert(!m_dictionaries.empty());
	DictionaryMap::const_iterator it = m_dictionaries.find(lang);
	if(it != m_dictionaries.end())
		return it->second;
	else
		return invalidVal;
}

} // namespace XFace