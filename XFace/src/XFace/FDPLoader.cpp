#include <XFace/FDPLoader.h>
#include <XFace/XercesString.h>

#include <cassert>
#include <string>
#include <iostream>
#include <stdlib.h>

#include <XFace/IInfluenceCalculator.h>
#include <XFace/InfluenceCalculatorMaker.h>

#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/util/XercesDefs.hpp>

#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

#ifdef _DEBUG
#pragma message("		Adding Xerces lib (Debug)...")
#pragma comment(lib, "xerces-c_2D")
#else
#pragma message("		Adding Xerces lib (Release)...")
#pragma comment(lib, "xerces-c_2")
#endif

namespace XFace{

FDPLoader::FDPLoader(void) : m_version("0.2"), m_pParser(0), m_pDoc(0)
{
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		// Do your failure processing here
		std::cerr << "FDPLoader: Cannot start Xerces parser!" << XercesString(toCatch.getMessage()) << std::endl;
	}
}

FDPLoader::~FDPLoader(void)
{
	delete m_pParser;
	XMLPlatformUtils::Terminate();
}

float FDPLoader::toFloat(const XMLCh* val)
{
	float retVal = 0;
	char* pStr = XMLString::transcode(val);
	if(pStr)
	{
		retVal = (float)atof(pStr);
		XMLString::release(&pStr);
	}

	return retVal;
}

bool FDPLoader::load(std::istream& input, boost::shared_ptr<FDP> pFDP)
{
	std::string xmlString;
	// get the input stream contents to a string
	while (!input.eof())
	{
		std::string dummy;
		input >> dummy;
		xmlString.append(dummy);
	}

	m_pFDP = pFDP;
	MemBufInputSource source((const unsigned char*)xmlString.c_str(), xmlString.size(), "42");
	return loadInput(source);
}

bool FDPLoader::load(const std::string& filename, boost::shared_ptr<FDP> pFDP)
{
	m_pFDP = pFDP;
	LocalFileInputSource source(XercesString(filename.c_str()));
	return loadInput(source);
}

bool FDPLoader::loadInput(const InputSource& input)
{
	bool bFailed = false; // assume all will go well
	// create new parser instance.
	delete m_pParser;
	m_pParser = new XercesDOMParser;
	if (m_pParser)
	{
		m_pParser->setValidationScheme(XercesDOMParser::Val_Auto);
		m_pParser->setDoNamespaces(false);
		m_pParser->setDoSchema(false);

		// skip this if you haven't written your own error reporter class.
//		DOMTreeErrorReporter *errReporter = new DOMTreeErrorReporter;
//		parser->setErrorHandler(errReporter);

		m_pParser->setCreateEntityReferenceNodes(false);
		//parser->setToCreateXMLDeclTypeNode(true);
		try
		{
			m_pParser->parse(input);
			bFailed = m_pParser->getErrorCount() != 0;
			if (bFailed)
			{
				std::cerr << "FDPLoader: Parsing error count: " << m_pParser->getErrorCount() << std::endl;
			}
		}
		catch (const DOMException& e)
		{
			std::cerr << "FDPLoader: DOM Exception parsing reports: ";
			// was message provided?
			if (e.msg)
			{
				// yes: display it as ascii.
				char *strMsg = XMLString::transcode(e.msg);
				std::cerr << strMsg << std::endl;
				XMLString::release(&strMsg);
			}
			else
				// no: just display the error code.
				std::cerr << e.code << std::endl;

			bFailed = true;
		}
		catch (const XMLException& e)
		{
			std::cerr << "FDPLoader: XML Exception parsing reports: " << e.getMessage() << std::endl;
			bFailed = true;
		}
		catch (...)
		{
			std::cerr << "FDPLoader: An exception occured while parsing." << std::endl;
			bFailed = true;
		}
		// did the input document parse okay?
		if (!bFailed)
		{
			m_pDoc = m_pParser->getDocument();
			bFailed = !parse();
		}
	}

	return !bFailed;
}

