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
	\file		SMILManager.h
	\brief		Main SMIL-Agent processing is controlled through here.
*/

#pragma once

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#define USE_XSAPI 0 
#define USE_ITALIAN_FLITE 0
#if (USE_XSAPI)
	#include "XSAPI.h"
#else 
	#include "ITTSEngine.h"
#endif
#include "SMILObjects.h"
#include "SGNode.h"

using namespace XERCES_CPP_NAMESPACE;
/*!
	\brief Main SMIL-Agent processing is controlled with this class.
	\author Koray Balci

	This class is responsible for parsing and interpreting the SMIL-Agent
	script. SMILProcessor class, which is derived from IScriptProcessor uses
	this class to manage the processing. 

	After parsing the script using xerces XML parser and creating a tree of nodes,
	proper visitors to the tree are applied and outputs (wave file, animation file,
	phoneme file) are created. Then, we are all set for rendering the animation.
*/
class SMILManager
{
	typedef std::map<std::string, boost::shared_ptr<ITTSEngine> > TTSEngineMap;
	TTSEngineMap m_TTSEngines;
	SGNode m_rootNode;
	boost::shared_ptr<SGGroup> m_tempGroup;

	std::string getExtlessFilename(const std::string & filename);

	std::stringstream m_errorString;

	void post_process();
	void process_speech(DOMNode* pSpeech);
	void process_block(DOMNode* pSeq, SMILBlock::BLOCKTYPE type);

	std::string m_currentLang, m_currentChannel;
	typedef std::map<std::string, float> SpeechMarkMap;
	SpeechMarkMap m_speechMarks;
	
	std::string m_contentName;
	bool parse(DOMNode* pHead);
	bool loadInput(const InputSource& input);
	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *m_pDoc;
	XercesDOMParser *m_pParser;
public:
	std::string getErrorString(bool clear = true);
	std::string getLanguage() const;
	std::string loadScript(const std::string& script);
	std::string loadScript(std::istream& input);
	SMILManager(std::map<std::string, boost::shared_ptr<ITTSEngine> > TTSEngines);
	~SMILManager(void);
};

