#include "TTSEngineMaker.h"
#include <iostream>

/*!
    Base class constructor registers the derived class identifier
    to its list of influencers.
    \param className derived class identifier (should be unique.)
*/	
TTSEngineMaker::TTSEngineMaker(const std::string& className)
{
	registry().insert(std::make_pair(className, this));
}

/*!
    Returns the list of makers as a vector. 
    \note Do NOT call this method frequently, every time a new vector is created 
    and returned.
*/
std::vector<std::string> TTSEngineMaker::getMakerList()
{
	std::vector<std::string> ret;
	MakerMap::const_iterator it = registry().begin();
	while(it != registry().end())
	{
		ret.push_back(it->first);
		++it;
	}

	return ret;
}

/*!
    Clients should call this static method, to create TTS engines. It accesses the
    proper factory (maker) and requests a new tts engine and returns it.
    \param className derived class unique maker identifier. 
*/
ITTSEngine* TTSEngineMaker::newTTSEngine(const std::string& className, const std::string& pathname, const std::string& ini)
{
	MakerMap::iterator it = registry().find(className);
	if(it != registry().end())
		return it->second->makeTTSEngine(pathname, ini);
	else
	{
		std::cerr << "Cannot create TTS Engine: " << className << std::endl;
		return 0;
	}
}
