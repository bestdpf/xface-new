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
 * The Original Code is Xface Core Library; XFaceApp.
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

#include <XFaceApp/XMLUtils.h>


#include <XFace/XercesString.h>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>


#ifdef _DEBUG
#pragma message("		Adding Xerces lib (Debug)...")
#pragma comment(lib, "xerces-c_2D")
#else
#pragma message("		Adding Xerces lib (Release)...")
#pragma comment(lib, "xerces-c_2")
#endif

#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

#include <iostream>
#include <strstream>
#include <fstream>
#include <sstream>

using XFace::XercesString;

namespace XFaceApp{
std::string XMLUtils::xmlify(const Task& task)
{
	std::stringstream retVal;
	retVal << "<task name=\"";
	retVal << task.getName();
	retVal << "\" ownerId=\"";
	retVal << task.getOwnerID();
	retVal << "\" taskId=\"";
	retVal << task.getID();
	retVal << "\">";
	retVal << std::endl;
	for(unsigned short i = 0; i < task.getParamCount(); ++i)
	{
		retVal << "<param>";
		retVal << task.getParameter(i);
		retVal << "</param>";
		retVal << std::endl;
	}
	retVal << "</task>";
	retVal << std::ends;

	return retVal.str();
}

std::string XMLUtils::xmlify(const Notification& note)
{
	std::stringstream retVal;
	//retVal << "<xface>";
	retVal << "<notify name=\"";
	retVal << note.getName();
	retVal << "\" ownerId=\"";
	retVal << note.getOwnerID();
	retVal << "\" taskId=\"";
	retVal << note.getTaskID();
	retVal << "\" status=\"";
	retVal << note.getStatus();
	retVal << "\"/>";
	retVal << std::ends;

	return retVal.str();
}

Notification XMLUtils::xmlToNotification(const std::string& xmlString)
{
	try 
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) 
	{
		// Do your failure processing here
		std::cerr << "XMLUtils: Cannot start Xerces parser!" << XercesString(toCatch.getMessage()) << std::endl;
		return Notification::error();
	}

	InputSource* source = new MemBufInputSource((const unsigned char*)xmlString.c_str(), 
							(unsigned int)xmlString.size(), 
							"44");
	
	XercesDOMParser* pParser = new XercesDOMParser;
	if (!pParser)
	{
		std::cerr << "XMLUtils: Could not create parser item!" << std::endl;
		return Notification::error();
	}

	pParser->setValidationScheme(XercesDOMParser::Val_Auto);
	pParser->setDoNamespaces(false);
	pParser->setDoSchema(false);
	pParser->setCreateEntityReferenceNodes(false);
	//parser->setToCreateXMLDeclTypeNode(true);
	try
	{
		pParser->parse(*source);
		if (pParser->getErrorCount() != 0)
		{
			std::cerr << "XMLUtils: Parsing error count: " << pParser->getErrorCount() << std::endl;
			return Notification::error();
		}
	}
	catch(...)
	{
		std::cerr << "XMLUtils: Parsing error in xmlToNotification" << std::endl;
		return Notification::error();
	}

	XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* pDoc = pParser->getDocument();
	XERCES_CPP_NAMESPACE_QUALIFIER DOMNodeList* notifyNodes = pDoc->getElementsByTagName(XercesString("notify"));
	if(notifyNodes->getLength() != 1)
		return Notification::error();
	
	
	std::string name = "ERROR";
	unsigned short ownerId = 0;
	unsigned int taskId = 0;
	int status = 0;

