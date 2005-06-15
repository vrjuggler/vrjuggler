#if defined(WIN32) || defined(WIN64)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT int function()
{
   return 1;
}
