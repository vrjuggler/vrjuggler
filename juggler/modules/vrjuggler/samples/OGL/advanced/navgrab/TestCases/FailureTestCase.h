#ifndef _FAILURE_TEST_CASE_H
#define _FAILURE_TEST_CASE_H

#include <vrj/Test/TestCase.h>
#include <vrj/Test/Asserter.h>

#include <iostream>
#include <string>

                     
/** A very small class that demonstrates how to 
* fail a test.
*/
class FailureTestCase : public vrj::test::TestCase
{
public:
   FailureTestCase()
      : vrj::test::TestCase("FailureTestCase")
   {;}

   /** Do any setup needed before the test starts */
   virtual void setUp()
   {
      std::cout << "FailureTestCase::setup: Nothing to setup\n";
   }

   /** Do any cleanup needed after running */
   virtual void tearDown()
   {
      std::cout << "FailureTestCase::tearDown: Nothing to tear down\n";
   }

   /** Gives the test time to process (ie. run & test)
   * This method is called by the runner at then end
   * of preFrame until isDone is true.
   * @pre isDone() == false
   * @throws vrj::test::TestFailure - Thrown if the test fails
   */
   virtual void processTest()
   {
      VRJTEST_ASSERT(false && "This is supposed to fail");

      /*
      vrj::test::Asserter::failIf( !(false), this,                         \
                                 vrj::test::Message( "assertion failed",  \
                                                      "Expression: "),    \
                                 __FILE__, __LINE__ );
                                 */

   }

   /** Return true when the test is done processing
   * testing 
   */
   virtual bool isDone()
   {
      return false;     /* should just error out instead */
   }

};


#endif

