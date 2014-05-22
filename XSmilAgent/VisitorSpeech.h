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
 * The Original Code is XSmilAgent
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */
/*!
	\file VisitorSpeech.h
	\brief visitor for speech tags in SMIL-Agent scripts
*/
#pragma once
#include "SGNode.h"
#include <map>

/*!
	\brief visitor for speech tags in SMIL-Agent scripts
	\author Koray Balci
	\version 1.0
	\date   Feb 2006

	This visitor (see Visitor design pattern for mechanics), traverses the script and 
	process speech tags (SMILSpeech instances). It acts as a base class for different 
	TTS engines, this basic implementation simply concatenates all the speech tags' 
	contents into one string. See derived classes for further info, TTS specific 
	implementations.
	
	\sa VisitorSAPISpeech
*/
class VisitorSpeech
	: public boost::static_visitor<>
{
protected:
	static std::string m_script;
	static std::string m_outFilename;
	static float m_time;
	static std::string getExtlessFilename(const std::string & filename);
public:
	static std::string getOutputFilename() {return m_outFilename;}
	//! returns the script, call this after you do the traversing/visiting the tree.
	static std::string getScript() {return m_script;}
	static float getTime() {return m_time;}
	static void reset();
	virtual void operator()(const SMILSpeech& node) const;
	//! If the node is not a group node or a speech node, we do nothing!
	virtual void operator()(const SMILBlock& node) const {};
	virtual void operator()(const boost::shared_ptr<SGGroup>& node) const;
	VisitorSpeech(){reset();}
};