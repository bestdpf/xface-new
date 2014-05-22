#include "DropFAPFileTarget.h"
#include "wxFacePlayerFrm.h"
#include <wx/filename.h>

bool DropFAPFileTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	size_t nFiles = filenames.GetCount();
    for ( size_t n = 0; n < nFiles; n++ ) 
	{
		wxFileName curr(filenames[n]);
		if(curr.GetExt().CmpNoCase(_T("fap")) == 0)
		{
			m_pOwner->LoadFAP(curr.GetFullPath());
			return true;
		}
		else if(curr.GetExt().CmpNoCase(_T("wav")) == 0)	// yes, one more type won't hurt (contradiction with class name)
		{
			m_pOwner->LoadWAV(curr.GetFullPath());
			return true;
		}
    }
	
	return false;
}
