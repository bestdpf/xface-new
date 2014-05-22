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
	\file		VisitorAnimCreator.h
	\brief		visitor for animation creation.
*/

#pragma once
#include "SGNode.h"
#include <fstream>

/*!
	\brief visitor for animation creation.
	\author Koray Balci

	This class is a visitor class for the node tree parsed from the script.

	It traverses and visits the relevant nodes in the tree and outputs the
	animation timing, and other properties in a file (with .anim extension). 
	This output file is a temporary file and used during animation realization.
*/
class VisitorAnimCreator : public boost::static_visitor<>
{
	static std::fstream m_animFile;

	std::string getChannelClass(const std::string& name) const;
public:
	static void finalize();
	static void reset(const std::string& filename);

	//! Add a speech to the anþmation.
	static void addSpeech(const std::string& speechFile, int duration, const std::string& lang);
	//! Set the blinking period (default is 5 seconds)
	static void blinkEyes(unsigned int period);
	VisitorAnimCreator(const std::string& filename);
	~VisitorAnimCreator();
	void operator()(const SMILBlock& node) const;
	void operator()(const SMILSpeechAnimation& node) const;
	void operator()(const SMILAction& node) const;
	void operator()(const SMILReferencedBlock& node) const;
    void operator()(const boost::shared_ptr<SGGroup>& node) const;
};
