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
 * Portions created by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

/*! 
	\file FDPWriter.h
	\brief FDP Writer.
*/
#ifndef FDPWRITER_H_
#define FDPWRITER_H_
#pragma once

#include <string>
#include <boost/shared_ptr.hpp>
#include <xercesc/dom/DOM.hpp>

#include <XFace/FDP.h>
#include <XEngine/Entity.h>

using XEngine::Entity;
namespace XFace{
class FDPWriter
{
public:
	typedef std::map<std::string, std::map<std::string, Entity> > MorphDictionary;
private:
	boost::shared_ptr<FDP> m_pFDP;
	void writeToFile(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *pDoc, const std::string& strPath);
	XERCES_CPP_NAMESPACE::DOMNode* saveHead(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc);
	XERCES_CPP_NAMESPACE::DOMNode* saveSource(XERCES_CPP_NAMESPACE::DOMDocument *doc);
	XERCES_CPP_NAMESPACE::DOMNode* saveEntity(XERCES_CPP_NAMESPACE::DOMDocument *doc, const Entity& ent, const std::string& alias, const std::string& category, bool bindings);
	XERCES_CPP_NAMESPACE::DOMNode* saveFDPItem(XERCES_CPP_NAMESPACE::DOMDocument* doc, const FDPItem* item);
	Entity m_faceEntity;
	std::map<std::string, std::string>  m_bindings;
	MorphDictionary m_morphTargets;
	
public:
	void setBindings(const std::map<std::string, std::string>& bindings ) {m_bindings = bindings;}
	void setFaceEntity(const Entity& face){m_faceEntity = face;}
	void setMorphTargets(const MorphDictionary& targets) {m_morphTargets = targets;}
	
	bool save(const std::string& filename, boost::shared_ptr<FDP> pFDP);
	FDPWriter(void);
	~FDPWriter(void);
};
} // namespace XFace

#endif