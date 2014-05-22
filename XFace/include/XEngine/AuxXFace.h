#ifndef AUXXFACE_H__
#define AUXXFACE_H__
#include <string>

#if __GNUC__
char *itoa(int value, char* result, int base);
std::string itoa(int value, int base);
#endif

#endif
