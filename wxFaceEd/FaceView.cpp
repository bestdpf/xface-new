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
 * The Original Code is wxFaceEd
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include "FaceView.h"

#include <wx/timer.h>
#include <wx/log.h>
#include <wx/utils.h>
#include <wx/sound.h>
#include <wx/image.h>

#include <GL/glu.h>
#include <iostream>
#include <fstream>

#include "VertexSelector.h"
#include "Mediator.h"
#include "../wxFacePlayer/wxFaceTimer.h"
#include <XEngine/MeshManager.h>
#include <XEngine/RendererGL.h>

#include "res/cursor_crosshair_target.xpm"
#include "res/cursor_move_outlined.xpm"


#ifdef WIN32
	#include "../wxFacePlayer/AVIGenerator.h"
#endif


BEGIN_EVENT_TABLE(FaceView, wxGLCanvas)
  EVT_SIZE(FaceView::OnSize)
  EVT_PAINT(FaceView::OnPaint)
  EVT_ERASE_BACKGROUND(FaceView::OnEraseBackground)
  EVT_LEFT_DOWN(FaceView::OnLeftButtonDown)
  EVT_RIGHT_DOWN(FaceView::OnRightButtonDown)
  EVT_RIGHT_UP(FaceView::OnRightButtonUp)
  EVT_MOTION(FaceView::OnMouseMove)
  EVT_MOUSE_EVENTS(FaceView::OnMouse)
  EVT_CHAR(FaceView::OnChar) 
END_EVENT_TABLE()

FaceView::FaceView(wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name):
  wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style, name )
{
	//wxLogNull nolog;
	SetCameraMode(NAVIGATE);
	//m_CameraMode = NAVIGATE;
	m_SelectMode = kNONE;
	m_currentFAPU	= 0;
	for (int i = 0; i < 7; ++i)
		m_bFAPU[i] = false;

    m_init = FALSE;
	m_globalTransform.setTranslation(m_viewCamera.getReferencePoint());
	m_globalTransform.setRotation(Quaternion(AxisAngle(Vector3(-0.999632, -0.0154467, 0.0223226), 0.407589)));
	m_globalTransform.updateLocalTransform();
}

FaceView::~FaceView()
{
}

const std::vector<float>& FaceView::getNextFAP(const boost::shared_ptr<XFace::IFapStream>& fapfile, bool start)
{
	static wxStopWatch timer;
	static wxStopWatch frametimer;
	static wxLongLong frame_count = 0;
	
	// this means, we are beginning playback now (first frame is about to be played)
	if(start)
	{
		// do init
		timer.Start();
		frametimer.Start();
		frame_count = 0;
		fapfile->rewind();
	}
	
	long total_elapsed_time = timer.Time();
	long frame_time = 1000 / fapfile->getFPS();
	long elapsed_frames = total_elapsed_time / frame_time;

	// if slow, catch up
	while(elapsed_frames > frame_count)
	{
		fapfile->next();
		frame_count++;
		frametimer.Start();
	}

	// if fast, slow down
	if(frametimer.Time() < frame_time)
		::wxMilliSleep(frame_time - frametimer.Time());
	
	// get the data finally
	const std::vector<float>& FAPs = fapfile->getCurrentFAP();
	fapfile->next();

	frame_count++;
	// don't forget to record the new timestamp
	frametimer.Start();

	return FAPs;
}

void FaceView::PreviewMorphing(FaceBase* pFace, wxSound& snd, bool sndOn, float total_time)
{
	if(!pFace)
	{
		wxLogMessage(_T("Are you sure you loaded a face (FDP File)?"));
		return;
	}

	wxFaceTimer timer;
	std::list<boost::shared_ptr<XEngine::Drawable> > oldDrawables = m_pDrawables;
	timer.start();
	if(sndOn && snd.IsOk())
		snd.Play();

	unsigned long elapsed = 0, yield_time = 0;
	while (total_time > 0)
	{
		elapsed = timer.getElapsedTime(true);
		const Entity& res = pFace->update(elapsed);
		total_time -= elapsed;

		// we need to give some time to the app, to prevent unwanted stalls.
		yield_time += elapsed;
		if(yield_time > 2000)	// if it has been 2 seconds since last yield.
		{
			wxTheApp->Yield();
			yield_time = 0;
		}
	
		XEngine::Transform tr = pFace->getTransform();
		tr.updateLocalTransform();
		tr.setTranslation(m_globalTransform.getTranslation());
		tr.update(m_globalTransform.getLocalTransform());
		m_globalTransform = tr;
		
		m_pDrawables.clear();
		const std::list<boost::shared_ptr<XEngine::Drawable> >& dummy = res.getDrawables();
		m_pDrawables.insert(m_pDrawables.end(), dummy.begin(), dummy.end());
		Render();
	}

	m_pDrawables.clear();
	m_pDrawables = oldDrawables;
}

