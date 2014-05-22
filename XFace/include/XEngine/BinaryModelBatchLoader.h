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

#pragma once
#ifndef BINARYMODELBATCHLOADER_H_
#define BINARYMODELBATCHLOADER_H_
#include "IModelLoader.h"
#include <list>
#include <map>
#include <boost/shared_ptr.hpp>

namespace XEngine{

class BinaryModelBatchLoader :
	public IModelLoader
{
	bool m_bLoaded;
	typedef std::list<boost::shared_ptr<Drawable> > DRAWABLES;
	std::list< std::pair< std::string, DRAWABLES> > m_data;
public:
	bool isLoaded() const {return m_bLoaded;}
	bool init(const std::string& filename, const std::string& path);
	BinaryModelBatchLoader(void);
	~BinaryModelBatchLoader(void);
	std::list<boost::shared_ptr<Drawable> > loadModel(const std::string &filename, const std::string& dir="./");
};

} // namespace XEngine

#endif // BINARYMODELBATCHLOADER_H_