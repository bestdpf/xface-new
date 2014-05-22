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
	\file		TTSEngineMaker.h
	\brief		Base class for TTS engine makers.
*/
#pragma once

#include <map>
#include <vector>
#include "ITTSEngine.h"
/*!
	\brief TTSEngine pluggable factory maker
	\author Koray Balci
	\version 1.0
	\date   March 2006

	Implementation of "Pluggable Factory" design pattern. This class is responsible
	for creation of ITTSEngine derived classes. Note that it is an abstract base class, 
	you should always derive a class and implement TTSEngineMaker::makeTTSEngine
    function with proper return value specialization for correct ITTSEngine type.
	
	An ITTSEngine derived class must always have a TTSEngineMaker derived class complementing
	it. Library automatically adds available ITTSEngine derived classes to its
	TTS engine set and exposes it to the end user. In addition, only this way the core library can 
    use the TTS engines properly.

	\sa SAPI51Maker ItalianFliteMaker
*/
class TTSEngineMaker
{
	typedef std::map<std::string, TTSEngineMaker* > MakerMap;
	static MakerMap& registry()
	{
		static MakerMap s_registry;
		return s_registry;
	}
public:
	static std::vector<std::string> getMakerList();
	static ITTSEngine* newTTSEngine(const std::string& name, const std::string& pathname, const std::string& ini);
	virtual ~TTSEngineMaker(void){};

protected:
    //! ctor only accessible to derived classes.
	TTSEngineMaker(const std::string& className);
	//! Derived classes should reimplement this pure virtual.
	virtual ITTSEngine* makeTTSEngine(const std::string& pathname, const std::string& ini) const = 0;	
};
