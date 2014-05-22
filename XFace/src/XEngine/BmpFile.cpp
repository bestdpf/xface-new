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
 * The Original Code is Xface Core Library; Bitmap Loader.
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


#include <XEngine/BmpFile.h>

#include <direct.h> // not sure if os independent (not think so)

#include <stdio.h>
#include <GL/glaux.h>

#ifdef WIN32
	#pragma message("		Adding GLaux lib...")
	#pragma comment( lib, "glaux.lib"	 )
#endif

namespace XEngine{

BmpFile::~BmpFile(void)
{
	if (m_pData)
	{
		free(m_pData); // unfortunately it was allocated using malloc in AUX lib.
		m_pData = 0;
	}
}

/*!
	Loads the BMP file to memory (acces data using ITextureFile base class members).
	\note Uses glaux library for the moment. It doesn't come with devcpp/gcc so do not
	forget to add it as a dependency.
*/
bool BmpFile::load(const std::string& filename)
{
	// Safety check
	FILE* fp = fopen(filename.c_str(), "rb");
	if(!fp)
		return false;
	else
		fclose(fp);

	AUX_RGBImageRec *pBitmap = auxDIBImageLoad(filename.c_str());	// Load the bitmap and store the data
	
	if(!pBitmap)		// If we can't load the file, quit!
		return false;

	m_Width = pBitmap->sizeX;
	m_Height = pBitmap->sizeY;
	
	m_pData = pBitmap->data;
	m_nBits = 24; // Aux lib loads only these I guess..
	delete pBitmap;
	return true;
}

} // namespace XFace

