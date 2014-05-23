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

#include<stdio.h>
#include<stdlib.h>
#include <XEngine/BmpFile.h>
#ifdef WIN32
	#include <direct.h> // not sure if os independent (not think so)
	#include <GL/glaux.h>
	#pragma message("		Adding GLaux lib...")
	#pragma comment( lib, "glaux.lib"	 )
#else
	#include<XEngine/bitmap.h>
	#include<math.h>
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
	printf("laod bmp file now\n");
	// Safety check
	FILE* fp = fopen(filename.c_str(), "rb");
	if(!fp)
		return false;
	else
		fclose(fp);
#ifdef WIN32
	AUX_RGBImageRec *pBitmap = auxDIBImageLoad(filename.c_str());	// Load the bitmap and store the data
	if(!pBitmap)		// If we can't load the file, quit!
		return false;

	m_Width = pBitmap->sizeX;
	m_Height = pBitmap->sizeY;
	
	m_pData = pBitmap->data;
	m_nBits = 24; // Aux lib loads only these I guess..
	delete pBitmap;
#else
	//for linux, using bitmap.h
	CBitmap fbmp=CBitmap(filename.c_str());
	if(fbmp.GetWidth()==0)return false;
	m_Width = fbmp.GetWidth();
	m_Height =fbmp.GetHeight();
	m_nBits = 24;//fbmp.GetBitCount();//should be 24
	unsigned int data_sz=m_Width*m_Height*m_nBits/8+sizeof(unsigned int);
	printf("data sz is %d bitcount is %d real bit count is  %d\n------------------------------\n",data_sz,m_nBits,fbmp.GetBitCount());
	m_pData = new unsigned char[data_sz];
	unsigned int sz=data_sz;
	unsigned ALPHA=0x00000000,RED= 0x000000FF,GREEN= 0x0000FF00,BLUE = 0x00FF0000;
	bool ret=fbmp.GetBits(m_pData,sz,RED,GREEN,BLUE,ALPHA,false);
	//m_nBits = fbmp.GetBitCount();
	if(!ret)printf("getbits fail\n----------------------------------------\n");
#endif
	return true;
}

} // namespace XFace

