#include <stdio.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT void otherFunc(const char* msg)
{
   printf("%s\n", msg);
}
