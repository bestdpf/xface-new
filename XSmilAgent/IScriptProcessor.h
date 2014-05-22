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
	\file		IScriptProcessor.h
	\brief		script caller interface
*/

#pragma once
#ifndef ISCRIPTPROCESSOR_H_
#define ISCRIPTPROCESSOR_H_


#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "TTSEngineMaker.h"


/*!
	\brief Script caller interface
	\author Koray Balci

	Provides the interface for calling script processor.
*/
class IScriptProcessor
{
protected:
	std::string m_path, m_executable, m_params1, m_params2, m_lang, m_errorString;
	std::string m_name, m_tempFileName;
	std::vector<std::string> m_outputFileNames;
	std::map<std::string, boost::shared_ptr<ITTSEngine> > m_TTSEngines;
	IScriptProcessor(const std::string& name) : m_name(name), m_path(".") {}
public:
	/*! Processes the script string 
		\param scriptString script contents
	*/
	virtual bool process(const std::string& scriptString, const std::string& filename = "TempFile") = 0;
	//! Sets the parameters to the script proceesor executable (used before script content)
	void setParameters1(const std::string& params){m_params1 = params;}
	//! Sets the parameters to the script proceesor executable (used after script content)
	void setParameters2(const std::string& params){m_params2 = params;}
	//! Sets the path to the script proceesor executable
	void setPath(const std::string& path) {m_path = path;}
	//! Gets the error string
	const std::string& getErrorString() const {return m_errorString;}
	//! Gets the path to the script proceesor executable
	const std::string& getPath() const {return m_path;}
	//! Sets the name of the script proceesor executable
	void setExecutable(const std::string& exe) {m_executable = exe;}
	//! Gets the name of the script proceesor executable
	const std::string& getExecutable() const {return m_executable;}
	//! Gets the name of the script proceesor parameters (used before script content)
	const std::string& getParameters1() const {return m_params1;}
	//! Gets the name of the script proceesor parameters (used after script content)
	const std::string& getParameters2() const {return m_params2;}
	//! Sets the language
	void setLanguage(const std::string& lang) {m_lang = lang;}
	//! Gets the language
	const std::string& getLanguage() const {return m_lang;}
	const std::vector<std::string>& getOutputFileNames() const {return m_outputFileNames;}
	void push_output(const std::string& name) {m_outputFileNames.push_back(name);}
	void push_ttsEngine(const std::string& engine, const std::string& lang, const std::string& path, const std::string& ini) 
	{
		m_TTSEngines.insert(std::make_pair(lang, TTSEngineMaker::newTTSEngine(engine, path, ini)));
	}
	std::map<std::string, boost::shared_ptr<ITTSEngine> > getTTSEngines() const {return m_TTSEngines;}

	const std::string& getName() const {return m_name;}
	const std::string& getTempFileName() const {return m_tempFileName;}
	void setTempFileName(const std::string& name) {m_tempFileName = name;}
	
	virtual ~IScriptProcessor(void){}
};

#endif // ISCRIPTPROCESSOR_H_