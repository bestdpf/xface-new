#include <XEngine/KeyframeInterpolator.h>

#include <list>
#include <cassert>

namespace XEngine {
const Entity& KeyframeInterpolator::interpolate(const Entity& fromEnt1, const Entity& fromEnt2, Entity& toEnt, float w)
{
	std::list<boost::shared_ptr<Drawable> > src1 = fromEnt1.getDrawables();
	std::list<boost::shared_ptr<Drawable> > src2 = fromEnt2.getDrawables();
	if(toEnt.getDrawableCount() != fromEnt1.getDrawableCount())
	{
		toEnt.release(true);
		toEnt.copyFrom(fromEnt1, true);
	}
	
	std::list<boost::shared_ptr<Drawable> > dst  = toEnt.getDrawables();
	std::list<boost::shared_ptr<Drawable> >::iterator it1 = src1.begin();
	std::list<boost::shared_ptr<Drawable> >::iterator it2 = src2.begin();
	std::list<boost::shared_ptr<Drawable> >::iterator it3 = dst.begin();
	while ( it1 != src1.end() ){
		assert( (it2 != src2.end()) && "Keyframe Morpher, src2 drawable count not matching src1's" );
		assert( (it3 != dst.end()) && "Keyframe Morpher, dst drawable count not matching src1's" );
	//	if(it3 == dst.end() )
	//		dst.push_back(Drawable(*it1));
		interpolateDrawables(*it1, *it2, *it3, w);
		++it1;++it2;++it3;
	}

	return toEnt;
}

void KeyframeInterpolator::interpolateDrawables(boost::shared_ptr<Drawable> pKey1, boost::shared_ptr<Drawable> pKey2, boost::shared_ptr<Drawable> pTarg, float w)
{
	MeshManager* pMM = MeshManager::getInstance();
	DeformableGeometry* pKeyMesh1 = pMM->getMesh(pKey1->getMeshName());
	DeformableGeometry* pKeyMesh2 = pMM->getMesh(pKey2->getMeshName());
	DeformableGeometry* pTargMesh = pMM->getMesh(pTarg->getMeshName());
//	std::cerr << pTarg->getMeshName() << std::endl;
	
	size_t vertCount = pKeyMesh1->getVertexCount();
	const std::vector<Vector3>& pVert1 = pKeyMesh1->getDeformedVerticesVector();
	const std::vector<Vector3>& pVert2 = pKeyMesh2->getDeformedVerticesVector();
	const std::vector<Vector3>& pNorm1 = pKeyMesh1->getNormals();
	const std::vector<Vector3>& pNorm2 = pKeyMesh2->getNormals();
	
	assert( (vertCount == pKeyMesh2->getVertexCount() ) && "vertex counts not matching in drawable morphing");
	
	std::vector<Vector3> pVertTarg, pNormTarg;
	std::fill_n(std::back_inserter(pVertTarg), vertCount, Vector3(0, 0, 0));
	std::fill_n(std::back_inserter(pNormTarg), vertCount, Vector3(0, 0, 0));
/*
	std::transform(pVert1.begin(), pVert1.end(), pVert2.begin(), pVertTarg.begin(), linearInt_functor(w));
	std::transform(pNorm1.begin(), pNorm1.end(), pNorm2.begin(), pNormTarg.begin(), linearInt_functor(w));
*/
	std::transform(pVert1.begin(), pVert1.end(), pVert2.begin(), pVertTarg.begin(), cubicInt_functor(w));
	std::transform(pNorm1.begin(), pNorm1.end(), pNorm2.begin(), pNormTarg.begin(), cubicInt_functor(w));

	pTargMesh->setDeformedVertices(pVertTarg);
	pTargMesh->setNormals(pNormTarg);
}
/*
Entity* KeyframeInterpolator::smooth(Entity* pEnt, float w)
{
	MorphController* controller = MorphController::getInstance();
	if(w < 1.0f)
	{
		Entity restEnt = controller->getDictionaryItem("Rest");
		
		if(pEnt->getDrawableCount() == 0)
			return pEnt;
	
		toInsert.copyFrom(controller->getDictionaryItem(pTrg->getAlias()), true);
		KeyframeInterpolator::interpolate(restEnt, pTrg->getEntity(), toInsert, w);
		pTrg->setEntity(toInsert);
	}
	
	return pEnt;
}
*/
} // namespace XEngine