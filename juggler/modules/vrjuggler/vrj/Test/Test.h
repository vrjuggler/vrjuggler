#ifndef _VRJ_TEST_TEST_H
#define _VRJ_TEST_TEST_H

#include <vpr/vpr.h>
#include <vrj/vrjConfig.h>
#include <vrj/Kernel/App.h>

#include <string>
#include <vector>

namespace vrj::test
{

/**
* Base interface for what all tests must look like.
* 
*/
class Test
{
public:
   Test()
    {;}

   /** Set the application that we are testing 
   * To do anything meaningful you will probably have to dynamic_cast the
   * app to you application type.
   */
   virtual void setApp(vrj::App* app) = 0;
   
   /** Do any setup needed before the test starts */
   virtual void setUp()
   {;}

   /** Do any cleanup needed after running */
   virtual void tearDown()
   {;}

   /** Gives the test time to process (ie. run & test)
   * This method is called by the runner at then end
   * of preFrame until isDone is true.
   * @pre isDone() == false
   * @throws vrj::test::TestFailure - Thrown if the test fails
   */
   virtual void processTest() = 0;

   /** Return true when the test is done processing
   * testing 
   */
   virtual bool isDone() = 0;

   /** Get the name of the test */
   virtual std::string getName() = 0;
};

}

#endif

