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
 * The Original Code is Xface Core Library; OpenGL Renderer.
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

#include <XEngine/RendererGL.h>
#ifdef WIN32
	#pragma message("		Adding OpenGL lib...")
	#pragma comment( lib, "OpenGL32.lib")
#endif
#include <XEngine/DeformableGeometry.h>
#include <XEngine/ITexture.h>

#include <XEngine/GLPrerrequisites.h>
#include <XEngine/Drawable.h>
#include <XEngine/MeshManager.h>
#include <XEngine/TextureManager.h>
#include <string>
#include <iostream>
#include <fstream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace XEngine{

RendererGL::RendererGL() : m_maxBufferID(0)//, m_rotateX(0), m_rotateY(0), m_rotateZ(0)
{
	initGLExtensions();
}
void RendererGL::initGLExtensions()
{
	// nullify the pointers
	glBindBufferARB           = NULL;
	glDeleteBuffersARB        = NULL;
	glGenBuffersARB           = NULL;
	glIsBufferARB             = NULL;
	glBufferDataARB           = NULL;
	glBufferSubDataARB        = NULL;
	glGetBufferSubDataARB     = NULL;
	glMapBufferARB            = NULL;
	glUnmapBufferARB          = NULL;
	glGetBufferParameterivARB = NULL;
	glGetBufferPointervARB    = NULL;
	glActiveTextureARB			= NULL;
	glClientActiveTextureARB	= NULL;

	// set the bool query storage to all false
	memset(&m_supported[0], 0, sizeof(bool)*MAX_EXTENSIONS);

	const unsigned char* ext = glGetString (GL_EXTENSIONS);
	std::string ext_str = (char*)ext;
/*
	std::ofstream extensionDump("extensions.txt");
	extensionDump << ext_str;
	extensionDump.close();
*/

	// GL_ARB_multitexture
	if(std::string::npos != ext_str.find("GL_ARB_multitexture") )
	{
		m_supported[XGL_ARB_multitexture] = true;

		/* Obtain the address of the extension entry points. */
		glActiveTextureARB		= (PFNGLACTIVETEXTUREARBPROC)	getProcAddress("glActiveTextureARB");
		glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) getProcAddress("glClientActiveTextureARB");
		// Load the rest IF you need them, no need to store unused function pointers, yes I am lazy as well..
/*
		glMultiTexCoord2fARB	= (PFNGLMULTITEXCOORD2FARBPROC)	getProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord1dARB = (PFNGLMULTITEXCOORD1DARBPROC) getProcAddress("glMultiTexCoord1dARB");
        glMultiTexCoord1dvARB = (PFNGLMULTITEXCOORD1DVARBPROC) getProcAddress("glMultiTexCoord1dvARB");
        glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) getProcAddress("glMultiTexCoord1fARB");
        glMultiTexCoord1fvARB = (PFNGLMULTITEXCOORD1FVARBPROC) getProcAddress("glMultiTexCoord1fvARB");
        glMultiTexCoord1iARB = (PFNGLMULTITEXCOORD1IARBPROC) getProcAddress("glMultiTexCoord1iARB");
        glMultiTexCoord1ivARB = (PFNGLMULTITEXCOORD1IVARBPROC) getProcAddress("glMultiTexCoord1ivARB");
        glMultiTexCoord1sARB = (PFNGLMULTITEXCOORD1SARBPROC) getProcAddress("glMultiTexCoord1sARB");
        glMultiTexCoord1svARB = (PFNGLMULTITEXCOORD1SVARBPROC) getProcAddress("glMultiTexCoord1svARB");
        glMultiTexCoord2dARB = (PFNGLMULTITEXCOORD2DARBPROC) getProcAddress("glMultiTexCoord2dARB");
        glMultiTexCoord2dvARB = (PFNGLMULTITEXCOORD2DVARBPROC) getProcAddress("glMultiTexCoord2dvARB");
        glMultiTexCoord2fvARB = (PFNGLMULTITEXCOORD2FVARBPROC) getProcAddress("glMultiTexCoord2fvARB");
        glMultiTexCoord2iARB = (PFNGLMULTITEXCOORD2IARBPROC) getProcAddress("glMultiTexCoord2iARB");
        glMultiTexCoord2ivARB = (PFNGLMULTITEXCOORD2IVARBPROC) getProcAddress("glMultiTexCoord2ivARB");
        glMultiTexCoord2sARB = (PFNGLMULTITEXCOORD2SARBPROC) getProcAddress("glMultiTexCoord2sARB");
        glMultiTexCoord2svARB = (PFNGLMULTITEXCOORD2SVARBPROC) getProcAddress("glMultiTexCoord2svARB");
        glMultiTexCoord3dARB = (PFNGLMULTITEXCOORD3DARBPROC) getProcAddress("glMultiTexCoord3dARB");
        glMultiTexCoord3dvARB = (PFNGLMULTITEXCOORD3DVARBPROC) getProcAddress("glMultiTexCoord3dvARB");
        glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) getProcAddress("glMultiTexCoord3fARB");
        glMultiTexCoord3fvARB = (PFNGLMULTITEXCOORD3FVARBPROC) getProcAddress("glMultiTexCoord3fvARB");
        glMultiTexCoord3iARB = (PFNGLMULTITEXCOORD3IARBPROC) getProcAddress("glMultiTexCoord3iARB");
        glMultiTexCoord3ivARB = (PFNGLMULTITEXCOORD3IVARBPROC) getProcAddress("glMultiTexCoord3ivARB");
        glMultiTexCoord3sARB = (PFNGLMULTITEXCOORD3SARBPROC) getProcAddress("glMultiTexCoord3sARB");
        glMultiTexCoord3svARB = (PFNGLMULTITEXCOORD3SVARBPROC) getProcAddress("glMultiTexCoord3svARB");
        glMultiTexCoord4dARB = (PFNGLMULTITEXCOORD4DARBPROC) getProcAddress("glMultiTexCoord4dARB");
        glMultiTexCoord4dvARB = (PFNGLMULTITEXCOORD4DVARBPROC) getProcAddress("glMultiTexCoord4dvARB");
        glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) getProcAddress("glMultiTexCoord4fARB");
        glMultiTexCoord4fvARB = (PFNGLMULTITEXCOORD4FVARBPROC) getProcAddress("glMultiTexCoord4fvARB");
        glMultiTexCoord4iARB = (PFNGLMULTITEXCOORD4IARBPROC) getProcAddress("glMultiTexCoord4iARB");
        glMultiTexCoord4ivARB = (PFNGLMULTITEXCOORD4IVARBPROC) getProcAddress("glMultiTexCoord4ivARB");
        glMultiTexCoord4sARB = (PFNGLMULTITEXCOORD4SARBPROC) getProcAddress("glMultiTexCoord4sARB");
        glMultiTexCoord4svARB = (PFNGLMULTITEXCOORD4SVARBPROC) getProcAddress("glMultiTexCoord4svARB");
*/
    }
	
	// GL_ARB_vertex_buffer_object
	if(std::string::npos != ext_str.find("GL_ARB_vertex_buffer_object") )
	{
		m_supported[XGL_ARB_vertex_buffer_object] = true;
		glBindBufferARB           = (PFNGLBINDBUFFERARBPROC)getProcAddress("glBindBufferARB");
        glDeleteBuffersARB        = (PFNGLDELETEBUFFERSARBPROC)getProcAddress("glDeleteBuffersARB");
        glGenBuffersARB           = (PFNGLGENBUFFERSARBPROC)getProcAddress("glGenBuffersARB");
        glIsBufferARB             = (PFNGLISBUFFERARBPROC)getProcAddress("glIsBufferARB");
        glBufferDataARB           = (PFNGLBUFFERDATAARBPROC)getProcAddress("glBufferDataARB");
        glBufferSubDataARB        = (PFNGLBUFFERSUBDATAARBPROC)getProcAddress("glBufferSubDataARB");
        glGetBufferSubDataARB     = (PFNGLGETBUFFERSUBDATAARBPROC)getProcAddress("glGetBufferSubDataARB");
        glMapBufferARB            = (PFNGLMAPBUFFERARBPROC)getProcAddress("glMapBufferARB");
        glUnmapBufferARB          = (PFNGLUNMAPBUFFERARBPROC)getProcAddress("glUnmapBufferARB");
        glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)getProcAddress("glGetBufferParameterivARB");
        glGetBufferPointervARB    = (PFNGLGETBUFFERPOINTERVARBPROC)getProcAddress("glGetBufferPointervARB");
	}
}

