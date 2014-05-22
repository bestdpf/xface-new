#ifndef WAVE2_H__
#define WAVE2_H__
#include <string>

class CWave
{
 public:
  /**Default constructor*/
  CWave();
  /**Constructor*/
  CWave(const std::string &filename);
 
  unsigned int getDuration();
};

#endif //WAVE2_H__
