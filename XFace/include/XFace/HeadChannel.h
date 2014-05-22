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
#ifndef HEADCHANNEL_H__
#define HEADCHANNEL_H__

#include <XEngine/MorphChannel.h>
#include <XMath/Perlin.h>

namespace XFace{

class HeadChannel :
	public XEngine::MorphChannel
{
	struct Pointing
	{
		float x, y, z, w;
		unsigned int start, end;
		Pointing() : x(0), y(0), z(0), w(1.0f), start(0), end(0) {}
		Pointing(unsigned int _start, unsigned int _end, float _x, float _y, float _z, float _w = 1.0f) 
			: x(_x), y(_y), z(_z), w(_w), start(_start), end(_end) {}
		unsigned int getDuration() const {return end - start;}
	};

	XMath::Perlin *m_pPerlinX, *m_pPerlinY, *m_pPerlinZ;
protected:
	virtual bool updateResult(XEngine::Entity& result, const XEngine::Entity& rest);
	virtual bool updateSelf(unsigned int elapsed_time);
	void updateIdleHeadMovements(float inc);
	std::list<std::pair<unsigned int, unsigned int> > m_stills;
	typedef std::list<Pointing> PointingList;
	PointingList m_pointing;
	bool isStayStill();
	bool updatePointing(unsigned int elapsed_time);
public:
	void clear(); // inherited from parent
	void addPointing(unsigned int start, unsigned int end, float x, float y, float z, float w = 1.0f);
	void addStayStill(unsigned int start, unsigned int end);
	HeadChannel(const std::string& name);
	~HeadChannel();
};

} //namespace XFace
#endif 
