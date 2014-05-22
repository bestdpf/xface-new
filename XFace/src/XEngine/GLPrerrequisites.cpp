#include "XEngine/GLPrerrequisites.h"

#ifdef WIN32
	#include <windows.h>
#else //LINUX
	#include <GL/glx.h>
	#include <GL/glxext.h>
extern "C" {
extern void (*glXGetProcAddressARB(const GLubyte *procName))( void );
};
#endif

namespace XEngine
{
	
void* getProcAddress(const char *procname)
{
#ifdef WIN32
  return (void*)wglGetProcAddress((const char*)procname);
#else //LINUX
  return (void*)glXGetProcAddressARB((const GLubyte*)procname);
#endif
}

}