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

#include <XFaceApp/TaskHandlerBase.h>

#include <iostream>


namespace XFaceApp{

/*!
	Pushes the Task directly to the task queue. 
*/
void TaskHandlerBase::newTask(const Task& task)
{
	m_taskQueue.push(task);
}

/*!
	Pushes the Task directly to the task queue. 
	Function added by Almer S. Tigelaar, patched on 17.06.2007
*/
void TaskHandlerBase::newTaskForce(const Task& task)
{
	Task lastCurrent = m_currentTask;
	m_currentTask = task;

	while (m_dispatching) {
		yield();
	}

	if(!dispatch(task)) {
		m_taskQueue.push(task);
		m_currentTask = lastCurrent;
	}
}

/*!
	Empties all the tasks in the queue. Not to be abused, might be necessary
	for high priority tasks like saving avi, and can be called before issueing
	such tasks.
*/
void TaskHandlerBase::discardPendingTasks()
{
	while(!m_taskQueue.empty())
		m_taskQueue.pop();
}

/*!
	Pops the front task from the queue and tries to dispatch. Should be called externally every
	now and then (even when a task is being processed), derived class ApplicationBase does it well.
	\return false only if the queue is empty, true otherwise. Even if the task could not be dispatched, 
	still we return true, because it means the system is busy, so we put back the task in the queue 
	actually. 
	\sa TaskHandlerBase::dispatch
*/
bool TaskHandlerBase::processTask()
{
	if(m_taskQueue.empty())
		return false;
	
	if(m_bPaused)
		return true; // designates the system is busy, but will try to take care of this task..
	
	Task tsk = m_currentTask = m_taskQueue.front();
	m_taskQueue.pop();
	if(!dispatch(tsk))
		m_taskQueue.push(tsk);

	return true;
}
/*!
	Dispatches the Task by checking its name, and routing it to proper task handler method.
	\return If the previous task is still in progress, false, otherwise true. This ensures 
	not missing any tasks.
*/
bool TaskHandlerBase::dispatch(const Task& task)
{
	m_dispatching = true;

	bool ret = true;
	std::string taskName = task.getName();

	if(!TaskDictionary::isTask(taskName))	// check if it is a valid task
		std::cerr << "Invalid task: " << taskName << std::endl;
	else if(taskName == "RESUME_PLAYBACK")
		ret = onResumePlayback();
	else if(taskName == "STOP_PLAYBACK")
		onStopPlayback();
	else if(taskName == "REWIND_PLAYBACK")
		onRewindPlayback();
	else if(taskName == "KILL_APP")
		onKillApplication();
	else if(taskName == "LOAD_FDP")
		ret = onLoadFDP(task.getParameter(0), task.getParameter(1));
	else if(taskName == "LOAD_FAP")
		ret = onLoadFAP(task.getParameter(0));	
	else if(taskName == "LOAD_PHO")
		ret = onLoadPHO(task.getParameter(0), task.getParameter(1));	
	else if(taskName == "LOAD_WAV")
		ret = onLoadWAV(task.getParameter(0));
	else if(taskName == "LOAD_ANIM")
		ret = onLoadANIM(task.getParameter(0));
	else if(taskName == "UPLOAD_FAP")
		ret = onUpLoadFAP(task.getParameter(0));
	else if(taskName == "UPLOAD_PHO")
		ret = onUpLoadPHO(task.getParameter(0), task.getParameter(1), task.getParameter(2));
	else if(taskName == "UPLOAD_ANIM")
		ret = onUpLoadANIM(task.getParameter(0));
//	else if(taskName == "UPLOAD_FDP")
//		ret = onUpLoadFDP(task.getParameter(0));
	else if(taskName == "UPLOAD_SCRIPT")
		ret = onUpLoadScript(task.getParameter(0), task.getParameter(1));
	else if(taskName == "RENDER_FRAME")
		onRenderFrame();
	else if(taskName == "ADVANCE_FRAME")
		onAdvanceFrame();
	else
		std::cerr << "Unhandled task : " << taskName << std::endl;

	m_dispatching = false;
	return ret;
}

}// namespace XFaceApp
