#pragma once
#include <wx/dir.h>
#include <wx/arrstr.h>

class DicTraverser : public wxDirTraverser
{
	wxArrayString m_filenames;
public:
	wxArrayString getFileNames() const {return m_filenames;}
	virtual wxDirTraverseResult OnFile(const wxString& filename);
	virtual wxDirTraverseResult OnDir(const wxString& dirname){return wxDIR_CONTINUE;};
    
	DicTraverser(void);
	~DicTraverser(void);
};
