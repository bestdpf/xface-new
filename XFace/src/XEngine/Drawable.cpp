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

#include "XEngine/Drawable.h"

namespace XEngine{

const std::vector<float>& Drawable::getAPs() 
{
	if(m_pFAPStream)
		return m_pFAPStream->getCurrentFAP(); 

	static std::vector<float> dummy;
	return dummy;
} 

void Drawable::destroyData() const
{
	if(m_dataExists)
		MeshManager::getInstance()->removeMesh(m_MeshName);
}
void Drawable::resetDeformedVertices()
{
	MeshManager* pMM = MeshManager::getInstance();
	DeformableGeometry* pMesh = pMM->getMesh(m_MeshName);
	pMesh->resetDeformedVertices();		
}

Drawable* Drawable::clone(bool duplicateData) const 
{
	static unsigned int cloneID = 0;	// to hash the clones
	Drawable* dr = new Drawable(*this);
	dr->setBinding(getBinding());
	if(duplicateData){
		char buffer[10];
		dr->m_MeshName = m_MeshName + "clone" + itoa(cloneID++, buffer, 10);
		MeshManager* pMM = MeshManager::getInstance();
		DeformableGeometry* pTargMeshSrc = pMM->getMesh(m_MeshName);
		DeformableGeometry* pTargMesh = new DeformableGeometry(dr->m_MeshName);
		pTargMesh->copyFrom(*pTargMeshSrc);
		dr->m_dataExists = duplicateData;
		pMM->registerMesh(pTargMesh);
	}

	return dr;
}

void Drawable::updateAnimation()
{
	if(m_bNeedUpdate)	// double check..
	{
		m_bNeedUpdate = false;

		DeformableGeometry* pGeo = MeshManager::getInstance()->getMesh(m_MeshName);
		if(pGeo)
			pGeo->update(m_pFAPStream);

		// update the transformation
		m_transform.reset();

		Matrix4 id;
		id.loadIdentity();
		m_transform.update(id);
	}
}
}
