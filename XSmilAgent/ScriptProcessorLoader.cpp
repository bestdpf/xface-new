#include "ScriptProcessorLoader.h"
#include "wxScriptProcessor.h"

#include "XercesString.h"
#include <cassert>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "SMILProcessor.h"
#include "TTSEngineMaker.h"
#include "ItalianFlite.h"

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

ScriptProcessorLoader::ScriptProcessorLoader(void) : m_pParser(0), m_pDoc(0)
{
	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		// Do your failure processing here
		std::cerr << "ScriptProcessorLoader: Cannot start Xerces parser!" << XercesString(toCatch.getMessage()) << std::endl;
	}
}

ScriptProcessorLoader::~ScriptProcessorLoader(void)
{
	delete m_pParser;
	XMLPlatformUtils::Terminate();
}

bool ScriptProcessorLoader::load(std::istream& input)
{
	std::string xmlString;
	// get the input stream contents to a string
	while (!input.eof())
	{
		std::string dummy;
		input >> dummy;
		xmlString.append(dummy);
	}

	MemBufInputSource source((const unsigned char*)xmlString.c_str(), xmlString.size(), "42");
	return loadInput(source);
}

bool ScriptProcessorLoader::load(const std::string& filename)
{
	LocalFileInputSource source(XercesString(filename.c_str()));
	return loadInput(source);
}

