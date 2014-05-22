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
 * The Original Code is XSmilAgent.
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

#include "SMILManager.h"

#include "XercesString.h"
#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include "SMILManager.h"
#include "VisitorTiming.h"
#include "VisitorInfoPrinter.h"
#include "VisitorSpeech.h"
#include "VisitorAnimCreator.h"
#include "VisitorSAPISpeech.h"
#ifdef WIN32
	#include "ItalianFlite.h"
	#include "Wave.h"	// Windows dependent file!
#else
	#include "Wave2.h"
#endif

#ifdef _DEBUG
#pragma message("		Adding Xerces lib (Debug)...")
#pragma comment(lib, "xerces-c_2D")
#else
#pragma message("		Adding Xerces lib (Release)...")
#pragma comment(lib, "xerces-c_2")
#endif

SMILManager::SMILManager(std::map<std::string, boost::shared_ptr<ITTSEngine> > TTSEngines)  : m_pParser(0), m_pDoc(0)
{
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		// Do your failure processing here
		std::cerr << "SMILManager: Cannot start Xerces parser!" << XercesString(toCatch.getMessage()) << std::endl;
	}

	m_TTSEngines = TTSEngines;
}

SMILManager::~SMILManager(void)
{
	delete m_pParser;
	XMLPlatformUtils::Terminate();
}

std::string SMILManager::loadScript(std::istream& input)
{
	std::string xmlString;
	// get the input stream contents to a string
	while (!input.eof())
	{
		std::string dummy;
		input >> dummy;
		xmlString.append(dummy);
	}

	MemBufInputSource source((const unsigned char*)xmlString.c_str(), (const unsigned int)xmlString.size(), "44");
	if(!loadInput(source))
		m_contentName.clear();

	return m_contentName;
}

std::string SMILManager::loadScript(const std::string& script)
{
	MemBufInputSource source((const unsigned char*)script.c_str(), (const unsigned int)script.size(), "43");
	if(!loadInput(source))
		m_contentName.clear();

	return m_contentName;
}

bool SMILManager::loadInput(const InputSource& input)
{
	m_contentName = "test";
	m_errorString.str("");
	bool bFailed = false; // assume all will go well
	// create new parser instance.
	delete m_pParser;
	m_pParser = new XercesDOMParser;
	if (m_pParser)
	{
		m_pParser->setValidationScheme(XercesDOMParser::Val_Auto);
		m_pParser->setDoNamespaces(false);
		m_pParser->setDoSchema(false);

		// skip this if you haven't written your own error reporter class.
//		DOMTreeErrorReporter *errReporter = new DOMTreeErrorReporter;
//		parser->setErrorHandler(errReporter);

		m_pParser->setCreateEntityReferenceNodes(false);
		m_pParser->setIncludeIgnorableWhitespace(false);
		//parser->setToCreateXMLDeclTypeNode(true);
		try
		{
			m_pParser->parse(input);
			bFailed = m_pParser->getErrorCount() != 0;
			if (bFailed)
				std::cerr << "SMILManager: Parsing error id: " << m_pParser->getErrorCount() << std::endl;
		}
		catch (const DOMException& e)
		{
			std::cerr << "SMILManager: DOM Exception parsing reports: ";
			// was message provided?
			if (e.msg)
			{
				// yes: display it as ascii.
				char *strMsg = XMLString::transcode(e.msg);
				std::cerr << strMsg << std::endl;
				XMLString::release(&strMsg);
			}
			else
				// no: just display the error code.
				std::cerr << e.code << std::endl;

			bFailed = true;
		}
		catch (const XMLException& e)
		{
			std::cerr << "SMILManager: XML Exception parsing reports: " << e.getMessage() << std::endl;
			bFailed = true;
		}
		catch (...)
		{
			std::cerr << "SMILManager: An exception occured while parsing." << std::endl;
			bFailed = true;
		}
		// did the input document parse okay?
		if (!bFailed)
		{
			m_pDoc = m_pParser->getDocument();
			m_tempGroup.reset(new SGGroup);
			m_rootNode = m_tempGroup;
	
			bFailed = !parse(m_pDoc);
			// now the post-process..
			if(!bFailed)
				post_process();
		}
		else
			m_errorString << "A syntactical error occured while trying to parse SMIL-Agent script (XML parse error)." << std::endl;
	}

	return !bFailed;
}

