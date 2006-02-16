#include <iostream>
#include <vector>
#include <boost/bind.hpp>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>

#include <TestCases/Thread/BarrierTest.h>

#define BARRIER_ITERS 30

namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION( BarrierTest );

// Spawn off several threads that sync with us using a barrier
// and verify that the variable values are correct.
void BarrierTest::testBarrier()
{
   mCounter = 0;
   mNumThreads = 5;
   mBarrier = new vpr::Barrier(mNumThreads+1);      // +1 for us

   std::vector<vpr::Thread*> threads(mNumThreads);

   for(unsigned t=0;t<mNumThreads;t++)
   {
      threads[t] = new vpr::Thread(boost::bind(&BarrierTest::testBarrier_thread,
                                               this));
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
   }
}

void BarrierTest::testBarrier_thread()
{
   for(unsigned i=0;i<BARRIER_ITERS;++i)
   {
      mMutex.acquire();
         mCounter += 1;
      mMutex.release();

      mBarrier->wait();
      //CPPUNIT_ASSERT(mNumThreads +1 == mCounter);
      mBarrier->wait();

      mMutex.acquire();
         mCounter -= 1;
      mMutex.release();

      mBarrier->wait();
      //CPPUNIT_ASSERT(0 == mCounter);
      mBarrier->wait();
   }
}

}
