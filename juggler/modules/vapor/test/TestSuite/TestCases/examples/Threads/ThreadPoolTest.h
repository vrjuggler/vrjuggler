#ifndef _THREAD_POOL_TEST_H
#define _THREAD_POOL_TEST_H

#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <Threads/ThreadPool.h>
#include <Threads/Thread.h>
#include <Threads/ThreadFunctor.h>
#include <Threads/Mutex.h>

#include <vector>
#include <hash_map>
#include <nspr.h>

#define ThreadPool_NumThreads 10

namespace gfxTest
{

class ThreadPoolTest : public TestCase
{
public:
   ThreadPoolTest( std::string name )
   : TestCase (name)
   {
   }

   virtual ~ThreadPoolTest()
   {}

   virtual void setUp()
   {
      mItemProtectionMutex = new gfx::Mutex();
      mCoutMutex = new gfx::Mutex();
      mWorkersUsedMutex = new gfx::Mutex();
      mCounter = 0;
      mThreadPool = new gfx::ThreadPool(ThreadPool_NumThreads);
      mWorkersUsed.clear();
      while(mWorkersUsed.size() < 1000)
         mWorkersUsed.push_back(0);
   }

   virtual void tearDown()
   {
   }

   //  Try to break ThreadPool by getting past barrier before
   // all threads have completed.
   // Do this by creating a bunch of threads and having them inc
   // counter after wasting some time.  This should give
   // barrier time to try to exit first.
   void testBarrier()
   {
      long num_incs(20);
      gfx::ThreadMemberFunctor<ThreadPoolTest>* inc_function;

      // Spawn off incs
      for(long cur_inc=0;cur_inc<num_incs;cur_inc++)
      {
         inc_function = new gfx::ThreadMemberFunctor<ThreadPoolTest>(this, &ThreadPoolTest::incCounter);
         mThreadPool->startFunc(inc_function);
      }

      // Hit barrier
      mThreadPool->barrier();

      // Assert values are equal
      mItemProtectionMutex->acquire();
         assertLongsEqual(num_incs,mCounter);
         //std::cerr << "Num incs: " << num_incs << "  mCounter:" << mCounter << std::endl;
      mItemProtectionMutex->release();
   }

   void incCounter(void* arg)
   {
      PR_Sleep(PR_MicrosecondsToInterval(20));    // Sleep for 20 micro seconds

      mItemProtectionMutex->acquire();
      mCounter++;
      mItemProtectionMutex->release();
   }

   // --------------------------------------------------------------------------- //
   void interactiveTestCPUGrind()
   {
      // Spawn off user specified number of threads
      // Have each grind the CPU until the user enters a value
      long iters_per_thread(0);
      int num_threads(0);

      // -- GET NUM THREADS -- //
      std::cout << "ThreadPool CPU grind: Enter num threads:";
      std::cin >> num_threads;
      std::cout << "Number of iters per thread: [20000]";
      std::cin >> iters_per_thread;
      std::cout << "Number of iters in CPU grind [500]:";
      std::cin >> mNumGrindIters;
      std::cout << "Create pool with " << num_threads << " threads.\n"
                << "   itersPerThread: " << iters_per_thread << std::endl
                << "   cpuGrindIters: " << mNumGrindIters << std::endl << std::flush;

      if(num_threads == 0)
         return;

      mThreadPool = new gfx::ThreadPool(num_threads);
      gfx::ThreadMemberFunctor<ThreadPoolTest>* work_func;

      for(long cur_iter=0;cur_iter<(num_threads*iters_per_thread);cur_iter++)
      {
         work_func = new gfx::ThreadMemberFunctor<ThreadPoolTest>(this, &ThreadPoolTest::grindCPUWorker);
         mThreadPool->startFunc(work_func);
      }

      mThreadPool->barrier();

      // --- Output workers used --- //
//      std::vector<int>::iterator w;
      for(unsigned w=0;w < mWorkersUsed.size(); w++)
      {
         if(mWorkersUsed[w] >= 1)
            std::cout << "Worker: " << w << "   used: " << mWorkersUsed[w] << std::endl;
      }

      // -- ASK FOR USER STOP -- //
      char answer;
      std::cout << "Did the CPUs grind: (y/n)?  --> ";
      std::cin >> answer;
      std::cout << std::endl;

      assertTest((answer == 'y') || (answer == 'Y'));
   }

   // This function just grinds the CPU and waits for the flag to flip
   void grindCPUWorker(void* arg)
   {
      long iters = mNumGrindIters;
      double bogus_sum(0.0);
      double da_arg(0.1);
      const double inc(0.005);

      //mCoutMutex->acquire();
      //std::cout << "worker: " << gfx::Thread::self()->getTID() << ", starting work" << std::endl;
      //mCoutMutex->release();
      //mWorkersUsedMutex->acquire();
      mWorkersUsed[gfx::Thread::self()->getTID()] += 1;
      //mWorkersUsedMutex->release();

      for(long i=0;i<iters;i++)
      {
         bogus_sum += (sin(da_arg) + cos(1.0/da_arg));
         da_arg += inc;
      }

   }

   // ---------------------------------------------------------- //

   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("ThreadPoolTest");
      test_suite->addTest( new TestCaller<ThreadPoolTest>("testBarrier", &ThreadPoolTest::testBarrier));
      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite *test_suite = new TestSuite ("InteractiveThreadPoolTest");
      test_suite->addTest( new TestCaller<ThreadPoolTest>("interactiveTestCPUGrind", &ThreadPoolTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:
   long              mNumGrindIters;            // Number of iters to grind

   gfx::ThreadPool*  mThreadPool;              // The thread pool to use
   gfx::Mutex*       mItemProtectionMutex;         // Protect an exclusive item
   gfx::Mutex*       mCoutMutex;                   // Protect stream output
   long              mCounter;                     // A counter that we will use for now
   std::vector<long> mWorkersUsed;             // Which worker was used?
   gfx::Mutex*       mWorkersUsedMutex;
};

};

#endif