void FaceView::PreviewFAP(FaceBase* pFace, const boost::shared_ptr<XFace::IFapStream>& faps, wxSound& snd, bool sndOn, wxSlider* pSlider)
{
	if(!faps->isOpen())
	{
		wxLogMessage(_T("Are you sure you loaded a FAP file?"));
		return;
	}

	if(!pFace)
	{
		wxLogMessage(_T("Are you sure you loaded a face (FDP File)?"));
		return;
	}

	// play it
	bool bPlaying(true), start(true);
	pFace->initInfluenceCalculators();
	
	wxBusyCursor wait;
	wxWindowDisabler stopper;
	Transform initTrans = m_globalTransform;
	while(bPlaying)	// if we are on animation
	{
		const std::vector<float>& FAPs = getNextFAP(faps, start);
		if(start)
		{
			if(sndOn && snd.IsOk())
				snd.Play();
			start = false;
		}

		if(!FAPs.empty())
		{
			PreviewSingleFAP(pFace, faps, initTrans.getRotation());
			assert(pSlider);
			pSlider->SetValue(faps->getCurrentFAPId());
		}
		else
		{
			bPlaying = false;
		}
	}

	pFace->resetDeformedVertices();

	faps->rewind();
	assert(pSlider);
	pSlider->SetValue(0);
}

void FaceView::PreviewSingleFAP(FaceBase* pFace, const boost::shared_ptr<XFace::IFapStream>& faps, Quaternion initialQuat)
{
	// updating faps updates the rotation 
	pFace->update(faps);

	// use the initial head rotation to fix 
	// store the global one first to a temporary
	Transform dummy = m_globalTransform; 
	m_globalTransform = pFace->getTransform();
	m_globalTransform.rotate(initialQuat);
	m_globalTransform.updateLocalTransform();

	Render();
	// reset the deformation
	pFace->resetDeformedVertices();
	// restore the global transform
	m_globalTransform = dummy;
}

void FaceView::Render()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
	glDepthRange(0.00002, 1.0);
		
	// render each drawable here
	static RendererGL renderer;
	renderer.setTransform(m_globalTransform);
	std::list<boost::shared_ptr<XEngine::Drawable> >::iterator it = m_pDrawables.begin();
	while (it != m_pDrawables.end())
	{
		renderer.render(*it/*->get()*/);
		++it;
	}
	
	//if(m_CameraMode == SELECT_VERTEX)
	if(m_pDrawables.empty())
	{
		SwapBuffers();
		return;
	}
	if (m_SelectMode == kFDP)
	{
		ShowAllVertices(GetCurrentMesh());
		ShowSelectedVertices();
	}
	else if (m_SelectMode == kFAPU)
		ShowFAPU();
	if (m_CameraMode == SELECT)
	{
		if (m_SelectMode == kFDP)
			m_Box.draw(m_wndWidth, m_wndHeight);
	}

	SwapBuffers();
}

void FaceView::OnPaint( wxPaintEvent& event )
{
    wxPaintDC dc(this);

#ifndef __WXMOTIF__
    if (!GetContext()) return;
#endif

    SetCurrent();
    /* init OpenGL once, but after SetCurrent */
    if (!m_init)
    {
        InitGL();
        m_init = TRUE;
    }

	Render();
}

void FaceView::OnSize(wxSizeEvent& event)
{
    // this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);

    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	int w, h;
    GetClientSize(&w, &h);
	m_wndWidth = w, m_wndHeight = h;
#ifndef __WXMOTIF__
    if (GetContext())
#endif
    {
        SetCurrent();
		m_viewCamera.resize(w, h);
    }
}

void FaceView::InitGL()
{
	SetCurrent();

	// Our shading model--Gouraud (smooth). 
    glShadeModel( GL_SMOOTH );

    // Culling. 
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    // Set the clear color. 
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //Background color

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// needed for normal scaling during zooming
	glEnable(GL_NORMALIZE);
	
	m_viewCamera.init(this);
}

