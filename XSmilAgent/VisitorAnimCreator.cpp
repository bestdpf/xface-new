#include "VisitorAnimCreator.h"
#include <iostream>

std::fstream VisitorAnimCreator::m_animFile;

VisitorAnimCreator::VisitorAnimCreator(const std::string& filename)
{
	reset(filename);
}

VisitorAnimCreator::~VisitorAnimCreator()
{
	finalize();
}

void VisitorAnimCreator::finalize()
{
	if(m_animFile.is_open())
		m_animFile.close();
}

void VisitorAnimCreator::reset(const std::string& filename)
{
	finalize();
	m_animFile.open((filename + ".anim").c_str(), std::ios_base::out);
}

void VisitorAnimCreator::operator()(const SMILBlock& node) const
{
}
void VisitorAnimCreator::operator()(const SMILSpeechAnimation& node) const
{
	std::string channel = getChannelClass(node.performance.channel);
	if(channel.empty())
		channel = "Expression";
	m_animFile << channel << " "
				<< node.performance.affect << " " 
				<< node.synch.beginVal << " " << node.synch.endVal << " " << node.intensity << std::endl;
}

void VisitorAnimCreator::operator()(const SMILAction& node) const
{
	m_animFile << getChannelClass(node.performance.channel) << " "
		<< node.action_type << " " 
		<< node.synch.beginVal << " " << node.synch.endVal << " " 
		<< node.intensity;
	for(size_t i = 0; i < node.params.size(); ++i)
		m_animFile << " " << node.params[i];
	m_animFile << std::endl;
}

void VisitorAnimCreator::operator()(const SMILReferencedBlock& node) const
{
}
void VisitorAnimCreator::operator()(const boost::shared_ptr<SGGroup>& node) const
{
	//process children
	std::for_each(node->m_children.begin(), node->m_children.end()
		,boost::apply_visitor(*this));
}

void VisitorAnimCreator::addSpeech(const std::string& speechFile, int duration, const std::string& lang)
{
	m_animFile << "Viseme " << speechFile << " " << duration << " " << lang << std::endl;
}

void VisitorAnimCreator::blinkEyes(unsigned int period)
{
	m_animFile << "BlinkEyes " << period << std::endl;
}

std::string VisitorAnimCreator::getChannelClass(const std::string& name) const
{
	if(name == "alice-face")
		return "Expression";
	else if(name == "alice-eyes")
		return "Eyes";
	else if (name == "alice-head")
		return "Head";
	else
		std::cerr << "An unknown channel name *" << name << "* is used in script, ignored!" << std::endl;
	
	return "";
}
