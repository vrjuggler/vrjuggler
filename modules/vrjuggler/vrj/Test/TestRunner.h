#ifndef _VRJ_TEST_TESTRUNNER_H
#define _VRJ_TEST_TESTRUNNER_H

#include <vpr/vpr.h>
#include <vrj/vrjConfig.h>
#include <vrj/Test/TestFailure.h>
#include <vrj/Test/Test.h>

#include <vrj/Kernel/App.h>

#include <vpr/Util/Debug.h>            // Needed for sync'ed output
#include <vpr/Util/Assert.h>

#include <string>
#include <vector>

namespace vrj::test
{

/**
*
* Groups tests together and is also in charge of running
* them and capturing a list of any failures that occur.
*/
class TestRunner
{
public:
   TestRunner()
      : mApp(NULL), mDoneProcessing(false), mCurTestIndex(-1)
   {;}

   /** Initialize the test suite.
   * Must be called before running the tests and
   * after all tests have been added.
   */
   void initialize(vrj::App* app)
   {
      mApp = app;

      // Initialize the tests
      vprASSERT(mTests.size() > 0);
      for(unsigned t=0;t<mTests.size();t++)
      {
         mTests[t]->setApp(app);
      }

      // Clear the failures
      mTestFailures.clear();
   }

   /** Progresses the state of testing 
   * Called at the end of preFrame by apps that want to run tests.
   */
   void processTests()
   {
      vprASSERT(NULL != mApp);

      bool test_failed(false);

      if(mCurTestIndex < (int)mTests.size())      // Check to see if there are tests to run
      {
         if(-1 == mCurTestIndex)                   // First time through
         {
            mCurTestIndex = 0;
            mTests[mCurTestIndex]->setUp();        // Setup the initial test
         }
         else
         {
            try
            {
               mTests[mCurTestIndex]->processTest();              // Process the current test
            }
            catch(vrj::test::TestFailure* tf)
            {
               mTestFailures.push_back(tf);
               test_failed = true;
            }

            if(test_failed || mTests[mCurTestIndex]->isDone())    // If current test is done or failed
            {
               mTest[mCurTestIndex]->tearDown();               // Tear down the test
               mCurTestIndex++;                                // Goto the next test
               if(mCurTestIndex < (int)mTests.size())          // If test is in range
               {
                  mTests[mCurTestIndex]->setUp();              // - Initialize it
               }
            }
         }
         
      }
   }

   /** Are the tests done processing
   * @return true - There is no more processing to do
   */
   bool doneProcessing()
   { return mDoneProcessing; }

   void addTest(Test* test)
   {
      mTests.push_back(test);
   }

   /** Print out the test failures */
   void printFailures()
   {
      if(mTestFailures.empty())
      {
         std::cout << "--- Tests all PASSED!!! ----\n" << std::flush;
      }
      else
      {
         std::cout << "----- Test failures: count: " << mTestFailures.size() << " -----\n" << std::flush;

         for(unsigned f=0; f<mTestFailures.size(); ++f)
         {
            TestFailure cur_failure = mTestFailures[f];
            std::cout << "   Failed: " << cur_failure.getFullDescription() << std::endl;
         }
      }
   }


protected:
   vrj::App*                  mApp;             /**< The application to test */
   bool                       mDoneProcessing;  /**< Flag to indicate completion of testing */
   int                        mCurTestIndex;    /**< Index of the current test to run. -1 means that we have to start. */
   std::vector<Test*>         mTests;           /**< List of tests to run */
   std::vector<TestFailure*>   mTestFailures;    /**< List of test failures */
};

}

#endif

