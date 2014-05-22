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
 * Portions creat
 ed by the Initial Developer are Copyright (C) 2004 - 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@gmail.com)
 * ***** END LICENSE BLOCK ***** */

/*! 
	\file	KeyframeInterpolator.h
	\brief	Keyframe interpolation done here.
*/

#pragma once
#ifndef KEYFRAMEINTERPOLATOR_H_
#define KEYFRAMEINTERPOLATOR_H_

#include <boost/shared_ptr.hpp>
#include <algorithm>

#include "Entity.h"

namespace XEngine{

/*!
	\brief Keyframe interpolator.
*/
class KeyframeInterpolator
{
	/*!
		Linear interpolation
		k(u) = k(0)×(1-u) + k(1)×(u)
		traditionally written with a single multiplication
		k(u) = k(0) + u×(k(1) - k(0))
	*/
	struct linearInt_functor : public std::binary_function<Vector3,Vector3,Vector3> 
	{
		linearInt_functor(float w_) : w(w_){}
		Vector3 operator()(const Vector3& in1, const Vector3& in2)
		{
			return in1 + (in2 - in1)*w;
		}
		private:
		float w;
	};
	/*!
		Cubic interpolation
		k(u) = k(0)×(2u3-3u2+1) + k(1)×(3u2-2u3)
	*/
	struct cubicInt_functor : public std::binary_function<Vector3,Vector3,Vector3> 
	{
		cubicInt_functor(float w) : w_2cube(2*w*w*w), w_3sqr(3*w*w){}
		Vector3 operator()(const Vector3& v1, const Vector3& v2)
		{
			return v1*(w_2cube - w_3sqr + 1) + v2*(w_3sqr - w_2cube);
		}
		private:
		float w_2cube;
		float w_3sqr;
	};

	static void interpolateDrawables(boost::shared_ptr<Drawable> pKey1, boost::shared_ptr<Drawable> pKey2, boost::shared_ptr<Drawable> pTarg, float w);
public:
//	Entity& scale(const Entity& src, float w, Entity& target) const;
//	static Entity* smooth(Entity* pEnt, float w);
	static const Entity& interpolate(const Entity& fromEnt1, const Entity& fromEnt2, Entity& toEnt, float w);
	
};


} // namespace XEngine
#endif // KEYFRAMEINTERPOLATOR_H_