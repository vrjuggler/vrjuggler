#include <iostream>
#include <vector>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/Thread/TSTable.h>
#include <vpr/Thread/TSObject.h>
#include <vpr/Thread/TSObjectProxy.h>
#include <vpr/Thread/ThreadManager.h>

#include <cppunit/extensions/MetricRegistry.h>

#include <TestCases/Thread/ThreadTest.h>

#include <math.h>

#include <boost/concept_check.hpp>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( ThreadTest );

class Tester
{
public:
   Tester()
   {
   }

   void doSomething(void* arg)
   {
      boost::ignore_unused_variable_warning(arg);
      vprASSERT(vpr::Thread::self() != NULL && "We should know ourselves by now");

      for ( vpr::Uint32 i = 0; i < mMaxInc; ++i )
      {
         mValue++;
      }
   }

   static const vpr::Uint32 mMaxInc;
   vpr::Uint32 mValue;
};

const vpr::Uint32 Tester::mMaxInc = 500;

class RunnableTest
{
public:
   RunnableTest() : mValue(0)
   {
   }

   void run()
   {
      for ( vpr::Uint32 i = 0; i < mMaxInc; ++i )
      {
         mValue++;
      }
   }

   static const vpr::Uint32 mMaxInc;
   vpr::Uint32 mValue;
};

const vpr::Uint32 RunnableTest::mMaxInc = 1000;

static const vpr::Uint32 ThreadTest_INC_COUNT = 5000;

void ThreadTest::testNoSpawnCtor()
{
   for(unsigned iter=0; iter<50; ++iter)
   {
      Tester test_obj;
      const vpr::Uint32 start_val(500);
      test_obj.mValue = start_val;

      vpr::Thread my_thread;
      CPPUNIT_ASSERT(! my_thread.valid() && "Thread should not be running yet");

      vpr::ThreadMemberFunctor<Tester> functor(&test_obj, &Tester::doSomething,
                                               NULL);
      CPPUNIT_ASSERT(functor.isValid() && "Functor should be valid");

      my_thread.setFunctor(&functor);
      CPPUNIT_ASSERT(! my_thread.valid() && "Thread should not be running yet");

      my_thread.start();
      CPPUNIT_ASSERT(my_thread.valid() && "Thread should be running now");

      my_thread.join();
      CPPUNIT_ASSERT_EQUAL(test_obj.mValue, (start_val + Tester::mMaxInc));
   }
}

void ThreadTest::testAutoSpawnCtor()
{
   for(unsigned iter=0; iter<50; ++iter)
   {
      Tester test_obj;
      const vpr::Uint32 start_val(500);
      test_obj.mValue = start_val;

      vpr::ThreadMemberFunctor<Tester> functor(&test_obj, &Tester::doSomething,
                                               NULL);
      CPPUNIT_ASSERT(functor.isValid() && "Functor should be valid");

      vpr::Thread my_thread(&functor);
      CPPUNIT_ASSERT(my_thread.valid() && "Thread should be running now");

      my_thread.join();
      CPPUNIT_ASSERT_EQUAL(test_obj.mValue, (start_val + Tester::mMaxInc));
   }
}

void ThreadTest::testRunnableFunctor()
{
   const vpr::Uint32 start_val = 500;
   RunnableTest obj;
   obj.mValue = start_val;

   vpr::ThreadRunFunctor<RunnableTest> runnable(&obj);
   vpr::Thread thread(&runnable);
   CPPUNIT_ASSERT(thread.valid() && "Thread did not start");

   thread.join();
   CPPUNIT_ASSERT(obj.mValue == (start_val + obj.mMaxInc));
}

void ThreadTest::testCreateJoin()
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

      // Spawns thread here.
      threads[t] = new vpr::Thread(functors[t]);
   }

   for(int t=0;t<num_threads;t++)
   {
      if(threads[t]->join() == false)
         CPPUNIT_ASSERT(false && "Thread was not able to be joined");
      delete threads[t];
      delete functors[t];
   }

   CPPUNIT_ASSERT_EQUAL((unsigned long) num_threads * ThreadTest_INC_COUNT,
                        (unsigned long) mCounter);
   //CPPUNIT_ASSERT(mCounter == (num_threads*50000));

   std::cout << " done\n" << std::flush;
}

void ThreadTest::incCounter(void* arg)
{
   boost::ignore_unused_variable_warning(arg);
   for(vpr::Uint32 i=0;i<ThreadTest_INC_COUNT;i++)
   {
      mItemProtectionMutex.acquire();
      {
         long temp_counter = mCounter;
         mCounter = 0;
         vpr::System::msleep(20);    // Sleep for 20 micro seconds
         mCounter = temp_counter + 1;
      }
      mItemProtectionMutex.release();
      //gfx::Thread::yield();
   }
}

