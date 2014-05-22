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
 * The Original Code is wxFacePlayer
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

/*!
	\brief XFaceApp link
	\file wxFace.h
*/

#pragma once
#ifndef WXFACE_H_
#define WXFACE_H_

#include <XFaceApp/ApplicationBase.h>
#include "ScriptProcessorLoader.h"

#include <wx/slider.h>
class FaceView;
namespace XFaceApp{

/*!
	\brief XFaceApp link
	\ingroup XFaceApp
	\author Koray Balci
	\version 1.0
	\date   May 2003
*/
class wxFace : public ApplicationBase
{
	std::string m_WAVFilename;
	FaceView* m_pWnd;
	wxSlider* m_pSlider;
	
	bool createScriptProcessors();
	ScriptProcessorLoader m_scriptProcLoader;
	boost::shared_ptr<ISound> createSound() const;
	boost::shared_ptr<XEngine::ITimer> createTimer() const;
	boost::shared_ptr<XFace::FaceBase> createFace() const;
	boost::shared_ptr<XEngine::IRenderer> createRenderer() const;
	boost::shared_ptr<XFace::IFapStream> createFapStream() const;
	boost::shared_ptr<IScriptProcessor> getScriptProcessor(const std::string& name) const;
	bool yield();
	void renderBegin() const;
	void renderEnd() const;
protected:
	void onKillApplication();
	bool onResumePlayback();
	void onRewindPlayback() const;
	void onStopPlayback();
	bool onLoadFDP(const std::string& param1, const std::string& param2);
	bool onLoadWAV(const std::string& param);
	bool onLoadFAP(const std::string& param);
	bool onUpLoadFAP(const std::string& param);
	bool onUpLoadScript(const std::string& param1, const std::string& param2);
	void fireNotification(const Notification& note) const;
public:
	bool init();
	bool processTask(); 
	bool saveWAV(const std::string& filename);
	bool saveFAP(const std::string& filename);
	void saveAVI(const std::string& filename);
	void newTask(const Task& task);
	void onRenderFrame() const;
	void jumpToFap(unsigned long fapID);
	wxFace(FaceView* pWnd, wxSlider* pSlider);
};

} // namespace XFaceApp

#endif // WXFACE
