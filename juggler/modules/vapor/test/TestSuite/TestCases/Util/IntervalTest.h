#ifndef _INTERVAL_TEST_H_
#define _INTERVAL_TEST_H_

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>



/*****************************************************************
 tests out the functionality expected of vpr::Interval
*******************************************************************/

namespace vprTest
{

class IntervalTest : public CppUnit::TestCase
{
public:
   IntervalTest() : CppUnit::TestCase ()
   {
   }

   IntervalTest(std::string name) : CppUnit::TestCase (name)
   {
   }

   virtual ~IntervalTest()
   {
   }

   void construction();

   void testSet();

   void testSetf();

   void testSetd();

   void testSec();

   void testSecf();

   void testSecd();

   void testMsec();

   void testMsecf();

   void testMsecd();

   void testUsec();

   void testUsecf();

   void testUsecd();

   void testSubtract();

   void testLessThen();



   void testSetNowOverhead();

   static CppUnit::Test* suite();

   static CppUnit::Test* metric_suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("IntervalTest_metric");

      test_suite->addTest( new CppUnit::TestCaller<IntervalTest>("testSetNowOverhead", &IntervalTest::testSetNowOverhead));

      return test_suite;
   }

};

}


#endif
