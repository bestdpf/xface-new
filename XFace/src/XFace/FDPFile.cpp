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
 * The Original Code is Xface Core Library; FDP File Loader.
 *
 * The Initial Developer of the Original Code is
 * ITC-irst, TCC Division (http://tcc.itc.it) Trento / ITALY.
 * For info, contact: xface-info@itc.it or http://xface.itc.it
 * Portions created by the Initial Developer are Copyright (C) 2004
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * - Koray Balci (koraybalci@yahoo.com)
 * ***** END LICENSE BLOCK ***** */

#include <XFace/FDPFile.h>
#include <XFace/FDPItem.h>
#include <XFace/IInfluenceCalculator.h>
#include <XFace/InfluenceCalculatorMaker.h>

#ifdef _DEBUG
#pragma message("		Adding TinyXML STL lib (Debug)...")
#pragma message("		Note: TinyXMLSTL should be compiled externally using /MDd compiler switch...")
#pragma comment(lib, "tinyxmld_STL")
#else
#pragma message("		Adding TinyXML STL lib...")
#pragma message("		Note: TinyXMLSTL should be compiled externally using /MD compiler switch...")
#pragma comment(lib, "tinyxml_STL")
#endif

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>
#ifdef TIXML_USE_STL
	#include <sstream>
#else
	#include <stdio.h>
#endif
#include <math.h> // for atof
#include <strstream>

using namespace XMath;
namespace XFace{

// class constructor
FDPFile::FDPFile()
{
}

bool FDPFile::save(const std::string& filename, boost::shared_ptr<FDP> pFDP)
{
	TiXmlDocument doc;
	TiXmlDeclaration decl("1.0", "utf-8", "yes");
	doc.InsertEndChild(decl);

	TiXmlComment comment;
	comment.SetValue("XFaceEd generated MPEG-4 FDP specification file");
	doc.InsertEndChild(comment);
	TiXmlElement x_xfdp("xfdp");	// main body
		comment.SetValue("Header information for the model");
		x_xfdp.InsertEndChild(comment);
	
		TiXmlElement x_head("head");	// header information
			comment.SetValue("Version of the FDP specification file (this file)");
			x_head.InsertEndChild(comment);
	
			TiXmlElement x_file("file");	// file element
			std::strstream fileversion;
			fileversion << getVersion() << '\0';
			x_file.SetAttribute("version", fileversion.str());
			x_head.InsertEndChild(x_file);

			comment.SetValue("3D model (mesh) info");
			x_head.InsertEndChild(comment);
			TiXmlElement x_model("model");	// model element
//			x_model.SetAttribute("filename", pFDP->getModelFilename().c_str());
			x_model.SetAttribute("fdpcount", pFDP->getItemCount());
			x_head.InsertEndChild(x_model);

			comment.SetValue("FAPU (Facial Animation Parameter Units) for the model");
			x_head.InsertEndChild(comment);
			TiXmlElement x_fapu("fapu");	// fapu element
			
			std::strstream es, irisd, ens, mns, mw;
			es		<< pFDP->getES()*1024		<< '\0';
			irisd	<< pFDP->getIRISD()*1024	<< '\0';
			ens		<< pFDP->getENS()*1024		<< '\0';
			mns		<< pFDP->getMNS()*1024		<< '\0';
			mw		<< pFDP->getMW()*1024		<< '\0';

			x_fapu.SetAttribute("ES0",		es.str());	// es0
			x_fapu.SetAttribute("IRISD0",	irisd.str());	// irisd0
			x_fapu.SetAttribute("ENS0",		ens.str());	// ens0
			x_fapu.SetAttribute("MNS0",		mns.str());	// mns0
			x_fapu.SetAttribute("MW0",		mw.str());	// mw0			
			x_head.InsertEndChild(x_fapu);

			// Global translation
			comment.SetValue("Global Translation info for the whole face");
			x_head.InsertEndChild(comment);
			TiXmlElement x_trans("translation");		// translation element
			Vector3 trans = pFDP->getGlobalTranslation();
			std::strstream transx, transy, transz;
			transx << trans.x << '\0';
			transy << trans.y << '\0';
			transz << trans.z << '\0';
			x_trans.SetAttribute("x", transx.str());	// translation in x
			x_trans.SetAttribute("y", transy.str());	// translation in y
			x_trans.SetAttribute("z", transz.str());	// translation in z
			x_head.InsertEndChild(x_trans);

			// Global rotation
			comment.SetValue("Global Rotation info for the whole face");
			x_head.InsertEndChild(comment);
			TiXmlElement x_rot("rotation");					// rotation element
			Vector3 rot = pFDP->getGlobalRotationAxis();
			std::strstream rotx, roty, rotz, rotang;
			rotx << rot.x << '\0';
			roty << rot.y << '\0';
			rotz << rot.z << '\0';
			rotang << pFDP->getGlobalRotationAngle() << '\0';
			x_rot.SetAttribute("axis_x", rotx.str());		// rotation axis x
			x_rot.SetAttribute("axis_y", roty.str());		// rotation axis y
			x_rot.SetAttribute("axis_z", rotz.str());		// rotation axis z
			x_rot.SetAttribute("axis_angle", rotang.str());	// rotation angle
			x_head.InsertEndChild(x_rot);

	x_xfdp.InsertEndChild(x_head);

	// mesh and texture binds
	saveSource(x_xfdp);

	// now the fdp's...
	comment.SetValue("FDP points and required information for each of them starts here");
	x_xfdp.InsertEndChild(comment);
	
	const FDPItem* pItem;
	const FDP::FDPITEMS items = pFDP->getItems();
	FDP::FDPITEMS::const_iterator it = items.begin();
	while (it != items.end())
	{
		pItem = *it;
		++it;
		TiXmlElement x_item("fdp");
		x_item.SetAttribute("name", pItem->getName().c_str());
		x_item.SetAttribute("index", pItem->getIndex());
		x_item.SetAttribute("affects", pItem->getAffects().c_str());
			// Now the indices of AOI
			TiXmlElement x_indices("indices");
			std::strstream indices;
			const FDPItem::AOI_STORE aoi_data = pItem->getAOI();
			if(aoi_data.empty())
				continue;
			FDPItem::AOI_STORE::const_iterator it2 = aoi_data.begin();
			while (it2 != aoi_data.end())
			{
				indices << *it2 << " ";
				++it2;
			}
			indices << '\0';
			
			TiXmlText x_aoi(indices.str());
			x_indices.InsertEndChild(x_aoi);

			x_item.InsertEndChild(x_indices);

		
			const std::vector<IInfluenceCalculator*>& influence = pItem->getInfluenceCalculators();
			std::vector<IInfluenceCalculator*>::const_iterator it = influence.begin();
			while(it != influence.end())
			{
				TiXmlElement x_influence("influence");
				std::strstream weight, fapID;
					// weight
					weight << (*it)->getCoefficient() << '\0';
					x_influence.SetAttribute("weight",  weight.str());
					
					// fap ID
					fapID << (*it)->getFapID() << '\0';
					x_influence.SetAttribute("fap", fapID.str());

					// type
					x_influence.SetAttribute("type", (*it)->getTypeName());
				x_item.InsertEndChild(x_influence);
				++it;
			}

		x_xfdp.InsertEndChild(x_item);
	}
	doc.InsertEndChild(x_xfdp);

	doc.SaveFile(filename.c_str());
	return true;
}

/*!
*/
void FDPFile::saveSource(TiXmlElement& x_xfdp)
{
	TiXmlComment comment;
	TiXmlElement x_source("source");	// source information

		// add a description comment
		comment.SetValue("3D model (mesh) info");
		x_source.InsertEndChild(comment);		

		//saveEntity(x_source, m_faceEntity, "Rest", true);

		// saving morph targets
		std::map<std::string, Entity>::const_iterator it_morph = m_morphTargets.begin();
		while (it_morph != m_morphTargets.end())
		{
			saveEntity(x_source, it_morph->second, it_morph->first, it_morph->first == "Rest");
			++it_morph;
		}

	x_xfdp.InsertEndChild(x_source);
}

void FDPFile::saveEntity(TiXmlElement &parent, const Entity& ent, const std::string& alias, bool bindings)
{
	std::set<const MeshInfo*> targetSet;
	
	// extract file names for each Entity object
	targetSet.clear();
	TiXmlElement x_morph("entity");	// binding element
	x_morph.SetAttribute("alias", alias);
	const std::list<boost::shared_ptr<Drawable> >& drawables = ent.getDrawables();
	std::list<boost::shared_ptr<Drawable> >::const_iterator it_morph_dr = drawables.begin();
	while (it_morph_dr != drawables.end())
	{
		const MeshInfo* pMeshInfo = ModelFileFactory::getMeshInfo((*it_morph_dr)->getMeshName());
		if(pMeshInfo)
			targetSet.insert(pMeshInfo);
		
		++it_morph_dr;
	}

	// then write the file set to fdp
	std::set<const MeshInfo*>::iterator it_targetSet = targetSet.begin();
	while (it_targetSet != targetSet.end())
	{
		TiXmlElement x_file("mesh");	// mesh element
		x_file.SetAttribute("file", (*it_targetSet)->file.c_str());
		x_file.SetAttribute("format", (*it_targetSet)->format.c_str());
		x_morph.InsertEndChild(x_file);
		++it_targetSet;
	}

	if(bindings)
	{
		// saving bindings
		std::map<std::string, std::string>::const_iterator it_bind = m_bindings.begin();
		while (it_bind != m_bindings.end())
		{
			TiXmlElement x_bind("bind");	// binding element
			if(it_bind->second != "None")
			{
				x_bind.SetAttribute("submesh", it_bind->first);
				x_bind.SetAttribute("item", it_bind->second);
				x_morph.InsertEndChild(x_bind);
			}
			++it_bind;
		}
	}

	parent.InsertEndChild(x_morph);
}

bool FDPFile::load(std::istream& input, boost::shared_ptr<FDP> pFDP)
{
	TiXmlDocument doc;
	std::string xmlString;
	// get the input stream contents to a string
	while (!input.eof())
	{
		std::string dummy;
		input >> dummy;
		xmlString.append(dummy);
	}

	// parse the xml string
	if(!doc.Parse(xmlString.c_str()))
		return false;

	return load(doc, pFDP);
}

bool FDPFile::load(const std::string& filename, boost::shared_ptr<FDP> pFDP)
{
	TiXmlDocument doc( filename.c_str() );
	if(!doc.LoadFile())
		return false;
	return load(doc, pFDP);
}

bool FDPFile::load(TiXmlDocument& doc, boost::shared_ptr<FDP> pFDP)
{
//	debug.open("fdp_debug.txt");
//	debug << "starting debug of fdp load" << std::endl;
	m_pFDP = pFDP;
	
	// get to the body
	TiXmlNode* pNode = 0;
	pNode = doc.FirstChild( "xfdp" );
	assert( pNode );
	
	// and move in to the header
	pNode = pNode->FirstChild( "head" );
	assert( pNode );

	// load header
	if(!loadHeader(pNode))
		return false;

//	debug << "loadHeader success!" << std::endl;

	// and move in to the source
	pNode = pNode->NextSibling( "source" );
	assert( pNode );
	if(!loadSource(pNode))
		return false;

//	debug << "loadSource success!" << std::endl;
	
	// make sure we skip everything and move to first FDP item in file
	pNode = pNode->NextSibling("fdp");
	while (pNode)
	{
		// load the fdp items
		loadFDPItem(pNode);
		pNode = pNode->NextSibling("fdp");
	}

//	debug.close();
	return true;
}

bool FDPFile::loadSource(const TiXmlNode* pNode)
{
	assert(pNode);	// safety check
	m_bindings.clear();
	m_morphTargetsMeshInfos.clear();
	m_faceEntityMeshInfo.clear();
	
	TiXmlNode* pChild = pNode->FirstChild( "entity" );
	while (pChild)
	{
		TiXmlNode* pGrandChild = pChild->FirstChild();
		TiXmlElement* pEntity = pChild->ToElement();
		
		while(pEntity && pGrandChild)
		{
			if(std::string(pGrandChild->Value()) == "mesh")
			{
				MeshInfo info;
				TiXmlElement* fileElement = 0;
 				fileElement = pGrandChild->ToElement();
				assert(fileElement);
				const char* filename = fileElement->Attribute("file");
				info.file.assign(filename);
				const char* fmt = fileElement->Attribute("format");
				info.format.assign(fmt);
				const char* path = fileElement->Attribute("path");
				if(!path)
					info.path.assign("");
				else
					info.path.assign(path);
			
//				debug << info.file  << " " << info.path << std::endl;

				///!!!!!!!!!!
				std::string alias = pEntity->Attribute("alias");
				if(alias == "Rest")
					m_faceEntityMeshInfo.push_back(info);
				// push the Rest state into the morph target dictionary as well
				m_morphTargetsMeshInfos[alias].push_back(info);
			}
			else if(std::string(pGrandChild->Value()) == "bind")
			{
				TiXmlElement* bindElement = pGrandChild->ToElement();
 				const char* submesh = bindElement->Attribute("submesh");
				const char* item = bindElement->Attribute("item");
				m_bindings.insert(std::make_pair(submesh, item));		
			}

			// traversing mesh/bind in an entity entry
			pGrandChild = pGrandChild->NextSibling();
		}
	
		// traversing entities
		pChild = pChild->NextSibling();
	}

	return true;
}

bool FDPFile::loadHeader(const TiXmlNode* pNode)
{
	assert(pNode);	// safety check
		
	TiXmlNode* pChild = 0;
	pChild = pNode->FirstChild( "file" );
	if(pChild)
	{
		TiXmlElement* fileElement = 0;
 		fileElement = pChild->ToElement();
		assert(fileElement);
		const char* version = fileElement->Attribute("version");
		if(std::string(version) != getVersion())
		{
//			debug << "FDP file is in old format!: << version << std::endl;
			return false;
		}
	}
	
	pChild = pNode->FirstChild( "model" );
	if(pChild)
	{
		TiXmlElement* pModelEl = 0;
		pModelEl = pChild->ToElement();
		assert(pModelEl);
		// NOT PROCESSING FDP COUNT YET
		//	std::cout << "Number of FDP is: "<< modelElement->Attribute("fdpcount") << std::endl;
	}

	pChild = pNode->FirstChild( "fapu" );
	if(pChild)
	{
		TiXmlElement* pFAPU = 0;
		pFAPU = pChild->ToElement();
		assert(pFAPU);
		float val = 0;
		const char* att = pFAPU->Attribute("ES0");
		if (att)
		{
			val = (float)atof(att);
			m_pFDP->setES0(val);
		}

		att = pFAPU->Attribute("IRISD0");
		if (att)
		{
			val = (float)atof(att);
			m_pFDP->setIRISD0(val);
		}
		att = pFAPU->Attribute("ENS0");
		if (att)
		{
			val = (float)atof(att);
			m_pFDP->setENS0(val);
		}
		att = pFAPU->Attribute("MNS0");
		if(att)
		{
			val = (float)atof(att);
			m_pFDP->setMNS0(val);
		}
		att = pFAPU->Attribute("MW0");
		if(att)
		{
			val = (float)atof(att);
			m_pFDP->setMW0(val);
		}
	}

	// load translation
	pChild = pNode->FirstChild( "translation" );
	if(pChild)
	{
		TiXmlElement* pTrans = 0;
		pTrans = pChild->ToElement();
		Vector3 trans(0, 0, 0);
		const char* att = pTrans->Attribute("x");
		if (att)
			trans.x = (float)atof(att);
		att = pTrans->Attribute("y");
		if (att)
			trans.y = (float)atof(att);
		att = pTrans->Attribute("z");
		if (att)
			trans.z = (float)atof(att);

		m_pFDP->setGlobalTranslation(trans);
	}
	
	// load global rotation
	pChild = pNode->FirstChild( "rotation" );
	if(pChild)
	{
		TiXmlElement* pRot = 0;
		pRot = pChild->ToElement();
		Vector3 rot(0, 0, 0);
		float ang = 0;
		const char* att = pRot->Attribute("axis_x");
		if (att)
			rot.x = (float)atof(att);
		att = pRot->Attribute("axis_y");
		if (att)
			rot.y = (float)atof(att);
		att = pRot->Attribute("axis_z");
		if (att)
			rot.z = (float)atof(att);
		att = pRot->Attribute("axis_angle");
		if (att)
			ang = (float)atof(att);

		m_pFDP->setGlobalRotation(AxisAngle(rot, ang));
	}

	return true;
}
void FDPFile::loadFDPItem(const TiXmlNode* pNode)
{
	assert(pNode);	// safety check
	TiXmlElement* pFDP = 0;
	pFDP = pNode->ToElement();
	assert(pFDP);
	if(strcmp("fdp", pFDP->Value()) != 0)
		return;

	// process the attributes of this FDP
	std::string name(pFDP->Attribute("name"));
	// create a new item (will be deleted in dtor of FDP class)
	FDPItem* pItem = new FDPItem(name);
	// get the index
	int index = 0;
	pFDP->Attribute("index", &index);
	pItem->setControlPoint((unsigned short)index);

	// get the affected mesh
	const char* att = pFDP->Attribute("affects");
	if (att)
	{
		std::string aff(att);
		pItem->setAffects(aff);
	}

	// get the indices	
	TiXmlNode* pChild = 0;
	pChild = pNode->FirstChild( "indices" );
	if(pChild)
	{
		TiXmlText* pText = pChild->FirstChild()->ToText();
		if(pText) // no indices inside
		{
			std::string indices(pText->Value());
			std::string delim(" ");
		
			size_t n = indices.length();
			size_t start, stop;
	 
			start = indices.find_first_not_of(delim);
			while ((start >= 0) && (start < n)) 
			{
				stop = indices.find_first_of(delim, start);
				if ((stop < 0) || (stop > n)) 
					stop = n;
				
				pItem->addAOIIndex((unsigned short)atoi(indices.substr(start, stop - start).c_str()));
				start = indices.find_first_not_of(delim, stop+1);
			}
		}
	}

	// load the influences
	pChild = 0;
	while( (pChild = pNode->IterateChildren("influence", pChild)) != 0 )
	{
		TiXmlElement* pDefItem = pChild->ToElement();
		if (pDefItem)
		{
			double w = 0.0;
			pDefItem->QueryDoubleAttribute("weight", &w);
			att = pDefItem->Attribute("weight");
//			if(att)
//				w = atof(att);
//			debug << att << "/" << w << " ";

			unsigned short fapID = 0;
			att = pDefItem->Attribute("fap");
			if(att)
				fapID = (unsigned short)atoi(att);
			
			att = pDefItem->Attribute("type");
			if(att)
			{
				IInfluenceCalculator* pInfluence = InfluenceCalculatorMaker::newInfluenceCalculator(att, (float)w, fapID);
				if(pInfluence)
					pItem->addInfluenceCalculator(pInfluence);
			}
		}
	}
//	debug << std::endl;	
	
	m_pFDP->insertItem(pItem);
}

}  // namespace XFace