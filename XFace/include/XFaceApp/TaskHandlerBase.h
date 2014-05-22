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
	\file		TaskHandlerBase.h
	\brief		Task handler base.
*/

#pragma once
#ifndef TASKHANDLERBASE_H_
#define TASKHANDLERBASE_H_

#include <string>
#include <queue>
#include "Task.h"

namespace XFaceApp{

/*!
	\brief	Task handler base
	\ingroup XFaceApp
	\author Koray Balci

	Task objects are issued and processed using this class. Current implementation has
	a Task queue (std::queue), and you can push new Tasks to the queue using 
	TaskHandlerBase::newTask method. Then, when TaskHandlerBase::processTask method is called, 
	the Task in queue is popped, and processed using TaskHandlerBase::dispatch method. 
	In TaskHandlerBase::dispatch (private & non-virtual method), depending on Task type, 
	the task is routed to appropriate handler method. If a time spanning task is being processed 
	when the system tries to process a new task, it automatically waits until the end of the 
	current task. That's why time spanning task handler methods return boolean values. All the 
	task handler methods are defined as virtual with empty bodies, which means this class, by 
	itself does nothing. You should never use this class directly, hence the destructor of the 
	class is protected.	This class only implements and hides the Task processing mechanism 
	from its clients.
	\sa ApplicationBase
*/
class TaskHandlerBase
{
	//! Dispatches Task.
	bool dispatch(const Task& task);
	bool m_bPaused;
	bool m_dispatching; // kind of a mutex for stalling dispatching
public:
	//! Post a new Task through here
	virtual void newTask(const Task& task);
	virtual void newTaskForce(const Task& task);
	//! Process the Task queue
	virtual bool processTask(); 
	//! Discard all pending Task objects in the queue
	void discardPendingTasks();
	//! Pauses the system
	void pause(bool bPause = true) {m_bPaused = bPause;}
protected:
	std::queue<Task> m_taskQueue;
	//! Pointer to current task that is being processed is stored for firing proper Notification(s)
	//Task* m_pCurrTask;
	Task m_currentTask;
	
	//!@{ Task handler with empty body.
	virtual void onRenderFrame() const {};
	virtual void onAdvanceFrame() const {};
	virtual bool onResumePlayback() {return true;};
	virtual void onStopPlayback() {};
	virtual void onKillApplication() {};
	virtual void onRewindPlayback() const {};
	virtual bool onLoadFDP(const std::string&, const std::string&) {return true;}
	virtual bool onLoadFAP(const std::string&) {return true;};
	virtual bool onLoadPHO(const std::string&, const std::string&) {return true;};
	virtual bool onLoadANIM(const std::string&) {return true;};
	virtual bool onLoadWAV(const std::string&) {return true;};
	virtual bool onUpLoadFAP(const std::string&) {return true;};
	virtual bool onUpLoadPHO(const std::string&, const std::string&, const std::string&) {return true;};
	virtual bool onUpLoadANIM(const std::string&) {return true;};
	virtual bool onUpLoadScript(const std::string&, const std::string&) {return true;};
//	virtual bool onUpLoadFDP(const std::string&){return true;};
	//!@}

	virtual bool yield() = 0;
	virtual ~TaskHandlerBase(void){};
	TaskHandlerBase(void) : m_bPaused(false), m_currentTask("INVALID_TASK"){}
};

} // namespace XFaceApp

#endif // TASKHANDLERBASE_H_
