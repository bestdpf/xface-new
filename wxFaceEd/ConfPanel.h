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

#pragma once
#ifndef CONFPANEL_H_
#define CONFPANEL_H_

#include <wx/grid.h>
#include <wx/panel.h>
#include <wx/choice.h>

#include <vector>
#include <string>
#include <deque>
#include <map>
#include <list>

#include <XEngine/Entity.h>
#include <XEngine/Drawable.h>
#include <XEngine/ModelFileFactory.h>

using XEngine::Entity;

class ConfPanel :
	public wxPanel
{
	std::deque<Entity> m_keyframes;
	std::deque<XEngine::MeshInfo> m_keyframeMeshes;
	unsigned short m_selectedKey, m_selectedRestMesh;
	wxGrid* m_pKeyGrid;
	XEngine::MeshInfo m_restMesh;
	int m_gridOnFocus;
	
	enum WIDGET_IDS
	{
		ID_KEY_GRID = 1601,	
		ID_KEY_REMOVE,
		ID_KEY_ADD,
		ID_PREVIEW_KEYGRID, 
		
		ID_CONF_MESHGRID,
		ID_CONF_UNLOADMESH,
		ID_CONF_LOADMESH
	};
	DECLARE_EVENT_TABLE()
//	void OnMeshListItemSelected(wxCommandEvent &event );
	void OnLoadMesh(wxCommandEvent &event );
	void OnBindingCellChanged(wxGridEvent& event);
	void OnMeshGridItemSelected(wxGridEvent& event);

	void OnAddKeyframe(wxCommandEvent &event );
	void OnRemoveKeyframe(wxCommandEvent &event );
	void OnKeyGridItemSelected(wxGridEvent& event);

	void AddKeyframe(XEngine::MeshInfo info);
	void UnloadRestMesh();
public:
	ConfPanel(wxWindow* parent = 0);
	void addDictionary(const std::map<std::string, std::map<std::string, Entity> >& dict);
	void resetDictionary();
	void clearKeyframes();
	const std::list<boost::shared_ptr<XEngine::Drawable> >& getSelectedDrawables() const;
	void setRestMeshes(const std::list<boost::shared_ptr<XEngine::Drawable> >& dr) {m_restMesh.drawables = dr;}
	void ClearPanel();
};

#endif // CONFPANEL_H_