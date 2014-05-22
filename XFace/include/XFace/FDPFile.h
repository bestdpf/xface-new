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
 * The Original Code is Xface Core Library; FDP Streaming
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.itc.it) Trento / ITALY.
 * For info, contact: xface-info@itc.it or http://xface.itc.it
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@yahoo.com)
 * ***** END LICENSE BLOCK ***** */

/*! 
	\file	FDPFile.h
	\brief	FDP file streaming
*/

#ifndef FDPFILE_H_
#define FDPFILE_H_

#define TIXML_USE_STL
#include <tinyxml.h>

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include "FDP.h"
#include <XEngine/ModelFileFactory.h>
#include <XEngine/Entity.h>


using XEngine::MeshInfo;
using XEngine::Entity;
using XEngine::Drawable;
using XEngine::ModelFileFactory;

namespace XFace{
/*!
   \brief MPEG-4 FDP loader/writer from/to disk.
   \ingroup XFace
   \author Koray Balci

 Reads/Writes the MPEG-4 Facial Definition Parameters stored in XML format. 
 
*/
class FDPFile
{
//	std::ofstream debug;
	boost::shared_ptr<FDP> m_pFDP;
    bool load(TiXmlDocument& doc, boost::shared_ptr<FDP> pFDP);
	bool loadHeader(const TiXmlNode* pNode);
	void loadFDPItem(const TiXmlNode* pNode);
	bool loadSource(const TiXmlNode* pNode);
	void saveSource(TiXmlElement& x_xfdp);
	void saveEntity(TiXmlElement &parent, const Entity& ent, const std::string& alias, bool bindings = false);
	std::list<MeshInfo> m_faceEntityMeshInfo;
	std::map<std::string, std::string>  m_bindings;
	std::map<std::string, Entity> m_morphTargets;
	std::map<std::string, std::list<MeshInfo> > m_morphTargetsMeshInfos;
	Entity m_faceEntity;
public:
	std::string getVersion() const {return "0.2";};	
	std::list<MeshInfo> getFaceEntityMeshList() const {return m_faceEntityMeshInfo;}	// yes make a full copy in return
	const std::map<std::string, std::list<MeshInfo> >& getMorphTargetsMeshInfos(){return m_morphTargetsMeshInfos;}
	void setBindings(const std::map<std::string, std::string>& bindings ) {m_bindings = bindings;}
	void setFaceEntity(const Entity& face){m_faceEntity = face;}
	std::map<std::string, std::string> getBindings() const {return m_bindings;}

	void setMorphTargets(const std::map<std::string, Entity>& targets) {m_morphTargets = targets;}
	std::map<std::string, Entity> getMorphTargets() const {return m_morphTargets;}


	bool load(const std::string& filename, boost::shared_ptr<FDP> pFDP);
	bool load(std::istream& input, boost::shared_ptr<FDP> pFDP);
	bool save(const std::string& filename, boost::shared_ptr<FDP> pFDP);
	
	// class constructor
	FDPFile();
};
} // namespace XFace
#endif // FDPFILE_H_

