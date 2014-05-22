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
 * The Original Code is Xface Core Library
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

#pragma once
#ifndef FACEENTITY_H_
#define FACEENTITY_H_

#include <boost/shared_ptr.hpp>
#include <map>
#include <string>

#include <XEngine/Entity.h>
#include <XFace/FDPItem.h>
#include <XFace/IFapStream.h>
#include <XFace/IInfluenceCalculator.h>
#include <XEngine/MeshManager.h>

namespace XFace{

using XEngine::Entity;
using XEngine::MeshManager;
using XEngine::DeformableGeometry;

class FaceEntity :
	public Entity
{
	/*!
	*/
	struct skinDeformer_functor
	{
	private:
		const MeshManager* const m_pMM;
		boost::shared_ptr<XFace::IFapStream> m_pFAPStream;
		//const std::vector<float>& m_FAPs;
	public:
		//skinDeformer_functor(const MeshManager* const pMM, const std::vector<float>& FAPs) : m_pMM(pMM), m_FAPs(FAPs) {}
		skinDeformer_functor(const MeshManager* const pMM, const boost::shared_ptr<XFace::IFapStream>& faps) : m_pMM(pMM), m_pFAPStream(faps) {}
		void operator()(boost::shared_ptr<XEngine::Drawable> dr) const{
			dr->updateAnimationParams(m_pFAPStream);
		}
	};
	
	/*!
		Cleans the old (if any) drawable contents (bones)
	*/
	struct skinResetter_functor
	{
	private:
		const MeshManager* const m_pMM;
	public:
		skinResetter_functor(const MeshManager* const pMM) : m_pMM(pMM) {}
		void operator()(boost::shared_ptr<XEngine::Drawable> dr) const{
			DeformableGeometry* pMesh = m_pMM->getMesh(dr->getMeshName());
			if(pMesh)
				pMesh->clearInfluences();
		}
	};
	
	/*!
		for each FDPItem, retrieves the member IInfluenceCalculator*'s, and executes deformerInitializer_functor on them.
		Then, after accumulating the weights, passes the bone info to DeformableGeometry objects in m_Drawables.
	*/
	struct deformationInitializer_functor
	{
	private:
		/*!
			for each std::pair<IInfluenceCalculator* const, std::string> (actually IInfluenceCalculator*), 
			calculates the weights in IInfluenceCalculator instances. Then transfers these weights to SkinnedMesh obj.
		*/
		struct influenceToSkin_functor
		{
		private:
			DeformableGeometry* const m_pMesh;
			const FDPItem* const m_pFdp;
		public:
			influenceToSkin_functor(DeformableGeometry* const pMesh, const FDPItem* const pFdp) : m_pMesh(pMesh), m_pFdp(pFdp) {}
			void operator() (IInfluenceCalculator* const inf) const{
				inf->init(m_pFdp);
				m_pMesh->addInfluence(m_pFdp->getAOI(), inf->getWeights(), inf->getFapID());
			}
		};
	
		MeshManager* m_pMM;
	public:
		deformationInitializer_functor(MeshManager* pMM) : m_pMM(pMM){}
		void operator()(const FDPItem* fdp) const{
			const std::vector<IInfluenceCalculator*>& infCalcs =  fdp->getInfluenceCalculators();
			DeformableGeometry* pMesh = m_pMM->getMesh(fdp->getAffects());
			std::for_each(infCalcs.begin(), infCalcs.end(), influenceToSkin_functor(pMesh, fdp));
		}
	};

	std::string expressionFAPToAlias(int id);
	std::string visemeFAPToAlias(int id);
public:
	void initEyePivots(const Vector3& leftEye, const Vector3& rightEye);
	void initBindings(const std::map<std::string, std::string>& bindings);
	void initInfluenceCalculators(const std::list<FDPItem*>& items);
	void update(const boost::shared_ptr<XFace::IFapStream>& faps);
	//void update(const std::vector<float>& FAPs);
	FaceEntity(void);
	~FaceEntity(void);

};

} // namespace XFace

#endif // FACEENTITY_H_