void SMILManager::process_block(DOMNode* pBlock, SMILBlock::BLOCKTYPE type)
{
	if(!pBlock)
		return;

	// get the attributes 
	boost::shared_ptr<SGGroup> obj(new SGGroup);
	obj->read(pBlock);
	obj->type = type;
	if(!obj->performance.tests.system.language.empty())
		m_currentLang = obj->performance.tests.system.language;
	else
		obj->performance.tests.system.language = m_currentLang;

	if(!obj->performance.channel.empty())
		m_currentChannel = obj->performance.channel;

	boost::shared_ptr<SGGroup> oldGr = m_tempGroup;
	m_tempGroup->addChild(obj);
	m_tempGroup = obj;

	parse(pBlock);
	m_tempGroup = oldGr;
}

void SMILManager::post_process()
{
	boost::apply_visitor(VisitorTiming(m_speechMarks), m_rootNode);
	VisitorTiming::reset();

	// process speech
	std::string phoName;
	float speechDur = 0;
	VisitorSpeech* pSpeechVisitor = 0;
	TTSEngineMap::iterator it = m_TTSEngines.find(m_currentLang);
	if(it != m_TTSEngines.end())
	{
		pSpeechVisitor = it->second->getSmilVisitor();
		if(!pSpeechVisitor)
			std::cerr << "no visitor\n";
		else
		{
			boost::apply_visitor(*pSpeechVisitor, m_rootNode);
			m_contentName = pSpeechVisitor->getOutputFilename();
			
			if(!pSpeechVisitor->getScript().empty())
			{
				it->second->speak(pSpeechVisitor->getScript(), m_contentName);
				speechDur = (float)it->second->getDuration();
			}
			else /*if(pSpeechVisitor->getTime() > 0)*/
				speechDur = pSpeechVisitor->getTime();

			pSpeechVisitor->reset();
		}
	}
	
	VisitorAnimCreator anim(m_contentName);
	if(speechDur > 0)
		VisitorAnimCreator::addSpeech((m_contentName + ".pho"), (int)speechDur, getLanguage());
	boost::apply_visitor(anim, m_rootNode);
	if(true) // change it with sth clever later!
		VisitorAnimCreator::blinkEyes(5000);
	VisitorAnimCreator::finalize();
/*	std::ofstream fp("sapi.txt");
	std::string dummy2;
	std::copy(dummy.begin(), dummy.end(), std::back_inserter(dummy2));
	fp << dummy2;
	fp.close();
*/	
//	boost::apply_visitor(VisitorInfoPrinter(), m_rootNode);
}

std::string SMILManager::getExtlessFilename(const std::string & filename)
{
	std::string extlessName;
	std::string::size_type index = filename.find_last_of(".", filename.size());
	if ( index != std::string::npos )
		std::copy(filename.begin(), filename.begin() + index , std::back_inserter(extlessName));
	
	return extlessName;
}

