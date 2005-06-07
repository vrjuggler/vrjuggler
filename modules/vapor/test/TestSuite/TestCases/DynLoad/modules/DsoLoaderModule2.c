#include <stdio.h>

#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT void function(const char* argType, void* arg)
{
   printf("2: %s\n", argType);
}
