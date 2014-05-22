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
 * The Original Code is Xface Core Library; Model Loading.
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
	\file	ModelFileFactory.h
	\brief	3d Models are loaded through here
*/
#ifndef MODELFILEFACTORY_H_
#define MODELFILEFACTORY_H_

#include <list>
#include <boost/shared_ptr.hpp>
#include "Drawable.h"
#include "BinaryModelBatchLoader.h"

namespace XEngine{
    /*!
	  \brief 3D Meshe info
      \ingroup XEngine
      \author Koray Balci
      \version 1.0
      \date   Nov 2003
      
      Stores the information about the mesh file, both the list of drawables
      and file info for file name, path and format.
    */
	struct MeshInfo
	{
		std::string file;
		std::string format;
		std::string path;
		std::string keyframe_alias;
		std::string keyframe_category;
		std::list<boost::shared_ptr<Drawable> > drawables;
	};

	/*!
	  \brief 3D Meshes are loaded from here.
      \ingroup XEngine
      \author Koray Balci
      \version 1.0
      \date   Nov 2003
      
      This class is used to keep track of which mesh files are currently loaded
      through MeshManager and in memory. It is a Monolithic design pattern impl where 
      all members are static.
      \todo I guess this is only necessary for XFaceEd, so we might consider moving
      this class there.
      
      */
    class ModelFileFactory
	{
		typedef std::list<MeshInfo> FILEMESHES;
		static FILEMESHES s_filenames;
		static BinaryModelBatchLoader* s_pBinaryBatchLoader;
	public:
	    //! returns the list of MeshInfo for the files already loaded.
		static const MeshInfo* getMeshInfo(const std::string& meshName);
		static bool initBinaryLoader(const std::string& filename, const std::string& path);
		static void releaseBinaryLoader();
		static const FILEMESHES& getFileMeshes(){return s_filenames;}
		static bool empty() {return s_filenames.empty();}
		static std::list<boost::shared_ptr<Drawable> > loadModelFile(const std::string& filename, const std::string& path = "./");
		static MeshInfo unloadModelFile(const std::string& filename);
		static const MeshInfo* isFileLoaded(const std::string& filename);
		static void unloadAllFiles();
		static void saveAllFilesBinary(const std::string& filename);
	};
} // namespace XFace

#endif // MODELFILEFACTORY_H_