bool FDPLoader::parseHeader(DOMNode* pHeader)
{
	DOMNodeIterator* iterator = m_pDoc->createNodeIterator(pHeader, 
		DOMNodeFilter::SHOW_ELEMENT | DOMNodeFilter::SHOW_ATTRIBUTE, NULL, true);
	// use the tree walker to print out the text nodes.
	for ( DOMNode* current = iterator->nextNode(); current != 0; current = iterator->nextNode() )
	{
		// there are "file", "model", "fapu", "translation" and "rotation" elements in this chunk
		if (XercesString(current->getNodeName()) == "file")
		{
			// sample: <file version="0.2" />

			DOMNamedNodeMap* attr = current->getAttributes();
			if(!attr) // sth wrong
				return false;
			DOMNode* versionAttr = attr->getNamedItem(XercesString("version"));
			if(XercesString(m_version.c_str()) != versionAttr->getNodeValue())
				// versions not matching, flee!!
				return false;
		}
		else if(XercesString(current->getNodeName()) == "fapu")
		{
			// sample: <fapu ES0="69.9977" IRISD0="16.0424" ENS0="51.8036" MNS0="30.1538" MW0="50.6392" />
			
			DOMNamedNodeMap* attrList = current->getAttributes();
			if(!attrList) // sth wrong
				return false;

			/////////////// ES0
			DOMNode* attr = attrList->getNamedItem(XercesString("ES0"));
			if(attr)
				m_pFDP->setES0(toFloat(attr->getNodeValue()));
			/////////////// IRISD0
			attr = attrList->getNamedItem(XercesString("IRISD0"));
			if(attr)
				m_pFDP->setIRISD0(toFloat(attr->getNodeValue()));
			/////////////// ENS0
			attr = attrList->getNamedItem(XercesString("ENS0"));
			if(attr)
				m_pFDP->setENS0(toFloat(attr->getNodeValue()));
			/////////////// MNS0
			attr = attrList->getNamedItem(XercesString("MNS0"));
			if(attr)
				m_pFDP->setMNS0(toFloat(attr->getNodeValue()));
			/////////////// MW0
			attr = attrList->getNamedItem(XercesString("MW0"));
			if(attr)
				m_pFDP->setMW0(toFloat(attr->getNodeValue()));
			
			// debug << "fapu item" << std::endl;
		}
		else if(XercesString(current->getNodeName()) == "translation")
		{
			// sample: <translation x="0" y="-1" z="-659" />
        
			DOMNamedNodeMap* attrList = current->getAttributes();
			if(!attrList) // sth wrong
				return false;

			float x = 0, y = 0, z = 0;
			/////////////// x translation
			DOMNode* attr = attrList->getNamedItem(XercesString("x"));
			if(attr)
				x = toFloat(attr->getNodeValue());
			
			/////////////// y translation
			attr = attrList->getNamedItem(XercesString("y"));
			if(attr)
				y = toFloat(attr->getNodeValue());
		
			/////////////// z translation
			attr = attrList->getNamedItem(XercesString("z"));
			if(attr)
				z = toFloat(attr->getNodeValue());
		
			m_pFDP->setGlobalTranslation(x, y, z);
		//	debug << "translation item " << x << " " << y << " " << z << std::endl;
		}
		else if(XercesString(current->getNodeName()) == "rotation")
		{
			// sample: <rotation axis_x="-0.998192" axis_y="0.0596591" axis_z="0.00728935" axis_angle="0.444541" />

			DOMNamedNodeMap* attrList = current->getAttributes();
			if(!attrList) // sth wrong
				return false;

			float x = 0, y = 0, z = 0, a = 0;
			/////////////// x rotation
			DOMNode* attr = attrList->getNamedItem(XercesString("axis_x"));
			if(attr)
				x = toFloat(attr->getNodeValue());
			
			/////////////// y rotation
			attr = attrList->getNamedItem(XercesString("axis_y"));
			if(attr)
				y = toFloat(attr->getNodeValue());
		
			/////////////// z rotation
			attr = attrList->getNamedItem(XercesString("axis_z"));
			if(attr)
				z = toFloat(attr->getNodeValue());
		
			/////////////// z rotation
			attr = attrList->getNamedItem(XercesString("axis_angle"));
			if(attr)
				a = toFloat(attr->getNodeValue());
		
			m_pFDP->setGlobalRotation(x, y, z, a);
		}
	}
  
	return true;
}

