//#include <gfxConfig.h>
#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <Threads/Thread.h>
#include <Threads/ThreadFunctor.h>
#include <Sync/Mutex.h>

#include <vector>

//#include <nspr.h>

#define ThreadTest_INC_COUNT 5000

//namespace gfxTest
//{

class ThreadTest : public TestCase
{
public:
   ThreadTest( std::string name )
   : TestCase (name)
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

   void testThreadCreation()
   {
      // Spawn off a bunch of threads (m)
      // Have each one increment counter n times
      // join all threads
      // Make sure counter is of valid value
      const int num_threads(10);
      std::vector<vpr::ThreadMemberFunctor<ThreadTest>*> functors(num_threads);
      std::vector<vpr::Thread*> threads(num_threads);

      for(int t=0;t<num_threads;t++)
      {
         functors[t] = new vpr::ThreadMemberFunctor<ThreadTest>(this,&ThreadTest::incCounter);

         // Spawns thread here
         threads[t] = new vpr::Thread(functors[t]);
      }

      for(int t=0;t<num_threads;t++)
      {
         if(threads[t]->join() == false)
            assertTest(false && "Thread was not able to be joined");
         delete threads[t];
         delete functors[t];
      }

      assertLongsEqual((num_threads*ThreadTest_INC_COUNT), mCounter);
      //assert(mCounter == (num_threads*50000));
   }

   void incCounter(void* arg)
   {
      for(int i=0;i<ThreadTest_INC_COUNT;i++)
      {
         mItemProtectionMutex->acquire();
         {
            long temp_counter = mCounter;
            mCounter = 0;
            //PR_Sleep(PR_MicrosecondsToInterval(20));    // Sleep for 10 micro seconds
            //PR_Sleep(200);
            mCounter = temp_counter + 1;
         }
         mItemProtectionMutex->release();
         //gfx::Thread::yield();
      }
   }

   // --------------------------------------------------------------------------- //
   void interactiveTestCPUGrind()
   {
      // Spawn off user specified number of threads
      // Have each grind the CPU until the user enters a value
      int num_threads(0);
      std::vector<vpr::ThreadMemberFunctor<ThreadTest>*> functors(num_threads);
      std::vector<vpr::Thread*>                          threads(num_threads);

      mStopGrindingCPU = false;

      // -- GET NUM THREADS -- //
      std::cout << "CPU grind: Enter num threads:";
      std::cin >> num_threads;
      std::cout << "\nSpawning " << num_threads << " threads.\n";
      if(num_threads == 0)
         return;

      // -- SPAWN THE THREADS -- //
      for(int t=0;t<num_threads;t++)
      {
         functors.push_back( new vpr::ThreadMemberFunctor<ThreadTest>(this,&ThreadTest::grindCPUWorker));
         threads.push_back( new vpr::Thread(functors[t]) );
      }

      // -- ASK FOR USER STOP -- //
      char answer;
      std::cout << "Are the CPUs grinding: (y/n)?  --> ";
      std::cin >> answer;
      std::cout << std::endl;

      mStopGrindingCPU = true;

      for(int t=0;t<num_threads;t++)
      {
         if(threads[t]->join() == false)
            assertTest(false && "Thread was not able to be joined");
         delete threads[t];
         delete functors[t];
      }

      assertTest((answer == 'y') || (answer == 'Y'));
   }

   // This function just grinds the CPU and waits for the flag to flip
   void grindCPUWorker(void* arg)
   {
      double bogus_sum(0.0);
      double da_arg(0.1);
      const double inc(0.005);

      while(!mStopGrindingCPU)
      {
         bogus_sum += (sin(da_arg) + cos(1.0/da_arg));
         da_arg += inc;
      }

   }

/*   // -------------------------------------------------------------------- //
   void testThreadStackSize()
   {
      // Spawn off a thread and have it consume some stack space
      mStackSpaceConsumed = 0;
      mNumRecursions = 200;
      const long stack_size = 64000;

      int arg;

      vpr::ThreadMemberFunctor<ThreadTest>* functor;
      vpr::Thread* the_thread;

      functor = new vpr::ThreadMemberFunctor<ThreadTest>(this,&ThreadTest::recurseConsumeResources, &arg);

//      the_thread = new vpr::Thread(functor, stack_size);
//      assertTest(the_thread != NULL);

//      assertTest(the_thread->join() && "Failed to join with testThreadStackSize thread");

      //assertTest(mCounter == (num_threads*50000));
   }

   // Recurse and consume some resources
   // Arg is a pointer to a long
   void recurseConsumeResources(void* arg)
   {
      // Allocate some stack variables
      long var1(5), var2(3), var3(7);
      static long total_sum;
      total_sum += (var1+var2+var3);
      assertTest(total_sum > 0);              // Just to use the vars

      mStackSpaceConsumed += (3 * sizeof(long));
      mNumRecursions--;

      if(mNumRecursions > 0)
         recurseConsumeResources(arg);
      else
         return;
   }
*/
   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("ThreadTest");
      test_suite->addTest( new TestCaller<ThreadTest>("testThreadCreation", &ThreadTest::testThreadCreation));
//      test_suite->addTest( new TestCaller<ThreadTest>("testThreadStackSize", &ThreadTest::testThreadStackSize));
      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractiveThreadTest");
      test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:
   vpr::Mutex*   mItemProtectionMutex;         // Protect an exclusive item
   long           mCounter;                     // A counter that we will use for now

   long           mNumRecursions;               // Number of recursions to go
   long           mStackSpaceConsumed;          // Amount of stack space that we have used

   bool           mStopGrindingCPU;             // Flag to tell the test to stop grinding the CPU
};

//};

