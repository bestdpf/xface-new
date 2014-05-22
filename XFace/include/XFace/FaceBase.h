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
	\file	FaceBase.h
	\brief	Main face object.
*/

#ifndef FACEBASE_H_
#define FACEBASE_H_

#include <string>
#include <map>
#include <vector>
#include <list>
#include <sstream>

#include <boost/shared_ptr.hpp>

#include <XEngine/Entity.h>
#include <XEngine/MorphTarget.h>
#include <XFace/FaceEntity.h>
#include <XFace/IFapStream.h>
#include <XFace/FDP.h>
#include <XEngine/ModelFileFactory.h>
#include <XEngine/MeshManager.h>

#include <XEngine/MorphController.h>
#include <XFace/AnimProcessor.h>

//using namespace XEngine;

namespace XFace{

/*!
	\brief Main XFace class.
	\ingroup XFace
	\author Koray Balci
	\version 1.0
	\date   June 2003

	Main entry point for XFace library related operations. Only exposing this class
	to users should be sufficient. Pass the FDP configuration file for initialization, 
	then call update and render when needed. 
	\code
	    FaceBase myFace;
	    myFace.init(FDPFilename, FDPPath); // load an FDP file
	    
	    // in a loop of all FAP data
	    myFace.update(FAPData);
	\endcode
	
	\note Alternatively, you can use XFaceApp module and use XML tasks as messages 
	(see related documentation) and forget about this class and all other XFace classes.
*/
class FaceBase
{
protected:
	AnimProcessor m_animProcessor;
	typedef std::map<std::string, std::pair<std::string, Entity> > MorphTargetMap;
	typedef std::list<XEngine::MorphTarget*> MorphTargetList;
	boost::shared_ptr<FDP> m_pFDP;
	std::stringstream m_errorString;
	FaceEntity m_face;
	std::list<std::string> m_filenames;
	bool initMorphTargets(const std::map<std::string, std::list<XEngine::MeshInfo> >& targets, const std::string& path );
public:
	void resetDeformedVertices() {m_face.resetDeformedVertices();}
	void addPhonemeDictionary(const std::string& dic) {m_animProcessor.addPhonemeDictionary(dic);}
	void reset();
	unsigned int getSpeechDuration() const {return m_animProcessor.getSpeechDuration();}
	int processAnims(std::istream& input);
	int processPhonemes(std::istream& input, const std::string& lang);
	void rewindKeyframeAnimation();
	void setTransform(const XEngine::Transform& tr) 
	{
		m_face.setTransform(tr);
		m_pFDP->setGlobalTranslation(tr.getTranslation());
		m_pFDP->setGlobalRotation(tr.getRotation());
	}
	const XEngine::Transform& getTransform() const {return m_face.getTransform();}
	const FaceEntity& getRestFrame() const {return m_face;}
	const std::list<boost::shared_ptr<XEngine::Drawable> >& getDrawables() const {return m_face.getDrawables();}
	const FAPU& getFAPU() const {return m_pFDP->getFAPU();};
	boost::shared_ptr<FDP> getFDP() const {return m_pFDP;}
	void setFDP(boost::shared_ptr<FDP> pFDP) {m_pFDP = pFDP;}

	std::string getErrorString(bool clear = true);
	void initEyePivots(const Vector3& leftEye, const Vector3& rightEye);
	virtual void initInfluenceCalculators();
	virtual bool init(const std::string& filename, const std::string& path = "./");
	bool initMeshes(const std::list<XEngine::MeshInfo>& filenames, const std::string& path);
	void releaseMeshes();
	void update(const boost::shared_ptr<XFace::IFapStream>& faps);
	const Entity& update(unsigned int elapsed_time);
	virtual ~FaceBase();
	FaceBase();
};
}
#endif // FACEBASE_H_
