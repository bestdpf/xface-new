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
/*!
	\file wxScriptProcessor.h
	\brief wxWidget implementation for script processors.
*/
#pragma once
#include "IScriptProcessor.h"

#include <string>

/*!
	\brief wxWidget implementation for script processors.
	\author Koray Balci

	Using wxWidgets file access functions, a script processor (one of many such as
	SMIL-Agent or something else) is executed. 
*/
class wxScriptProcessor : public IScriptProcessor
{
public:
	bool process(const std::string& scriptString, const std::string& filename = "TempFile");
	wxScriptProcessor(const std::string& name);
	~wxScriptProcessor(void);
};