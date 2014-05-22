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
	\file		SDLTimer.h
	\brief		SDL based timer
*/

#pragma once
#ifndef SDLTIMER_H_
#define SDLTIMER_H_

#include <XEngine/ITimer.h>

using XEngine::ITimer;
/*!
	Wrapper for SDL timer operations.
*/
class SDLTimer : public ITimer
{
	unsigned long m_startTime, m_lastMark;
public:
	unsigned long start();
	void wait(unsigned long);
	unsigned long getElapsedTime(bool mark);
	unsigned long getTotalElapsedTime(bool mark);
	
	SDLTimer(void);
	~SDLTimer(void);
};

#endif // SDLTIMER_H_