#include "VisitorSAPISpeech.h"
#include <iostream>
#include <strstream>

/*!
	For SAPI 5.1, we can process the marks and convert them to SAPI XML tag 
	"bookmark" before sending to the TTS engine. Also, if there is a delay
	to be applied, it is correctly converted to "silence" tag of SAPI XML. 
	Note that, here, we only create the SAPI script to be synthesized,
	actual synthesis is done elsewhere (in SAPI51 class).
*/
void VisitorSAPISpeech::operator()(const SMILSpeech& obj) const
{
	// if we have a filename reference and not a text to synthesize
	if(!obj.filename.empty())
	{
		m_time += obj.synch.endVal - obj.synch.beginVal;
		m_outFilename = getExtlessFilename(obj.filename);
	}
	else
		m_outFilename = obj.performance.id;
	
	std::vector<SMILSpeechContent>::const_iterator it = obj.content.begin();
	while (it != obj.content.end())
	{
		if (it->type == SMILSpeechContent::kText)
		{
			// if there is a delay
			if(it->beginVal > m_time)
			{
				m_script.append(" <silence msec=\"");
				std::strstream t;
				t << (int)(it->beginVal - m_time) << '\0';
				std::string tVal = t.str();
				std::copy(tVal.begin(), tVal.end(), std::back_inserter(m_script));
				m_script.append("\"/> ");
			}
			m_time = it->endVal;

			std::copy(it->value.begin(), it->value.end(), std::back_inserter(m_script));
		}
		else if (it->type == SMILSpeechContent::kMark)
		{
			std::string mark = obj.performance.id + it->value; 
			m_script.append(" <BOOKMARK MARK=\"");
			std::copy(mark.begin(), mark.end(), std::back_inserter(m_script));
			m_script.append("\"/> ");
		}
		++it;
	}
	if(!m_script.empty())	// perhaps we have no script yet, audio is provided in wav file..
		m_script.append(" ");
}

void VisitorSAPISpeech::operator()(const boost::shared_ptr<SGGroup>& node) const
{
	VisitorSpeech::operator ()(node);
}

void VisitorSAPISpeech::operator()(const SMILBlock& node) const
{
	VisitorSpeech::operator ()(node);
}