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

#include "SDLTimer.h"

#include <SDL.h>

#pragma message("		Adding SDL lib...")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")

SDLTimer::SDLTimer(void)
{
	m_lastMark = m_startTime = SDL_GetTicks();
}

SDLTimer::~SDLTimer(void)
{
}

unsigned long SDLTimer::start()
{
	m_lastMark = m_startTime = SDL_GetTicks();
	return m_startTime;
}

void SDLTimer::wait(unsigned long sometime)
{
	SDL_Delay(sometime);
}

unsigned long SDLTimer::getElapsedTime(bool mark)
{
	unsigned long now = SDL_GetTicks();
	unsigned long retVal = now - m_lastMark;
	if(mark)
		m_lastMark = now;
	return retVal;
}

unsigned long SDLTimer::getTotalElapsedTime(bool mark)
{
	unsigned long now = SDL_GetTicks();
	if(mark)
		m_lastMark = now;

	return now - m_startTime;
}
