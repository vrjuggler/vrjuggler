#ifndef _VPR_TEST_SIGNAL_TEST_H_
#define _VPR_TEST_SIGNAL_TEST_H_

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace vprTest
{

class SignalTest : public CppUnit::TestCase
{
public:
   SignalTest ()
      : CppUnit::TestCase()
   {
      /* Do nothing. */ ;
   }

   SignalTest (std::string name)
      : CppUnit::TestCase(name)
   {
      /* Do nothing. */ ;
   }

   virtual ~SignalTest (void)
   {
      /* Do nothing. */ ;
   }

   void testSegFault(void);

   static CppUnit::Test* suite ()
   {
      CppUnit::TestSuite* suite = new CppUnit::TestSuite("SignalTest");

      suite->addTest(new CppUnit::TestCaller<SignalTest>("testSegFault",
                                                         &SignalTest::testSegFault));

      return suite;
   }
};

}

#endif
