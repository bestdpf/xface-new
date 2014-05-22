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

#include <XFace/FDPWriter.h>
#include <XEngine/ModelFileFactory.h>	// fpr MeshInfo class
#include <XEngine/Drawable.h>	// fpr MeshInfo class
#include <XFace/XercesString.h>
#include <XFace/IInfluenceCalculator.h>
#include <XFace/InfluenceCalculatorMaker.h>

#define X XercesString
#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

#include <strstream>
#include <map>
#include <iostream>

#ifdef _DEBUG
#pragma message("		Adding Xerces lib (Debug)...")
#pragma comment(lib, "xerces-c_2D")
#else
#pragma message("		Adding Xerces lib (Release)...")
#pragma comment(lib, "xerces-c_2")
#endif

using XEngine::MeshInfo;
using XEngine::Entity;
using XEngine::Drawable;
using XEngine::ModelFileFactory;

namespace XFace{

FDPWriter::FDPWriter(void)
{
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		// Do your failure processing here
		std::cerr << "FDPWriter: Cannot start Xerces parser!" << XercesString(toCatch.getMessage()) << std::endl;
	}
}

FDPWriter::~FDPWriter(void)
{
	XMLPlatformUtils::Terminate();
}

bool FDPWriter::save(const std::string& filename, boost::shared_ptr<FDP> pFDP)
{
	DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(XercesString("LS"));
	bool retVal = false;
    if (impl != NULL)
    {
        try
        {
			m_pFDP = pFDP;

            XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* doc = impl->createDocument(
                        0,              // root element namespace URI.
                        X("xfdp"),		// root element name
                        0);             // document type object (DTD).

            DOMElement* rootElem = doc->getDocumentElement();

			DOMComment* pCom1 = doc->createComment(X("XFaceEd generated MPEG-4 FDP specification file"));
            rootElem->appendChild(pCom1);

			DOMComment* pCom2 = doc->createComment(X("Header information for the model"));
			rootElem->appendChild(pCom2);
			rootElem->appendChild(saveHead(doc));

			DOMComment* pCom3 = doc->createComment(X("3D model files (mesh) configuration"));
			rootElem->appendChild(pCom3);
			rootElem->appendChild(saveSource(doc));

			const FDP::FDPITEMS items = m_pFDP->getItems();
			FDP::FDPITEMS::const_iterator it = items.begin();
			while (it != items.end())
			{
				rootElem->appendChild(saveFDPItem(doc, *it));
				++it;
			}

			writeToFile(doc, filename);
            doc->release();
			retVal = true;
        }
        catch (const OutOfMemoryException& o)
        {
			std::cerr << "FDPWriter::save: OutOfMemoryException" << std::endl;
			retVal = false;
        }
        catch (const DOMException& e)
        {
			std::cerr << "FDPWriter::save: DOMException code is:  " << e.code << std::endl;
			retVal = false;
        }
        catch (...)
        {
			std::cerr << "FDPWriter::save: An error occurred creating the document" << std::endl;
			retVal = false;
        }
    }  // (inpl != NULL)
    else
    {
		std::cerr << "FDPWriter::save: Requested Xerces implementation is not supported" << std::endl;
		retVal = false;
    }

	return retVal;
}