void FaceView::SetGlobalTransform(const Transform& tr) 
{
    SetCurrent();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_globalTransform = tr; 
	m_viewCamera.setReferencePoint(tr.getTranslation());
}

IndexedFaceSet* FaceView::GetCurrentMesh()
{
	if(m_pDrawables.empty())
		return 0;
	return MeshManager::getInstance()->getMesh((*m_pDrawables.begin())->getMeshName());
}

void FaceView::OnMouse(wxMouseEvent& event)
{
	int state = 0;
	wxPoint pos = event.GetPosition();
	if(event.Dragging())
	{
		m_viewCamera.move(pos.x, pos.y);
		return;
	}
	else if (event.ButtonDown())
		state = 1;
	else if (event.ButtonUp())
		state = 0;
	else
		return;

	m_viewCamera.mouse(event.GetButton(), state, pos.x, pos.y);
}

void FaceView::OnLeftButtonDown(wxMouseEvent& event)
{
	if(m_CameraMode == NAVIGATE || m_pDrawables.empty()) // safety check
	{
		event.Skip();
		return;
	}
	
	SetCurrent();
	int x(event.m_x);
	int y(event.m_y);
	//case SELECT:
	if(m_SelectMode == kNONE)
		return;
	VertexSelector selector;
	if (m_SelectMode == kFAPU)	// FAPU selection mode
	{
		std::list<boost::shared_ptr<XEngine::Drawable> >::iterator it = m_pDrawables.begin();
		while (it != m_pDrawables.end())
		{
			IndexedFaceSet* pMesh = MeshManager::getInstance()->getMesh((*it)->getMeshName());
			m_bFAPU[m_currentFAPU] = selector.screenToFace(x, y, pMesh, m_globalTransform, m_FAPUPoints[m_currentFAPU]);
			if(m_bFAPU[m_currentFAPU])
				break;
			++it;
		}
	}
	else		// FDP selection mode
	{
		// a long way to get the current mesh
		unsigned short ind;
		
		if(selector.selectVertex(x, y, GetCurrentMesh(), m_globalTransform, ind))
		{
			Mediator* pM = Mediator::getInstance();
			if ( event.ControlDown()) 
			{
				pM->onControlPoint(ind);
				m_controlPoint = ind;
			}
			else if ( event.AltDown()) 
			{
				pM->onRemoveIndex(ind);
				m_selectedIndices.erase(ind);
			}
			else
			{
				pM->onAddIndex(ind);
				m_selectedIndices.insert(ind);
			}
		}
	}
	Refresh(FALSE);
}

void FaceView::OnRightButtonDown(wxMouseEvent& event)
{
	if(m_CameraMode == NAVIGATE || m_pDrawables.empty()) // safety check
	{
		event.Skip();
		return;
	}
	
	SetCurrent();
	if( m_SelectMode == kFDP )
		m_Box.selectArea(event.m_x, m_wndHeight - event.m_y, true);
	
	Refresh(FALSE);
}

void FaceView::OnRightButtonUp(wxMouseEvent& event)
{
	if(m_CameraMode == NAVIGATE || m_pDrawables.empty()) // safety check
	{
		event.Skip();
		return;
	}

	if(event.ControlDown()) // we are doing sth else when ctrl down
	{
		event.Skip();
		return;
	}

	SetCurrent();
	if (m_SelectMode == kFDP)
	{
		const IndexedFaceSet* pCurrentMesh = GetCurrentMesh(); 
		std::set<unsigned short> rectset = VertexSelector::selectRect(pCurrentMesh, m_Box.getRectangle(), m_globalTransform.getLocalTransform());
		std::set<unsigned short>::const_iterator it = rectset.begin();
		while (it != rectset.end())
		{
			Mediator::getInstance()->onAddIndex(*it);
			++it;
		}
		m_selectedIndices.insert(rectset.begin(), rectset.end());
		//if ( event.AltDown()) 
		//	m_currFDPItem->removeAOI(indices);
		
		m_Box.clear();
	}
	
	Refresh(FALSE);
}

void FaceView::OnMouseMove(wxMouseEvent& event)
{
	if(m_CameraMode == NAVIGATE || m_pDrawables.empty()) // safety check
	{
		event.Skip();
		return;
	}
	
	if(event.ButtonDown(3))
	{
		SetCurrent();
		if(m_SelectMode == kFDP)
			m_Box.selectArea(event.m_x, m_wndHeight - event.m_y);
		Refresh(FALSE);
	}
	else
		event.Skip();
}

