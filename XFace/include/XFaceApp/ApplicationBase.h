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
	\file		ApplicationBase.h
	\brief		Base for application dev classes
*/

#pragma once
#ifndef APPLICATIONBASE_H_
#define APPLICATIONBASE_H_

#include <list>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <XEngine/RenderManager.h>
#include <XFaceApp/TaskHandlerBase.h>
#include <XFaceApp/Notification.h>
#include <XFace/FaceBase.h>
#include <XFaceApp/ISound.h>
#include <XEngine/ITimer.h>
#include <XFace/IFapStream.h>
#include <IScriptProcessor.h>

/*!
	\brief	Application related classes are under this namespace.
	\ingroup XFaceApp
	\author Koray Balci
	\version 1.0
	\date   May 2003
*/
namespace XFaceApp{

/*!
	\brief	Base for application dev classes
	\ingroup XFaceApp
	\author Koray Balci
	
	Abstract base class for Xface related Task processing. Implements the empty Task handlers
	defined in base class TaskHandlerBase, but pure virtual create* methods also enforces
	derivation from this class. For basic use, task handlers in this class does the job, at 
	least Xface related job. For example, for adding GUI related things, you should 
	reimplement these task handlers, and call the implementations provided here from derived ones.
	So, the main idea is to provide the basic mechanism and hide the Xface related implementation
	while letting users to develop their own application using various available libraries and 
	technologies. 
*/
class ApplicationBase :	public TaskHandlerBase
{
	bool m_bBusy;
	bool m_bPausePlayback;
public:
	virtual void newTask(const Task& task);
	bool isBusy() const {return m_bBusy;}
	std::string getErrorString(bool clear);
protected:
	std::stringstream m_errorString;
	enum RenderMode{
		FAP,
		KEYFRAME
	};
	unsigned int m_sequenceDuration;
	RenderMode m_renderMode;
	XEngine::RenderManager m_renderManager;
	boost::shared_ptr<XFace::FaceBase> m_pFace;
	//boost::shared_ptr<ISound> m_pSound;
	boost::shared_ptr<XEngine::ITimer> m_pTimer;
	boost::shared_ptr<XEngine::IRenderer> m_pRenderer;
	boost::shared_ptr<XFace::IFapStream> m_pFapStream;

	//!@{ Task Handler
	virtual void onRenderFrame() const;
	virtual void onAdvanceFrame();
	virtual bool onResumePlayback();
	virtual void onStopPlayback();
	virtual void onRewindPlayback() const;
	//! Do not forget to reimplement this on derived classes, otherwise your application won't be killed.
	virtual void onKillApplication() {};
	virtual bool onLoadFDP(const std::string& param1, const std::string& param2);
	virtual bool onLoadFAP(const std::string& param);
	virtual bool onLoadPHO(const std::string& param1, const std::string& param2);
	virtual bool onLoadANIM(const std::string& param1);
	virtual bool onLoadWAV(const std::string& param);
	virtual bool onUpLoadFAP(const std::string& param);
	virtual bool onUpLoadPHO(const std::string& param1, const std::string& param2, const std::string& param3);
	virtual bool onUpLoadANIM(const std::string& param1);
	virtual bool onUpLoadScript(const std::string& param1, const std::string& param2);
//	virtual bool onUpLoadFDP(const std::string& param);
	//!@}

	/*!
		You should provide an application specific (probably external library dependent)
		yield functionality which enables processing the task queue, while a task is 
		already being done. We use wxWidgets yield method to achieve this in XfacePlayer
		implementation (see wxFace class).
	*/
	//!	Creates an object implementing IScriptProcessor interface.
	//virtual bool createScriptProcessors() = 0;
	//virtual boost::shared_ptr<IScriptProcessor> getScriptProcessor(const std::string& name) const = 0;

	//!	Creates an object implementing ITimer interface.
	virtual boost::shared_ptr<XEngine::ITimer> createTimer() const = 0;
	//!	Creates an object implementing ISound interface.
	//virtual boost::shared_ptr<ISound> createSound() const = 0;
	//!	Creates a FaceBase object (or a derived one)
	virtual boost::shared_ptr<XFace::FaceBase> createFace() const = 0;
	//!	Creates an object implementing XEngine::IRenderer interface.
	virtual boost::shared_ptr<XEngine::IRenderer> createRenderer() const = 0;
	//!	Creates an object implementing IFapStream interface.
	virtual boost::shared_ptr<XFace::IFapStream> createFapStream() const = 0;
	
	void fireNotification(const Notification::TaskStatus status) const;
	/*!
		Fires the Notification, implementation is probably TCP/IP related, 
		hence left out of core library and implemented in derived class in XfacePlayer.
	*/
	virtual void fireNotification(const Notification& note) const{};

	/*!
		Some GUI libraries (wxWidgets, SDL, MFC) for OpenGL windowing operation require
		pre and especially post processing for double buffering. Derived classes should 
		re-implement these empty methods properly
	*/
	virtual void renderBegin() const {};
	/*!
		Some GUI libraries (wxWidgets, SDL, MFC) for OpenGL windowing operation require
		pre and especially post processing for double buffering. Derived classes should 
		re-implement these empty methods properly
	*/
	virtual void renderEnd() const {};
	unsigned long synchronize(bool);
	ApplicationBase(void);
public:
	/*!
		Basic query mechanism. To be changed..
	*/
	enum QUERY{
		QUERY_FAPS_FINISHED, 
		QUERY_FAPS_LOADED
//		QUERY_BUSY
	};

	boost::shared_ptr<XFace::FaceBase> getFace() const {return m_pFace;}
	virtual void muteAudio(bool bSnd);
	bool query(const QUERY& q);
	virtual bool init();
	virtual ~ApplicationBase(void);
};

}

#endif // APPLICATIONBASE_H_