void FDPLoader::loadEntity(DOMNode* pSource)
{
	DOMNamedNodeMap* attrList = pSource->getAttributes();
	std::string alias, category;
	DOMNode* attr = attrList->getNamedItem(XercesString("alias"));
	if(attr)
	{
		char* aliasPtr = XMLString::transcode(attr->getNodeValue());
		alias.assign(aliasPtr);
		XMLString::release(&aliasPtr);
	}

	attr = attrList->getNamedItem(XercesString("category"));
	if(attr)
	{
		char* catPtr = XMLString::transcode(attr->getNodeValue());
		category.assign(catPtr);
		XMLString::release(&catPtr);
	}
		
	DOMNodeIterator* iterator = m_pDoc->createNodeIterator(pSource, 
		DOMNodeFilter::SHOW_ELEMENT | DOMNodeFilter::SHOW_ATTRIBUTE, NULL, true);
	// use the tree walker to print out the text nodes.
	for ( DOMNode* current = iterator->nextNode(); current != 0; current = iterator->nextNode() )
	{
		if(XercesString(current->getNodeName()) == "mesh")
		{
			char *fileTmp, *formatTmp, *pathTmp;
			XEngine::MeshInfo info;
			attrList = current->getAttributes();
			attr = attrList->getNamedItem(XercesString("file"));
			if(attr)
			{
				fileTmp = XMLString::transcode(attr->getNodeValue());
				info.file.assign(fileTmp);
				XMLString::release(&fileTmp);
			}

			attr = attrList->getNamedItem(XercesString("format"));
			if(attr)
			{
				formatTmp = XMLString::transcode(attr->getNodeValue());
				info.format.assign(formatTmp);
				XMLString::release(&formatTmp);
			}

			attr = attrList->getNamedItem(XercesString("path"));
			if(attr)
			{
				pathTmp = XMLString::transcode(attr->getNodeValue());
				info.path.assign(pathTmp);
				XMLString::release(&pathTmp);
			}
			else
				info.path.assign("");
			
			info.keyframe_alias = alias;
			info.keyframe_category = category;

			if(alias == "Rest")
				m_faceEntityMeshInfo.push_back(info);
			// push the Rest state into the morph target dictionary as well
			m_morphTargetsMeshInfos[alias].push_back(info);
		}
		else if(XercesString(current->getNodeName()) == "bind")
		{
			XEngine::MeshInfo info;
			std::string submesh, item;
			attrList = current->getAttributes();
			attr = attrList->getNamedItem(XercesString("submesh"));
			if(attr)
			{
				char* submeshTmp = XMLString::transcode(attr->getNodeValue());
				submesh.assign(submeshTmp);
				XMLString::release(&submeshTmp);
			}
			
			attr = attrList->getNamedItem(XercesString("item"));
			if(attr)
			{
				char* itemTmp = XMLString::transcode(attr->getNodeValue());
				item.assign(itemTmp);
				XMLString::release(&itemTmp);
			}
			if(item == "LeftEye" || item == "RightEye") // eye pivots
			{
				Vector3 eye(0, 0, 0);
				//std::string x, y, z;
				attr = attrList->getNamedItem(XercesString("pivotX"));
				if(attr)
					eye.x = toFloat(attr->getNodeValue());
				attr = attrList->getNamedItem(XercesString("pivotY"));
				if(attr)
					eye.y = toFloat(attr->getNodeValue());
				attr = attrList->getNamedItem(XercesString("pivotZ"));
				if(attr)
					eye.z = toFloat(attr->getNodeValue());
				if(item == "LeftEye")
					m_pFDP->setLeftEyePivot(eye);
				else
					m_pFDP->setRightEyePivot(eye);
			}
			
			m_bindings.insert(std::make_pair(submesh, item));
		}
	}
}

void FDPLoader::parseSource(DOMNode* pSource)
{
	DOMNodeIterator* iterator = m_pDoc->createNodeIterator(pSource, 
		DOMNodeFilter::SHOW_ELEMENT | DOMNodeFilter::SHOW_ATTRIBUTE, NULL, true);
	// use the tree walker to print out the text nodes.
	for ( DOMNode* current = iterator->nextNode(); current != 0; current = iterator->nextNode() )
	{
		if(XercesString(current->getNodeName()) == "entity")
			loadEntity(current);
	}
}

