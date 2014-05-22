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
	\file		SDLSound.h
	\brief		SDL based sound player
*/

#pragma once
#ifndef SDLSOUND_H_
#define SDLSOUND_H_

#include <XFaceApp/ISound.h>

#include <SDL.h>

namespace XFaceApp{

class SDLSound : public ISound
{
	static bool s_bMute;
	static Uint8 *s_audio_chunk;
	Uint32 m_audio_len;
	static Uint32 s_audio_remains;
	static Uint8 *s_audio_pos;
	static void playSound(void *udata, Uint8 *stream, int len);
public:
	void jump(float percentage);
	bool loadWAV(const std::string& filename);
	void mute(bool bMute);
	void play();
	void pause();
	void stop();
	void unload();
	
	SDLSound(void);
	~SDLSound(void);
};


} // namespace XFaceApp

#endif // SDLSOUND_H_