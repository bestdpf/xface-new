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
#include <XFace/EyeChannel.h>
#include <XEngine/MeshManager.h>

namespace XFace{

using XEngine::Drawable;
using XEngine::MeshManager;
using XEngine::DeformableGeometry;

bool EyeChannel::updateResult(XEngine::Entity& result, const XEngine::Entity& rest)
{
	if( (!result.getDrawableCount()) || (!m_result.getDrawableCount()) )
		return false;
	
	MeshManager* pMM = MeshManager::getInstance();
	const std::list<boost::shared_ptr<Drawable> >& restDr = rest.getDrawables();
	std::list<boost::shared_ptr<Drawable> > resDr = result.getDrawables();
	const std::list<boost::shared_ptr<Drawable> >& eyeDr = m_result.getDrawables();
	std::list<boost::shared_ptr<Drawable> >::const_iterator it_rest = restDr.begin();
	std::list<boost::shared_ptr<Drawable> >::const_iterator it_res = resDr.begin();
	std::list<boost::shared_ptr<Drawable> >::const_iterator it_eye = eyeDr.begin();
	while(it_res != resDr.end())
	{
		if( (m_name != "BlinkEyes") && (*it_rest)->getBinding() != "LeftEye" && (*it_rest)->getBinding() != "RightEye")
		{
			++it_rest;
			++it_res;
			++it_eye;
			continue;
		}
		DeformableGeometry* pRes = pMM->getMesh((*it_res)->getMeshName());
		const DeformableGeometry* pEye = pMM->getMesh((*it_eye)->getMeshName());
		const DeformableGeometry* pRest = pMM->getMesh((*it_rest)->getMeshName());
		
		std::vector<Vector3> vRest = pRest->getVertices();
		std::vector<Vector3> vRes = pRes->getDeformedVerticesVector();
		std::vector<Vector3> vResNorm = pRes->getNormals();
		const std::vector<Vector3>& vEye = pEye->getDeformedVerticesVector();
		const std::vector<Vector3>& vEyeNorm = pEye->getNormals();
		for(size_t i = 0; i < pRes->getVertexCount(); ++i)
		{
			Vector3 test = vRest[i] - vEye[i];
			if( fabs(test.x) + fabs(test.y) + fabs(test.z) > 0.1f)
			{
				vRes[i] = vEye[i];
				vResNorm[i] = vEyeNorm[i];
			}
		}
		pRes->setDeformedVertices(vRes);
		pRes->setNormals(vResNorm);
		
		++it_rest;
		++it_res;
		++it_eye;
	}
	return true;
}

}//namespace XFace