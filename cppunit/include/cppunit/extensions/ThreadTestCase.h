#ifndef _THREAD_TEST_CASE_H_
#define _THREAD_TEST_CASE_H_

#include <iostream>
#include <string>
#include <cppunit/SourceLine.h>
#include <cppunit/TestCase.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>


namespace CppUnit {

class ThreadTestCase : public TestCase
{
protected:
   struct assertData
   {
      std::string         cond_exp;
      CppUnit::SourceLine source_line;
   };


public:
   ThreadTestCase ()
       : TestCase()
   {
      /* Do nothing. */ ;
   }

   ThreadTestCase (std::string name)
       : TestCase(name)
   {
      /* Do nothing. */ ;
   }

   virtual ~ThreadTestCase ()
   {
      /* Do nothing. */ ;
   }

   void assertThreadImplementation (bool condition,
                                    std::string conditionExpression,
                                    CppUnit::SourceLine sourceLine)
   {
      vpr::Guard<vpr::Mutex> assert_guard(mAssertionMutex);

      if(!condition)
      {
         assertData info;
         info.cond_exp    = conditionExpression;
         info.source_line = sourceLine;
         mAssertions.push_back(info);
      }

   }


   // Reset the assertion info
   void threadAssertReset()
   {
   vpr::Guard<vpr::Mutex> assert_guard(mAssertionMutex);

      mAssertions.clear();      // Clear the assertions
   }

   // Check the assertions and throw them if need be
   void checkThreadAssertions()
   {
   vpr::Guard<vpr::Mutex> assert_guard(mAssertionMutex);

      for(unsigned i=0;i<mAssertions.size();i++)
      {
         assertData info = mAssertions[i];
         CppUnit::Asserter::failIf(true, info.cond_exp, info.source_line);
      }
   }


protected:
      std::vector<assertData>    mAssertions;      // A list of assertions that have happend
      vpr::Mutex                 mAssertionMutex;  // Mutex to protect the assertion list
};

}

#ifdef CPPUNIT_SOURCEANNOTATION
#    undef assertTestThread
#    define assertTestThread(condition) (this->assertThreadImplementation ((condition),(#condition),CPPUNIT_SOURCELINE()))
#else
#    undef assertTestThread
#    define assertTestThread(condition) (this->assertThreadImplementation ((condition),"", CPPUNIT_SOURCELINE()))
#endif


#endif
