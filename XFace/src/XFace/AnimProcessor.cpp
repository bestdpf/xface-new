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
 * The Original Code is Xface Core
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
#include <XFace/AnimProcessor.h>
#include <XFace/PHOLoader.h>
#include <XFace/EyeChannel.h>
#include <XFace/ExpressionChannel.h>
#include <iostream>
#include <strstream>


namespace XFace{

using namespace XEngine;
	
AnimProcessor::AnimProcessor() : m_speechDuration(0)
{
	m_headChannel.reset(new HeadChannel("Head"));
}

int AnimProcessor::processAnim(std::istream& input)
{
	MorphController* controller = MorphController::getInstance();
	controller->reset();
	std::string channel;
	int duration = 0;
	unsigned int blinkEyes = 0;
	m_speechDuration = 0;

	while (!input.eof())
	{
		input >> channel;
		if(channel == "Expression")
		{
			duration = std::max(processFaceChannel(input), duration);
		}
		else if(channel == "Eyes")
		{
			duration = std::max(processEyeChannel(input), duration);
		}
		else if (channel == "Viseme")
		{
			controller->addChannel("Viseme");
			std::string filename, lang;
			int end = 0;
			input >> filename >> end >> lang;
			std::ifstream inputPho(filename.c_str());
			processPhonemes(inputPho, lang);
			inputPho.close();
			duration = std::max(duration, end);
		}
		else if (channel == "Head")
		{
			duration = std::max(processHeadChannel(input), duration);
		}
		else if (channel == "BlinkEyes")
		{
			input >> blinkEyes;
		}
		
		channel.clear();
	}
	
	if(blinkEyes)
		prepareEyeBlink(blinkEyes, duration);
	
	// idle and controlled head movements go here..
	controller->addChannel(m_headChannel);
	controller->rewind();

	return duration;
}

int AnimProcessor::processEyeChannel(std::istream& input) const
{
	MorphController* controller = MorphController::getInstance();
	int begin = 0, end = 0;
	float intensity = 1.0f;
	std::string action, exp;
	input >> action >> begin >> end >> intensity >> exp;
	if(intensity > 1.0f)
	{
		std::cerr << "Warning: Intensity for expression " << exp << " is out of acceptable range, clamped." << std::endl;
		intensity = 1.0f;
	}
	Entity toInsert;
	toInsert.copyFrom(controller->getDictionaryItem(exp), true);
	Entity restTest;
	restTest.copyFrom(controller->getDictionaryItem("Rest"), true);
	if(toInsert.getDrawableCount())
	{
		std::list<MorphTarget*> eyes;
		unsigned int delta = std::min((end - begin) / 4 + 1, 400);
		
		eyes.push_back(new MorphTarget(restTest, "Rest", begin, begin + delta / 2, 1));
		eyes.push_back(new MorphTarget(toInsert, exp, begin + delta / 2, begin + delta, intensity));
		eyes.push_back(new MorphTarget(toInsert, exp, begin + delta, end - delta, intensity));
		eyes.push_back(new MorphTarget(toInsert, exp, end - delta, end - delta / 2, intensity));
		eyes.push_back(new MorphTarget(restTest, "Rest", end - delta / 2, end, 1));

		controller->addChannel(boost::shared_ptr<MorphChannel>(new EyeChannel("Eyes")));
		controller->pushTargets(eyes, "Eyes");
	}
	else
		std::cerr << "Cannot find action keyframe " << exp << " for eye channel for action: " << action << std::endl;

	return end;
}

int AnimProcessor::processFaceChannel(std::istream& input) const
{
	MorphController* controller = MorphController::getInstance();
	int begin = 0, end = 0;
	std::string exp;
	float intensity = 1.0f;
	input >> exp >> begin >> end >> intensity;
	if(intensity > 1.0f || intensity < 0)
	{
		std::cerr << "Warning: Intensity for expression " << exp << " is out of acceptable range, clamped." << std::endl;
		intensity = 1.0f;
	}
	std::list<MorphTarget*> expressions;
	Entity toInsert;
	toInsert.copyFrom(controller->getDictionaryItem(exp), true);
	if(toInsert.getDrawableCount())
	{
		unsigned int delta = std::min((end - begin) / 4 + 1, 400);
		expressions.push_back(new MorphTarget(toInsert, exp, begin, begin + delta, intensity));
		expressions.push_back(new MorphTarget(toInsert, exp, begin + delta, end - delta, intensity));
		expressions.push_back(new MorphTarget(toInsert, exp, end - delta, end - delta / 2, intensity));
		Entity restTest;
		restTest.copyFrom(controller->getDictionaryItem("Rest"), true);
		expressions.push_back(new MorphTarget(restTest, "Rest", end - delta / 2, end, 1));

		controller->addChannel(boost::shared_ptr<MorphChannel>(new ExpressionChannel("Expression")));
		controller->pushTargets(expressions, "Expression");
	}
	else
		std::cerr << "Cannot find expression " << exp << " in dictionary! Ignoring..." << std::endl;
	
	return end;
}

int AnimProcessor::processPhonemes(std::istream& input, const std::string& lang)
{
	MorphController* cont = MorphController::getInstance();
	// cleanup
	m_speechDuration = 0;
	boost::shared_ptr<MorphChannel> ch = cont->findChannel("Viseme");
	if(!ch)
		cont->addChannel("Viseme");
	cont->clearChannel("Viseme");
	
	if(input.fail())
		return 0;
	
	// Do the processing of the content
	float smoothFactor = 0.6f;
	
	PHOLoader phonemeLoader;
	std::list<std::pair<std::string, unsigned int> > phoSeq = phonemeLoader.load(input);
	std::list<std::pair<std::string, unsigned int> >::const_iterator it = phoSeq.begin();
	m_speechDuration = phonemeLoader.getTotalDuration();
	unsigned int currentDuration = 0;
	const std::map<std::string, std::string>& phonDict = getPhonDictionary(lang);
	while (it != phoSeq.end())
	{
		// if there is silence for a long period of time, we should prevent perlin head movement.
		// injecting kind of a hack here
		std::map<std::string, std::string>::const_iterator it_phon = phonDict.find(it->first);
		// if it is a silence and lasts longer than 200 ms
		if(it_phon == phonDict.end())
		{
			std::cerr << "Could not find alias for phoneme: " << it->first << std::endl;
			pushPhoneme("Rest", currentDuration, currentDuration + it->second, smoothFactor); 
		}
		else if(it_phon->second == "Rest" && it->second > 200) 
		{
			// split the rest frame into two so that interpolation is correctly done, introduce a 50 ms rest phoneme
			pushPhoneme(it_phon->second, currentDuration, currentDuration + it->second - 50, smoothFactor); 
			pushPhoneme(it_phon->second, currentDuration + it->second - 50, currentDuration + it->second, smoothFactor); 
			m_headChannel->addStayStill(currentDuration, currentDuration + it->second);
		}
		else
			pushPhoneme(it_phon->second, currentDuration, currentDuration + it->second, smoothFactor); 
		
		currentDuration += it->second;

		++it;
	}
	
	return m_speechDuration;
}

void AnimProcessor::pushPhoneme(const std::string& alias, unsigned int start, unsigned int end, float weight) const
{
	if(alias.empty() || end - start <= 0)
		return;
	MorphController* cont = MorphController::getInstance();
	Entity toInsert = cont->getDictionaryItem(alias);
	if(!toInsert.getDrawableCount())
	{
		std::cerr << "cannot find viseme: " << alias << std::endl;
		return;
	}

	MorphTarget* pTrg = new MorphTarget(toInsert, alias, start, end, weight);
	cont->pushTarget(pTrg, "Viseme");
}

void AnimProcessor::prepareEyeBlink(unsigned int period, unsigned int duration)
{
	MorphController* cont = MorphController::getInstance();
	std::list<MorphTarget*> eyes;
	
	Entity toInsert, rest;
	toInsert.copyFrom(cont->getDictionaryItem("BlinkEyes"), true);
	rest.copyFrom(cont->getDictionaryItem("Rest"), true);
	unsigned int counter = 1500;
	if(toInsert.getDrawableCount())
	{
		while (duration > counter)
		{
			eyes.push_back(new MorphTarget(rest, "Rest", counter, counter + 50, 1.0f));
			counter += 50;
			eyes.push_back(new MorphTarget(toInsert, "BlinkEyes", counter, counter + 150, 1.0f));
			counter += 150;
			eyes.push_back(new MorphTarget(rest, "Rest", counter, counter + 50, 1.0f));
			counter += 50;
			counter += period;
		}
		cont->addChannel(boost::shared_ptr<MorphChannel>(new EyeChannel("BlinkEyes")));
		cont->pushTargets(eyes, "BlinkEyes");
	}
}

int AnimProcessor::processHeadChannel(std::istream& input) const
{
	std::string type;
	float x, y, z, w;
	int begin, end;
	input >> type >> begin >> end >> w >> x >> y >> z;
	if(type == "pointing")
	{
		m_headChannel->addPointing(begin, end, x, y, z, w);
	}

	return end;
}

} // namespace XFace