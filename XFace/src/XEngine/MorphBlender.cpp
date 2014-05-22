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
#include <XEngine/MorphBlender.h>
#include <XEngine/MeshManager.h>
#include <list>

namespace XEngine{

Entity MorphBlender::prioritizeChannel(const std::map<std::string, Entity>& dictionary, const Entity& rest)
{
	m_restEntity = rest;
	Entity dictResult;
	dictResult.copyFrom(m_restEntity, true); // final result go here
	
	// reset all vertices content to 0, 0, 0 for the priority maps
	resetEntity(dictResult);
	
	std::map<std::string, Entity>::const_iterator itDict = dictionary.begin();
	while(itDict != dictionary.end())
	{
		if(itDict->first != "Rest")
			computeEntityDifference(m_restEntity, dictResult, itDict->second);
		++itDict;
	}

	// normalize
	normalizeEntity(dictResult);

	return prepareChannelPriority(dictResult);
}

void MorphBlender::resetEntity(Entity& ent) const
{
	MeshManager* pMM = MeshManager::getInstance();
	std::list<boost::shared_ptr<Drawable> > drawables = ent.getDrawables();
	std::list<boost::shared_ptr<Drawable> >::iterator it = drawables.begin();
	while(it != drawables.end())
	{
		DeformableGeometry* pMesh = pMM->getMesh((*it)->getMeshName());
		
		std::vector<Vector3> vertices;
		for(size_t i = 0; i < pMesh->getVertexCount(); ++i)
			vertices.push_back(Vector3(0, 0, 0));
		pMesh->setVertices(vertices);
		
		++it;
	}
}

void MorphBlender::computeEntityDifference(const Entity& rest, Entity& dst, const Entity& ent) const
{
	MeshManager* pMM = MeshManager::getInstance();
	std::list<boost::shared_ptr<Drawable> > entDr = ent.getDrawables();
	std::list<boost::shared_ptr<Drawable> > dstDr = dst.getDrawables();
	std::list<boost::shared_ptr<Drawable> > restDr = rest.getDrawables();
	std::list<boost::shared_ptr<Drawable> >::iterator it_dst = dstDr.begin();
	std::list<boost::shared_ptr<Drawable> >::iterator it_ent = entDr.begin();
	std::list<boost::shared_ptr<Drawable> >::iterator it_rest = restDr.begin();
	while(it_dst != dstDr.end())
	{
		DeformableGeometry* pDst = pMM->getMesh((*it_dst)->getMeshName());
		DeformableGeometry* pEnt = pMM->getMesh((*it_ent)->getMeshName());
		DeformableGeometry* pRest = pMM->getMesh((*it_rest)->getMeshName());
		
		std::vector<Vector3> vDst = pDst->getVertices();
		std::vector<Vector3> vEnt = pEnt->getVertices();
		std::vector<Vector3> vRest = pRest->getVertices();
		for(size_t i = 0; i < pDst->getVertexCount(); ++i)
		{
			vDst[i].x += fabs(vEnt[i].x - vRest[i].x);
			vDst[i].y += fabs(vEnt[i].y - vRest[i].y);
			vDst[i].z += fabs(vEnt[i].z - vRest[i].z);
		}
		
		pDst->setVertices(vDst);
		
		++it_dst;
		++it_ent;
		++it_rest;
	}
}
	
void MorphBlender::normalizeEntity(Entity& dst) const
{
	MeshManager* pMM = MeshManager::getInstance();
	std::list<boost::shared_ptr<Drawable> > dstDr = dst.getDrawables();
	std::list<boost::shared_ptr<Drawable> >::iterator it_dst = dstDr.begin();
	while(it_dst != dstDr.end())
	{
		DeformableGeometry* pDst = pMM->getMesh((*it_dst)->getMeshName());
		
		std::vector<Vector3> vDst = pDst->getVertices();
		Vector3 maxVal = Vector3(0, 0, 0);
		for(size_t i = 0; i < pDst->getVertexCount(); ++i)
		{
			if(fabs(vDst[i].x) > maxVal.x)
				maxVal.x = fabs(vDst[i].x);
			if(fabs(vDst[i].y) > maxVal.y)
				maxVal.y = fabs(vDst[i].y);
			if(fabs(vDst[i].z) > maxVal.z)
				maxVal.z = fabs(vDst[i].z);
		}
		for(size_t i = 0; i < pDst->getVertexCount(); ++i)
		{
			if(maxVal.x)
				vDst[i].x /= maxVal.x;
			if(maxVal.y)
				vDst[i].y /= maxVal.y;
			if(maxVal.z)
				vDst[i].z /= maxVal.z;
		}
		
		pDst->setVertices(vDst);
		
		++it_dst;
	}
}

/*!
*/
Entity MorphBlender::prepareChannelPriority(const Entity& channel) 
{
	m_priorityChannel.release(true);
	m_priorityChannel.copyFrom(channel, true);
	
	MeshManager* pMM = MeshManager::getInstance();
	std::list<boost::shared_ptr<Drawable> > finDr = m_priorityChannel.getDrawables();
	std::list<boost::shared_ptr<Drawable> > chDr = channel.getDrawables();
	std::list<boost::shared_ptr<Drawable> >::iterator it_fin = finDr.begin();
	std::list<boost::shared_ptr<Drawable> >::iterator it_ch = chDr.begin();
	while(it_fin != finDr.end())
	{
		DeformableGeometry* pFin = pMM->getMesh((*it_fin)->getMeshName());
		DeformableGeometry* pCh = pMM->getMesh((*it_ch)->getMeshName());
		
		std::vector<Vector3> vFin = pFin->getVertices();
		std::vector<Vector3> vCh = pCh->getVertices();
		for(size_t i = 0; i < pFin->getVertexCount(); ++i)
		{
			if(fabs(vCh[i].x) > 0.25f)
				vFin[i].x = 1;	// channel dominant
			else
				vFin[i].x = 0;	// channel NOT dominant
			
			if(fabs(vCh[i].y) > 0.05f)
				vFin[i].y = 1;
			else
				vFin[i].y = 0;
			if(fabs(vCh[i].z) > 0.25f)
				vFin[i].z = 1;
			else
				vFin[i].z = 0;
		}
		pFin->setVertices(vFin);
		
		++it_fin;
		++it_ch;
	}

	return m_priorityChannel;
}

bool MorphBlender::blend(Entity& dst, const Entity& exp)
{
	if( (!exp.getDrawableCount()) || (!dst.getDrawableCount()) )
	{
		std::cerr << "Blending has a problem with input entites!" << std::endl;
		return false;
	}
	if(m_priorityChannel.getDrawableCount() == 0)
	{
		std::cerr << "Blender priorities are not ready for process!" << std::endl;
		return false;
	}
	MeshManager* pMM = MeshManager::getInstance();
	
	// get drawables
	if(!m_restEntity.getDrawableCount())
		std::cerr << "Rest frame for blending is not correct!\n";
	const std::list<boost::shared_ptr<Drawable> >& restDr = m_restEntity.getDrawables();
	std::list<boost::shared_ptr<Drawable> > finDr = dst.getDrawables();
	const std::list<boost::shared_ptr<Drawable> >& blendDr = m_priorityChannel.getDrawables();
	const std::list<boost::shared_ptr<Drawable> >& expDr = exp.getDrawables();

	// get iterators
	std::list<boost::shared_ptr<Drawable> >::const_iterator it_rst = restDr.begin();
	std::list<boost::shared_ptr<Drawable> >::const_iterator it_fin = finDr.begin();
	std::list<boost::shared_ptr<Drawable> >::const_iterator it_bld = blendDr.begin();
	std::list<boost::shared_ptr<Drawable> >::const_iterator it_exp = expDr.begin();
	while(it_fin != finDr.end())
	{
		if((*it_rst)->getBinding() == "LeftEye" || (*it_rst)->getBinding() == "RightEye" || (*it_rst)->getBinding() == "Hair")
		{
			++it_fin;
			++it_exp;
			++it_bld;
			++it_rst;
			continue;
		}
		
		DeformableGeometry* pRst = pMM->getMesh((*it_rst)->getMeshName());
		DeformableGeometry* pFin = pMM->getMesh((*it_fin)->getMeshName());
		DeformableGeometry* pBld = pMM->getMesh((*it_bld)->getMeshName());
		DeformableGeometry* pExp = pMM->getMesh((*it_exp)->getMeshName());
		
		// get lip area.
		std::set<unsigned short> lipArea = getLipArea((*it_rst)->getMeshName());
			
		const std::vector<Vector3>& vRst = pRst->getVertices();
		std::vector<Vector3> vFin = pFin->getDeformedVerticesVector();
		const std::vector<Vector3>& vBld = pBld->getVertices();
		const std::vector<Vector3>& vExp = pExp->getDeformedVerticesVector();
		
		for(unsigned short i = 0; i < pFin->getVertexCount(); ++i)
		{
			if(vBld[i].x < 0.1f)
				vFin[i].x = vExp[i].x;
			else
				vFin[i].x = 0.25f*vFin[i].x + 0.75f*vExp[i].x;
		
			if(vBld[i].z < 0.1f)
				vFin[i].z = vExp[i].z;
			else
				vFin[i].z = 0.5f*vFin[i].z + 0.5f*vExp[i].z;
			
			if(vBld[i].y < 0.1f)
				vFin[i].y = vExp[i].y;
			else	// lips must be closed for visemes priority.
			{
				// if lip area from FDP is empty, perhaps FDP has not been set
				// so, play it safe and use visemes only for the lips
				if(!lipArea.empty()) 
				{
					if(lipArea.find(i) == lipArea.end()) // NOT in the critical area
						vFin[i].y = 0.75f*vFin[i].y + 0.25f*vExp[i].y;
					else
					{
						float diffVis_y = fabs(vRst[i].y - vFin[i].y);
						if (diffVis_y > m_pFDP->getMW()*102.4)	// makes sense! 1/10 of lip width
							vFin[i].y = 0.7f*vFin[i].y + 0.3f*vExp[i].y;
					}
				}	
			}
		}
		
		pFin->setDeformedVertices(vFin);
		
		++it_fin;
		++it_exp;
		++it_bld;
		++it_rst;
	}

	return true;
}

/*!
	Gets the lip area from the FDP region (if available).
	If not available, tries to create it.
*/
const std::set<unsigned short>& MorphBlender::getLipArea(const std::string& meshName)
{
	std::map<std::string, std::set<unsigned short> >::const_iterator it = m_lipAreas.find(meshName);
	if(it != m_lipAreas.end())
		return it->second;
	
	// not created before, try to prepare it then..
	static std::set<unsigned short> lipArea;
	lipArea.clear();
	if(m_pFDP)
	{
		std::set<unsigned short> dummy;
		const XFace::FDPItem* pItem = 0;
		if(pItem = m_pFDP->findItem("2.2", meshName))
			lipArea = pItem->getAOI();
		if(pItem = m_pFDP->findItem("2.3", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
		if(pItem = m_pFDP->findItem("2.7", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}

		if(pItem = m_pFDP->findItem("2.6", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
		if(pItem = m_pFDP->findItem("2.9", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
		if(pItem = m_pFDP->findItem("2.8", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
		//
		if(pItem = m_pFDP->findItem("8.1", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
		if(pItem = m_pFDP->findItem("8.2", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
		if(pItem = m_pFDP->findItem("8.5", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
		if(pItem = m_pFDP->findItem("8.6", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
		if(pItem = m_pFDP->findItem("8.8", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
		if(pItem = m_pFDP->findItem("8.7", meshName))
		{
			dummy = pItem->getAOI();
			lipArea.insert(dummy.begin(), dummy.end());
		}
	}	
	if(!lipArea.empty())
		m_lipAreas.insert(std::make_pair(meshName, lipArea));

	return lipArea;
}
} // namespace XEngine