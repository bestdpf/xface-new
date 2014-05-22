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

#include "SDLSound.h"
#include <iostream>

#pragma message("		Adding SDL lib...")
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")


namespace XFaceApp{

Uint8 * SDLSound::s_audio_chunk = 0;
Uint32 SDLSound::s_audio_remains = 0;
Uint8 * SDLSound::s_audio_pos = 0;
bool SDLSound::s_bMute = false;

SDLSound::SDLSound(void) : m_audio_len(0)
{
	SDL_Init(SDL_INIT_AUDIO);
}

SDLSound::~SDLSound(void)
{
	unload();
}

/* The audio function callback takes the following parameters:
    stream:  A pointer to the audio buffer to be filled
    len:     The length (in bytes) of the audio buffer
*/
void SDLSound::playSound(void *udata, Uint8 *stream, int len)
{
	/* Only play if we have data left */
    if ( s_audio_remains == 0 )
        return;

    /* Mix as much data as possible */
    len = ( len > (int)s_audio_remains ? s_audio_remains : len );
	if(!s_bMute)
		SDL_MixAudio(stream, s_audio_pos, len, SDL_MIX_MAXVOLUME);
    s_audio_pos += len;
    s_audio_remains -= len;
}

bool SDLSound::loadWAV(const std::string& filename)
{
	unload();
	bool oldMute = s_bMute;
	s_bMute = true;
	SDL_AudioSpec wav_spec;

	// Load the WAV 
	if( SDL_LoadWAV(filename.c_str(), &wav_spec, &s_audio_chunk, &m_audio_len) == NULL )
	{
		std::cerr << "Could not open " <<  filename << "; " << SDL_GetError() << std::endl;
		return false;
	}
	else
		std::cout << "Loaded speech file " << filename << std::endl;

	// Set the audio format details (callback function)
    //wav_spec.samples = 1024;  /* Good low-latency value for callback */
    wav_spec.callback = SDLSound::playSound;
    wav_spec.userdata = NULL;

    // Open the audio device, forcing the desired format
	SDL_CloseAudio(); // if already open (NOT NECESSARY)
    if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ) 
	{
		std::cerr << "Couldn't open audio: " << SDL_GetError() << std::endl;
        return false;
    }

	s_audio_pos = s_audio_chunk;
	s_audio_remains = m_audio_len;

	s_bMute = oldMute;
	return true;
}

void SDLSound::mute(bool bMute)
{
	s_bMute = bMute;
}
/*!
	Jumps to the designated portion of the file.
	\param percentage Offset as a percentage of the total size of the wav file.
*/
void SDLSound::jump(float percentage)
{
	if(percentage > 1.0f)
		return;
	Uint32 newpos = Uint32(percentage*(float)m_audio_len);
	if(newpos % 2) // position should be an even number
		newpos++;
	s_audio_pos = &s_audio_chunk[newpos];
	s_audio_remains = m_audio_len - newpos;
}

void SDLSound::play()
{
	SDL_LockAudio();
	if(m_audio_len)
	{
		s_audio_pos = s_audio_chunk;
		s_audio_remains = m_audio_len;
		SDL_PauseAudio(0);
	}
	else
		std::cerr << "Audio length is zero! Cannot playback audio." << std::endl;

	SDL_UnlockAudio();
			
}

void SDLSound::pause()
{
	SDL_LockAudio();
	SDL_audiostatus status = SDL_GetAudioStatus();
	if (SDL_AUDIO_PLAYING == status)
		SDL_PauseAudio(1);
	else if ((SDL_AUDIO_PAUSED == status) || (SDL_AUDIO_STOPPED == status))
		SDL_PauseAudio(0);

	SDL_UnlockAudio();
}

void SDLSound::stop()
{
	SDL_LockAudio();
	SDL_PauseAudio(1);
	s_audio_pos = s_audio_chunk;
	s_audio_remains = m_audio_len;
	SDL_UnlockAudio();
}

void SDLSound::unload()
{
	SDL_audiostatus status = SDL_GetAudioStatus();
	if (SDL_AUDIO_PLAYING == status)
		SDL_PauseAudio(1);
	
	SDL_FreeWAV(s_audio_chunk);
	s_audio_chunk = 0;
	m_audio_len = 0;
	s_audio_remains = 0;
	s_audio_pos = 0;

	SDL_CloseAudio();
}

}// namespace XFaceApp