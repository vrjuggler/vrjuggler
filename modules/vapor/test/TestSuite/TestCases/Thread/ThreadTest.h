#ifndef _VPR_TEST_THREAD_TEST_H_
#define _VPR_TEST_THREAD_TEST_H_

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Sync/Mutex.h>


namespace vprTest{

class ThreadTest : public CppUnit::TestCase
{
public:
   ThreadTest()
   : CppUnit::TestCase ()
   {
      mNumRecursions = 0;
   }

   ThreadTest(std::string name)
   : CppUnit::TestCase (name)
   {
      mNumRecursions = 0;
   }

   virtual ~ThreadTest()
   {}

   virtual void setUp()
   {
      mItemProtectionMutex = new vpr::Mutex();
      mCounter = 0;
   }

   virtual void tearDown()
   {
   }

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

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("ThreadTest");

      test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("testCreateJoin", &ThreadTest::testCreateJoin));
      test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("testSuspendResume", &ThreadTest::testSuspendResume));
      test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("testPriority", &ThreadTest::testPriority));
      test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("testThreadStackSize", &ThreadTest::testThreadStackSize));

      return test_suite;
   }

   static CppUnit::Test* interactiveSuite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("InteractiveThreadTest");
      test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:
   vpr::Mutex*   mItemProtectionMutex;         // Protect an exclusive item
   long           mCounter;                    // A counter that we will use for now
   vpr::Mutex     mCounterMutex;
   long           mCounter1;                   // A nother counter
   vpr::Mutex     mCounter1Mutex;
   long           mNumRecursions;              // Number of recursions to go
   long           mStackSpaceConsumed;         // Amount of stack space that we have used

   bool           mStopGrindingCPU;            // Flag to tell the test to stop grinding the CPU
};

}

#endif
