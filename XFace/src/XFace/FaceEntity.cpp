#include <algorithm>

#include <XFace/FaceEntity.h>
#include <XFace/RaisedCosInfluence.h>
#include <XEngine/MorphController.h>
#include <XEngine/Entity.h>
#include <XEngine/KeyframeInterpolator.h>



using namespace XEngine;

namespace XFace{

FaceEntity::FaceEntity(void)
{
}

FaceEntity::~FaceEntity(void)
{
}

void FaceEntity::initInfluenceCalculators(const std::list<FDPItem*>& items)
{
	// we have to make this function call in order to make static variables initialized in influence site
	RaisedCosInfluenceSph def(0, 5);

	// clean the old (if any) drawable contents (bones)
	std::for_each(m_drawables.begin(), m_drawables.end(), skinResetter_functor(MeshManager::getInstance()));

	// calls another for_each inside for initializing weights in influence calc. and transferring them to skinned meshes
	std::for_each(items.begin(), items.end(), deformationInitializer_functor(MeshManager::getInstance()));
}

void FaceEntity::initBindings(const std::map<std::string, std::string>& bindings)
{
	std::list<boost::shared_ptr<Drawable> >::iterator it_draw = m_drawables.begin();
	while (it_draw != m_drawables.end())
	{
		std::map<std::string, std::string>::const_iterator it_found = 
				bindings.find((*it_draw)->getMeshName());
		if(it_found != bindings.end())
			(*it_draw)->setBinding(it_found->second);

		++it_draw;
	}
}

void FaceEntity::initEyePivots(const Vector3& leftEye, const Vector3& rightEye)
{
	std::list<boost::shared_ptr<Drawable> >::iterator it_draw = m_drawables.begin();
	while (it_draw != m_drawables.end())
	{
		if((*it_draw)->getBinding() == "LeftEye")
			(*it_draw)->setPivot(leftEye);	
		else if((*it_draw)->getBinding() == "RightEye")
			(*it_draw)->setPivot(rightEye);	
		++it_draw;
	}
}

void FaceEntity::update(const boost::shared_ptr<XFace::IFapStream>& faps)
{
	bool highLevel = false;
	const std::vector<float>& FAPs = faps->getCurrentFAP();
	MorphController* morpher = MorphController::getInstance();

	if(FAPs[0]) // we have a viseme
	{
		highLevel = true;
		XFace::IFapStream::VisemeStruct vis = faps->getCurrentViseme();
		// do the interpolation
		// final viseme = (viseme 1) * (viseme_blend / 63) + (viseme 2) * (1 - viseme_blend / 63)
		float weight = 0.5f*(1.0f - (vis.viseme_blend / 63.0f));
		Entity v1 = morpher->getDictionaryItem(visemeFAPToAlias(vis.viseme_select1));
		Entity v2 = morpher->getDictionaryItem(visemeFAPToAlias(vis.viseme_select2));
		
		if(v1.getDrawableCount() && v2.getDrawableCount())
			KeyframeInterpolator::interpolate(v1, v2, *this, weight);
		else
		{
			if(!v1.getDrawableCount())
				std::cerr << "Cannot find viseme " << vis.viseme_select1 << " named " << visemeFAPToAlias(vis.viseme_select1) << std::endl;
			if(!v2.getDrawableCount())
				std::cerr << "Cannot find viseme " << vis.viseme_select2 << " named " << visemeFAPToAlias(vis.viseme_select2) << std::endl;
		}
	}

	if(FAPs[1]) // we have an expression
	{
		highLevel = true;
		XFace::IFapStream::ExpressionStruct exp = faps->getCurrentExpression();
		// do the interpolation
		// final expression = expression1 * (expression_intensity1 / 63)+ expression2 * (expression_intensity2 / 63)
	/*	// test data
			exp.expression_select1 = 3;
			exp.expression_intensity1 = 31;
			exp.expression_select2 = 5;
			exp.expression_intensity2 = 31;
	*/	// test data end
		Entity rest = morpher->getDictionaryItem("Rest");
		std::string exp1 = expressionFAPToAlias(exp.expression_select1);
		std::string exp2 = expressionFAPToAlias(exp.expression_select2);
				
		if(exp1 != "Rest") // no need to use rest frame
		{
			Entity v1 = morpher->getDictionaryItem(exp1);
			
			if(v1.getDrawableCount() && exp.expression_intensity1)
			{
				Entity result1;
				result1.copyFrom(rest, true);

				// smooth
				KeyframeInterpolator::interpolate(rest, v1, result1, exp.expression_intensity1 / 63.0f);
				
				// blend
				morpher->getBlender()->blend(*this, result1);
			}
			else if(v1.getDrawableCount() == 0)
				std::cerr << expressionFAPToAlias(exp.expression_select1) << " has no drawables" << std::endl;
		}
		if(exp2 != "Rest") // no need to use rest frame
		{
			Entity v2 = morpher->getDictionaryItem(exp2);
			if(v2.getDrawableCount() && exp.expression_intensity2)
			{
				Entity result2;
				result2.copyFrom(rest, true);
				
				// smooth
				KeyframeInterpolator::interpolate(rest, v2, result2, exp.expression_intensity2 / 63.0f);
				
				// blend
				morpher->getBlender()->blend(*this, result2);
			}
			else if(v2.getDrawableCount() == 0)
				std::cerr << expressionFAPToAlias(exp.expression_select2) << " has no drawables" << std::endl;
		}
	}
	
	if(!highLevel)
		std::for_each(m_drawables.begin(), m_drawables.end(), skinDeformer_functor(MeshManager::getInstance(), faps));

	m_transform.setRotation(Quaternion(-FAPs[48]*100.001f, FAPs[49]*100.001f, -FAPs[47]*100.001f));
///	m_transform.rotate(Quaternion(FAPs[48]*3.0f, FAPs[49]*3.0f, FAPs[47]*3.0f));
	m_transform.updateLocalTransform();
}

std::string FaceEntity::expressionFAPToAlias(int id)
{
	switch(id) 
	{
		case 1: return "SmileClosed";
		case 2: return "Sad";
		case 3: return "Anger";
		case 4: return "Fear";
		case 5: return "Disgust";
		case 6: return "Surprise";
		default: return "Rest";
	}
}

std::string FaceEntity::visemeFAPToAlias(int id)
{
	switch(id) 
	{
		case 0: return "Rest";
		case 1: return "b";
		case 2: return "f";
		case 3: return "th";
		case 4: return "d";
		case 5: return "k";
		case 6: return "ch";
		case 7: return "d";
		case 8: return "n";
		case 9: return "r";
		case 10: return "aah";
		case 11: return "eh";
		case 12: return "i";
		case 13: return "oh";
		case 14: return "q";
		default: return "Rest";
	}
}

} // namespace XFace