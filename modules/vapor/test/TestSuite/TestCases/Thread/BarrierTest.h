#ifndef _VPR_TEST_BARRIER_TEST_
#define _VPR_TEST_BARRIER_TEST_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <cppunit/extensions/ThreadTestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Sync/Barrier.h>

namespace vprTest
{

class BarrierTest : public CppUnit::ThreadTestCase
{
CPPUNIT_TEST_SUITE(BarrierTest);
CPPUNIT_TEST( testBarrier);
CPPUNIT_TEST_SUITE_END();

public:
   BarrierTest() : CppUnit::ThreadTestCase ()
   {;}

   BarrierTest(std::string name) : CppUnit::ThreadTestCase (name)
   {;}

   virtual ~BarrierTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {;}


   void testBarrier();
   void testBarrier_thread(void* arg);

public:
   unsigned       mCounter;      /**< Helper counter variable. */
   unsigned       mNumThreads;
   vpr::Mutex     mMutex;
   vpr::Barrier*  mBarrier;
};

}

#endif

