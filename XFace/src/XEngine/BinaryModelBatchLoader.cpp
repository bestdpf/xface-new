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

#include <XEngine/BinaryModelBatchLoader.h>
#include <XEngine/TextureManager.h>
#include <cstdio>
#include <cstring>
#define size_t uint
namespace XEngine{

	BinaryModelBatchLoader::BinaryModelBatchLoader(void) : m_bLoaded(false)
{
}

BinaryModelBatchLoader::~BinaryModelBatchLoader(void)
{
}

std::list<boost::shared_ptr<Drawable> > BinaryModelBatchLoader::loadModel(const std::string &filename, const std::string& dir)
{
	std::list< std::pair< std::string, DRAWABLES> >::const_iterator it = m_data.begin();
	while(it != m_data.end())
	{
		if(it->first == filename)
			return it->second;
		++it;
	}
	
	DRAWABLES retVal;
	return retVal;
}

bool BinaryModelBatchLoader::init(const std::string& filename, const std::string& path)
{
    printf("binmb init %s %s\n",filename.c_str(),path.c_str());
	m_bLoaded = false;
	FILE* fp = fopen((path + filename).c_str(), "rb");
	if(!fp){
        printf("fail file\n");
		return false;
    }

	m_data.clear();
	boost::shared_ptr<Drawable> pDrawable;
			
	size_t sz_files = 0;
    printf("size_t size %d\n",sizeof(sz_files));
	fread(&sz_files, sizeof(sz_files), 1, fp);
	for(size_t i = 0; i < sz_files; ++i)
	{
        printf("read %d sz file\n ",i);
		size_t sz = 0, sz_drawables = 0;
        printf("0.1\n");
		fread(&sz, sizeof(sz), 1, fp);
        printf("0.2 new char size of %d\n",sz+1);
		char* fname = new char[sz + 1];
        fflush(stdout);
		fread(fname, sizeof(char), sz, fp);
		fname[sz] = 0; // null termination
		fread(&sz_drawables, sizeof(size_t), 1, fp);
		DRAWABLES drawables;
		for(size_t j = 0; j < sz_drawables; ++j)
		{
            printf("sz draw %d\n",j);
			Drawable *dr = new Drawable;
			
			// mesh name
			fread(&sz, sizeof(size_t), 1, fp);
			char* drname = new char[sz + 1];
			fread(drname, sizeof(char), sz, fp);
			drname[sz] = 0; // null termination
			dr->setMeshName(drname);
			delete []drname;

			// tex name
			fread(&sz, sizeof(size_t), 1, fp);
			drname = new char[sz + 1];
			fread(drname, sizeof(char), sz, fp);
			drname[sz] = 0; // null termination
			dr->setTexName(drname);
			TextureManager::getInstance()->load(path + drname, drname);
			delete []drname;

			// transform (translation and rotation)
			Transform tr;
			Vector3 trans;
			Quaternion q;
			fread(&trans, sizeof(Vector3), 1, fp);
			tr.setTranslation(trans);
			fread(&q, sizeof(Quaternion), 1, fp);
			tr.setRotation(q);
			dr->setTransform(tr);

			pDrawable.reset(dr);
			drawables.push_back(pDrawable);
		}
		std::string dummy;
		dummy.assign(fname);
		m_data.push_back(std::make_pair(dummy, drawables));
		delete [] fname;
	}
    printf("process data\n");
	// process the data now
	MeshManager* pMM = MeshManager::getInstance();
	std::list< std::pair< std::string, DRAWABLES> >::const_iterator it = m_data.begin();
	while(it != m_data.end() )
	{
		DRAWABLES::const_iterator it_dr = it->second.begin();
		while(it_dr != it->second.end())
		{
			DeformableGeometry* pGeo = new DeformableGeometry((*it_dr)->getMeshName());
			pGeo->readBinary(fp);
			pMM->registerMesh(pGeo);
			++it_dr;
		}
		++it;
	}

	fclose(fp);
	m_bLoaded = true;
    printf("20\n");
	return true;
}

} // namespace XEngine