void SMILManager::process_speech(DOMNode* pSpeech)
{
	if(!pSpeech)
		return;

	// get the attributes 
	SMILSpeech obj;
	obj.read(pSpeech);
	if(!obj.performance.tests.system.language.empty())
		m_currentLang = obj.performance.tests.system.language;
	else
		obj.performance.tests.system.language = m_currentLang;

	float speechDuration = 0;
	// if it is already a file created, read the duration..
	if(!obj.filename.empty())
	{
		CWave f(obj.filename);
		// if there is no id, use filename's name part (discard file extension)
		if(obj.performance.id.empty())
			obj.performance.id = getExtlessFilename(obj.filename);

		speechDuration = (float)f.getDuration();
	}
	else
	{
		// get a suitable TTS visitor
		VisitorSpeech* pSpeechVisitor = 0;
		TTSEngineMap::iterator it = m_TTSEngines.find(m_currentLang);
		if(it != m_TTSEngines.end())
		{
			pSpeechVisitor = it->second->getSmilVisitor();
			if(!pSpeechVisitor)
			{
				std::cerr << "no speech visitor found for " << m_currentLang << std::endl;
				pSpeechVisitor = new VisitorSpeech;
			}
		}
		else
			pSpeechVisitor = new VisitorSpeech;

		// process the contents of the speech
		pSpeechVisitor->operator ()(obj);
		// synthesize speech
		if(!pSpeechVisitor->getScript().empty())
		{
			if(it != m_TTSEngines.end())
			{
				if(!it->second)
					std::cerr << "no valid TTS Processor!";
				else
				{
					it->second->speak(pSpeechVisitor->getScript(), obj.performance.id);
					speechDuration = (float)it->second->getDuration();
					SpeechMarkMap marks = it->second->getMarks();
					m_speechMarks.insert(marks.begin(), marks.end());
				}
			}
		}
		VisitorSpeech::reset();
	}	

	// adjust timing
	obj.synch.beginVal = 0;
	obj.synch.endVal = speechDuration;
	
	m_tempGroup->addChild(obj);
}

bool SMILManager::parse(DOMNode* pHead)
{
	static int tabs = 0;
	tabs++;
	DOMTreeWalker* iterator = m_pDoc->createTreeWalker(pHead,
		DOMNodeFilter::SHOW_ELEMENT | DOMNodeFilter::SHOW_ATTRIBUTE | DOMNodeFilter::SHOW_TEXT, NULL, true);
	DOMNode* current = iterator->nextNode();
	while ( current != 0 )
	{
		if(current->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			char* dummy = XMLString::transcode(current->getNodeName());
			std::string nodeName = dummy;
			if(nodeName == "par")
			{
				process_block(current, SMILBlock::kPar);
				current = iterator->nextSibling();
			}
			else if (nodeName == "seq")
			{
				process_block(current, SMILBlock::kSeq);
				current = iterator->nextSibling();
			}
			else if (nodeName == "speech")
			{
				process_speech(current);
				current = iterator->nextNode();
			}
			else if (nodeName == "speech-animation")
			{
				SMILSpeechAnimation obj;
				obj.read(current);
				if(obj.performance.channel.empty())
					obj.performance.channel = m_currentChannel;
				
				current = iterator->nextNode();
				m_tempGroup->addChild(obj);
			}
			else if(nodeName == "action")
			{
				SMILAction obj;
				obj.read(current);
				if(obj.performance.channel.empty())
					obj.performance.channel = m_currentChannel;
				
				current = iterator->nextNode();
				m_tempGroup->addChild(obj);
			}
			else if(nodeName == "emotion-display") // does the same thing with action
			{
				SMILAction obj;
				obj.read(current);
				if(obj.performance.channel.empty())
					obj.performance.channel = m_currentChannel;
				
				current = iterator->nextNode();
				m_tempGroup->addChild(obj);
			}
			else if(nodeName == "switch")
			{
				m_errorString << "Tag " << nodeName << " is not processed in this version!\n";
				current = iterator->nextNode();
			}
			else
				current = iterator->nextNode();

			XMLString::release(&dummy);
		}
		else
			current = iterator->nextNode();
	}
	tabs--;
	
	return true;
}

/*! return value used in onLoadPHO to load the correct phoneme dictionary.
	we (should) have a bunch of *.dic file located in the lang directory, 
	for every language phonemes are converted to xface visemes through the 
	dictionary file. 
*/
std::string SMILManager::getLanguage() const
{
	return m_currentLang;
/*
	if(m_currentLang == "english")
		return "en_SAPI";
	else if (m_currentLang == "italian")
		return "it";
	
	return "en"; // by default..
*/
}

std::string SMILManager::getErrorString(bool clear) 
{
	std::string retVal = m_errorString.str();
	if(clear)
		m_errorString.str("");
	
	return retVal;
}

