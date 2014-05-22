#pragma once
#include <wx/wx.h>
#include <wx/dnd.h>

class wxFacePlayerFrame;
class DropFAPFileTarget :	public wxFileDropTarget
{
	const wxFacePlayerFrame* const m_pOwner;
public:
	DropFAPFileTarget(const wxFacePlayerFrame* const pOwner) : m_pOwner(pOwner) {}
	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
};
