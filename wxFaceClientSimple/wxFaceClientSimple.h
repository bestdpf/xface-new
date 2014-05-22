#pragma once
#include <wx/wx.h>

// Define a new application type, each program should derive a class from wxApp
class FaceClientSimpleApp : public wxApp
{
public:
	void OnFatalException();
	// this one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit(void);
};
DECLARE_APP(FaceClientSimpleApp)

