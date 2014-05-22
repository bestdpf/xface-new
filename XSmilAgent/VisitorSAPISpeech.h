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
	\file VisitorSAPISpeech.h
	\brief visitor for MS SAPI 5.1
*/
#pragma once
#include "VisitorSpeech.h"

/*!
	\brief visitor for MS SAPI 5.1
	\author Koray Balci
	\date   Feb 2006

	This class is a visitor class for the node tree parsed from the script.

	It traverses and visits the all the SMILSpeech nodes in the tree and 
	processes them to be used as input to TTS engine. 
*/
class VisitorSAPISpeech :
	public VisitorSpeech
{
public:
	void operator()(const SMILBlock& node) const;
	void operator()(const boost::shared_ptr<SGGroup>& node) const;
	void operator()(const SMILSpeech& obj) const;
};
