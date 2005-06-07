#include <iostream>
#include <vector>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>

#include <TestCases/Thread/BarrierTest.h>

#include <boost/concept_check.hpp>

#define BARRIER_ITERS 30

namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION( BarrierTest );

// Spawn off several threads that sync with us using a barrier
// and verify that the variable values are correct.
void BarrierTest::testBarrier()
{
   threadAssertReset();
   mCounter = 0;
   mNumThreads = 5;
   mBarrier = new vpr::Barrier(mNumThreads+1);      // +1 for us

   std::vector<vpr::ThreadMemberFunctor<BarrierTest>*> functors(mNumThreads);
   std::vector<vpr::Thread*> threads(mNumThreads);

   for(unsigned t=0;t<mNumThreads;t++)
   {
      functors[t] = new vpr::ThreadMemberFunctor<BarrierTest>(this,&BarrierTest::testBarrier_thread);
      threads[t] = new vpr::Thread(functors[t]);
   }

   for(unsigned i=0;i<BARRIER_ITERS;++i)
   {
      mMutex.acquire();
        mCounter += 1;
      mMutex.release();

      mBarrier->wait();
      CPPUNIT_ASSERT(mNumThreads +1 == mCounter);
      mBarrier->wait();

      mMutex.acquire();
        mCounter -= 1;
      mMutex.release();

      mBarrier->wait();
      CPPUNIT_ASSERT(0 == mCounter);
      mBarrier->wait();
   }

   for(unsigned t=0;t<mNumThreads;t++)
   {
      threads[t]->join();
      delete threads[t];
      delete functors[t];
   }

   checkThreadAssertions();
}

void BarrierTest::testBarrier_thread(void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   for(unsigned i=0;i<BARRIER_ITERS;++i)
   {
      mMutex.acquire();
         mCounter += 1;
      mMutex.release();

      mBarrier->wait();
      assertTestThread(mNumThreads +1 == mCounter);
      mBarrier->wait();

      mMutex.acquire();
         mCounter -= 1;
      mMutex.release();

      mBarrier->wait();
      assertTestThread(0 == mCounter);
      mBarrier->wait();
   }
}

}
