#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT int function()
{
   return 1;
}
