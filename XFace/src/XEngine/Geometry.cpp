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
 * The Original Code is Xface Core Library; Mesh Data.
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

#include <XEngine/Geometry.h>

#include <algorithm>
#include <iostream>
#include <cstdio>
#define size_t uint
namespace XEngine{

/*!
	We do NOT override the = operator but implement a new method, because in some cases,
	we only want to copy the data, not the name of the Geometry.
*/
Geometry& Geometry::copyFrom(const Geometry& rhs)
{
	// self assignment control
	if (this == &rhs) 
		return *this;

	m_vertices	= rhs.m_vertices;
	m_normals	= rhs.m_normals;
	m_texCoords = rhs.m_texCoords;

	return *this;
}


/*!
	Vertices are set through here. 
*/
void Geometry::setVertices(const std::vector<Vector3> &vertices)
{
	m_vertices = vertices;
}

/*!
	\todo check back here
*/
void Geometry::setVertices(const Vector3* pVert, unsigned int size)
{
	m_vertices.assign(pVert, pVert + size);
}

/*!
	\todo check back here
*/
void Geometry::setNormals(const Vector3* pNorm, unsigned int size)
{
	m_normals.assign(pNorm, pNorm + size);
}

/*!
	Vertex normals are set through here. 
*/
void Geometry::setNormals(const std::vector<Vector3> &normals)
{
	m_normals = normals;
}

/*!
	Texture coordinates are set through here. 
*/
void Geometry::setTexCoords(const Vertex2D* pTex, unsigned int size)
{
	m_texCoords.assign(pTex, pTex + size);
}

/*!
	Texture coordinates are set through here. 
*/
void Geometry::setTexCoords(const std::vector<Vertex2D> &texCoords)
{
	m_texCoords = texCoords;
}

/*!
	Computes the facenormal by crossing two vectors defined by (p2-p1) and (p3-p1).
	The parameters are assumed to be passed in counter-clockwise order.
*/
Vector3 Geometry::computeFaceNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	Vector3 u, v;
	u.x = p2.x - p1.x; u.y = p2.y - p1.y; u.z = p2.z - p1.z;
	v.x = p3.x - p1.x; v.y = p3.y - p1.y; v.z = p3.z - p1.z;

	Vector3 facenormal = u.unitCross(v);

	return facenormal;
}

void Geometry::writeBinary(FILE* fp)
{
	size_t sz = m_vertices.size();
	fwrite(&sz, sizeof(size_t), 1, fp);
	fwrite(&m_vertices[0], sizeof(Vector3), sz, fp);
	sz = m_normals.size();
	fwrite(&sz, sizeof(size_t), 1, fp);
	fwrite(&m_normals[0], sizeof(Vector3), sz, fp);
	sz = m_texCoords.size();
	fwrite(&sz, sizeof(size_t), 1, fp);
	fwrite(&m_texCoords[0], sizeof(Vertex2D), sz, fp);
}

void Geometry::readBinary(FILE* fp)
{
	size_t sz;
	fread(&sz, sizeof(size_t), 1, fp);
	if(!sz)
		return;
	Vector3* pData = new Vector3[sz];
	fread(pData, sizeof(Vector3), sz, fp);
	setVertices(pData, sz);

	fread(&sz, sizeof(size_t), 1, fp);
	fread(pData, sizeof(Vector3), sz, fp);
	setNormals(pData, sz);

	fread(&sz, sizeof(size_t), 1, fp);
	Vertex2D* pTex = new Vertex2D[sz];
	fread(pTex, sizeof(Vertex2D), sz, fp);
	setTexCoords(pTex, sz);
	
	delete []pData;
	delete []pTex;
}

Vector3 Geometry::computeMeanOfVertices()
{
	Vector3 mean(0, 0, 0);
	std::vector<Vector3>::const_iterator it = m_vertices.begin();
	while(it != m_vertices.end() )
	{
		mean += (*it)/(float)m_vertices.size();
		++it;
	}
	return mean;
}

/*!
	Calculates the centroid of the object by finding the bounding box and getting the mean of it.
*/
Vector3 Geometry::computeCentroid()
{
	Vector3 mean(0, 0, 0);
	Vector3 bMin(1000000, 1000000, 1000000);
	Vector3 bMax(-1000000, -1000000, -1000000);
	
	std::vector<Vector3>::const_iterator it = m_vertices.begin();
	while(it != m_vertices.end() )
	{
		if(bMin.x > it->x)
			bMin.x = it->x;
		if(bMin.y > it->y)
			bMin.y = it->y;
		if(bMin.z > it->z)
			bMin.z = it->z;

		if(bMax.x < it->x)
			bMax.x = it->x;
		if(bMax.y < it->y)
			bMax.y = it->y;
		if(bMax.z < it->z)
			bMax.z = it->z;
		++it;
	}

	mean = (bMax + bMin) / 2;
	return mean;
}

void Geometry::subtractMeanFromVertices(const Vector3& mean)
{
	std::vector<Vector3>::iterator it = m_vertices.begin();
	while(it != m_vertices.end() )
	{
		(*it) -= mean;
		++it;
	}
}
}  // namespace XFace
