#define WIN32_LEAN_AND_MEAN
#include "VisitorSpeech.h"
#include "ItalianFlite.h"
#include <wx/file.h>
#include <wx/utils.h>
#include <Xface/PhoLoader.h>

const ItalianFliteMaker ItalianFliteMaker::m_regItalianFlite;
ItalianFlite::ItalianFlite(const std::string& pathname)
	: m_pathname(pathname), ITTSEngine()
{
	createSmilVisitor();
}

std::map<std::string, float> ItalianFlite::speak(const std::string& speech, const std::string& filename)
{
	// example:
	//it_flite -o TempFile.wav --sets lab_filename=TempFile.pho --sets voice=lp -t "ciao, come stai?" 
	wxString params = m_pathname.c_str();
	params.append("it_flite -o ");
	params.append(filename.c_str());
	params.append(".wav --sets lab_filename=");
	params.append(filename.c_str());
	params.append(".pho --sets voice=lp -t ");
	
	// add the content to be synthesized
	params.append("\"");
	params.append(speech.c_str());
	params.append("\"");

	bool retVal = (wxExecute(params, wxEXEC_SYNC) == 0); // success = 0
	if(!retVal)
		std::cerr << "An error occured while trying to synthesize speech using italian flite\n";
	else
	{
		XFace::PHOLoader phonemeLoader;
		phonemeLoader.load((filename + ".pho"));
		m_duration = phonemeLoader.getTotalDuration();
	}
	
	return m_marks;
}


void ItalianFlite::createSmilVisitor() 
{
	m_smilVisitor = new VisitorSpeech; // base visitor is sufficient}
}