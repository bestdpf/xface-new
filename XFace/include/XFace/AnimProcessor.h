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
 * The Original Code is Xface Core Library
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

#pragma once
#ifndef ANIMPROCESSOR_H__
#define ANIMPROCESSOR_H__

#include <XEngine/MorphController.h>
#include <iostream>
#include <XFace/PhonemeDictionary.h>
#include <XFace/HeadChannel.h>

namespace XFace{
class AnimProcessor
{
	unsigned int m_speechDuration;
	const PhonemeDictionary::DictionaryContent& getPhonDictionary(const std::string& lang) const {return m_phonDictionaries.getDictionary(lang);}
	PhonemeDictionary m_phonDictionaries;
	void pushPhoneme(const std::string& alias, unsigned int start, unsigned int end, float weight) const;
	void prepareEyeBlink(unsigned int period, unsigned int duration);
	int processEyeChannel(std::istream& input) const;
	int processFaceChannel(std::istream& input) const;
	int processHeadChannel(std::istream& input) const;
	boost::shared_ptr<HeadChannel> m_headChannel;
public:
	AnimProcessor();
	void addPhonemeDictionary(const std::string& dic) {m_phonDictionaries.loadDictionary(dic);}
	unsigned int getSpeechDuration() const {return m_speechDuration;}
	std::string getErrorString(bool clear = true);
	int processAnim(std::istream& input);
	int processPhonemes(std::istream& input, const std::string& lang);
};

} // namespace XFace

#endif
