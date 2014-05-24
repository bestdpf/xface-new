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

#include <XFaceApp/ApplicationBase.h>
#include <XFace/PHOLoader.h>

#include <fstream>
#include <iostream>
#include <strstream>
#include <cassert>

namespace XFaceApp{ 

using namespace XEngine;
using namespace XFace;
	
ApplicationBase::ApplicationBase(void) 
	: m_bPausePlayback(true), m_bBusy(false), m_sequenceDuration(0), m_renderMode(FAP)
{
}

ApplicationBase::~ApplicationBase(void)
{
}

/*!
	Provides basic query mechanism. 
	\todo The mechanism can be improved and reverted to Task/Notification type easily. 
	Or at least we can add a new Task type for QUERY and implement handlers. 
*/
bool ApplicationBase::query(const QUERY& q)
{
	switch(q)
	{
	case QUERY_FAPS_FINISHED:
		assert(m_pFapStream);
		return m_pFapStream->isEnd();
		break;
	case QUERY_FAPS_LOADED:
		assert(m_pFapStream);
		return m_pFapStream->isOpen();
		break;
	//case QUERY_BUSY:
	//	return m_bBusy;
	}

	return false;
}

/*!
	Pure virtual create* methods are called here to create necessary objects and stored in 
	interface pointers. This way, we provide the mechanism but developers might be interested/need
	using platform dependent or external library dependent classes, or just improve the already 
	supplied classes in Xface.

	Sound, Xface::FaceBase, timer, renderer, fap stream, script processor objects are created.
*/
bool ApplicationBase::init()
{
	//m_pSound = createSound();
	m_pTimer = createTimer();
	m_pFace	 = createFace();
	m_pRenderer = createRenderer();
	m_renderManager.setRenderer(m_pRenderer);
	
	m_pFapStream = createFapStream();
	//return createScriptProcessors();
	return true;
}
/*!
	This cute hack prioritizes loading tasks to execute immediately.
	Function added by Almer S. Tigelaar, patched on 17.06.2007
*/
void ApplicationBase::newTask(const Task& task)
{
	/*
	 * This cute hack prioritizes loading tasks to execute immediately.
	 */
	if (task.getName() == "LOAD_WAV" || task.getName() == "LOAD_FAP" || task.getName() == "UPLOAD_WAV" || task.getName() == "UPLOAD_FAP") {
		TaskHandlerBase::newTaskForce(task);
	} else {
		TaskHandlerBase::newTask(task);
	}
}

/*
const boost::shared_ptr<IScriptProcessor>& ApplicationBase::getScriptProcessor(const std::string& name) const 
{
	static boost::shared_ptr<IScriptProcessor> invalidRetVal;
	ScriptProcessorMap::const_iterator it = m_scriptProcessors.find(name);
	if (it != m_scriptProcessors.end())
		return it->second;
	else 
		return invalidRetVal;
}
*/
/*!
	Mutes the audio. Good for testing purposes during development!
*/
void ApplicationBase::muteAudio(bool bSnd)
{
	//m_pSound->mute(bSnd);
}

/*!
	Renders the current frame. 
	No Notification is fired.
*/
void ApplicationBase::onRenderFrame() const
{
	assert(m_pFace && "ApplicationBase::onRenderFrame()");
	
	renderBegin();
		m_renderManager.render();
	renderEnd();
}

/*!
	Advances the FAP frame, and updates the deformation.
	No Notification is fired.
*/
void ApplicationBase::onAdvanceFrame()
{
	assert(m_pFapStream && m_pFace && "ApplicationBase::onAdvanceFrame()");

	if (m_renderMode == FAP)
	{
		const std::vector<float>& FAPs = m_pFapStream->getCurrentFAP();
		if(!FAPs.empty())
		{
			m_pFace->update(m_pFapStream);
			m_renderManager.setGlobalTransform(m_pFace->getTransform());
			m_pFapStream->next();
		}
	}
/*	else if (m_renderMode == KEYFRAME)
	{
		float elapsed = m_pTimer->getElapsedTime(true);//1000.0f;
		const Entity& res = m_pFace->update(elapsed);

		m_renderManager.setGlobalTransform(m_pFace->getTransform());
		m_renderManager.resetDrawables();
		m_renderManager.addDrawables(res.getDrawables());
	}
*/
}

/*!
	Starts/Resumes the playback of FAP stream and the WAV file and sends back 
	Notification::kStarted in the beginning and Notification::kFinished in the end to the client. On error, Notification::kError
	is returned. Also yields the message pump for checking if there is a pause
	or stop Task arrived or not. If by chance, another resume playback Task is 
	in the task queue, it is not executed until tht current task finishes.
*/
bool ApplicationBase::onResumePlayback()
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	if(m_renderMode == FAP && !m_pFapStream->isOpen())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;	// if no fap, sth wrong, skip this Task (returning true)
	}
	m_bBusy = true;

	// Notification
	if(m_currentTask.getName() == "RESUME_PLAYBACK")
		fireNotification(Notification::kStarted);
	
	if(m_renderMode == FAP)
		synchronize(true);
	/*
	if(m_bPausePlayback)
		m_pSound->pause();
	*/
	
	m_bPausePlayback = false;
	if(m_renderMode == FAP)
	{
		do{
			onAdvanceFrame();
			onRenderFrame();
			synchronize(false);
			if(yield())
				processTask();
		}
		while(!m_pFapStream->isEnd() && !m_bPausePlayback);
	}
	else if (m_renderMode == KEYFRAME && m_sequenceDuration > 0)
	{
		MorphController* con = MorphController::getInstance();
		con->rewind();
		m_pTimer->start();
		int _sequenceDuration = m_sequenceDuration;
		std::list<boost::shared_ptr<Drawable> > oldDrawables = m_pFace->getDrawables();
		do
		{
			onRenderFrame();
			unsigned int elapsed = m_pTimer->getElapsedTime(true);
			const Entity& res = m_pFace->update(elapsed);
			_sequenceDuration -= elapsed;

			m_renderManager.setGlobalTransform(m_pFace->getTransform());
			m_renderManager.resetDrawables();
			m_renderManager.addDrawables(res.getDrawables());
	
			if(yield())
				processTask();
		
		}while (_sequenceDuration > 0 && !m_bPausePlayback);
		m_renderManager.resetDrawables();
		m_renderManager.addDrawables(oldDrawables);
		onRenderFrame();
	}

	m_bBusy = false;

	if(!m_bPausePlayback)
	{
		onStopPlayback();
		m_bPausePlayback = true;
	}
	
	// Notification
	if(m_currentTask.getName() == "RESUME_PLAYBACK")
		fireNotification(Notification::kFinished);

	return true;
}