void FaceView::ResetCamera()
{
	glLoadIdentity();
	m_viewCamera.reset();
	Refresh();
	Update();
}

void FaceView::OnChar(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case WXK_F7:
		SetCameraMode(NAVIGATE);break;
	case WXK_F8:
		SetCameraMode(SELECT);break;
	case WXK_F9:
		ResetCamera(); break;
	};
}

void FaceView::UpdateCursor()
{
	// cursor stuff
	if(m_SelectMode == kFAPU || m_SelectMode == kFDP)
	{
		wxBitmap* curBmp = 0;
		if(m_CameraMode == SELECT)
			curBmp = new wxBitmap(cursor_crosshair_target_xpm);
		else if (m_CameraMode == NAVIGATE)
			curBmp = new wxBitmap(cursor_move_outlined_xpm);
		
		if(curBmp)
		{
			wxImage img = curBmp->ConvertToImage();
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, img.GetWidth() / 2 + 1);
			img.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, img.GetHeight() / 2 + 1);
			wxCursor cur(img);
			SetCursor(cur);

			delete curBmp;		
		}
	}
	else
		SetCursor(wxCURSOR_ARROW);
}

void FaceView::SetCameraMode(CameraMode mode)
{
	m_CameraMode = mode;
	switch (mode)
	{
	case NAVIGATE:
		wxLogMessage(_T("Mode changed to \"navigate\"."));
		break;
	case SELECT:
		wxLogMessage(_T("Mode changed to \"select\"."));
		break;
	};

	UpdateCursor();
}

void FaceView::ShowAllVertices(const IndexedFaceSet* pMesh)
{
	if(!pMesh)
		return;
	const std::vector<Vector3>& pVert = pMesh->getVertices();
	const std::vector<Vector3>& pNorm = pMesh->getNormals();
	size_t numVert = pMesh->getVertexCount();	

	glPushMatrix();
		glMultMatrixf(m_globalTransform.getLocalTransform());
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_POINT_SMOOTH);
	
	glDepthRange(0.0, 1.0 - 0.00002);
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT, GL_POINT);
	glPointSize(2);
	glColor3f(1, 0, 0);

	glBegin(GL_POINTS);
	for (size_t i = 0; i < numVert; ++i)
	{
		glVertex3fv(pVert[i]);
		glNormal3fv(pNorm[i]);
	}
	glEnd();

	glPopAttrib();
	glPopMatrix();
}

void FaceView::ShowSelectedVertices()
{
	if(m_selectedIndices.empty() && (m_controlPoint == -1) )
		return;

	const std::vector<Vector3>& pVert = GetCurrentMesh()->getVertices();

	glPushMatrix();
		glMultMatrixf(m_globalTransform.getLocalTransform());
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_POINT_SMOOTH);
	glDepthRange(0.0, 1.0 - 0.00002);
	glDisable(GL_LIGHTING);
	glPointSize(5);
	
	glBegin(GL_POINTS);
		glColor3f(0, 0, 1);
		std::set<unsigned short>::const_iterator it = m_selectedIndices.begin();
		while (it != m_selectedIndices.end())
		{
			if(*it != m_controlPoint) // ignore if this is the control point
				glVertex3fv(pVert[*it]);
			++it;
		}

	glEnd();

	// put the control point
	if(m_controlPoint >= 0)
	{
		glPointSize(7);
		glColor3f(0, 1, 0);
		unsigned short i(m_controlPoint);
		glBegin(GL_POINTS);
			glVertex3f(pVert[i].x, pVert[i].y, pVert[i].z);
		glEnd();
	}

	glPopAttrib();

	glPopMatrix();
}

