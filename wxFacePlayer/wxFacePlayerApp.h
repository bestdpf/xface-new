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
 * The Original Code is wxFacePlayer
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
#ifndef WXFACEPLAYERAPP_H_
#define WXFACEPLAYERAPP_H_

#include <iostream>
#include <fstream>

class wxFacePlayerFrame;

// Define a new application type, each program should derive a class from wxApp
class wxFacePlayerApp : public wxApp
{
	std::streambuf* old_cerr;
	std::fstream debuglog;
	wxFacePlayerFrame *m_pFrame;
	DECLARE_EVENT_TABLE()
public:
    wxFacePlayerApp();
    virtual ~wxFacePlayerApp();

    // override base class virtuals
    // ----------------------------

	void OnFatalException();
    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
	void OnKeyUp(wxKeyEvent& event);
	bool OnCmdLineParsed(wxCmdLineParser& parser);
	void OnInitCmdLine(wxCmdLineParser& parser);
};

DECLARE_APP(wxFacePlayerApp)

#endif //WXFACEPLAYERAPP_H_