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
 * The Original Code is XSmilAgent.
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

#include "SMILObjects.h"
#include <iostream>
#include "XercesString.h"

void SMILReferencedBlock::print() const
{
	std::cerr << "content-id: "		<< content_id		<< std::endl;
	std::cerr << "content-begin: "	<< content_begin	<< std::endl;
	std::cerr << "content-end: "	<< content_end		<< std::endl;
	SMILBlock::print();
}

void SMILSpeechAnimation::print() const
{
	std::cerr << "*****Dumping speech-animation object contents:*****\n";
	SMILReferencedBlock::print();
	std::cerr << "fill: "			<< fill				<< std::endl;
	std::cerr << "intensity: "		<< intensity		<< std::endl;
	std::cerr << "******************************************\n";
}

void SMILBlock::print() const
{
	synch.print();
	performance.print();
}

void SMILSpeech::print() const
{
	std::cerr << "*****Dumping speech object contents:*****\n";
	SMILBlock::print();
	std::cerr << "Filename: " << filename << std::endl;
	std::cerr << "Content:\n";
	for(size_t i = 0; i < content.size(); ++i)
	{
		if(content.at(i).type == SMILSpeechContent::kMark)
			std::cerr << "\tMark id: " << content.at(i).value << std::endl;
		else
			std::cerr << "\tText: " << content.at(i).value << " begins: " << content.at(i).beginVal << " ends: " << content.at(i).endVal << std::endl;
	}
	std::cerr << "******************************************\n";
}

void SMILSynchronizationAttributes::print() const
{
	std::cerr << "begin: "	<< begin	<< std::endl;
	std::cerr << "end: "	<< end		<< std::endl;
	std::cerr << "beginVal: "	<< beginVal	<< std::endl;
	std::cerr << "endVal: "	<< endVal		<< std::endl;
	std::cerr << "dur: "	<< dur		<< std::endl;
	std::cerr << "repeat: "	<< repeat	<< std::endl;
}

void SMILSystemAttributes::print() const
{
	std::cerr << "system-language: "	<< language		<< std::endl;
	std::cerr << "system-required: "	<< required		<< std::endl;
	std::cerr << "system-screen-size: "	<< screen_size	<< std::endl;
	std::cerr << "system-screen-depth: "<< screen_depth	<< std::endl;
}

void SMILTestAttributes::print() const
{
	system.print();
}

void SMILPerformanceAttributes::print() const
{
	std::cerr << "channel: "	<< channel	<< std::endl;
	std::cerr << "id: "			<< id		<< std::endl;
	std::cerr << "affect: "		<< affect	<< std::endl;
	std::cerr << "title: "		<< title	<< std::endl;
	tests.print();
}

void SMILAction::print() const
{
	std::cerr << "*****Dumping action object contents:*****\n";
	SMILReferencedBlock::print();
	std::cerr << "action-type: "	<< action_type		<< std::endl;
	std::cerr << "fill: "			<< fill				<< std::endl;
	std::cerr << "intensity: "		<< intensity		<< std::endl;
	std::cerr << "There are "		<< params.size()	<< " parameters." << std::endl;
	for(size_t i = 0; i < params.size(); ++i)
		std::cerr << "\t" << i + 1 << ". " << params[i] << std::endl;
	std::cerr << "******************************************\n";
}

void SMILSynchronizationAttributes::read(DOMNode* pNode)
{
	// get the attributes 
	DOMNamedNodeMap* outputAttr = pNode->getAttributes();
	DOMNode* attr = outputAttr->getNamedItem(XercesString("begin"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		begin = dummy;
		XMLString::release(&dummy);
	}
	
	attr = outputAttr->getNamedItem(XercesString("end"));			
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		end = dummy;
		XMLString::release(&dummy);
	}
	
	attr = outputAttr->getNamedItem(XercesString("dur"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		dur = dummy;
		XMLString::release(&dummy);
	}

	attr = outputAttr->getNamedItem(XercesString("repeat"));			
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		repeat = atoi(dummy);
		XMLString::release(&dummy);
	}
}

void SMILSystemAttributes::read(DOMNode* pNode)
{
	// get the attributes 
	DOMNamedNodeMap* outputAttr = pNode->getAttributes();
	DOMNode* attr = outputAttr->getNamedItem(XercesString("system-language"));			
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		language = dummy;
		XMLString::release(&dummy);
	}
	
	attr = outputAttr->getNamedItem(XercesString("system-required"));			
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		required = dummy;
		XMLString::release(&dummy);
	}
	
	attr = outputAttr->getNamedItem(XercesString("system-screen-size"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		screen_size = dummy;
		XMLString::release(&dummy);
	}

	attr = outputAttr->getNamedItem(XercesString("system-screen-depth"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		screen_depth = dummy;
		XMLString::release(&dummy);
	}
}

void SMILTestAttributes::read(DOMNode* pNode)
{
	system.read(pNode);
}

void SMILPerformanceAttributes::read(DOMNode* pNode)
{
	// get the attributes 
	DOMNamedNodeMap* outputAttr = pNode->getAttributes();
	DOMNode* attr = outputAttr->getNamedItem(XercesString("channel"));			
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		channel = dummy;
		XMLString::release(&dummy);
	}
	
	attr = outputAttr->getNamedItem(XercesString("affect"));			
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		affect = dummy;
		XMLString::release(&dummy);
	}
	
	attr = outputAttr->getNamedItem(XercesString("id"));			
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		id = dummy;
		XMLString::release(&dummy);
	}

	attr = outputAttr->getNamedItem(XercesString("title"));			
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		title = dummy;
		XMLString::release(&dummy);
	}
	tests.read(pNode);
}