void FDPLoader::loadFDPItem(DOMNode* pFDPItem)
{
	DOMNamedNodeMap* attrList = pFDPItem->getAttributes();
	FDPItem* pItem = 0;
	// get the name (id)
	DOMNode* attr = attrList->getNamedItem(XercesString("name"));
	if(attr)
	{
		char* name = XMLString::transcode(attr->getNodeValue());
		// create a new item (will be deleted in dtor of FDP class)
		pItem = new FDPItem(name);
		XMLString::release(&name);
	}
	else
		return;
	
	// get the control vertex index
	attr = attrList->getNamedItem(XercesString("index"));
	if(attr)
	{
		char* index = XMLString::transcode(attr->getNodeValue());
		pItem->setControlPoint((unsigned short)atoi(index));
		XMLString::release(&index);
	}

	// get the affecting mesh name
	attr = attrList->getNamedItem(XercesString("affects"));
	if(attr)
	{
		char* affects = XMLString::transcode(attr->getNodeValue());
		pItem->setAffects(affects);
		XMLString::release(&affects);
	}
	
	DOMNodeIterator* iterator = m_pDoc->createNodeIterator(pFDPItem,
		DOMNodeFilter::SHOW_ELEMENT | DOMNodeFilter::SHOW_ATTRIBUTE, NULL, true);
	// use the tree walker to print out the text nodes.
	for ( DOMNode* current = iterator->nextNode(); current != 0; current = iterator->nextNode() )
	{
		if(XercesString(current->getNodeName()) == "indices")
		{
			DOMNodeList* children = current->getChildNodes(); 
			// we should have only one child, text node, just a safety net here
			if ( (children->getLength() == 1) && (children->item(0)->getNodeType() == DOMNode::TEXT_NODE) )//(XercesString(children->item(0)->getNodeName()) == "#text") )
			{
				char* pStr = XMLString::transcode(children->item(0)->getNodeValue());
				std::string str(pStr);
				processIndices(str, pItem);
				XMLString::release(&pStr);
			}
		}
		else if (XercesString(current->getNodeName()) == "influence")	// can have multiple of those
		{
			// sample: <influence weight="0.25" fap="3" type="RaisedCosInfluenceWaveY" />
			DOMNamedNodeMap* influenceAttr = current->getAttributes();
			// get the weight
			float w = toFloat(influenceAttr->getNamedItem(XercesString("weight"))->getNodeValue());
			unsigned short fap = (unsigned short)toFloat(influenceAttr->getNamedItem(XercesString("fap"))->getNodeValue());
			char* typeTmp = XMLString::transcode(influenceAttr->getNamedItem(XercesString("type"))->getNodeValue());
			std::string type;
			type.assign(typeTmp);
			XMLString::release(&typeTmp);
			
			IInfluenceCalculator* pInfluence = InfluenceCalculatorMaker::newInfluenceCalculator(type, w, fap);
			if(pInfluence)
				pItem->addInfluenceCalculator(pInfluence);
		}
	}

	m_pFDP->insertItem(pItem);
}

void FDPLoader::processIndices(const std::string& indices, FDPItem* pItem)
{
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

bool FDPLoader::parse()
{
	m_bindings.clear();
	m_morphTargetsMeshInfos.clear();
	m_faceEntityMeshInfo.clear();

	// get the header 
	DOMNodeList* nodes = m_pDoc->getElementsByTagName(XercesString("head"));
	if(!nodes || nodes->getLength() != 1) // sth wrong
		return false;
	if(!parseHeader(nodes->item(0)) )
		return false;
	
	// get the source info
	nodes = m_pDoc->getElementsByTagName(XercesString("source"));
	if(!nodes || nodes->getLength() != 1) // sth wrong
		return false;
	parseSource(nodes->item(0));
		
	// process fdp's now
	nodes = m_pDoc->getElementsByTagName(XercesString("fdp"));
	XMLSize_t sz = nodes->getLength();
	for(XMLSize_t i = 0; i < sz; ++i)
		loadFDPItem(nodes->item(i));
		
	return true;
}
}