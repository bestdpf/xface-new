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
 * The Original Code is XSmilAgent.
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
/*! 
	\file SMILObjects.h
	\brief	Classes used in SMIL-Agent script processing (for the parse tree).
*/
#pragma once
#ifndef SMILOBJECTS_H_
#define SMILOBJECTS_H_

#include <string>
#include <vector>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XercesDefs.hpp>

using namespace XERCES_CPP_NAMESPACE;

/*! 
	\brief	Synchronization attributes of a node.
*/
class SMILSynchronizationAttributes{
public:
	//! real (floating point) begin and end values of a node's timing, calculated during processing.
	float beginVal, endVal;
	//! string values for begin, end and duration as they are read from the script.
	std::string begin, end, dur;
	//! Should the node be repeated as soon as it is ended? (currently have no effect in result)
	unsigned int repeat;
	SMILSynchronizationAttributes() : repeat(1), beginVal(0), endVal(0){};
	//! Prints the values stored.
	void print() const;
	//! Reads the values from the XML tree (using xerces)
	void read(DOMNode* pNode);
};

/*! 
	\brief	System related attributes of a node.

	\note Not used in the current version, only parsed.
*/
class SMILSystemAttributes{
public:
	std::string language, required, screen_size, screen_depth;
	//! Prints the values stored.
	void print() const;
	//! Reads the values from the XML tree (using xerces)
	void read(DOMNode* pNode);
};

/*! 
	\brief	Test attributes of a node.
*/
class SMILTestAttributes{
public:
	SMILSystemAttributes system;
	//! Prints the values stored.
	void print() const;
	//! Reads the values from the XML tree (using xerces)
	void read(DOMNode* pNode);
};
	
/*! 
	\brief	Performance attributes of a node.
*/
class SMILPerformanceAttributes{
public:
	//! Has a test attributes instance.
	SMILTestAttributes tests;
	//! channel name, which affect is used, its id and its title.
	std::string channel, affect, id, title;
	//! Prints the values stored.
	void print() const;
	//! Reads the values from the XML tree (using xerces)
	void read(DOMNode* pNode);
	SMILPerformanceAttributes() : affect("Rest"){}
};

/*! 
	\brief	Base class for important (has valuable content) script nodes.

	Speech text, action, speech animation are the real content that we want to
	process in a SMIL-Agent script. This class acts as a base for all those classes
	as a building block.
	
	\sa SMILSpeech SMILReferencedBlock SMILSpeechAnimation, SMILAction, SGGroup
*/
class SMILBlock{
public:
	enum BLOCKTYPE{
		kNone = 0,
		kPar,
		kSeq
	};
	BLOCKTYPE type;
	SMILBlock() : type(kNone){}
	//! Has synchronization attributes
	SMILSynchronizationAttributes synch;
	//! Has performance attributes
	SMILPerformanceAttributes performance;
	//! Prints the values stored.
	virtual void print() const;
	//! Reads the values from the XML tree (using xerces)
	void read(DOMNode* pBlock);
};

/*! 
	\brief	Content of a speech tag is parsed and stored in here.
*/
class SMILSpeechContent{
public:
	/*!
		A speech content can have two things inside, actual text to be synthesized,
		and marks (as xml tags) to specify the change of emotion, expression for other
		channels at that time instance. With a mark, we tag that very time instance (in
		between the synthesized text) and this tag is later used in different channels 
		as start or end of an action, expression, emotion or gesture. 

		A SMILSpeechContent instance can either be a marker or text.
	*/
	enum SPEECH_CONTENT
	{
		kMark,
		kText
	};

	//! timing info
	float beginVal, endVal;
	SMILSpeechContent() : type(kText), beginVal(0), endVal(0){};
	//! Actual content
	std::string value;
	//! Instance type
	SPEECH_CONTENT type;
};

/*! 
	\brief	speech tag in SMIL-Agent script is parsed and stored.

	Speech content is parsed and stored here. One of the mostly used classes 
	in script processing process. 

	See SMIL-Agent specification for more info on this tag.
*/
class SMILSpeech : public SMILBlock
{
public:
	//! Has a vector of speech contents (text - marker - text - marker ...)
	std::vector<SMILSpeechContent> content;
	std::string filename;
	
	//! Prints the values stored.
	void print() const;
	//! Reads the values from the XML tree (using xerces)
	void read(DOMNode* pSpeech);
};

/*! 
	\brief	Base class for referenced blocks.

	action and speech-animation (and any other similar ones in the future) tags 
	in the script are derived from this class. They are normally referred, as 
	parallel to a speech tag, i.e. the animation is driven by timing from the 
	speech synthesis, and referenced classes obey that timing.
*/
class SMILReferencedBlock: public SMILBlock
{
public:
	//! Prints the values stored.
	void print() const;
	std::string content_id, content_begin, content_end;
};

/*! 
	\brief	speech-animation tag in SMIL-Agent script is parsed and stored.

	See SMIL-Agent specification for more info on this tag.
*/
class SMILSpeechAnimation : public SMILReferencedBlock
{
public:
	std::string content, fill;
	float intensity;

	//! Prints the values stored.
	void print() const;
	SMILSpeechAnimation() : intensity(1.0f), fill("default"){};
	//! Reads the values from the XML tree (using xerces)
	void read(DOMNode* pAnim);
};

/*! 
	\brief	action tag in SMIL-Agent script is parsed and stored.

	See SMIL-Agent specification for more info on this tag.
*/
class SMILAction : public SMILReferencedBlock
{
public:
	//! An action can have a number of parameters inside.
	std::vector<std::string> params;
	std::string action_type, fill;
	float intensity;
	//! Prints the values stored.
	void print() const;
	//! Reads the values from the XML tree (using xerces)
	void read(DOMNode* pAction);
	SMILAction() : intensity(1.0f), fill("default"){};
};

#endif