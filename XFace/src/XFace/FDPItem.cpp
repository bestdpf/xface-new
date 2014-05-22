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
 * The Original Code is Xface Core Library; FDP.
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

#include <algorithm>
#include <XFace/FDPItem.h>
#include <XFace/IInfluenceCalculator.h>
#include <XFace/InfluenceCalculatorMaker.h>


namespace XFace{

void FDPItem::clearInfluenceCalculators()
{
	std::for_each(m_InfluenceCalculators.begin(), m_InfluenceCalculators.end(), clearInfluenceCalculator_functor());
}

/*!
	
*/
bool FDPItem::removeInfluenceCalculator(const std::string& type, float w, unsigned short fap)
{
	// seek the coresponding InfluenceCalculator in fdp item
	std::vector<IInfluenceCalculator*>::iterator it = m_InfluenceCalculators.begin();
	while(it != m_InfluenceCalculators.end())
	{
		if( ((*it)->getFapID() == (fap - 1)) // fap parameter is a value starting from 0, while fap id in FDPItem is stored as zero based...
			&& (fabs((*it)->getCoefficient() - w) < 0.00001 )
			&& ((*it)->getTypeName() == type))
		{
			delete *it;
			m_InfluenceCalculators.erase(it);	// and erase it!
			return true;
		}
		++it;
	}

	return false;
}

void FDPItem::modifyInfluenceCalculator(unsigned int order, const std::string& type, float w, unsigned short fap)
{
	// if it is existing, seek & destroy first
	if(order < m_InfluenceCalculators.size())
		delete m_InfluenceCalculators[order];
	
	IInfluenceCalculator* pInfluence = InfluenceCalculatorMaker::newInfluenceCalculator(type, w, fap);
	pInfluence->init(this);
	if(order < m_InfluenceCalculators.size())
		m_InfluenceCalculators[order] = pInfluence;
	else
		m_InfluenceCalculators.push_back(pInfluence);
}

} // namespace XFace