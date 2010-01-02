/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

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

namespace vrj {
namespace test
{

/** \class TestRunner TestRunner.h vrj/Test/TestRunner.h
 *
 * Groups tests together and is also in charge of running
 * them and capturing a list of any failures that occur.
 */
class TestRunner
{
public:
   enum State
   {
      Uninitialized,
      Processing,
      DoneProcessing
   };

public:
   TestRunner()
      : mApp(NULL)
      , mCurState(Uninitialized)
      , mCurTestIndex(-1)
   {;}

   /**
    * Initializes the test suite.
    * Must be called before running the tests and after all tests have been
    * added.
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

      mCurState = Processing;
   }

   /**
    * Progresses the state of testing.
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
            std::cout << "TestRunner: Starting test: " << mTests[mCurTestIndex]->getName() << std::endl;
            mTests[mCurTestIndex]->setUp();        // Setup the initial test
         }
         else
         {
            try
            {
               mTests[mCurTestIndex]->processTest();              // Process the current test
            }
            catch(vrj::test::TestFailure& tf)
            {
               mTestFailures.push_back(tf);
               test_failed = true;
            }
            catch(...)
            {
               std::cout << "Recieved unknown exception in TestRunner. rethrowing." << std::endl;
               throw;
            }

            if(test_failed || mTests[mCurTestIndex]->isDone())    // If current test is done or failed
            {
               mTests[mCurTestIndex]->tearDown();               // Tear down the test
               mCurTestIndex++;                                // Goto the next test
               if(mCurTestIndex < (int)mTests.size())          // If test is in range
               {
                  std::cout << "TestRunner: Starting test: " << mTests[mCurTestIndex]->getName() << std::endl;
                  mTests[mCurTestIndex]->setUp();              // - Initialize it
               }
               else
               {
                  mCurState = DoneProcessing;
                  printFailures();
               }
            }
         }
         
      }
   }

   void addTest(Test* test)
   {
      mTests.push_back(test);
   }

   /**
    * Are the tests done processing?
    * @return true if there is no more processing to do.
    */
   State getState()
   {
      return mCurState;
   }

   /** Prints out the test failures. */
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
   State                      mCurState;        /**< Store the current state of processing */
   int                        mCurTestIndex;    /**< Index of the current test to run. -1 means that we have to start. */
   std::vector<Test*>         mTests;           /**< List of tests to run */
   std::vector<TestFailure>   mTestFailures;    /**< List of test failures */
};

} }


#endif
