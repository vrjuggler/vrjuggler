#include <iostream>
#include <vector>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Thread/TSTable.h>
#include <vpr/Thread/TSObject.h>
#include <vpr/Thread/TSObjectProxy.h>
#include <vpr/Thread/ThreadManager.h>
#include <vpr/Sync/Mutex.h>

#define ThreadTest_INC_COUNT 5000

namespace vprTest{

class ThreadTest : public CppUnit::TestCase
{
public:
   ThreadTest()
   : CppUnit::TestCase ()
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
   void testCreateJoin()
   {
      // Spawn off a bunch of threads (m)
      // Have each one increment counter n times
      // join all threads
      // Make sure counter is of valid value
      
      
      //std::cout<<"]==================================================\n"<<std::flush; 
      //std::cout<<" Thread CreateJoin: \n"<<std::flush; 
      
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
            CPPUNIT_ASSERT(false && "Thread was not able to be joined");
         delete threads[t];
         delete functors[t];
      }

      CppUnit::TestAssert::assertEquals<long>((num_threads*ThreadTest_INC_COUNT),
                                              mCounter);
      //CPPUNIT_ASSERT(mCounter == (num_threads*50000));
      
      std::cout << " done\n" << std::flush;
   }

   void incCounter(void* arg)
   {
      for(int i=0;i<ThreadTest_INC_COUNT;i++)
      {
         mItemProtectionMutex->acquire();
         {
            long temp_counter = mCounter;
            mCounter = 0;
            vpr::System::msleep(20);    // Sleep for 20 micro seconds
            mCounter = temp_counter + 1;
         }
         mItemProtectionMutex->release();
         //gfx::Thread::yield();
      }
   }
   
   // =========================================================================
   // thread SuspendResume test
   // =========================================================================
   void counter1Func(void* arg)
   {
      for(int i=0;i<10000;i++)
      {
         vpr::System::msleep(10);    // Sleep for 20 micro seconds
         mCounterMutex.acquire();
         {
            long temp_counter = mCounter;
            mCounter = 0;
            vpr::System::msleep(10);    // Sleep for 20 micro seconds
            mCounter = temp_counter + 1;
         }
         mCounterMutex.release();
      }         
   } 
     
   long sampleCompare(int num)
   {
      long sampleValue1=0;
      long sampleValue2=0;
      
      if (num==1) {
         mCounterMutex.acquire();
            sampleValue1=mCounter;
         mCounterMutex.release();
      }
      else {
         mCounter1Mutex.acquire();
            sampleValue1=mCounter1;
         mCounter1Mutex.release();
      }
      
      vpr::System::msleep(500 );      
      
      if (num==1) {
         mCounterMutex.acquire();
            sampleValue2=mCounter;
         mCounterMutex.release();
      }
      else {
         mCounter1Mutex.acquire();
            sampleValue2=mCounter1;
         mCounter1Mutex.release();
      }
      std::cout<<sampleValue1<<" : "<<sampleValue2<<std::endl;
      return sampleValue2-sampleValue1;
   }

   
   void testSuspendResume()
   {
      //std::cout<<"]==================================================\n"<<std::flush; 
      //std::cout<<" Thread SuspendResume: \n"<<std::flush;
      
      mCounter=0;
      
      // spawn an counter thread
      vpr::ThreadMemberFunctor<ThreadTest> counter_functor( this, &ThreadTest::counter1Func );
      vpr::Thread counter_thread( & counter_functor);
      
      vpr::System::msleep(100 );
      
      CPPUNIT_ASSERT(sampleCompare(1)!=0 && "Counter doesn't work");
      
      counter_thread.suspend();
      vpr::System::msleep(100);
        
      CPPUNIT_ASSERT(sampleCompare(1)==0 && "thread can not be suspended");
      
      counter_thread.resume();
      vpr::System::msleep(100);
      
      CPPUNIT_ASSERT(sampleCompare(1)!=0 && "thread can not be resumed");
      
      counter_thread.kill();
      std::cout << " done\n" << std::flush;
   }

   
   // =========================================================================
   // thread Priority test
   // =========================================================================
    
   void counter2Func(void* arg)
   {
      for(int i=0;i<10000;i++)
      {
         vpr::System::msleep(10);    // Sleep for 20 micro seconds
         mCounter1Mutex.acquire();
         {
            long temp_counter = mCounter1;
            mCounter = 0;
            vpr::System::msleep(10);    // Sleep for 20 micro seconds
            mCounter1 = temp_counter + 1;
         }
         mCounter1Mutex.release();
      }         
   }
   
