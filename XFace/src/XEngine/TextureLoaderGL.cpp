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
 * The Original Code is Xface Core Library; OpenGL Texture Loader.
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

#include <XEngine/TextureLoaderGL.h>

#include <XEngine/BmpFile.h>
#include <XEngine/TgaFile.h>

#ifdef WIN32
	#include <windows.h>
	#pragma message("		Adding GLU lib...")
	#pragma comment( lib, "glu32.lib"	 )
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <algorithm>
#include <string>



namespace XEngine{

TextureLoaderGL::TextureLoaderGL()
{

}

/*!
	IMPORTANT: Clears the texture list and deletes the textures if there are any.
*/
TextureLoaderGL::~TextureLoaderGL()
{
	for(TEXLIST::iterator it = m_TextureList.begin();
		it != m_TextureList.end(); ++it)
			glDeleteTextures(1, &(*it));
	m_TextureList.clear();
}

/*!
	Unloads a texture attached to a Texture object.
*/
void TextureLoaderGL::unLoad(const ITexture* pTexture)
{
	if(!pTexture)
		return;

	TEXLIST::iterator it = std::find(m_TextureList.begin(), m_TextureList.end(), pTexture->getTextureID());
	glDeleteTextures(1, &(*it));
	m_TextureList.erase(it);
}

/*!
	Loads a texture from a file and attaches it to an ITexture object.
	\param filename Path to an image file to load the texture.
	\param pTexture A valid pointer to an existing ITexture instance to attach the loaded texture.
	\note Currently only bitmap (using gluaux library) and tga files are supported.
*/
bool TextureLoaderGL::load(const std::string& filename, ITexture* pTexture)
{
    printf("load texturer %s\n-----------------------------\n",filename.c_str());
	if(!pTexture)
		return false;
	// make a format check and use base class pointer for loader
	ITextureFile* loader = 0;
		
	try
	{
		std::string namefile(filename);
	//#ifdef WIN32
		if(namefile.find_last_of(".bmp") == namefile.size() - 1)
			loader = new BmpFile;
		else 
	//#endif
		if(namefile.find_last_of(".tga") == namefile.size() - 1)
			loader = new TgaFile;
		else
		{
			return false;
		}
		
		if(!loader->load(filename))
		{
			delete loader;
			return false;
		}
		
		GLenum mode = GL_RGB8;
		switch (loader->getNBits())
		{
		case 8: mode = GL_ALPHA; break;
		case 24: mode = GL_RGB; break;
		case 32: mode = GL_RGBA; break;
		}

		// Generate a texture with the associative texture ID stored in the array
		glGenTextures(1, &pTexture->m_TexID);

		// This sets the alignment requirements for the start of each pixel row in memory.
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

		// Bind the texture to the texture arrays index and init the texture
		glBindTexture(GL_TEXTURE_2D, pTexture->getTextureID());

		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
		// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR is the smoothest.
		// GL_NEAREST is faster than GL_LINEAR, but looks blochy and pixelated.  Good for slower computers though.
		// Read more about the MIN and MAG filters at the bottom of main.cpp	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glDepthMask(GL_TRUE);
		// Build Mipmaps (builds different versions of the picture for distances - looks better)
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, loader->getWidth(), loader->getHeight(), mode, GL_UNSIGNED_BYTE, loader->getData());

		m_TextureList.push_back(pTexture->getTextureID());

		delete loader;
		return true;
	}
	catch(...)
	{
		delete loader;
		return false;
	}
}

}  // namespace XFace

