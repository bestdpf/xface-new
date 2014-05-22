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
 * The Original Code is Xface Core Library.
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.fbk.eu) Trento / ITALY.
 * For info, contact: xface-info@fbk.eu or http://xface.fbk.eu
 * Portions creat
 ed by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

/*! 
	\file	ITimedController.h
	\brief	ABS for timed controllers.
*/

#pragma once
#ifndef ITIMEDCONTROLLER_H_
#define ITIMEDCONTROLLER_H_


namespace XEngine{
/*!
	\brief ABS for timed controllers
*/
class ITimedController
{
	bool m_enabled;
public:
	void enable(bool flag) {m_enabled = flag;}
	bool isEnabled() const {return m_enabled;}
	virtual void update(unsigned int elapsed_time) = 0;
	virtual ~ITimedController(void){}
	ITimedController(void) : m_enabled(true){}
};

} // namespace XEngine	

#endif // ITIMEDCONTROLLER_H_