   void testPriority()
   {
      //std::cout<<"]==================================================\n"<<std::flush; 
      //std::cout<<" Thread Priority: \n"<<std::flush;
      
      mCounter=0;
      mCounter1=0;
      
      long diff1=0;
      long diff2=0;
      
      // spawn two counter threads
      vpr::ThreadMemberFunctor<ThreadTest> counter1_functor( this, &ThreadTest::counter1Func );
      vpr::Thread counter1_thread( & counter1_functor);      
      vpr::System::msleep(500 );
      
      vpr::ThreadMemberFunctor<ThreadTest> counter2_functor( this, &ThreadTest::counter2Func );
      vpr::Thread counter2_thread( & counter2_functor);
//      counter2_thread.suspend();
      vpr::System::msleep(500 );
//      counter2_thread.resume();
      
      diff1=sampleCompare(1);
      diff2=sampleCompare(2);
      std::cout<<"diff1= "<<diff1<<" : "<<std::endl;
      std::cout<<"diff2= "<<diff2<<" : "<<std::endl;
//      CPPUNIT_ASSERT(abs(diff2-diff1)<2 && "Counters don't work correctly); 
      
      counter1_thread.setPrio(vpr::BaseThread::VPR_PRIORITY_HIGH);
      counter2_thread.setPrio(vpr::BaseThread::VPR_PRIORITY_LOW);
      vpr::System::msleep(100 );
      
      diff1=sampleCompare(1);
      diff2=sampleCompare(2);
      std::cout<<"diff1= "<<diff1<<" : "<<std::endl;
      std::cout<<"diff2= "<<diff2<<" : "<<std::endl;
//      CPPUNIT_ASSERT(abs(diff2-diff1)<2 && "Counters don't work correctly); 

      counter1_thread.setPrio(vpr::BaseThread::VPR_PRIORITY_LOW);
      counter2_thread.setPrio(vpr::BaseThread::VPR_PRIORITY_HIGH);
      vpr::System::msleep(100 );
      
      diff1=sampleCompare(1);
      diff2=sampleCompare(2);
      std::cout<<"diff1= "<<diff1<<" : "<<std::endl;
      std::cout<<"diff2= "<<diff2<<" : "<<std::endl;
//      CPPUNIT_ASSERT(abs(diff2-diff1)<2 && "Counters don't work correctly); 
      
      counter1_thread.kill();
      counter2_thread.kill();
   }

   // =========================================================================
   // interactive test
   // =========================================================================
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
            CPPUNIT_ASSERT(false && "Thread was not able to be joined");
         delete threads[t];
         delete functors[t];
      }

      CPPUNIT_ASSERT((answer == 'y') || (answer == 'Y'));
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

   // -------------------------------------------------------------------- //
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

      the_thread = new vpr::Thread(functor, vpr::BaseThread::VPR_PRIORITY_NORMAL, vpr::BaseThread::VPR_LOCAL_THREAD, vpr::BaseThread::VPR_JOINABLE_THREAD, stack_size);
      CPPUNIT_ASSERT(the_thread != NULL);

      CPPUNIT_ASSERT(the_thread->join() && "Failed to join with testThreadStackSize thread");

      //CPPUNIT_ASSERT(mCounter == (num_threads*50000));
   }

   // Recurse and consume some resources
   // Arg is a pointer to a long
   void recurseConsumeResources(void* arg)
   {
      // Allocate some stack variables
      long var1(5), var2(3), var3(7);
      static long total_sum;
      total_sum += (var1+var2+var3);
      CPPUNIT_ASSERT(total_sum > 0);              // Just to use the vars

      mStackSpaceConsumed += (3 * sizeof(long));
      mNumRecursions--;

      if(mNumRecursions > 0)
         recurseConsumeResources(arg);
      else
         return;
   }

   void registerTests (CppUnit::TestSuite* suite)
   {
      suite->addTest( new CppUnit::TestCaller<ThreadTest>("testCreateJoin", &ThreadTest::testCreateJoin));
      suite->addTest( new CppUnit::TestCaller<ThreadTest>("testSuspendResume", &ThreadTest::testSuspendResume));
      suite->addTest( new CppUnit::TestCaller<ThreadTest>("testPriority", &ThreadTest::testPriority));
      suite->addTest( new CppUnit::TestCaller<ThreadTest>("testThreadStackSize", &ThreadTest::testThreadStackSize));
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
