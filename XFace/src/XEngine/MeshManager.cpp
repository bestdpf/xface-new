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
 * The Original Code is Xface Core Library; Mesh Manager.
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

#include <XEngine/MeshManager.h>
#include <iostream>

namespace XEngine{

MeshManager* MeshManager::m_pThis = 0;
MeshManager::MeshManager(void)
{
}

MeshManager::~MeshManager(void)
{
	destroyAll();
}

/*!
	Singleton class accessor for the one and only instance of this class.
*/
MeshManager* MeshManager::getInstance()
{
	if (!m_pThis)
	{
		m_pThis = new MeshManager();
		DeformableGeometry::prepareFAPLookup();
	}
	return m_pThis;
}

/*!
	Releases the one and only instance.
*/
void MeshManager::releaseInstance()
{
	delete m_pThis;
	m_pThis = 0;
}

/*!
	Registers a mesh object to the storage.
*/
void MeshManager::registerMesh(DeformableGeometry* pMesh)
{
	removeMesh(pMesh->getName());
	m_Storage.insert(std::make_pair(pMesh->getName().c_str(), pMesh));
}

bool MeshManager::removeMesh(const std::string& name)
{
	MESHSTORAGE::iterator it = m_Storage.find(name.c_str());
	if(it == m_Storage.end())
		return false;
	
	delete it->second;
	m_Storage.erase(it);
	
	return true;
}
/*!
	Retrieves a mesh with the name passed. 
	\return 0 if unsucessful, a valid pointer to IMesh if successful.
*/
DeformableGeometry* const MeshManager::getMesh(const std::string& name) const
{
	MESHSTORAGE::const_iterator it = m_Storage.find(name.c_str());
	if(it == m_Storage.end())
		return 0;

	return it->second;
}

/*!
    This is a bit of a hack which returns the most recently 'read'
	filename prefix. I.e. it returns "alice" or "kenneth" based on the
	fact that the loaded models start with "alice_BlahBlah.wrl"
	Function added by Almer S. Tigelaar, patched on 17.06.2007
 */
std::string MeshManager::getPrefix() const
{
	std::string prefix;

	// Determine the model name prefix.
	// We either split on the first _ or . encountered in the filename
	// Downside of placing this here
	if (m_Storage.size() > 1) {
		std::string name = (++(m_Storage.rbegin()))->second->getName(); // Always take the last loaded model file as reference
		size_t pos1 = name.find_first_of("_");
		size_t pos2 = name.find_first_of(".");

		if (pos2 != std::string::npos && pos2 < pos1) {
			prefix = name.substr(0, pos2);
		} else if (pos1 != std::string::npos) {
			prefix = name.substr(0, pos1);
		}
	}
	std::cerr << "prefix: " << prefix << std::endl;

	return prefix;
}

/*!
	Destroys all of the meshes in the storage.
*/
void MeshManager::destroyAll()
{
	MESHSTORAGE::iterator it = m_Storage.begin();
	while (it != m_Storage.end())
	{
		delete it->second;
		++it;
	}
	m_Storage.clear();
}

}  // namespace XFace
