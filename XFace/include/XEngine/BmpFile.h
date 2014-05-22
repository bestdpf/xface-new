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
 * The Original Code is Xface Core Library;.
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
	\file	BmpFile.h
	\brief	BMP file loader for textures.
*/
#ifndef BMPFILE_H_
#define BMPFILE_H_

#include "ITextureFile.h"

namespace XEngine{
	/*!
		\brief	BMP file loader for textures.
		\ingroup XEngine
		\author	Koray Balci
		\version 1.0
		\date   April 2003

		Loads BMP files as texture map.
	*/
	class BmpFile : public ITextureFile
	{
	public:
		bool load(const std::string& filename);
		BmpFile(void){};
		~BmpFile(void);
	};
}
#endif // BMPFILE_H_
