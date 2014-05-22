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
 * The Original Code is Xface Core Library; FaceBase imp.
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


#include <XFace/FaceBase.h>
#include <XFace/FDPLoader.h>
#include <XFace/PHOLoader.h>

#include <iostream>
#include <XEngine/TextureManager.h>
#include <XEngine/MorphController.h>
#include <XEngine/RendererGL.h>
#include <XEngine/Transform.h>
#include <XFace/RaisedCosInfluence.h>

namespace XFace{

using namespace XEngine;


FaceBase::FaceBase()
{
}

FaceBase::~FaceBase()
{
	reset();
	TextureManager::releaseInstance();
	MeshManager::releaseInstance();
	MorphController::releaseInstance();
}

bool FaceBase::init(const std::string& filename, const std::string& path)
{
    printf("fb init\n");
	// load configuration and fdp
	// FDP items load
	// if we already have an object and re-initing
	reset();
    printf("afater reset\n");
	FDPLoader fdp_file;
	if(fdp_file.load(path + filename, m_pFDP))
	{   
        printf("0.5\n");
		std::string datname = filename;
		datname.replace(datname.end() - 3, datname.end(), "dat");
        printf("0.6\n");
		ModelFileFactory::unloadAllFiles();
		printf("0.7\n");
        fflush(stdout);
        ModelFileFactory::initBinaryLoader(datname, path);
        printf("0.8\n");
        fflush(stdout);
		MorphController::getInstance()->clearDictionary();
	    printf("0.9\n");	
		// initialize the textures and meshes
		if( !initMeshes(fdp_file.getFaceEntityMeshList(), path) ){
            printf("fb init text and mesh fail\n");
			return false;
        }

		if(initMorphTargets(fdp_file.getMorphTargetsMeshInfos(), path))
		{
            printf("reset controller\n");
			// also resets the controller
			MorphController::getInstance()->setFDP(m_pFDP);
		}
        printf("fb 1\n");	
		m_face.initInfluenceCalculators(m_pFDP->getItems());
        printf("fb 2\n");
		m_face.initBindings(fdp_file.getBindings());
        printf("fb 3\n");
		m_face.initEyePivots(m_pFDP->getLeftEyePivot(), m_pFDP->getRightEyePivot());
        printf("fb 4\n");
		ModelFileFactory::releaseBinaryLoader();
        printf("fb 5\n");
	}
	else
	{
        printf("fb init false\n");
		m_errorString << "Unable to load FDP file: " << filename << " at " << path << std::endl;
		return false;
	}
    printf("finsh fb load fdp\n");
    fflush(stdout);
	// initialized without loading the FDP file (first time creation)
	return true;	
}

void FaceBase::releaseMeshes()
{
	std::list<std::string>::iterator it = m_filenames.begin();
	while (it != m_filenames.end())
	{
		ModelFileFactory::unloadModelFile(*it);
		++it; 
	}
	m_filenames.clear();
}

void FaceBase::reset()
{
	releaseMeshes();
	TextureManager::getInstance()->destroyAll(); // clear the textures
	m_face.release(true);
	m_pFDP.reset(new FDP);
	m_errorString.clear();
}

bool FaceBase::initMeshes(const std::list<MeshInfo>& filenames, const std::string& path)
{
	// load new ones
	std::list<MeshInfo>::const_iterator it = filenames.begin();
	while (it != filenames.end())
	{
		std::string total_path = path + it->path;
		// we do not use the format, we resolve it inside the loader
		std::list<boost::shared_ptr<Drawable> > drawables = ModelFileFactory::loadModelFile(it->file, total_path);
		if(!drawables.empty())
		{
			m_face.addDrawables(drawables); 
			m_filenames.push_back(it->file);
		}
		
		++it;
	}

	if(m_face.getDrawableCount() == 0)
		return false;

	return true;
}

bool FaceBase::initMorphTargets(const std::map<std::string, std::list<MeshInfo> >& targets, const std::string& path )
{
	if(targets.empty())
		return false;
	
	std::map<std::string, std::list<MeshInfo> >::const_iterator it = targets.begin();
	std::string category;
	while ( it != targets.end() )
	{
		std::list<MeshInfo>::const_iterator it_meshInfos = it->second.begin();
		Entity *pEnt = new Entity;
		while (it_meshInfos != it->second.end())
		{
			std::string total_path = path + it_meshInfos->path;
			// we do not use the format, we resolve it inside the loader
			std::list<boost::shared_ptr<Drawable> > drawables = 
				ModelFileFactory::loadModelFile(it_meshInfos->file, total_path);
			if(drawables.size() == 0)
				m_errorString << "Could not load mesh: " << it_meshInfos->file << std::endl;
			else			
				pEnt->addDrawables(drawables);
			category = it_meshInfos->keyframe_category;
			
			++it_meshInfos;
		}
		if(pEnt->getDrawableCount())
			MorphController::getInstance()->addDictionaryTarget(it->first, category, *pEnt);
		//else
			delete pEnt;
		++it;
	}
	
	return true;
}

void FaceBase::initEyePivots(const Vector3& leftEye, const Vector3& rightEye)
{
	m_face.initEyePivots(leftEye, rightEye);
}
void FaceBase::initInfluenceCalculators()
{
	if(m_pFDP)
		m_face.initInfluenceCalculators(m_pFDP->getItems());
}

void FaceBase::update(const boost::shared_ptr<XFace::IFapStream>& faps)
{
	m_face.update(faps);
}

const Entity& FaceBase::update(unsigned int elapsed_time)
{
	MorphController::getInstance()->update(elapsed_time);
	setTransform(MorphController::getInstance()->getTransform());
	return MorphController::getInstance()->getResult();
}

void FaceBase::rewindKeyframeAnimation() 
{
	MorphController::getInstance()->rewind();
}

int FaceBase::processAnims(std::istream& input)
{
	return m_animProcessor.processAnim(input);
}

int FaceBase::processPhonemes(std::istream& input, const std::string& lang)
{
	return m_animProcessor.processPhonemes(input, lang);
}

std::string FaceBase::getErrorString(bool clear) 
{
	std::string ret = m_errorString.str();
	if(clear)
		m_errorString.str("");
	return ret;
}
} // namespace XFace