void RendererGL::prepareBufferedMesh(IndexedFaceSet* pMesh)
{
	if(!isExtSupported(XGL_ARB_vertex_buffer_object))
		return;
 /*   
	// Create a Vertex Buffer Object and an Element Buffer Object
	int sz = pMesh->getVertexCount();
    int nArrayObjectSize = (2* sizeof(Vector3) + sizeof(Vertex2D)) * pMesh->getVertexCount();
	char *pData = new char[nArrayObjectSize];
	memcpy(pData, pMesh->getVertices(), sizeof(Vector3)*sz);
	memcpy(&pData[sizeof(Vector3)*sz], pMesh->getNormals(), sizeof(Vector3)*sz);
	memcpy(&pData[2*sizeof(Vector3)*sz], pMesh->getTexCoords(), sizeof(Vertex2D)*sz);

    int nParam_ArrayObjectSize = 0;

	unsigned int id = 0;
    glGenBuffersARB( 1, &id );
	pMesh->setBufferID(id);
	m_maxBufferID = max(id, m_maxBufferID);

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, id );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, nArrayObjectSize, pData, GL_STATIC_DRAW_ARB );

    glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

    if( nParam_ArrayObjectSize <= 0 )
        return;
    
    // Don't forget the Element Buffer Object to hold our quad's indices.
	nArrayObjectSize = sizeof(unsigned short) * pMesh->getIndexCount();
    nParam_ArrayObjectSize = 0;

    glGenBuffersARB( 1, &id );
	pMesh->setIndexBufferID(id);
	m_maxBufferID = max(id, m_maxBufferID);

    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, id );
    glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nArrayObjectSize, pMesh->getIndices(), GL_STATIC_DRAW_ARB );

    glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

    if( nParam_ArrayObjectSize <= 0 )
		return;

    glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
    glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );

	delete []pData;
*/
}

