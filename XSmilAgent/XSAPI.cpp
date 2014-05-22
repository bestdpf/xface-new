#include "XSAPI.h"
#include "sphelper.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "VisitorSAPISpeech.h"

const SAPI51Maker SAPI51Maker::m_regSAPI51Maker;
SAPI51::SAPI51(const std::string& ini) : m_pVoice(0)
{
	::CoInitialize(NULL);
	if(!ini.empty())
	{
		createSmilVisitor();
		std::ifstream iniFile(("./lang/" + ini).c_str());
		if(iniFile.is_open())
		{
			std::string opt;
			std::getline(iniFile, opt);
			std::copy(opt.begin(), opt.end(), std::back_inserter(m_options1));
			opt.clear();
			std::getline(iniFile, opt);
			std::copy(opt.begin(), opt.end(), std::back_inserter(m_options2));
			iniFile.close();
		}
		else
			std::cerr << "cannot find sapi ini file: " << ini << std::endl;
	}
}

SAPI51::~SAPI51(void)
{
//	m_pVoice->Release();
    ::CoUninitialize();
	delete m_smilVisitor;
	m_smilVisitor = 0;
}

std::map<std::string, float> SAPI51::speak(const std::string& speech, const std::string& filename)
{
	// convert the string to wstring
	std::wstring speechString;
	std::copy(speech.begin(), speech.end(), std::back_inserter(speechString));

	std::fstream phoFile((filename + ".pho").c_str(), std::ios_base::out);
	HRESULT hr = S_OK;
	CComPtr <ISpVoice>  cpVoice;
	CComPtr <ISpStream>  cpStream; // Pass-through object between the voice and file stream object
	CComPtr <ISpStream> cpFileStream; // Actually generates wave file

	if(SUCCEEDED(hr))
		hr = cpVoice.CoCreateInstance( CLSID_SpVoice );

	if(SUCCEEDED(hr))
		hr = cpVoice->SetNotifyWin32Event(); // State that notifications will be done by calling WaitForNotifyEvent

	if(SUCCEEDED(hr))
		hr = cpVoice->SetInterest(SPFEI(SPEI_VISEME) | SPFEI(SPEI_END_INPUT_STREAM) | SPFEI(SPEI_TTS_BOOKMARK)
		,SPFEI(SPEI_VISEME) | SPFEI(SPEI_END_INPUT_STREAM) | SPFEI(SPEI_TTS_BOOKMARK));

	CSpStreamFormat format(SPSF_44kHz16BitMono, &hr); //set the format for the wav

	// set the voice to a female voice
	CComPtr<ISpObjectToken>             cpVoiceToken;
	CComPtr<IEnumSpObjectTokens>        cpEnum;
	//Enumerate the available voices 
	if(SUCCEEDED(hr))
	{
		if(!m_options1.empty())
		{
			hr = SpEnumTokens(SPCAT_VOICES, m_options1.c_str(), m_options2.c_str(), &cpEnum);
			if(!SUCCEEDED(hr))
				hr = SpEnumTokens(SPCAT_VOICES, L"Language=409;Name=Microsoft Mary;Gender=Female", L"Age=Adult", &cpEnum);
		}
		else
			hr = SpEnumTokens(SPCAT_VOICES, L"Language=409;Name=Microsoft Mary;Gender=Female", L"Age=Adult", &cpEnum);
	}

	if(SUCCEEDED(hr))
		hr = cpEnum->Next( 1, &cpVoiceToken, NULL );
	if(SUCCEEDED(hr))
		hr = cpVoice->SetVoice(cpVoiceToken);

	// Create wav file stream
	if(SUCCEEDED(hr))
		hr = SPBindToFile((filename + ".wav").c_str(),  SPFM_CREATE_ALWAYS,  &cpFileStream
				, &format.FormatId(),format.WaveFormatExPtr() );

	// Create pass-through stream and set its base stream to be the file stream
	if(SUCCEEDED(hr))
		hr = cpStream.CoCreateInstance(CLSID_SpStream);
	if(SUCCEEDED(hr))
		hr = cpStream->SetBaseStream(cpFileStream, format.FormatId(), format.WaveFormatExPtr());

	// Connect the voice to the pass-through stream
	if(SUCCEEDED(hr))
		hr = cpVoice->SetOutput(cpStream, FALSE);
	if(SUCCEEDED(hr))
		hr = cpVoice->Speak(speechString.c_str(), SPF_ASYNC|SPF_IS_XML, NULL); // Speak asynchronously

	CSpEvent        event;  // helper class in sphelper.h for events that releases any 
                            // allocated memory in it's destructor - SAFER than SPEVENT
  	bool fDone = false; // Get set to true when speaking has finished
	m_duration = 0;
	m_marks.clear();
	while (!fDone)
	{
		if(SUCCEEDED(hr))
			hr = cpVoice->WaitForNotifyEvent(INFINITE); // Wait for some events
		while( event.GetFrom(cpVoice) == S_OK )
		{
			switch( event.eEventId )
			{
				case SPEI_END_INPUT_STREAM:
					fDone = true; // Stream ended
					break;
				case SPEI_VISEME:
					m_duration += HIWORD(event.wParam);
					phoFile << event.Viseme() << " " << float(m_duration)/1000.0f << std::endl;
					break;
				case SPEI_TTS_BOOKMARK:
					{
						std::wstring dummy = event.BookmarkName();
						std::string marker;
						std::copy(dummy.begin(), dummy.end(), std::back_inserter(marker));
						m_marks.insert(std::make_pair(marker, (float)m_duration));
					}
				default:
					break;
			}
		}
	}

	cpFileStream->Close();
	cpStream->Close();

	//Release the stream and voice object
	cpFileStream.Release();
	cpStream.Release ();
	cpVoice.Release();
	phoFile.close();

	if(!SUCCEEDED(hr))
		std::cerr << "Some error occured during speech synthesis by SAPI!\n";

	return m_marks;
}


void SAPI51::createSmilVisitor()
{
	m_smilVisitor = new VisitorSAPISpeech;
}