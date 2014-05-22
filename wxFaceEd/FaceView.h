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

#ifndef FACEVIEW_H_
#define FACEVIEW_H_

#include <wx/glcanvas.h>
#include <wx/window.h>
#include <wx/wx.h>
#include <wx/sound.h>


#include <XFaceApp/ModelCamera.h>
#include "RubberBandBox.h"
#include <XFace/FDP.h>
#include <XEngine/Drawable.h>
#include <XEngine/IndexedFaceSet.h>

#include <XMath/XTriangle.h>
#include <XMath/Ray3.h>
#include <XMath/Quaternion.h>
#include <XFace/FaceBase.h>
#include <XFace/FapFile.h>

#include <list>
#include <set>
#include <string>
#include <boost/shared_ptr.hpp>
#include "ModelViewerCamera.h"

using namespace XFace;
using namespace XMath;

class FaceView : public wxGLCanvas
{
	friend class Mediator;
	bool GetFAPU(boost::shared_ptr<FDP> pFDP);
	float distance(const Vector3& a, const Vector3& b) const
	{
		return float( sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y) + (a.z - b.z)*(a.z - b.z)));
	};
	void OnChar(wxKeyEvent& event);
	std::list<boost::shared_ptr<XEngine::Drawable> > m_pDrawables;
	Vector3 m_FAPUPoints[7];
	bool m_bFAPU[7];
	unsigned short m_currentFAPU;
	void ShowFAPU();
	std::set<unsigned short> m_selectedIndices;
	short m_controlPoint;
	IndexedFaceSet* GetCurrentMesh(); // utility function
	RubberBandBox m_Box;
	const std::vector<float>& getNextFAP(const boost::shared_ptr<XFace::IFapStream>& fapfile, bool start);
	Transform m_globalTransform;
	ModelViewerCamera m_viewCamera;
public:
	enum SelectMode{
		kNONE = -1,
		kFAPU,
		kFDP,
	};

	enum CameraMode{
		NAVIGATE = 0,
		SELECT,
	};
	void SetSelectMode(SelectMode mode){m_SelectMode = mode; UpdateCursor();}
	void SetFAPUSelectMode(unsigned short id){m_currentFAPU = id;}
private:
	CameraMode m_CameraMode;
	SelectMode m_SelectMode;

	void UpdateCursor();
	void ShowAllVertices(const IndexedFaceSet* pMesh);
	void ShowSelectedVertices();
	long m_wndHeight, m_wndWidth;
	
public:
	void ResetCamera();
	const Transform& GetGlobalTransform() const {return m_globalTransform;}
	void SetGlobalTransform(const Transform& tr);
	void PreviewMorphing(FaceBase* pFace, wxSound& snd, bool sndOn, float total_time);
	void PreviewSingleFAP(FaceBase* pFace, const boost::shared_ptr<XFace::IFapStream>& faps, Quaternion initialQuat);
	void PreviewFAP(FaceBase* pFace, const boost::shared_ptr<XFace::IFapStream>& faps, wxSound& snd, bool sndOn, wxSlider* pSlider);
	void SetCameraMode(CameraMode mode);
	int GetCameraMode() const {return (int)m_CameraMode;};
	FaceView(wxWindow *parent, const wxWindowID id = -1, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = 0, const wxString& name = _T("FaceView"));
 	
	~FaceView(void);
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnEraseBackground(wxEraseEvent& event){};
	void OnMouse(wxMouseEvent& event);
	void OnLeftButtonDown(wxMouseEvent& event);
	void OnRightButtonDown(wxMouseEvent& event);
	void OnRightButtonUp(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	
	void InitGL(void);
	
private:
	void Render( void );
	bool   m_init;
	
	DECLARE_EVENT_TABLE()
};

#endif // FACEVIEW_H_