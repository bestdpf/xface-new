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

#include "SMILProcessor.h"

#include "SMILManager.h"
#if (USE_XSAPI)
	#include "XSAPI.h"
#endif
#if (USE_ITALIAN_FLITE)
	#include "ItalianFlite.h"
#endif

SMILProcessor::SMILProcessor(const std::string& name) : IScriptProcessor(name)
{
#if (USE_XSAPI)
	// this is a bad hack to make pluggable factory work!
	SAPI51 sapi("");
#endif
#if (USE_ITALIAN_FLITE)
	ItalianFlite flite("null");
#endif
}

SMILProcessor::~SMILProcessor(void)
{
}

bool SMILProcessor::process(const std::string& scriptString, const std::string& filenameUnused)
{
	m_errorString.clear();
	SMILManager man(m_TTSEngines);
	std::string filename = man.loadScript(scriptString);
	m_lang = man.getLanguage(); // base class needs this
	m_errorString = man.getErrorString();

	if(filename.empty())
		return false;

	// change the output filenames accordingly..
	std::vector<std::string>::iterator it = m_outputFileNames.begin();
	while(it != m_outputFileNames.end())
	{
		std::string dummy = filename;
		std::string::size_type index = it->find_last_of(".", it->size());
		if ( index != std::string::npos )
		{
			std::copy(it->begin() + index, it->end(), std::back_inserter(dummy));
			*it = dummy;
		}
		
		++it;
	}
	return true;
}