#ifndef _VPR_TEST_THREAD_TEST_H_
#define _VPR_TEST_THREAD_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Thread/TSObjectProxy.h>


namespace vprTest
{

class ThreadTest : public CppUnit::TestCase
{
CPPUNIT_TEST_SUITE(ThreadTest);
CPPUNIT_TEST(testNoSpawnCtor);
CPPUNIT_TEST(testAutoSpawnCtor);
CPPUNIT_TEST(testUncaughtException);
CPPUNIT_TEST(testRunnableFunctor);
//CPPUNIT_TEST( testCreateJoin);
//CPPUNIT_TEST( testSuspendResume);
//CPPUNIT_TEST( testPriority);
//CPPUNIT_TEST( testThreadStackSize);
CPPUNIT_TEST( testThreadSpecificData);
CPPUNIT_TEST_SUITE_END();

public:
   ThreadTest() : CppUnit::TestCase ()
   {
      mNumRecursions = 0;
   }

   ThreadTest(std::string name) : CppUnit::TestCase (name)
   {
      mNumRecursions = 0;
   }

   virtual ~ThreadTest()
   {}

   virtual void setUp()
   {
      mCounter = 0;
   }

   virtual void tearDown()
   {
   }

   void testNoSpawnCtor();
   void testAutoSpawnCtor();
   void testUncaughtException();

   void testRunnableFunctor();

   // =========================================================================
   // thread CreateJoin test
   // =========================================================================
   void testCreateJoin();

   void incCounter(void* arg);

   // =========================================================================
   // thread SuspendResume test
   // =========================================================================
   void counter1Func(void* arg);

   long sampleCompare(int num);


   void testSuspendResume();

   // =========================================================================
   // thread Priority test
   // =========================================================================

   void counter2Func(void* arg);

   void testPriority();

   // =========================================================================
   // interactive test
   // =========================================================================
   void interactiveTestCPUGrind();

   // This function just grinds the CPU and waits for the flag to flip
   void grindCPUWorker(void* arg);

   // -------------------------------------------------------------------- //
   void testThreadStackSize();

   // Recurse and consume some resources
   // Arg is a pointer to a long
   void recurseConsumeResources(void* arg);

   // ------------------------------------ //
   // ---- Thread specific data stuff ---- //
   // ------------------------------------ //
   void testThreadSpecificData();

   /**
   * @param arg - ptr to std::string id of thread
   */
   void tsIncCounter(void* arg);

 
   /*
   static CppUnit::Test* interactiveSuite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("InteractiveThreadTest");
      test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }
   */

protected:
   vpr::Mutex     mItemProtectionMutex;        // Protect an exclusive item
   long           mCounter;                    // A counter that we will use for now
   vpr::Mutex     mCounterMutex;
   long           mCounter1;                   // A nother counter
   vpr::Mutex     mCounter1Mutex;
   long           mNumRecursions;              // Number of recursions to go
   long           mStackSpaceConsumed;         // Amount of stack space that we have used
   vpr::TSObjectProxy<unsigned long>   mTSCounter; // Thread specific counter variable

   bool           mStopGrindingCPU;            // Flag to tell the test to stop grinding the CPU
};

}

#endif