	DOMNamedNodeMap* attrList = notifyNodes->item(0)->getAttributes();
	DOMNode* attr = attrList->getNamedItem(XercesString("name"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		name.assign(dummy);
		XMLString::release(&dummy);
	}

	attr = attrList->getNamedItem(XercesString("ownerId"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		ownerId = (unsigned short)atoi(dummy);
		XMLString::release(&dummy);
	}

	attr = attrList->getNamedItem(XercesString("taskId"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		taskId = (unsigned int)atoi(dummy);
		XMLString::release(&dummy);
	}
	
	Notification note(name, ownerId, taskId);
	attr = attrList->getNamedItem(XercesString("status"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		status = (int)atoi(dummy);
		note.setStatus((Notification::TaskStatus) status);
		XMLString::release(&dummy);
	}
//	std::cerr << "xmlToNote " << name << " " << ownerId << " " << taskId << std::endl;

	delete source;
	delete pParser;
	XMLPlatformUtils::Terminate();
	
	return note;
}
/*!
*/
Task XMLUtils::xmlToTask(const std::string& xmlString)
{
	Task task("", 0);	// don't worry, we will fill inside soon..
	try 
	{
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) 
	{
		// Do your failure processing here
		std::cerr << "XMLUtils: Cannot start Xerces parser!" << XercesString(toCatch.getMessage()) << std::endl;
		return task;
	}

	//MemBufInputSource source((const unsigned char*)xmlString.c_str(), (unsigned int)xmlString.size(), "42");
	InputSource* source = new MemBufInputSource((const unsigned char*)xmlString.c_str(), 
							(unsigned int)xmlString.size(), 
							"45");
	XercesDOMParser* pParser = new XercesDOMParser;
	if (!pParser)
		return task;
	
	pParser->setValidationScheme(XercesDOMParser::Val_Auto);
	pParser->setDoNamespaces(false);
	pParser->setDoSchema(false);
	pParser->setCreateEntityReferenceNodes(false);
	//parser->setToCreateXMLDeclTypeNode(true);
	try
	{
		pParser->parse(*source);
		if (pParser->getErrorCount() != 0)
		{
			std::cerr << "XMLUtils: Parsing error count: " << pParser->getErrorCount() << std::endl;
			return task;
		}
	}
	catch(...)
	{
		std::cerr << "XMLUtils: Parsing error in xmlToNotification" << std::endl;
		return task;
	}

	DOMDocument* pDoc = pParser->getDocument();
	DOMNodeList* taskNodes = pDoc->getElementsByTagName(XercesString("task"));
	if(taskNodes->getLength() != 1)
		return task;
	
	std::string name;
	unsigned short ownerId = 0;
	unsigned int taskId = 0;
	
	DOMNamedNodeMap* attrList = taskNodes->item(0)->getAttributes();
	DOMNode* attr = attrList->getNamedItem(XercesString("name"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		name.assign(dummy);
		task.setName(name);
		XMLString::release(&dummy);
	}

	attr = attrList->getNamedItem(XercesString("ownerId"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		ownerId = (unsigned short)atoi(dummy);
		task.setOwnerID(ownerId);
		XMLString::release(&dummy);
	}

	attr = attrList->getNamedItem(XercesString("taskId"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		taskId = (unsigned int)atoi(dummy);
		task.setID(taskId);
		XMLString::release(&dummy);
	}
	
	// get the parameters
	DOMImplementation *impl  = DOMImplementationRegistry::getDOMImplementation(XercesString("LS"));
	DOMWriter *writer = ((DOMImplementationLS*)impl)->createDOMWriter();
	MemBufFormatTarget *target = new MemBufFormatTarget(4096);
	// write the resulting document.
	writer->setEncoding( XercesString("UTF-8") );
	
	DOMNodeList* nodes = pDoc->getElementsByTagName(XercesString("param"));
	for(size_t i = 0; i < nodes->getLength(); ++i)
	{
		target->reset();
		DOMNodeList* children = nodes->item(i)->getChildNodes();
		for(size_t i2 = 0; i2 < children->getLength(); ++i2)
			writer->writeNode(target, *children->item(i2));
		
		task.pushParameter((char*)target->getRawBuffer());
	}
	
	delete target;
	delete source;
	delete pParser;
	XMLPlatformUtils::Terminate();
	
	return task;

}
} //namespace XFaceApp
