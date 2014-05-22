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
	\file		XSAPI.h
	\brief		MS SAPI 5.1 TTS connection.
*/
#ifndef XSAPI_H_
#define XSAPI_H_

#pragma once
#include <sapi.h>
#include <string>
#include <map>
#include <iostream>

#include "ITTSEngine.h"
#include "TTSEngineMaker.h"

/*!
	\brief MS SAPI 5.1 TTS connection.
	\author Koray Balci

	Implementation of SAPI 5.1 TTS engine calls for speech synthesis.
*/
class SAPI51 : public ITTSEngine
{
	ISpVoice * m_pVoice;
	void createSmilVisitor();
	std::wstring m_options1, m_options2;
public:
	std::map<std::string, float> speak(const std::string& speech, const std::string& filename = "TempFile");
	SAPI51(const std::string& ini);
	~SAPI51(void);
};

/*!
	\brief Maker class for SAPI51 class.
	\author Koray Balci
	
	Pluggable factory pattern maker class for SAPI51. 
*/
class SAPI51Maker : public TTSEngineMaker
{
private:
	SAPI51Maker() : TTSEngineMaker("SAPI51"){}
	ITTSEngine* makeTTSEngine(const std::string& unusedpathname, const std::string& ini) const
	{
		return new SAPI51(ini);
	}
	static const SAPI51Maker m_regSAPI51Maker;
};

#endif //XSAPI_H_