#include <TestCases/DynLoad/modules/TestInterface.h>

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

void* entryFunc()
{
   return new TestClass();
}

}
