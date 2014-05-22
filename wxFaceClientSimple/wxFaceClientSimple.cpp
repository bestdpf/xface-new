#include "wxFaceClientSimple.h"
#include "FaceClientSimpleDlg.h"

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. 
//wxFaceClientSimple and not wxApp)
IMPLEMENT_APP(FaceClientSimpleApp)

void FaceClientSimpleApp::OnFatalException()
{
	wxMessageBox("XfaceClientSimple did something wrong and is about to shut itself down!\nPlease report this problem to xface-info@fbk.eu if you think it is a bug.\n\nThank you for your patience and cooperation.\n-Xface Team-", "Fatal Xface Error", wxICON_ERROR | wxCENTRE, 0);
}

// 'Main program' equivalent: the program execution "starts" here
bool FaceClientSimpleApp::OnInit(void)
{
	::wxHandleFatalExceptions();
	// create the main application window
	FaceClientSimpleDlg* l_dialog = new FaceClientSimpleDlg(
		NULL, -1, _("FaceClientSimple"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	l_dialog->Show(TRUE);
	SetTopWindow(l_dialog);
	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned FALSE here, the
	// application would exit immediately.
	return TRUE;
}