void SMILAction::read(DOMNode* pAction)
{
	DOMNamedNodeMap* outputAttr = pAction->getAttributes();
	DOMNode* attr = outputAttr->getNamedItem(XercesString("content-id"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		content_id = dummy;
		XMLString::release(&dummy);
	}

	attr = outputAttr->getNamedItem(XercesString("action-type"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		action_type = dummy;
		XMLString::release(&dummy);
	}
	
	attr = outputAttr->getNamedItem(XercesString("content-begin"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		content_begin = dummy;
		XMLString::release(&dummy);
	}
	
	attr = outputAttr->getNamedItem(XercesString("content-end"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		content_end = dummy;
		XMLString::release(&dummy);
	}

	performance.read(pAction);
	synch.read(pAction);

	attr = outputAttr->getNamedItem(XercesString("intensity"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		intensity = (float)atof(dummy);
		XMLString::release(&dummy);
	}

	attr = outputAttr->getNamedItem(XercesString("fill"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		fill = dummy;
		XMLString::release(&dummy);
	}

	// now process the children
	DOMNodeList* pTest = pAction->getChildNodes();
	for(XMLSize_t j = 0; j < pTest->getLength(); ++j)
	{
		std::string param;
		if(pTest->item(j)->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			char* dummy = XMLString::transcode(pTest->item(j)->getNodeName());
			std::string nodeName = dummy;
			XMLString::release(&dummy);

			if(nodeName == "parameter")
			{
				DOMNodeList* pParam = pTest->item(j)->getChildNodes();
				for(XMLSize_t k = 0; k < pParam->getLength(); ++k)
				{
					if(pParam->item(k)->getNodeType() == DOMNode::TEXT_NODE)
					{
						if(((DOMText*)pParam->item(k))->isIgnorableWhitespace())
							continue;
						XMLCh* data = const_cast<XMLCh*>(pParam->item(k)->getNodeValue()); //dangerous, naah!!
						if(XMLString::isAllWhiteSpace(data))	
							continue;
						XMLString::collapseWS(data);
						char* dummy = XMLString::transcode(data);
						param = dummy;
						XMLString::release(&dummy);

						params.push_back(param);
					}
				}
			}	
		}		
	}
}

void SMILSpeechAnimation::read(DOMNode* pAnim)
{
	// get the attributes 
	DOMNamedNodeMap* outputAttr = pAnim->getAttributes();
	DOMNode* attr = outputAttr->getNamedItem(XercesString("content-id"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		content_id = dummy;
		XMLString::release(&dummy);
	}

	attr = outputAttr->getNamedItem(XercesString("content-begin"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		content_begin = dummy;
		XMLString::release(&dummy);
	}
	
	attr = outputAttr->getNamedItem(XercesString("content-end"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		content_end = dummy;
		XMLString::release(&dummy);
	}

	attr = outputAttr->getNamedItem(XercesString("content"));			
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		content = dummy;
		XMLString::release(&dummy);
	}
		
	performance.read(pAnim);
	synch.read(pAnim);

	attr = outputAttr->getNamedItem(XercesString("intensity"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		intensity = (float)atof(dummy);
		XMLString::release(&dummy);
	}

	attr = outputAttr->getNamedItem(XercesString("fill"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		fill = dummy;
		XMLString::release(&dummy);
	}
}

void SMILSpeech::read(DOMNode* pSpeech)
{
	DOMNode* attr = 0;
	DOMNamedNodeMap* outputAttr = pSpeech->getAttributes();
	
	
	performance.read(pSpeech);
	synch.read(pSpeech);

	attr = outputAttr->getNamedItem(XercesString("file"));
	if(attr)
	{
		char* dummy = XMLString::transcode(attr->getNodeValue());
		filename = dummy;
		XMLString::release(&dummy);
	}
	
	// now process the children
	DOMNodeList* pTest = pSpeech->getChildNodes();
	for(XMLSize_t j = 0; j < pTest->getLength(); ++j)
	{
		SMILSpeechContent cont;
		if(pTest->item(j)->getNodeType() == DOMNode::TEXT_NODE)
		{
			if(((DOMText*)pTest->item(j))->isIgnorableWhitespace())
				continue;
			XMLCh* data = const_cast<XMLCh*>(pTest->item(j)->getNodeValue()); //dangerous, naah!!
			if(XMLString::isAllWhiteSpace(data))	
				continue;
			XMLString::collapseWS(data);
			char* dummy = XMLString::transcode(data);
			cont.value = dummy;
			cont.type = SMILSpeechContent::kText;
			XMLString::release(&dummy);	
		}
		else if(pTest->item(j)->getNodeType() == DOMNode::ELEMENT_NODE)
		{
			outputAttr = pTest->item(j)->getAttributes();
			attr = outputAttr->getNamedItem(XercesString("id"));
			if(attr)
			{
				char* dummy = XMLString::transcode(attr->getNodeValue());
				cont.value = dummy;
				cont.type = SMILSpeechContent::kMark;
				XMLString::release(&dummy);
			}
		}
		if(cont.value.size() == 0 )	// delay push
			continue;
		content.push_back(cont);
	}
}

void SMILBlock::read(DOMNode* pBlock)
{
	synch.read(pBlock);
	performance.read(pBlock);
}