DOMNode* FDPWriter::saveFDPItem(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* doc, const FDPItem* item)
{
	DOMElement*  fdpElem = doc->createElement(X("fdp"));
	
	fdpElem->setAttribute(X("name"), X(item->getName().c_str()));
	char buf[65];
	itoa(item->getIndex(), buf, 10);
	fdpElem->setAttribute(X("index"), X(buf));
	fdpElem->setAttribute(X("affects"), X(item->getAffects().c_str()));

	// Now the indices of AOI
	const FDPItem::AOI_STORE aoi_data = item->getAOI();
	if(!aoi_data.empty())
	{
		DOMElement*  indElem = doc->createElement(X("indices"));
		std::strstream indices;
		FDPItem::AOI_STORE::const_iterator it2 = aoi_data.begin();
		while (it2 != aoi_data.end())
		{
			indices << *it2 << " ";
			++it2;
		}
		indices << '\0';
	
		// to XML
		DOMText* indDataVal = doc->createTextNode(X(indices.str()));
		indElem->appendChild(indDataVal);
		fdpElem->appendChild(indElem);
	}

	// now the influences
	const std::vector<IInfluenceCalculator*>& influence = item->getInfluenceCalculators();
	std::vector<IInfluenceCalculator*>::const_iterator it = influence.begin();
	while(it != influence.end())
	{
		DOMElement*  infElem = doc->createElement(X("influence"));
		std::strstream weight, fapID;
			// weight
			weight << (*it)->getCoefficient() << '\0';
			infElem->setAttribute(X("weight"), X(weight.str()));
			
			// fap ID
			fapID << (*it)->getFapID() << '\0';
			infElem->setAttribute(X("fap"), X(fapID.str()));
			
			// type
			infElem->setAttribute(X("type"), X((*it)->getTypeName().c_str()));
		fdpElem->appendChild(infElem);
		++it;
	}

	return fdpElem;
}

DOMNode* FDPWriter::saveEntity(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* doc, const Entity& ent, const std::string& alias, const std::string& category, bool bindings)
{
	std::set<const MeshInfo*> targetSet;
	
	// extract file names for each Entity object, an Entity may be composed of multiple mesh files
	DOMElement*  entElem = doc->createElement(X("entity"));
	entElem->setAttribute(X("alias"), X(alias.c_str()));
	entElem->setAttribute(X("category"), X(category.c_str()));
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
		DOMElement*  meshElem = doc->createElement(X("mesh"));
		meshElem->setAttribute(X("file"), X((*it_targetSet)->file.c_str()));
		meshElem->setAttribute(X("format"), X((*it_targetSet)->format.c_str()));
		entElem->appendChild(meshElem);
		++it_targetSet;
	}

	if(bindings)
	{
		// saving bindings
		std::map<std::string, std::string>::const_iterator it_bind = m_bindings.begin();
		while (it_bind != m_bindings.end())
		{
			// binding element
			DOMElement*  bindElem = doc->createElement(X("bind"));
			if(it_bind->second != "None")
			{
				bindElem->setAttribute(X("submesh"), X(it_bind->first.c_str()));
				bindElem->setAttribute(X("item"), X(it_bind->second.c_str()));
				if(it_bind->second == "LeftEye" || it_bind->second == "RightEye") // eye pivots
				{
					std::strstream x, y, z;
					Vector3 eye;
					if(it_bind->second == "LeftEye")
						eye = m_pFDP->getLeftEyePivot();
					else
						eye = m_pFDP->getRightEyePivot();
					x << eye.x  << '\0'; y << eye.y << '\0'; z << eye.z << '\0';
					bindElem->setAttribute(X("pivotX"), X(x.str()));
					bindElem->setAttribute(X("pivotY"), X(y.str()));
					bindElem->setAttribute(X("pivotZ"), X(z.str()));
				}
				entElem->appendChild(bindElem);
			}
			++it_bind;
		}
	}

	return entElem;
}


DOMNode* FDPWriter::saveSource(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc)
{
	DOMElement*  sourceElem = doc->createElement(X("source"));

	// saving morph targets
	MorphDictionary::const_iterator it_morph = m_morphTargets.begin();
	while (it_morph != m_morphTargets.end())
	{
		std::map<std::string, Entity>::const_iterator it_dict = it_morph->second.begin();
		while(it_dict != it_morph->second.end())
		{
			sourceElem->appendChild(saveEntity(doc, it_dict->second, 
						it_dict->first, it_morph->first, it_dict->first == "Rest"));
			++it_dict;
		}
		++it_morph;
	}
	// save rest face too
	//sourceElem->appendChild(saveEntity(doc, m_faceEntity, "Rest", "Expression", true);
			
	return sourceElem;
}

