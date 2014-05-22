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
	\file		SGNode.h
	\brief		Base class for all the node types in SMIL scripts.
*/

#pragma once
#include <deque>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include "SMILObjects.h"

/*! 
	We define this boost::variant type for using in our visitor pattern
	easily. An SGNode can be one of these types.
*/
typedef boost::variant<
	class SMILSpeech, 
	class SMILAction,
	class SMILSpeechAnimation,
	boost::recursive_wrapper< boost::shared_ptr<class SGGroup> > 
> SGNode;

/*! 
	\brief		Grouping node for SMIL-Agent tree.
	\author		Koray Balci

	An SGGroup object is itself a SMILBlock but its sole purpose is to act as
	a scene graph grouping node, i.e. it stores only other nodes, and it's 
	never a leaf node. Nothing confusing, it only has a deque of SGNode's 
	inside. 
*/
class SGGroup : public SMILBlock
{
public:
	typedef std::deque<SGNode> CHILDREN;
	
	CHILDREN m_children;
	void addChild(const SGNode& node) {m_children.push_back(node);}
};

