#ifndef _VPRTEST_SIM_SELECTOR_TEST_H_
#define _VPRTEST_SIM_SELECTOR_TEST_H_

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace vprTest
{

class SimSelectorTest : public CppUnit::TestCase {
public:
   SimSelectorTest (std::string name)
      : CppUnit::TestCase(name)
   {
      /* Do nothing. */ ;
   }

   SimSelectorTest ()
      : CppUnit::TestCase()
   {
      /* Do nothing. */ ;
   }

   virtual ~SimSelectorTest (void) {
      /* Do nothing. */ ;
   }

   void singleThreadTest(void);

   static CppUnit::Test*
   suite (void) {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("SimSelectorTest");
      test_suite->addTest( new CppUnit::TestCaller<SimSelectorTest>("singleThreadTest", &SimSelectorTest::singleThreadTest));

      return test_suite;
   }
};

}

#endif