bool ScriptProcessorLoader::loadInput(const InputSource& input)
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
				std::cerr << "ScriptProcessorLoader: Parsing error count: " << m_pParser->getErrorCount() << std::endl;
			}
		}
		catch (const DOMException& e)
		{
			std::cerr << "ScriptProcessorLoader: DOM Exception parsing reports: ";
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
			std::cerr << "ScriptProcessorLoader: XML Exception parsing reports: " << e.getMessage() << std::endl;
			bFailed = true;
		}
		catch (...)
		{
			std::cerr << "ScriptProcessorLoader: An exception occured while parsing." << std::endl;
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
bool ScriptProcessorLoader::parse()
{
	// get the scripters 
	DOMNodeList* nodes = m_pDoc->getElementsByTagName(XercesString("scripter"));
	if(!nodes) // sth wrong
		return false;
	XMLSize_t sz = nodes->getLength();
	for(XMLSize_t i = 0; i < sz; ++i)
		loadScript(nodes->item(i));

	return true;
}

void ScriptProcessorLoader::loadScript(DOMNode* pScript)
{
	DOMNamedNodeMap* scrAttrList = pScript->getAttributes();
	boost::shared_ptr<IScriptProcessor> pItem;
	// get the name
	char *scripter_name, *lang;
	
	DOMNode* attr = scrAttrList->getNamedItem(XercesString("name"));
	if(attr)
		scripter_name = XMLString::transcode(attr->getNodeValue());
	else	// sth totally wrong
		return;

	attr = scrAttrList->getNamedItem(XercesString("type"));
	if(attr)
	{
		// create a new item
		if (XercesString(attr->getNodeValue()) == "external")
			pItem.reset(new wxScriptProcessor(scripter_name));
		else if (XercesString(attr->getNodeValue()) == "SMIL")
			pItem.reset(new SMILProcessor(scripter_name));
	}
	// language
	attr = scrAttrList->getNamedItem(XercesString("lang"));
	if(attr)
	{
		lang = XMLString::transcode(attr->getNodeValue());
		pItem->setLanguage(lang);
		XMLString::release(&lang);
	}
	XMLString::release(&scripter_name);

		
	DOMNodeIterator* iterator = m_pDoc->createNodeIterator(pScript,
		DOMNodeFilter::SHOW_ELEMENT | DOMNodeFilter::SHOW_ATTRIBUTE, NULL, true);
	// use the tree walker to print out the text nodes.
	for ( DOMNode* current = iterator->nextNode(); current != 0; current = iterator->nextNode() )
	{
		if (XercesString(current->getNodeName()) == "output")	// can have multiple of those
		{
			// sample: <output filename="TempFile.wav"/>
			DOMNamedNodeMap* outputAttr = current->getAttributes();
			std::string outfile;

			attr = outputAttr->getNamedItem(XercesString("filename"));			
			if(attr)
			{
				char* dummy = XMLString::transcode(attr->getNodeValue());
				outfile = dummy;
				XMLString::release(&dummy);
			}
			pItem->push_output(outfile);
		}
		else if (XercesString(current->getNodeName()) == "TTSEngine")	// can have multiple of those
		{
			// sample: <TTS lang="english" engine="SAPI51"/>
			DOMNamedNodeMap* outputAttr = current->getAttributes();
			std::string ttsLang, ttsEngine, ttsPath, ttsIni;

			attr = outputAttr->getNamedItem(XercesString("lang"));	// for which language should we use it
			if(attr)
			{
				char* dummy = XMLString::transcode(attr->getNodeValue());
				ttsLang = dummy;
				XMLString::release(&dummy);
			}
			attr = outputAttr->getNamedItem(XercesString("engine")); // which TTS engine (already loaded)
			if(attr)
			{
				char* dummy = XMLString::transcode(attr->getNodeValue());
				ttsEngine = dummy;
				XMLString::release(&dummy);
			}
			attr = outputAttr->getNamedItem(XercesString("path")); // does it have a specific (external) executable path?
			if(attr)
			{
				char* dummy = XMLString::transcode(attr->getNodeValue());
				ttsPath = dummy;
				XMLString::release(&dummy);
			}
			attr = outputAttr->getNamedItem(XercesString("ini")); // does it have a ini file?
			if(attr)
			{
				char* dummy = XMLString::transcode(attr->getNodeValue());
				ttsIni = dummy;
				XMLString::release(&dummy);
			}
			pItem->push_ttsEngine(ttsEngine, ttsLang, ttsPath, ttsIni);
		}
		// are we using an external executable?
		else if (XercesString(current->getNodeName()) == "executable")	// can have multiple of those
		{
			DOMNamedNodeMap* attrList = current->getAttributes();
			
			// get the executable
			attr = attrList->getNamedItem(XercesString("exename"));
			if(attr)
			{
				char* exe = XMLString::transcode(attr->getNodeValue());
				pItem->setExecutable(exe);
				XMLString::release(&exe);
			}

			// get the path
			attr = attrList->getNamedItem(XercesString("path"));
			if(attr)
			{
				char* path = XMLString::transcode(attr->getNodeValue());
				pItem->setPath(path);
				XMLString::release(&path);
			}

			// get the parameters
			attr = attrList->getNamedItem(XercesString("parameters1"));
			if(attr)
			{
				char* param = XMLString::transcode(attr->getNodeValue());
				pItem->setParameters1(param);
				XMLString::release(&param);
			}

			attr = attrList->getNamedItem(XercesString("parameters2"));
			if(attr)
			{
				char* param = XMLString::transcode(attr->getNodeValue());
				pItem->setParameters2(param);
				XMLString::release(&param);
			}

			// should we use temp xml file
			attr = attrList->getNamedItem(XercesString("tempfile"));
			if(attr)
			{
				char* tmpfile = XMLString::transcode(attr->getNodeValue());
				pItem->setTempFileName(tmpfile);
				XMLString::release(&tmpfile);
			}
		}
	}
	
	m_processors.push_back(pItem);
}

boost::shared_ptr<IScriptProcessor> ScriptProcessorLoader::getScriptProcessor(const std::string& name) const
{
	static boost::shared_ptr<IScriptProcessor> invalidRetVal;
	std::vector<boost::shared_ptr<IScriptProcessor> >::const_iterator it = m_processors.begin();
	while (it != m_processors.end())
	{
		if((*it)->getName() == name)
			return *it;
		++it;
	}

	return invalidRetVal;
}

std::vector<std::string> ScriptProcessorLoader::getScripterNames() const
{
	std::vector<std::string> retVal;
	std::vector<boost::shared_ptr<IScriptProcessor> >::const_iterator it = m_processors.begin();
	while (it != m_processors.end())
	{
		retVal.push_back((*it)->getName());
		++it;
	}
	return retVal;
}
