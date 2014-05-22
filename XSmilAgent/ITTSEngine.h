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
	\file		ITTSEngine.h
	\brief		Interface for TTS engine connection classes.
*/
#pragma once

#include <string>
#include <map>
//#include "VisitorSpeech.h"
class VisitorSpeech; // getting INTERNAL COMPILER ERROR if used include above directly 

/*!
	\brief Interface for TTS engine connection classes.
	\author Koray Balci

	Interface for TTS engine connection classes. For every TTS engine, one has to 
	reimplement this class methods in order to fit to their own TTS calls.
*/
class ITTSEngine
{
protected:
	VisitorSpeech* m_smilVisitor;
	unsigned int m_duration;
	std::map<std::string, float> m_marks;
	virtual void createSmilVisitor() = 0;
public:
	VisitorSpeech* getSmilVisitor() const {return m_smilVisitor;}
	unsigned int getDuration() const {return m_duration;}
	std::map<std::string, float> getMarks() const {return m_marks;}

	virtual std::map<std::string, float> speak(const std::string& speech, const std::string& filename) = 0;
	ITTSEngine(void) : m_duration(0), m_smilVisitor(0) {/*createSmilVisitor();*/}
};