/*!
	Stops and rewinds the FAP stream and the WAV file to beginning and fires
	Notification::kFinished.
*/
void ApplicationBase::onStopPlayback()
{
	m_bPausePlayback = true;
	//m_pSound->stop();
	if(m_renderMode == FAP)
	{
		m_pFapStream->rewind();
		const std::vector<float>& FAPs = m_pFapStream->getCurrentFAP();
	
		if(!FAPs.empty())
		{
			m_pFace->update(m_pFapStream);
			m_renderManager.setGlobalTransform(m_pFace->getTransform());
			renderBegin();
				m_renderManager.render();
			renderEnd();		
		}
	}

	m_pFace->resetDeformedVertices();
	// Notification
	if(m_currentTask.getName() == "STOP_PLAYBACK")
		fireNotification(Notification::kFinished);
}

/*!
	Rewinds the FAP stream and the WAV file to beginning and fires
	Notification::kFinished.
*/
void ApplicationBase::onRewindPlayback() const
{
	if(m_pFapStream->isOpen())
		m_pFapStream->rewind();	
	//m_pSound->stop();
	m_pFace->rewindKeyframeAnimation();

	// Notification
	if(m_currentTask.getName() == "REWIND_PLAYBACK")
		fireNotification(Notification::kFinished);
}

