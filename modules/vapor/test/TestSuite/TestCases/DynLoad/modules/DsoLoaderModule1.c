#include <stdio.h>

#if defined(WIN32) || defined(WIN64)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT void function(const char* argType, void* arg)
{
   printf("1: %s\n", argType);
}
