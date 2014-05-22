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
 * The Original Code is XfaceApp Application Library.
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
/*! 
	\file		ScriptProcessorLoader.h
	\brief		Class that load scriptProcs.xml file.
*/
#pragma once
#ifndef SCRIPTPROCESSORLOADER_H_
#define SCRIPTPROCESSORLOADER_H_

#include "IScriptProcessor.h"

#include <vector>
#include <boost/shared_ptr.hpp>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <iostream>
#include <fstream>

/*!
	\brief	Class that load scriptProcs.xml file.
	\author Koray Balci

	scriptProcs.xml file stores the configurational information for TTS engines, 
	languages, script processing options. This file should be edited manually 
	to actually match with the user's own configuration (available TTS engine 
	installed, etc..). This class is used to parse (using xerces parser) and 
	get the contents of scriptProcs file.
*/
class ScriptProcessorLoader
{
	std::vector<boost::shared_ptr<IScriptProcessor> > m_processors;
	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *m_pDoc;
	XERCES_CPP_NAMESPACE::XercesDOMParser *m_pParser;
	bool parse();
	void loadScript(XERCES_CPP_NAMESPACE::DOMNode* pScript);
	bool loadInput(const XERCES_CPP_NAMESPACE::InputSource& input);
public:
	std::vector<std::string> getScripterNames() const;
	boost::shared_ptr<IScriptProcessor> getScriptProcessor(const std::string& name) const;
	ScriptProcessorLoader();
	~ScriptProcessorLoader();
	bool load(std::istream& input);
	bool load(const std::string& filename);
};

#endif // SCRIPTPROCESSORLOADER_H_