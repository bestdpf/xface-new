#include "DropFDPFileTarget.h"
#include "Mediator.h"
#include <wx/filename.h>

bool DropFDPFileTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	size_t nFiles = filenames.GetCount();
    for ( size_t n = 0; n < nFiles; n++ ) 
	{
		wxFileName curr(filenames[n]);
		if(curr.GetExt().CmpNoCase(_T("fdp")) == 0)
		{
			if(Mediator::getInstance()->loadConfiguration((const char*)curr.GetFullName().c_str(), 
								(const char*)curr.GetPath(wxPATH_GET_SEPARATOR | wxPATH_GET_VOLUME).c_str()))
				return true;
		}
    }
	
	return false;
}
