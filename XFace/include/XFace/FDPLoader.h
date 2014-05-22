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
	\file FDPLoader.h
	\brief FDP Loader.
*/
#ifndef FDPLOADER_H_
#define FDPLOADER_H_
#pragma once
/*
#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif
*/
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XercesDefs.hpp>

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>

#include "FDP.h"

#include <XEngine/ModelFileFactory.h>
#include <XEngine/Entity.h>
#include <map>
#include <list>


namespace XFace{

	using XEngine::MeshInfo;
	using XEngine::Entity;
/*!
	\brief FDP Loader
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   Feb 2005
*/
class FDPLoader
{
//	std::ofstream debug;
	boost::shared_ptr<FDP> m_pFDP;
	std::string m_version;

	std::list<MeshInfo> m_faceEntityMeshInfo;
	std::map<std::string, std::string>  m_bindings;
	std::map<std::string, Entity> m_morphTargets;
	std::map<std::string, std::list<MeshInfo> > m_morphTargetsMeshInfos;
	Entity m_faceEntity;
	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *m_pDoc;
	XERCES_CPP_NAMESPACE::XercesDOMParser *m_pParser;

	static float toFloat(const XMLCh* val);
	void processIndices(const std::string& indices, FDPItem* pItem);
	bool parse();
	void parseSource(XERCES_CPP_NAMESPACE::DOMNode* pSource);
	bool parseHeader(XERCES_CPP_NAMESPACE::DOMNode* pHeader);
	void loadEntity(XERCES_CPP_NAMESPACE::DOMNode* pSource);
	void loadFDPItem(XERCES_CPP_NAMESPACE::DOMNode* pFDPItem);
	bool loadInput(const XERCES_CPP_NAMESPACE::InputSource& input);
public:
	FDPLoader(void);
	~FDPLoader(void);
	bool load(const std::string& filename, boost::shared_ptr<FDP> pFDP);
	bool load(std::istream& input, boost::shared_ptr<FDP> pFDP);

	std::map<std::string, std::string> getBindings() const {return m_bindings;}
	std::list<MeshInfo> getFaceEntityMeshList() const {return m_faceEntityMeshInfo;}	// yes make a full copy in return
	const std::map<std::string, std::list<MeshInfo> >& getMorphTargetsMeshInfos(){return m_morphTargetsMeshInfos;}
};

} // namespace XFace
#endif