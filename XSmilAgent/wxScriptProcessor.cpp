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
 * The Original Code is XfaceApp Application Library.
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * For info, contact: xface-info@fbk.eu or http://xface.fbk.eu
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

#include <wx/file.h>
#include <wx/utils.h>
#include "wxScriptProcessor.h"

wxScriptProcessor::wxScriptProcessor(const std::string& name) : IScriptProcessor(name)
{
}

wxScriptProcessor::~wxScriptProcessor(void)
{
}

bool wxScriptProcessor::process(const std::string& scriptString, const std::string& filename)
{
	m_errorString.clear();
	// get the current working dir
	wxString oldWD = wxGetCwd();
	
	// move to dir
	::wxSetWorkingDirectory(wxString(m_path.c_str(), wxConvLibc));
	
	// execute the command
	wxString cmd((m_path + "\\" + m_executable + " ").c_str(), wxConvLibc);
	cmd.append(wxString(m_params1.c_str(), wxConvLibc));

	if (!m_tempFileName.empty())
	{
		// write script string to an xml file
		wxFile tempFile(wxString((filename + ".xml").c_str(), wxConvLibc), wxFile::write);
		tempFile.Write(wxString(scriptString.c_str(), wxConvLibc));
		tempFile.Close();
		cmd.append(_T(" "));
		cmd.append(wxString((filename + ".xml ").c_str(), wxConvLibc));
	}
	else
	{
		cmd.append(_T("\""));
		cmd.append(wxString(scriptString.c_str(), wxConvLibc));
		cmd.append(_T("\""));
	}
	
	cmd.append(wxString(m_params2.c_str(), wxConvLibc));
	
	bool retVal = (wxExecute(cmd, wxEXEC_SYNC) == 0); // success = 0
	if(!retVal)
	{
		m_errorString = "You have encountered an external error, not created by Xface!\nThe error comes from the script processor \"";
		m_errorString.append(m_executable.c_str());
		m_errorString.append("\"\nSo, please make sure\n\t- you have installed your script processor properly.");
	}
		
	// set the wd old back
	::wxSetWorkingDirectory(oldWD);

	return retVal;
}