void RendererGL::deleteBuffers()
{
	if(!isExtSupported(XGL_ARB_vertex_buffer_object))
		return;

	for (unsigned int id = 1; id < m_maxBufferID; ++id)
	{
		if( glIsBufferARB(id) )
			glDeleteBuffersARB( 1, &id );
	}
}

void RendererGL::doTexture(const ITexture& tex) const
{
	if(tex.getTextureType() == ITexture::TEXTURE2D)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex.getTextureID());
	}
}

void RendererGL::doGeometry(DeformableGeometry& mesh) const
{
//	if(mesh.getBufferID() > 0)
	if(false)
	{
/*		glBindBufferARB( GL_ARRAY_BUFFER_ARB, mesh.getBufferID() );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, mesh.getIndexBufferID());

		long offset = sizeof(Vector3)*mesh.getVertexCount();
		glVertexPointer( 3, GL_FLOAT, 0, BUFFER_OFFSET(0) );
		glNormalPointer( GL_FLOAT, 0, BUFFER_OFFSET(offset));
		glTexCoordPointer( 2, GL_FLOAT, 0, BUFFER_OFFSET(2*offset));

		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY);

		glDrawElements( GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
		//glDrawArrays( GL_TRIANGLES, 0, mesh.getIndexCount()/3);

		glDisableClientState( GL_VERTEX_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY);

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
*/	}
	else
	{
		glVertexPointer( 3, GL_FLOAT, 0, mesh.getDeformedVertices());
		if(!mesh.getNormals().empty())
		{
			glNormalPointer( GL_FLOAT, 0, &(mesh.getNormals()[0]));
			glEnableClientState( GL_NORMAL_ARRAY );
		}
		
		if(!mesh.getTexCoords().empty())
		{
			glTexCoordPointer( 2, GL_FLOAT, 0, &(mesh.getTexCoords()[0]));
			glEnableClientState( GL_TEXTURE_COORD_ARRAY);
		}

		glEnableClientState( GL_VERTEX_ARRAY );
				
		glDrawElements( GL_TRIANGLES, (GLsizei)mesh.getIndexCount(), GL_UNSIGNED_SHORT, mesh.getIndices() );
		//glDrawArrays( GL_TRIANGLES, 0, mesh.getIndexCount()/3);
		
		glDisableClientState( GL_VERTEX_ARRAY );
		if(!mesh.getNormals().empty())
			glDisableClientState( GL_NORMAL_ARRAY );
		
		if(!mesh.getTexCoords().empty())
			glDisableClientState( GL_TEXTURE_COORD_ARRAY);

	}
}

void RendererGL::doTransform(const Transform& trans) const
{
	glMultMatrixf(trans.getWorldTransform());
/*	Vector3 rotAx = trans.getRotationAxis();
	glRotatef(trans.getRotationAngle(), rotAx.x, rotAx.y, rotAx.z);

	Vector3 tr = trans.getTranslation();
	glTranslatef(tr.x, tr.y, tr.z);

	Vector3 sc = trans.getScale();
	glScalef(sc.x, sc.y, sc.z);
*/
}

/*!
	Renders a Drawable instance using OpenGL.
*/
void RendererGL::render(boost::shared_ptr<Drawable> pDrawable) const
{
	std::string MeshName = pDrawable->getMeshName();

	DeformableGeometry* pMesh = MeshManager::getInstance()->getMesh(MeshName);
	if(!pMesh)
		return;
	glPushMatrix();
	glMultMatrixf(m_globalTransform.getLocalTransform());

	if(pDrawable->needUpdate())
   {

		if (pDrawable->getBinding() == "LeftEye")
		{
			Vector3 eye = pDrawable->getPivot();
			//double eyePos[3] = { -33.8, -33.2, -28.8 };

			glTranslatef (-eye.x, -eye.y, -eye.z);
			glRotatef (pDrawable->getAPs()[24], 0.0, 1.0, 0.0);
			glRotatef (pDrawable->getAPs()[22], 1.0, 0.0, 0.0);
			glTranslatef (eye.x, eye.y, eye.z);
		}

		if (pDrawable->getBinding() == "RightEye")
		{
			Vector3 eye = pDrawable->getPivot();
			///double eyePos[3] = { 33.8, -33.2, -28.8 };

			glTranslatef (-eye.x, -eye.y, -eye.z);
			glRotatef (pDrawable->getAPs()[25], 0.0, 1.0, 0.0);
			glRotatef (pDrawable->getAPs()[23], 1.0, 0.0, 0.0);
			glTranslatef (eye.x, eye.y, eye.z);
		}

		pDrawable->updateAnimation();	
   }

	doTransform(pDrawable->getTransform());
	
	if(pDrawable->isTextureOn())
	{
		const ITexture* pTexture = TextureManager::getInstance()->getTexture(pDrawable->getTexName());
		if(pTexture)
			doTexture(*pTexture);
	}
	
	doGeometry(*pMesh);

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

}  // namespace XFace
