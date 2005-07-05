#include <TestCases/DynLoad/modules/TestInterface.h>

#if defined(WIN32) || defined(WIN64)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

class TestClass : public TestInterface
{
public:
   virtual bool function()
   {
      return true;
   }
};

extern "C"
{

EXPORT void* entryFunc()
{
   return new TestClass();
}

}
