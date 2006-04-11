#include <stdio.h>

#if defined(WIN32) || defined(WIN64)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT void otherFunc(const char* msg)
{
   printf("%s\n", msg);
}
