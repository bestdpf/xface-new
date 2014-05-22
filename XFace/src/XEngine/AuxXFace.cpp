#include "XEngine/AuxXFace.h"
#include <algorithm>
#if __GNUC__
char* itoa(int val, char* res, int base) 
{
  // check if the base is valid
  if (base < 2 || base > 16) { 
    *res = 0; 
    
    return res;
  }

  char* out = res;
  int quot = val;
  do {
    *out = "0123456789abcdef"[std::abs( quot % base )];
    ++out;
    quot /= base;
  } while (quot);

  if (val < 0 && base == 10) 
    *out++ = '-';

  std::reverse(res, out);

  *out = 0;

  return res;
}
#endif
