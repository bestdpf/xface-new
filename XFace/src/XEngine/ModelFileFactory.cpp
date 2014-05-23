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
 * The Original Code is Xface Core Library; 3D Model File Loader.
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

#include <XEngine/ModelFileFactory.h>

#include <XEngine/OBJLoader.h>
#include <XEngine/VRML1Loader.h>
#include <XEngine/VRML97Loader.h>
#include <XEngine/MeshManager.h>
#include <fstream>
#include <iostream>

#include <algorithm>
#define size_t uint
namespace XEngine{

//! List of files in memory are stored here as a static var
	BinaryModelBatchLoader*	ModelFileFactory::s_pBinaryBatchLoader(0);
	std::list<MeshInfo> ModelFileFactory::s_filenames;

/*!
    Load the mesh from here.
*/
std::list<boost::shared_ptr<Drawable> > ModelFileFactory::loadModelFile(const std::string& filename, const std::string& path)
{
	std::list<boost::shared_ptr<Drawable> > drawables;
	MeshInfo info;
	IModelLoader* pLoader = 0;
	bool delLoader = true;
	std::string ext(filename.begin() + filename.find_last_of('.') + 1, filename.end());
	if( s_pBinaryBatchLoader && s_pBinaryBatchLoader->isLoaded() )
	{
		delLoader = false;
		pLoader = s_pBinaryBatchLoader;
	}
	else if(ext == "obj")
	{
		pLoader = new OBJLoader;
	}
	else if( (ext == "wrl")	|| (ext == "WRL"))
	{
		std::string modelfile = path + filename;
		std::ifstream fp(modelfile.c_str());
		if(fp.fail())
			return drawables;

		std::string format, version;
		fp >> format >> version;
		fp.close();
		if(format != "#VRML")
			return drawables;

		if(version == "V1.0")
			pLoader = new VRML1Loader;
		else
			pLoader = new VRML97Loader;
	}
	else 
		return drawables;

	// load the model, return value stores the drawables
	if(pLoader)
		drawables = pLoader->loadModel(filename, path);
	
	// we should not delete binary loader
	if(delLoader)
		delete pLoader;
	
	// if the file is not loaded correctly, this list is empty
	if(drawables.empty())
		return drawables;
	
	// save the load info
	info.format = ext;
	info.file = filename;
	info.drawables = drawables;

	s_filenames.push_back(info);

	return drawables;
}

/*!
    UnLoad a single mesh file from here. filenames should be unique regardless of the path they have.
	i.e. you cannot have a mesh file with the same file name located at different paths!
*/
MeshInfo ModelFileFactory::unloadModelFile(const std::string& filename)
{
	MeshInfo retVal;
	MeshManager* pMM = MeshManager::getInstance();
	FILEMESHES::iterator it = s_filenames.begin();
	while (it != s_filenames.end())
	{
		if(it->file == filename)
		{
			std::list<boost::shared_ptr<Drawable> >::iterator mesh_it = 
					it->drawables.begin();
			while (mesh_it != it->drawables.end())
			{
				pMM->removeMesh((*mesh_it)->getMeshName());
				++mesh_it;
			}
			retVal = *it;
			s_filenames.erase(it);
			return retVal;
		}
		++it;
	}

	return retVal;
}

	
void ModelFileFactory::saveAllFilesBinary(const std::string& filename)
{
	FILE* fp = fopen(filename.c_str(), "wb");

	MeshManager* pMM = MeshManager::getInstance();

	// write header
	FILEMESHES::iterator it = s_filenames.begin();
	size_t sz = s_filenames.size();
	fwrite(&sz, sizeof(size_t), 1, fp);
	while (it != s_filenames.end())
	{
		sz = it->file.size();
		fwrite(&sz, sizeof(size_t), 1, fp);
		fwrite(it->file.c_str(), sizeof(char), sz, fp);
		sz = it->drawables.size();
		fwrite(&sz, sizeof(size_t), 1, fp);
		std::list<boost::shared_ptr<Drawable> >::iterator mesh_it = 
					it->drawables.begin();
		while (mesh_it != it->drawables.end())
		{
			// sizeof the drawable mesh name
			sz = (*mesh_it)->getMeshName().size();
			fwrite(&sz, sizeof(size_t), 1, fp);
			fwrite((*mesh_it)->getMeshName().c_str(), sizeof(char), sz, fp);

			// sizeof the drawable texture name
			sz = (*mesh_it)->getTexName().size();
			fwrite(&sz, sizeof(size_t), 1, fp);
			fwrite((*mesh_it)->getTexName().c_str(), sizeof(char), sz, fp);

			// transform of drawable
			Transform tr = (*mesh_it)->getTransform();
			Vector3 trans = tr.getTranslation();
			fwrite(&trans, sizeof(Vector3), 1, fp);
			Quaternion rot = tr.getRotation();
			fwrite(&rot, sizeof(Quaternion), 1, fp);
			
			++mesh_it;
		}
		++it;
	}

	// write data
	it = s_filenames.begin();
	while (it != s_filenames.end())
	{
		std::list<boost::shared_ptr<Drawable> >::iterator mesh_it = 
					it->drawables.begin();
		while (mesh_it != it->drawables.end())
		{
			pMM->getMesh((*mesh_it)->getMeshName())->writeBinary(fp);
			++mesh_it;
		}
		++it;
	}
	
	fclose(fp);
}

/*!
    UnLoads all the mesh files from memory.
*/
void ModelFileFactory::unloadAllFiles()
{
	FILEMESHES::iterator it = s_filenames.begin();
	while (it != s_filenames.end() && !s_filenames.empty())
	{
		unloadModelFile(it->file);
		it = s_filenames.begin();
	}
	s_filenames.clear();
}

/*!
    Checks is the file is already loaded or not.
    \return 0 pointer if it is not loaded, pointer to MeshInfo already loaded otherwise.
*/
const MeshInfo* ModelFileFactory::isFileLoaded(const std::string& filename)
{
	FILEMESHES::const_iterator it = s_filenames.begin();
	while (it != s_filenames.end())
	{
		if(it->file == filename)
			return &(*it);
		++it;
	}
	return 0;
}

const MeshInfo* ModelFileFactory::getMeshInfo(const std::string& meshName)
{
	FILEMESHES::const_iterator it = s_filenames.begin();
	std::list<boost::shared_ptr<Drawable> >::const_iterator dr_it;
	while (it != s_filenames.end())
	{
		dr_it = it->drawables.begin();
		while (dr_it != it->drawables.end())
		{
			if((*dr_it)->getMeshName() == meshName)
				return &(*it);
			++dr_it;
		}
		++it;
	}
	return 0;
}

bool ModelFileFactory::initBinaryLoader(const std::string& filename, const std::string& path)
{
	if (s_pBinaryBatchLoader)
		delete s_pBinaryBatchLoader;

	s_pBinaryBatchLoader = new BinaryModelBatchLoader;
    printf("init bin loader 1\n");
	return s_pBinaryBatchLoader->init(filename, path);
}

void ModelFileFactory::releaseBinaryLoader()
{
	if (s_pBinaryBatchLoader)
		delete s_pBinaryBatchLoader;
	s_pBinaryBatchLoader = 0;
}
}// namespace XFace
