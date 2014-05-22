#pragma once
#include <wx/wx.h>
#include <wx/dnd.h>

class DropFDPFileTarget :
	public wxFileDropTarget
{
public:
	bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
};
