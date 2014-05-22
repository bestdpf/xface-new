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

#include "wxFace.h"
#include "FaceView.h"

#include <wx/utils.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
#include <wx/progdlg.h>
#include <wx/busyinfo.h>

#include "wxFacePlayerFrm.h"
#define FPS_FOR_AVI 25.0f

#include "SDLSound.h"
#include "wxFaceTimer.h"
#include "DicTraverser.h"
#include <XFace/FapFile.h>
#include <XEngine/RendererGL.h>
#include <XFace/FaceBase.h>
#ifdef WIN32
	#include "AVIGenerator.h"
#endif

#ifdef WIN32
	#include "ItalianFlite.h"
#endif
 
namespace XFaceApp{

using namespace XFace;
using namespace XEngine;

wxFace::wxFace(FaceView* pWnd, wxSlider* pSlider) : m_pSlider(pSlider), m_pWnd(pWnd)
{
	TaskDictionary::registerTask("SAVE_AVI", 1);
}

bool wxFace::init()
{
    printf("wxFace init\n");
	if(ApplicationBase::init())
	{
		DicTraverser traverser;
		wxDir dir(_T("./lang/"));
		dir.Traverse(traverser);
		wxArrayString dics = traverser.getFileNames();
		for (int i = 0; i < dics.GetCount(); ++i)
			m_pFace->addPhonemeDictionary((const char*)dics[i].c_str());
		if(dics.GetCount() == 0)
			std::cerr << "No dictionary file found!\n";

		return true;
	}
	
	return false;
}

bool wxFace::createScriptProcessors()
{
    printf("wxFace createScriptProcessors\n");
	return m_scriptProcLoader.load("scriptProcs.xml");
}

boost::shared_ptr<IScriptProcessor> wxFace::getScriptProcessor(const std::string& name) const
{
    printf("getScriptProcessor\n");
	return m_scriptProcLoader.getScriptProcessor(name);
}

boost::shared_ptr<ISound> wxFace::createSound() const
{
    printf("create sound\n");
	boost::shared_ptr<ISound> ret(new SDLSound);
	return ret;
}

boost::shared_ptr<XEngine::ITimer> wxFace::createTimer() const
{
    printf("create timer\n");
	boost::shared_ptr<XEngine::ITimer> ret(new wxFaceTimer);
	return ret;
}

boost::shared_ptr<XFace::FaceBase> wxFace::createFace() const
{
    printf("create face\n");
	boost::shared_ptr<XFace::FaceBase> ret(new XFace::FaceBase);
	return ret;
}

boost::shared_ptr<XEngine::IRenderer> wxFace::createRenderer() const
{
    printf("create render\n");
	boost::shared_ptr<XEngine::IRenderer> ret(new XEngine::RendererGL);
	return ret;
}

boost::shared_ptr<XFace::IFapStream> wxFace::createFapStream() const
{
    printf("create fap stream\n");
	boost::shared_ptr<XFace::IFapStream> ret(new XFace::FAPFile);
	return ret;
}

bool wxFace::yield()
{
    return true;
    printf("yield\n");
	return wxTheApp->Yield(TRUE);
}
void wxFace::renderBegin() const
{
    printf("renderbegin\n");
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
//		glLoadIdentity( );
//		glTranslatef( 0.0, 20.0, -650.0 );
}

void wxFace::renderEnd() const
{
    printf("render end\n");
	glPopMatrix();
	m_pWnd->SwapBuffers();
}

void wxFace::onKillApplication()
{
    printf("on kill app\n");
	ApplicationBase::fireNotification(Notification::kFinished);
	wxExit();
}

bool wxFace::onLoadFAP(const std::string& param)
{
    printf("on load fap\n");
	//wxString msg;
	//msg.Printf(_T("Loading %s, please wait..."), param.c_str());
	//wxLogStatus(msg, 0);
	bool ret = ApplicationBase::onLoadFAP(param);
	if(m_pSlider)
		m_pSlider->SetRange(0, m_pFapStream->getFAPCount() - 1);
	//wxLogStatus(wxT("File loaded successfully."));
    printf("wx face load fap succ\n");
	return ret;
}

bool wxFace::onResumePlayback()
{
	bool retVal = false;
	if(m_pSlider && m_renderMode == KEYFRAME && m_pSlider->IsShown())
		m_pSlider->Hide();
	else if(m_pSlider && m_renderMode == FAP && !m_pSlider->IsShown())
		m_pSlider->Show(true);
	if(m_renderMode == KEYFRAME)
	{
		wxBusyCursor busy;
		retVal = ApplicationBase::onResumePlayback();
	}
	else if(m_renderMode == FAP)
	{
		retVal = ApplicationBase::onResumePlayback();
	}

	if(m_pSlider)
		m_pSlider->Show(true);
	return retVal;
}

void wxFace::onRewindPlayback() const
{
	if(m_pSlider)
		m_pSlider->SetValue(m_pFapStream->getCurrentFAPId());
	ApplicationBase::onRewindPlayback();
}

void wxFace::onRenderFrame() const
{
    printf("on render frame\n");
	if(m_pSlider)
		m_pSlider->SetValue(m_pFapStream->getCurrentFAPId());
	ApplicationBase::onRenderFrame();
}

void wxFace::onStopPlayback()
{
	ApplicationBase::onStopPlayback();
	if(m_pSlider)
		m_pSlider->SetValue(m_pFapStream->getCurrentFAPId());
}

void wxFace::jumpToFap(unsigned long fapID)
{
	unsigned long cnt = m_pFapStream->getFAPCount();
	if(cnt < fapID)
		return;

	if(m_pFapStream->getCurrentFAPId() > fapID)
		m_pFapStream->rewind();
	while (m_pFapStream->getCurrentFAPId() != fapID)
		m_pFapStream->next();

	const std::vector<float>& FAPs = m_pFapStream->getCurrentFAP();
	if(!FAPs.empty())
	{
		m_pSound->jump((float)fapID / (float)cnt);
		m_pFace->update(m_pFapStream);
		m_renderManager.setGlobalTransform(m_pFace->getTransform());
		renderBegin();
			m_renderManager.render();
		renderEnd();		
	}
	
}

bool wxFace::onUpLoadScript(const std::string& param1, const std::string& param2)
{
	wxBusyCursor busy;
	return ApplicationBase::onUpLoadScript(param1, param2);
}

bool wxFace::onLoadFDP(const std::string& param1, const std::string& param2)
{
    printf("on load fdp\n");
	wxBusyInfo wait(_T("Please wait, loading fdp file...\n(it can take a while)"), m_pWnd);

	return ApplicationBase::onLoadFDP(param1, param2);
}

bool wxFace::onUpLoadFAP(const std::string& param)
{
	bool ret = ApplicationBase::onUpLoadFAP(param);
	if(m_pSlider)
		m_pSlider->SetRange(0, m_pFapStream->getFAPCount() - 1);
	
	return ret;
}

bool wxFace::onLoadWAV(const std::string& param)
{
	wxString msg;
	m_WAVFilename = param;
	msg.Printf(_T("Loading %s, please wait..."), param.c_str());
	wxLogStatus(msg, 0);
	bool ret = ApplicationBase::onLoadWAV(param);
	if(ret)
		wxLogStatus(wxT("Wav file loaded successfully."));
	else
		wxLogStatus(wxT("Could not load wav file."));

	return ret;
}

void wxFace::newTask(const Task& task)
{
    printf("new task\n");
	static int test = 0;
	if(task.getID() != 0)
	{
		wxString msg;
		msg.Printf(_T("Task: %d"), task.getID());
		wxLogStatus(msg, 1);
	}
	ApplicationBase::newTask(task);
}

void wxFace::fireNotification(const Notification& note) const
{
	m_pWnd->fireNotification(note);
}

/*!
	Save wav actually tricks the user! We keep the name of the currently open wav file,
	and just copy that file to the new location. After all we do NOT support sound 
	streaming, always working with wave files.
*/
bool wxFace::saveWAV(const std::string& filename)
{
	return ::wxCopyFile(wxString(m_WAVFilename.c_str(), wxConvLibc), wxString(filename.c_str(), wxConvLibc));
}
bool wxFace::saveFAP(const std::string& filename)
{
	return XFace::FAPFile::save(*m_pFapStream, m_pFace->getFAPU(), filename);
}

/*!
	Saves animation as avi.
*/
void wxFace::saveAVI(const std::string& filename)
{
#ifdef WIN32
	m_pWnd->getMainFrame()->HideControls();
	// remove any old file with the same name
	::wxRemoveFile((filename + ".avi").c_str());
	
	wxSize wndSize = m_pWnd->GetVirtualSize();
	unsigned int w = wndSize.GetWidth(), h = wndSize.GetHeight();
	
	CAVIGenerator AviGen;	// generator
	BYTE* bmBits = 0;	// image buffer
	HRESULT hr;

	AviGen.SetFileName(filename.c_str());
	if(m_pFapStream->getFPS() > 0 && m_renderMode == FAP )
		AviGen.SetRate(m_pFapStream->getFPS());	
	else
		AviGen.SetRate( FPS_FOR_AVI );	// set 25fps for keyframe mode FPS_FOR_AVI is 25 at the top as define

	// initialize m_bih
	BITMAPINFOHEADER bih;
	memset(&bih,0, sizeof(BITMAPINFOHEADER));
	// filling bitmap info structure.
	bih.biSize		= sizeof(BITMAPINFOHEADER);
	bih.biWidth		= w - w%4;
	bih.biHeight	= h - h%4;
	bih.biPlanes	= 1;
	bih.biBitCount	= 24;
	bih.biSizeImage	= ((bih.biWidth*bih.biBitCount+31)/32 * 4)*bih.biHeight; 
	bih.biCompression=BI_RGB;		//BI_RGB means BRG in reality

	AviGen.SetBitmapHeader(&bih);	// get bitmap info out of the view
	hr = AviGen.InitEngine();	// start engine
	
	if (hr != AVIERR_OK)
	{
		AviGen.ReleaseEngine();
		wxLogMessage("Unable to initialize engine! Please try a different codec...");
		return;
	}

	wxMessageBox("Xface may refuse responding for a while, but please be patient,\nit will come back alive after saving video.\nPlease do not keep player window on top of other windows, some video cards does not like otherwise...");
	// rewind the animation
	discardPendingTasks();

	// allocating memory for bmBits
	bmBits = new BYTE[3 * bih.biWidth* bih.biHeight];	
	if(m_renderMode == FAP)
	{
		while (!query(XFaceApp::ApplicationBase::QUERY_FAPS_FINISHED))	
		{
			glReadPixels(0,0,bih.biWidth,bih.biHeight, 
					GL_BGR_EXT,GL_UNSIGNED_BYTE,bmBits);
		
			onRenderFrame();
			onAdvanceFrame();
	
			// adding frame and continue if OK
			hr=AviGen.AddFrame(bmBits);
			if (FAILED(hr))
			{
				wxLogStatus(AviGen.GetLastErrorMessage());
				break;
			}
		}
	}
	else if (m_renderMode == KEYFRAME)
	{
		XEngine::MorphController* cont = XEngine::MorphController::getInstance();
		m_pFace->rewindKeyframeAnimation();
		std::list<boost::shared_ptr<XEngine::Drawable> > oldDrawables = m_pFace->getDrawables();
		float _sequenceDuration = m_sequenceDuration, elapsed = 1000.0/FPS_FOR_AVI;

		do
		{
			onRenderFrame();

			glReadPixels(0,0,bih.biWidth,bih.biHeight, 
				GL_BGR_EXT,GL_UNSIGNED_BYTE,bmBits);

			// adding frame and continue if OK
			hr=AviGen.AddFrame(bmBits);
			if (FAILED(hr))
			{
				wxLogStatus(AviGen.GetLastErrorMessage());
				std::cerr << "Video saving error: " << AviGen.GetLastErrorMessage() << std::endl;
				break;
			}

			_sequenceDuration -= elapsed;
			const XEngine::Entity& res = m_pFace->update(elapsed);

			if(!res.getDrawableCount())
				break;
			m_renderManager.setGlobalTransform(m_pFace->getTransform());
			m_renderManager.resetDrawables();
			m_renderManager.addDrawables(res.getDrawables());
	
		}while (_sequenceDuration > 0);
		m_renderManager.resetDrawables();
		m_renderManager.addDrawables(oldDrawables);
		onRenderFrame();
	}
	
	AviGen.AddWav(m_WAVFilename.c_str());
	
	// cleaning memory	
	AviGen.ReleaseEngine(); // releasing resources
	delete[] bmBits;	// release resources
	bmBits = 0;	
	wxLogMessage("Saved video successfully.");

	// rewind the animation to beginning
	onRewindPlayback();
	m_pWnd->getMainFrame()->HideControls();
#else
	wxLogStatus(_T("Save Avi is not supported under this platform yet!"));
#endif
}

}
bool wxFace::processTask()
{
    printf("wx face process task\n");
	if (m_taskQueue.size() == 0) {
        printf("task queue is 0\n");
		return false;
	}

	const Task &tsk_c = m_taskQueue.front();
	if(tsk_c.getName() == "SAVE_AVI")
	{
		// get the task out for proper notification handling 
		m_currentTask = m_taskQueue.front();
		//Task tsk = m_taskQueue.front();
		//m_pCurrTask = &tsk;
		m_taskQueue.pop();
	
		saveAVI(m_currentTask.getParameter(0));
		ApplicationBase::fireNotification(Notification::kFinished);

		return true;
	}
	
	return ApplicationBase::processTask();
}
