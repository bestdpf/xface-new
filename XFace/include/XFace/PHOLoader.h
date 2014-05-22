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

#pragma once
#ifndef PHOLOADER_H_
#define PHOLOADER_H_

#include <list>
#include <string>
#include <iostream>
#include <fstream>


namespace XFace{

class PHOLoader
{
	float m_total_duration;
public:
	//! returns total duration of phoneme file (in seconds)
	unsigned int getTotalDuration() const {return (unsigned int)m_total_duration*1000;}
	std::list<std::pair<std::string, unsigned int> > load(const std::string& filename);
	std::list<std::pair<std::string, unsigned int> > load(std::istream& input);
};

}// namespace XFace

#endif // PHOLOADER_H_