void FaceView::ShowFAPU()
{
	glPushMatrix();
		glMultMatrixf(m_globalTransform.getLocalTransform());
	
	glEnable(GL_POINT_SMOOTH);
	glDepthRange(0.0, 1.0 - 0.00002);
	glDisable(GL_LIGHTING);
	glPointSize(5);
	float oldColor[4];
	glGetFloatv(GL_CURRENT_COLOR, (float*)&oldColor);

	// show the FAPU point
	glColor3f(0, 0.9f, 0);
	glBegin(GL_POINTS);
		for( int i = 0; i < 7; ++i)
		{
			if(m_bFAPU[i])
				glVertex3f(m_FAPUPoints[i].x, m_FAPUPoints[i].y, m_FAPUPoints[i].z);
		}	
	glEnd();

	// put a cool line to make it more understandable
	glColor3f(0, 0, 0.8f);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_LINES);
		//ES
		if(m_bFAPU[0] && m_bFAPU[1])
		{
			glVertex3f(m_FAPUPoints[0].x, m_FAPUPoints[0].y, m_FAPUPoints[0].z);
			glVertex3f(m_FAPUPoints[1].x, m_FAPUPoints[1].y, m_FAPUPoints[1].z);
		}
		//IRISD
		if(m_bFAPU[2] && m_bFAPU[3])
		{
			glVertex3f(m_FAPUPoints[2].x, m_FAPUPoints[2].y, m_FAPUPoints[2].z);
			glVertex3f(m_FAPUPoints[3].x, m_FAPUPoints[3].y, m_FAPUPoints[3].z);
		}
		//ENS
		if(m_bFAPU[0] && m_bFAPU[1] && m_bFAPU[4])
		{
			glVertex3f((m_FAPUPoints[0].x + m_FAPUPoints[1].x) / 2.0f, 
					(m_FAPUPoints[0].y + m_FAPUPoints[1].y) / 2.0f, 
					(m_FAPUPoints[0].z + m_FAPUPoints[1].z) / 2.0f);
			glVertex3f(m_FAPUPoints[4].x, m_FAPUPoints[4].y, m_FAPUPoints[4].z);
		}
		// MNS
		if(m_bFAPU[4] && m_bFAPU[5] && m_bFAPU[6])
		{
			glVertex3f((m_FAPUPoints[5].x + m_FAPUPoints[6].x) / 2.0f, 
					(m_FAPUPoints[5].y + m_FAPUPoints[6].y) / 2.0f, 
					(m_FAPUPoints[5].z + m_FAPUPoints[6].z) / 2.0f);
			glVertex3f(m_FAPUPoints[4].x, m_FAPUPoints[4].y, m_FAPUPoints[4].z);
		}
		//MW
		if(m_bFAPU[5] && m_bFAPU[6])
		{
			glVertex3f(m_FAPUPoints[5].x, m_FAPUPoints[5].y, m_FAPUPoints[5].z);
			glVertex3f(m_FAPUPoints[6].x, m_FAPUPoints[6].y, m_FAPUPoints[6].z);
		}
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glColor3fv((float*)&oldColor);
	glEnable(GL_LIGHTING);

	glPopMatrix();
}
	
bool FaceView::GetFAPU(boost::shared_ptr<FDP> pFDP)
{
	assert(pFDP && "FaceView::GetFAPU(FDP* pFDP) pFDP is NULL");

	// compute FAPU values OR use old ones (if they exist)
	if(m_bFAPU[0] && m_bFAPU[1])
		pFDP->setES0(distance(m_FAPUPoints[0], m_FAPUPoints[1]));
	else if (!pFDP->getES())
		return false;
	
	if(m_bFAPU[2] && m_bFAPU[3])
		pFDP->setIRISD0(distance(m_FAPUPoints[2], m_FAPUPoints[3]));
	else if (!pFDP->getIRISD())
		return false;
	
	if(m_bFAPU[0] && m_bFAPU[1] && m_bFAPU[4])
	{
		Vector3 dummy((m_FAPUPoints[0].x + m_FAPUPoints[1].x) / 2.0f, 
					(m_FAPUPoints[0].y + m_FAPUPoints[1].y) / 2.0f, 
					(m_FAPUPoints[0].z + m_FAPUPoints[1].z) / 2.0f);
		pFDP->setENS0(distance(dummy, m_FAPUPoints[4]));
	}
	else if (!pFDP->getENS())
		return false;
	
	if(m_bFAPU[5] && m_bFAPU[6] && m_bFAPU[4])
	{
		Vector3 dummy2((m_FAPUPoints[5].x + m_FAPUPoints[6].x) / 2.0f, 
					(m_FAPUPoints[5].y + m_FAPUPoints[6].y) / 2.0f, 
					(m_FAPUPoints[5].z + m_FAPUPoints[6].z) / 2.0f);
		pFDP->setMNS0(distance(dummy2, m_FAPUPoints[4]));
	}
	else if (!pFDP->getMNS())
		return false;
	
	if(m_bFAPU[5] && m_bFAPU[6])
		pFDP->setMW0(distance(m_FAPUPoints[5], m_FAPUPoints[6]));
	else if (!pFDP->getMW())
		return false;
	
	return true;		
}
