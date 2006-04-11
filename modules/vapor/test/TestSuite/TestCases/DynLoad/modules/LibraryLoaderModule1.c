#if defined(WIN32) || defined(WIN64)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

EXPORT int sumFunc(int a1, int a2)
{
   return a1 + a2;
}
