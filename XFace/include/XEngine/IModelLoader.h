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
	\file	IModelLoader.h
	\brief	Base class for the scene graph loaders.
*/

#ifndef IMODELLOADER_H_
#define IMODELLOADER_H_


#include <list>
#include <string>
#include <boost/shared_ptr.hpp>
#include "Drawable.h"

namespace XEngine{
	
/*!
	\brief	Base class for the scene graph loaders.
	\ingroup XEngine
	\author	Koray Balci
	\version 1.0
	\date   March 2003

	This class should be the base for the scene graph loaders. Currently,
	has only one pure virtual function to define the interface to load scenes.

	With this abstract base class, we ensure future extendibility for various
	3D model/scene formats.
*/
class IModelLoader
{
public:
	/*! 
		Pure virtual function for loader. 
		\param filename Name of the model file.
		\param dir Path of the model file.
		\return the list of the names of meshes loaded.
	*/
	virtual std::list<boost::shared_ptr<Drawable> > loadModel(const std::string &filename, const std::string& dir="./") = 0;
};
} // namespace XEngine
#endif // IMODELLOADER_H_
