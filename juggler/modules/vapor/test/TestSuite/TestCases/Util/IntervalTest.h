#ifndef _INTERVAL_TEST_H_
#define _INTERVAL_TEST_H_

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vpr/Util/Interval.h>


/*****************************************************************
 tests out the functionality expected of vpr::Interval
*******************************************************************/

namespace vprTest
{

class IntervalTest : public TestCase
{
public:
   IntervalTest( std::string name ) : TestCase (name)
   {
   }


   virtual ~IntervalTest()
   {
   }

   void construction()
   {
      vpr::Interval val1(21,vpr::Interval::SEC);
      assertTest(val1.sec() == 21);

      vpr::Interval val2(21,vpr::Interval::MSEC);
      assertTest(val2.msec() == 21);

      vpr::Interval val3(21,vpr::Interval::USEC);
      assertTest(val3.usec() == 21);
   }

   void testSet()
   {
      vpr::Interval val;

      val.set(21,vpr::Interval::SEC);
      assertTest(val.sec() == 21);

      val.set(21,vpr::Interval::MSEC);
      assertTest(val.msec() == 21);

      val.set(21,vpr::Interval::USEC);
      assertTest(val.usec() == 21);
   }

   void testSetf()
   {
      vpr::Interval val;

      val.setf(12.75f, vpr::Interval::SEC);
      assertTest(val.msec() == 12750);

      val.setf(12.75f, vpr::Interval::MSEC);
      assertTest(val.usec() == 12750);

      val.setf(12.75f, vpr::Interval::USEC);
      assertTest(val.usec() == 12);
   }

   void testSec()
   {
      vpr::Interval val;

      val.sec(21);
      assertTest(val.sec() == 21);
      assertTest(val.msec() == 21000);
      assertTest(val.usec() == 21000000);
   }

   void testSecf()
   {
      vpr::Interval val;

      val.secf(12.345678);

      float val_sec = val.secf();
      float val_msec = val.msecf();
      float val_usec = val.usecf();

      assertTest(val.secf()  == 12.345678f);
      assertTest(val.msecf() == 12345.678f);
      assertTest(val.usecf() == 12345678.0f);
   }

   void testMsec()
   {
      vpr::Interval val;

      val.msec(12345);
      assertTest(val.sec()  == 12);
      assertTest(val.msec() == 12345);
      assertTest(val.usec() == 12345000);
   }

   void testMsecf()
   {
      vpr::Interval val;

      val.msecf(12345.678f);

      float val_sec = val.secf();
      float val_msec = val.msecf();
      float val_usec = val.usecf();

      assertTest(val.secf()  == 12.345678f);
      assertTest(val.msecf() == 12345.678f);
      assertTest(val.usecf() == 12345678.0f);
   }

   void testUsec()
   {
      vpr::Interval val;

      val.usecf(1234567);
      assertTest(val.sec()  == 1);
      assertTest(val.msec() == 1234);
      assertTest(val.usec() == 1234567);
   }

   void testUsecf()
   {
      vpr::Interval val;

      val.usecf(12345678.0f);

      float val_sec = val.secf();
      float val_msec = val.msecf();
      float val_usec = val.usecf();


      assertTest(val.secf()  == 12.345678f);
      assertTest(val.msecf() == 12345.678f);
      assertTest(val.usecf() == 12345678.0f);
      //assertTest(false && "Got here");
   }


   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("StatusTest");

      test_suite->addTest( new TestCaller<IntervalTest>("construction", &IntervalTest::construction));
      test_suite->addTest( new TestCaller<IntervalTest>("testSet", &IntervalTest::testSet));
      test_suite->addTest( new TestCaller<IntervalTest>("testSetf", &IntervalTest::testSetf));
      test_suite->addTest( new TestCaller<IntervalTest>("testSec", &IntervalTest::testSec));
      test_suite->addTest( new TestCaller<IntervalTest>("testSecf", &IntervalTest::testSecf));
      test_suite->addTest( new TestCaller<IntervalTest>("testMsec", &IntervalTest::testMsec));
      test_suite->addTest( new TestCaller<IntervalTest>("testMsecf", &IntervalTest::testMsecf));
      test_suite->addTest( new TestCaller<IntervalTest>("testUsec", &IntervalTest::testUsec));
      test_suite->addTest( new TestCaller<IntervalTest>("testUsecf", &IntervalTest::testUsecf));

      return test_suite;
   }
};

}


#endif