/*!
	Loads the FDP file from the server (XfacePlayer) harddisk.
	\param param1 is the filename
	\param param2 is the path
	Notification::kError is fired on error and Notification::kFinished is fired on success.
*/
bool ApplicationBase::onLoadFDP(const std::string& param1, const std::string& param2)
{
    printf("appbase on load fdp\n");
	if(m_bBusy){
        printf("busy now\n");
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	}
	if(param1.empty()) // param2 may be empty..
	{
        printf("para is null\n");
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	// Notification
	if(m_currentTask.getName() == "LOAD_FDP")
		fireNotification(Notification::kStarted);
    printf("1\n");
	m_pFace->init(param1, param2);
    printf("1.5\n");
    fflush(stdout);
	MorphController::getInstance()->processDictionary();
    printf("2\n");
	m_renderManager.resetDrawables();
	m_renderManager.addDrawables(m_pFace->getDrawables());

	m_sequenceDuration = 0;

	// Notification
	if(m_currentTask.getName() == "LOAD_FDP")
		fireNotification(Notification::kFinished);
	return true;
}

/*!
	Loads the FAP file from the server (XfacePlayer) side. 
	\param param is the filename
	Notification::kError is fired on error and Notification::kFinished is fired on success.
*/
bool ApplicationBase::onLoadFAP(const std::string& param)
{
    printf("app base on load fap\n");
	if(m_bBusy){
        printf("we are busy\n");
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	}
    printf("1\n");
	if(param.empty())
	{
        printf("param empty\n");
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	printf("2\n");
	// Notification
	if(m_currentTask.getName() == "LOAD_FAP"){
        printf("task is load fap\n");
		fireNotification(Notification::kStarted);
    }
    printf("3\n");
	std::ifstream input(param.c_str());
	if(input.fail())
	{
        printf("input fail\n");
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
    printf("4\n");
	if(!m_pFapStream->open(input, m_pFace->getFAPU()))
		std::cout << "Unable to load FAP file: " << param << "\n";
	input.close();
    printf("5\n");
	// Notification
	if(m_currentTask.getName() == "LOAD_FAP"){
        printf("task is load fap low\n");
		fireNotification(Notification::kFinished);
    }
    printf("here lwo\n");
	m_renderMode = FAP;
	return true;
}

/*!
	Loads the ANIM file from the server (XfacePlayer) side. 
	\param param1 is the filename
	Notification::kError is fired on error and Notification::kFinished is fired on success.
*/
bool ApplicationBase::onLoadANIM(const std::string& param1)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param1.empty())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	
	// Notification
	if(m_currentTask.getName() == "LOAD_ANIM")
		fireNotification(Notification::kStarted);

	std::ifstream input(param1.c_str());
	if(input.fail())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	m_sequenceDuration = m_pFace->processAnims(input);
	input.close();

	// Notification
	if(m_currentTask.getName() == "LOAD_ANIM")
		fireNotification(Notification::kFinished);

	m_renderMode = KEYFRAME;
	
	return true;
}
/*!
	Loads the PHO file from the server (XfacePlayer) side. 
	\param param1 is the filename
	\param param2 is the language
	Notification::kError is fired on error and Notification::kFinished is fired on success.
*/
bool ApplicationBase::onLoadPHO(const std::string& param1, const std::string& param2)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param1 == "")
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	
	// Notification
	if(m_currentTask.getName() == "LOAD_PHO")
		fireNotification(Notification::kStarted);

	std::ifstream input(param1.c_str());
	if(input.fail())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	// process phonemes
	m_sequenceDuration = m_pFace->processPhonemes(input, param2);
	input.close();

	// Notification
	if(m_currentTask.getName() == "LOAD_PHO")
		fireNotification(Notification::kFinished);
	
	m_renderMode = KEYFRAME;
	return true;
}

