#ifndef _VRJ_TEST_TEST_H
#define _VRJ_TEST_TEST_H

#include <vpr/vpr.h>
#include <vrj/vrjConfig.h>

#include <string>
#include <vector>

#include <vrj/Test/Test.h>

namespace vrj::test
{

/**
*
* Base class for normal single test case.
* 
*/
class TestCase : public Test
{
public:
   Test(std::string name)
      : mName(name)
   {;}

   virtual void setApp(vrj::App* app)
   { mApp = app; }

   virtual std::string getName()
   { return mName; }

protected:
   vrj::App*   mApp;          /**< Pointer to the application to be testing */
   std::string mName;         /**< Name of the test */
};

}

#endif

