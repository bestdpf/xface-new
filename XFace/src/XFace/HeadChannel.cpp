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
#include <XFace/HeadChannel.h>
#include <XEngine/MorphController.h>

namespace XFace{

using namespace XEngine;

HeadChannel::HeadChannel(const std::string& name) 
	: MorphChannel(name), m_pPerlinX(0), m_pPerlinY(0), m_pPerlinZ(0)
{
	m_pPerlinX = new XMath::Perlin(7, 8, 0.5, 42);
	m_pPerlinY = new XMath::Perlin(7, 8, 0.5, 43);
	m_pPerlinZ = new XMath::Perlin(7, 8, 0.5, 44);
}

HeadChannel::~HeadChannel()
{
	delete m_pPerlinX;
	delete m_pPerlinY;
	delete m_pPerlinZ;
}

void HeadChannel::clear() 
{
	m_stills.clear();
	m_pointing.clear();
	MorphChannel::clear();
}

bool HeadChannel::updateResult(XEngine::Entity& result, const XEngine::Entity& rest)
{
	return true;
}

bool HeadChannel::updateSelf(unsigned int elapsed_time)
{
	MorphChannel::updateSelf(elapsed_time); // update base class for total time update, will return immediately afterwards
	if(!isStayStill() && !updatePointing(elapsed_time))
		updateIdleHeadMovements((float)elapsed_time);

	return true;
}

bool HeadChannel::updatePointing(unsigned int elapsed_time)
{
	// update pointing
	PointingList::const_iterator it = m_pointing.begin();
	while (it != m_pointing.end())
	{
		if(it->start < m_totalTimePassed && it->end > m_totalTimePassed)
		{
			// 1/4 of the time to rotate to the direction
			// 2/4 of the time to stay there
			// 1/4 of the time to rotate back to original position
			float dur4 = float(it->getDuration()) / 4.0f;
			
			// no rotation when in between durations below
			float inc = 0.0f;
			
			// rotating to target
			if(it->start + dur4 > m_totalTimePassed)
				inc = (float)elapsed_time / dur4;
			// going back to original position
			else if (it->end - dur4 < m_totalTimePassed)
				inc = -(float)elapsed_time / dur4;
			else	// stay there!
				return true;
			
			// consider also weight if any
			inc *= it->w;

			Transform tr = MorphController::getInstance()->getTransform();
			tr.rotate(Quaternion(inc*it->x, inc*it->y, inc*it->z));
			tr.updateLocalTransform();
			MorphController::getInstance()->setTransform(tr);
			
			return true;
		}
		++it;
	}

	// if no matching pointing found for the current time..
	return false;
}

void HeadChannel::updateIdleHeadMovements(float inc)
{
	static float r1 = 0, r2 = 0, r3 = 0;
	float rx, ry, rz;
	rx = m_pPerlinX->Get1D(r1);
	ry = m_pPerlinY->Get1D(r2);
	rz = m_pPerlinZ->Get1D(r3);

	// below code makes head movements fps dependent, removed on 12.10.2007
	inc /= 3000.0f;
	//inc = 0.0075;
	r1 += inc;
	r2 += inc;
	r3 += inc;

	Transform tr = MorphController::getInstance()->getTransform();
	tr.rotate(Quaternion (rx, ry, rz));
	tr.updateLocalTransform();
	MorphController::getInstance()->setTransform(tr);
}

void HeadChannel::addStayStill(unsigned int start, unsigned int end)
{
	m_stills.push_back(std::make_pair(start, end));
}

bool HeadChannel::isStayStill()
{
	// could be optimized, no need to traverse all the elements, it's fast anyhow..
	std::list<std::pair<unsigned int, unsigned int> >::const_iterator it = m_stills.begin();
	while (it != m_stills.end())
	{
		if(it->first < m_totalTimePassed && it->second > m_totalTimePassed)
			return true;
		++it;
	}
	return false;
}

void HeadChannel::addPointing(unsigned int start, unsigned int end, float x, float y, float z, float w)
{
	HeadChannel::Pointing p(start, end, x, y, z, w);
	m_pointing.push_back(p);

}
} // namespace XFace