/*!
	Loads the WAV file from the server (XfacePlayer) disk. 
	\param param is the filename
	Notification::kError is fired on error and Notification::kFinished is fired on success.
*/
bool ApplicationBase::onLoadWAV(const std::string& param)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param == "")
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	
	// Notification
	if(m_currentTask.getName() == "LOAD_WAV")
		fireNotification(Notification::kStarted);

	//m_pSound->loadWAV(param);
	// Notification
	if(m_currentTask.getName() == "LOAD_WAV")
		fireNotification(Notification::kFinished);
	return true;
}

/*!
	\param param is the FAP contents
*/
bool ApplicationBase::onUpLoadFAP(const std::string& param)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param == "")
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	
	// Notification
	if(m_currentTask.getName() == "UPLOAD_FAP")
		fireNotification(Notification::kStarted);

	std::istrstream input(param.c_str(), (std::streamsize)param.size());
	std::istream mem_input(input.rdbuf());
	if(input.fail())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	if(!m_pFapStream->open(mem_input, m_pFace->getFAPU()))
	{
		std::cout << "Unable to load FAP file: " << param << "\n";
		fireNotification(Notification::kError);
	}

	// Notification
	if(m_currentTask.getName() == "UPLOAD_FAP")
	{
		fireNotification(Notification::kFinished);
		m_renderMode = FAP;
	}
	return true;
}

/*!
	\param param1 is the PHO contents
	\param param2 is the PHO language
*/
bool ApplicationBase::onUpLoadPHO(const std::string& param1, const std::string& param2, const std::string& param3)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param1 == "")
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	
	// Notification
	if(m_currentTask.getName() == "UPLOAD_PHO")
		fireNotification(Notification::kStarted);

	std::istrstream input(param1.c_str(), (std::streamsize)param1.size());
	if(input.fail())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	// save phonemes for future use by processAnims 
	// if necessary; normally if anim is coming after this, it IS necessary, if only phoneme file is sent, no need)
	std::ofstream phoFile(param3.c_str());
	phoFile.write(param1.c_str(), (std::streamsize)param1.size());
	phoFile.close();
	// process phonemes
	m_sequenceDuration = m_pFace->processPhonemes(input, param2);
	
	// Notification
	if(m_currentTask.getName() == "UPLOAD_PHO")
	{
		fireNotification(Notification::kFinished);
		m_renderMode = KEYFRAME;
	}
	return true;
}

/*!
	\param param1 is the ANIM contents
*/
bool ApplicationBase::onUpLoadANIM(const std::string& param1)
{
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait
	
	if(param1 == "")
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}
	
	// Notification
	if(m_currentTask.getName() == "UPLOAD_ANIM")
		fireNotification(Notification::kStarted);

	std::istrstream input(param1.c_str(), (std::streamsize)param1.size());
	if(input.fail())
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	m_pFace->processAnims(input);

	// Notification
	if(m_currentTask.getName() == "UPLOAD_ANIM")
		fireNotification(Notification::kFinished);
	
	return true;
}