DOMNode* FDPWriter::saveHead(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *doc)
{
	DOMElement*  headElem = doc->createElement(X("head"));

	// file version
	DOMComment* pCom1 = doc->createComment(X("Version of the FDP specification file (this file)"));
	headElem->appendChild(pCom1);
	DOMElement*  fileElem = doc->createElement(X("file"));
	fileElem->setAttribute(X("version"), X("0.2"));
	headElem->appendChild(fileElem);

	// FAPU (Facial Animation Parameter Units) for the model
	DOMComment* pCom2 = doc->createComment(X("FAPU (Facial Animation Parameter Units) for the model"));
	headElem->appendChild(pCom2);
	DOMElement*  fapuElem = doc->createElement(X("fapu"));
	// get from FDP
	std::strstream es, irisd, ens, mns, mw;
	es		<< m_pFDP->getES()*1024		<< '\0';
	irisd	<< m_pFDP->getIRISD()*1024	<< '\0';
	ens		<< m_pFDP->getENS()*1024	<< '\0';
	mns		<< m_pFDP->getMNS()*1024	<< '\0';
	mw		<< m_pFDP->getMW()*1024		<< '\0';
	// to XML
	fapuElem->setAttribute(X("ES0")		, X(es.str()));
	fapuElem->setAttribute(X("IRISD0")	, X(irisd.str()));
	fapuElem->setAttribute(X("ENS0")	, X(ens.str()));
	fapuElem->setAttribute(X("MNS0")	, X(mns.str()));
	fapuElem->setAttribute(X("MW0")		, X(mw.str()));
	headElem->appendChild(fapuElem);

	// Global Translation info for the whole face
	DOMComment* pCom3 = doc->createComment(X("Global Translation info for the whole face"));
	headElem->appendChild(pCom3);
	DOMElement*  trElem = doc->createElement(X("translation"));
	// get from FDP
	Vector3 trans = m_pFDP->getGlobalTranslation();
	std::strstream transx, transy, transz;
	transx << trans.x << '\0';
	transy << trans.y << '\0';
	transz << trans.z << '\0';
	// to XML
	trElem->setAttribute(X("x"), X(transx.str()));
	trElem->setAttribute(X("y"), X(transy.str()));
	trElem->setAttribute(X("z"), X(transz.str()));
	headElem->appendChild(trElem);

	// Global Rotation info for the whole face
	DOMComment* pCom4 = doc->createComment(X("Global Rotation info for the whole face"));
	headElem->appendChild(pCom4);
	DOMElement*  rotElem = doc->createElement(X("rotation"));
	// get from FDP
	Vector3 rot = m_pFDP->getGlobalRotationAxis();
	std::strstream rotx, roty, rotz, rotang;
	rotx << rot.x << '\0';
	roty << rot.y << '\0';
	rotz << rot.z << '\0';
	rotang << m_pFDP->getGlobalRotationAngle() << '\0';
	// to XML
	rotElem->setAttribute(X("axis_x"), X(rotx.str()));
	rotElem->setAttribute(X("axis_y"), X(roty.str()));
	rotElem->setAttribute(X("axis_z"), X(rotz.str()));
	rotElem->setAttribute(X("axis_angle"), X(rotang.str()));
	headElem->appendChild(rotElem);

	return headElem;
}

void FDPWriter::writeToFile(XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *pDoc, const std::string& strPath)
{
	XercesString wstrPath( XMLString::transcode(strPath.c_str()) );
	if (wstrPath)
	{
		DOMImplementation *impl  = DOMImplementationRegistry::getDOMImplementation(XercesString("LS"));
		DOMWriter *writer = ((DOMImplementationLS*)impl)->createDOMWriter();
		
		// turn on serializer "pretty print" option
		if ( writer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true) )
  			writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

		LocalFileFormatTarget fileTarget(wstrPath);

		// write the resulting document.
		writer->setEncoding( XercesString("UTF-8") );
		writer->writeNode(&fileTarget, *pDoc);
	}
}
} // namespace XFace