void ThreadTest::counter1Func(void* arg)
{
   boost::ignore_unused_variable_warning(arg);
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

long ThreadTest::sampleCompare(int num)
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

void ThreadTest::testSuspendResume()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" Thread SuspendResume: \n"<<std::flush;

   mCounter=0;

   // spawn an counter thread
   vpr::ThreadMemberFunctor<ThreadTest>* counter_functor =
      new vpr::ThreadMemberFunctor<ThreadTest>( this, &ThreadTest::counter1Func );
   vpr::Thread counter_thread(counter_functor);

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

void ThreadTest::counter2Func(void* arg)
{
   boost::ignore_unused_variable_warning(arg);
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

void ThreadTest::testPriority()
{
   //std::cout<<"]==================================================\n"<<std::flush;
   //std::cout<<" Thread Priority: \n"<<std::flush;

   mCounter=0;
   mCounter1=0;

   long diff1=0;
   long diff2=0;

   // spawn two counter threads
   vpr::ThreadMemberFunctor<ThreadTest>* counter1_functor =
      new vpr::ThreadMemberFunctor<ThreadTest>( this, &ThreadTest::counter1Func );
   vpr::Thread counter1_thread(counter1_functor);
   vpr::System::msleep(500 );

   vpr::ThreadMemberFunctor<ThreadTest>* counter2_functor =
      new vpr::ThreadMemberFunctor<ThreadTest>( this, &ThreadTest::counter2Func );
   vpr::Thread counter2_thread(counter2_functor);
//   counter2_thread.suspend();
   vpr::System::msleep(500 );
//   counter2_thread.resume();

   diff1=sampleCompare(1);
   diff2=sampleCompare(2);
   std::cout<<"diff1= "<<diff1<<" : "<<std::endl;
   std::cout<<"diff2= "<<diff2<<" : "<<std::endl;
//   CPPUNIT_ASSERT(abs(diff2-diff1)<2 && "Counters don't work correctly);

   counter1_thread.setPrio(vpr::BaseThread::VPR_PRIORITY_HIGH);
   counter2_thread.setPrio(vpr::BaseThread::VPR_PRIORITY_LOW);
   vpr::System::msleep(100 );

   diff1=sampleCompare(1);
   diff2=sampleCompare(2);
   std::cout<<"diff1= "<<diff1<<" : "<<std::endl;
   std::cout<<"diff2= "<<diff2<<" : "<<std::endl;
//   CPPUNIT_ASSERT(abs(diff2-diff1)<2 && "Counters don't work correctly);

   counter1_thread.setPrio(vpr::BaseThread::VPR_PRIORITY_LOW);
   counter2_thread.setPrio(vpr::BaseThread::VPR_PRIORITY_HIGH);
   vpr::System::msleep(100 );

   diff1=sampleCompare(1);
   diff2=sampleCompare(2);
   std::cout<<"diff1= "<<diff1<<" : "<<std::endl;
   std::cout<<"diff2= "<<diff2<<" : "<<std::endl;
//   CPPUNIT_ASSERT(abs(diff2-diff1)<2 && "Counters don't work correctly);

   counter1_thread.kill();
   counter2_thread.kill();
}

void ThreadTest::interactiveTestCPUGrind()
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
      vpr::Thread* cur_thread = new vpr::Thread(functors[t]);
      threads.push_back(cur_thread);
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
void ThreadTest::grindCPUWorker(void* arg)
{
   boost::ignore_unused_variable_warning(arg);
   double bogus_sum(0.0);
   double da_arg(0.1);
   const double inc(0.005);

   while(!mStopGrindingCPU)
   {
      bogus_sum += (sin(da_arg) + cos(1.0/da_arg));
      da_arg += inc;
   }
}

void ThreadTest::testThreadStackSize()
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
void ThreadTest::recurseConsumeResources(void* arg)
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


// ------------------------------------ //
// ---- Thread specific data stuff ---- //
// ------------------------------------ //
void ThreadTest::testThreadSpecificData()
{
   threadAssertReset();

   // Spawn off a bunch of threads (m)
   // Have each one increment counter n times
   // join all threads
   // Make sure counter is of valid value

   const int num_threads(10);
   std::vector<vpr::ThreadMemberFunctor<ThreadTest>*> functors(num_threads);
   std::vector<vpr::Thread*> threads(num_threads);
   std::vector<std::string*> thread_names(num_threads);

   for(int t=0;t<num_threads;t++)
   {
      char buffer[256];
      sprintf(buffer, "%d", t);
      thread_names[t] = new std::string(buffer);

      functors[t] = new vpr::ThreadMemberFunctor<ThreadTest>(this,&ThreadTest::tsIncCounter, thread_names[t]);

      // Spawns thread here.
      threads[t] = new vpr::Thread(functors[t]);
   }

   for(int t=0;t<num_threads;t++)
   {
      /*
      if(threads[t]->join() == false)
      {
         CPPUNIT_ASSERT(false && "Thread was not able to be joined");
      }
      */
      threads[t]->join();
      delete threads[t];
      delete functors[t];
      delete thread_names[t];
   }

   checkThreadAssertions();
}

/**
* @param arg - ptr to std::string id of thread
*/
void ThreadTest::tsIncCounter(void* arg)
{
   std::string* thread_name = static_cast<std::string*>(arg);
   std::string test_name("TSDataOverhead");
   test_name += (*thread_name);

   {
      const unsigned long IncCount(rand()%100);

      (*mTSCounter) = 0;
      vpr::System::usleep(rand()%1000);
      assertTestThread((*mTSCounter) == 0);

      try
      {
         for(unsigned long i=0;i<IncCount;i++)
         {
            (*mTSCounter) = (*mTSCounter) + 1;
            vpr::System::usleep(30);    // Sleep for 10 micro seconds
         }
         //std::cout << test_name << " ts:" << *mTSCounter << " inc:" << IncCount << std::endl;
         assertTestThread((*mTSCounter) == IncCount);
      }
      catch (...)
      {
         std::cout << "F" << std::flush;
      }
   }

   /*
   {
      const unsigned long IncCount(100000);

      (*mTSCounter) = 0;

      try
      {
         CPPUNIT_METRIC_START_TIMING();

         for(unsigned long i=0;i<IncCount;i++)
         {
            (*mTSCounter) = (*mTSCounter) + 1;
         }

         assertTestThread((*mTSCounter) == IncCount);

         CPPUNIT_METRIC_STOP_TIMING();
         CPPUNIT_ASSERT_METRIC_TIMING_LE(test_name, IncCount, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
      }
      catch (...)
      {
         std::cout << "F" << std::flush;
      }
   }
   */
}


} // End of vprTest namespace