/*!
	Processes the script string passed (from remote client possibly). Current 
	IScriptProcessor implementation (Expl2fapProcessor class) also loads the FAP 
	and WAV file produced automatically by issuing proper Tasks.
	\param param1 is the script string to process
	\param param2 is the name of script processor
	Notification::kError is sent on error and Notification::kStarted is sent on start and 
	Notification::kFinished when finished, to the client.
*/
bool ApplicationBase::onUpLoadScript(const std::string& param1, const std::string& param2)
{
  return true;
	/*
	if(m_bBusy)
		return false;	// if we are busy (already inside here?) false means, keep the task in the task queue and wait

	if(param1.empty())	// param2 might be empty
	{
		// Notification
		fireNotification(Notification::kError);
		return true;
	}

	// Notification
	if(m_currentTask.getName() == "UPLOAD_SCRIPT")
		fireNotification(Notification::kStarted);

	// actual processing
	boost::shared_ptr<IScriptProcessor> processor = getScriptProcessor(param2);
	if(!processor)
	{
		std::cerr << "Cannot find a matching script processor named " << param2 << " while trying to process UPLOAD_SCRIPT task!" << std::endl;
		return true;
	}

	if(!processor->process(param1))
	{
		fireNotification(Notification::kError);
		return true;
	}

	std::string path = processor->getPath();
	std::vector<std::string> outputFiles = processor->getOutputFileNames();
	std::vector<std::string>::const_iterator it = outputFiles.begin();
	while (it != outputFiles.end())
	{
		std::string ext, extlessName;
		std::string::size_type index = it->find_last_of(".", it->size());
		if ( index != std::string::npos )
		{
			std::copy(it->begin() + index + 1, it->end(), std::back_inserter(ext));
			std::copy(it->begin() , it->begin() + index , std::back_inserter(extlessName));
		}
		
		//std::string ext = it->substr(it->size() - 3, 3);
		std::string filename = path + "/" + *it;
		// hack
		if(param2 == "SMIL-Agent") 
		{
			if(ext == "anim")
			{
				onLoadANIM(filename);
				if(m_pFace->getSpeechDuration())
					onLoadWAV(extlessName + ".wav");
				//else
					//m_pSound->unload();
			}
		}
		else
		{
			if(ext == "fap")
				onLoadFAP(filename);
			else if(ext == "pho")
				onLoadPHO(filename, processor->getLanguage());
			else if(ext == "wav")
				onLoadWAV(filename);
			else if(ext == "anim")
				onLoadANIM(filename);
		}
		
		++it;
	}
	// Notification
	if(m_currentTask.getName() == "UPLOAD_SCRIPT")
		fireNotification(Notification::kFinished);
	
	return true;
	*/
}

void ApplicationBase::fireNotification(const Notification::TaskStatus status) const
{
    printf("fire notify\n");
	if(m_currentTask.getOwnerID())	// if it is not this thread who scheduled the task
	{
        printf("not my id\n");
		Notification note(m_currentTask, status);
		fireNotification(note);
	}
}
/*!
	Synchronizes the animation to frames per seconds required by current FAP
	being played back. 
	\todo Check this function, not tested extensively. Especially in debug mode, we seem to have 
	some problem, should skip the frames, but not as it seems, lagging animation. 
*/
unsigned long ApplicationBase::synchronize(bool bStart)
{
	static unsigned int frame_count = 0;
	static unsigned int missed_frames = 0;
	static float frame_time = 1000.0f / m_pFapStream->getFPS();
	
	// this means, we are beginning playback now (first frame is about to be played)
	if(bStart)
	{
		m_pTimer->start(); 
		// do init
		missed_frames = 0;
		frame_count = 0;
		frame_time = 1000.0f / m_pFapStream->getFPS(); // might have changed fap file
	}
	
	unsigned int elapsed_frames = (unsigned int)(m_pTimer->getTotalElapsedTime(false) / frame_time);

	// if slow, catch up
	while(elapsed_frames > frame_count)
	{
		frame_count++;
		missed_frames++;
		onAdvanceFrame();
		std::cerr << "missed frames: " << missed_frames << std::endl;
	}

	// if fast, slow down
	if( m_pTimer->getElapsedTime(false) < frame_time)
		m_pTimer->wait((unsigned long)(frame_time - m_pTimer->getElapsedTime(false)));

	frame_count++;
	// don't forget to record the new timestamp
	return m_pTimer->getElapsedTime(true);
}

std::string ApplicationBase::getErrorString(bool clear) 
{
	m_errorString << m_pFace->getErrorString(clear);
	std::string retVal = m_errorString.str();
	if(clear)
		m_errorString.str("");
	
	return retVal;
}
}
