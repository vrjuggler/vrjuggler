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
      vpr::Interval val1(21,vpr::Interval::Sec);
      assertLongsEqual(val1.sec(), 21);

      vpr::Interval val2(21,vpr::Interval::Msec);
      assertLongsEqual(val2.msec(), 21);

      vpr::Interval val3(21,vpr::Interval::Usec);
      assertLongsEqual(val3.usec(), 21);
   }

   void testSet()
   {
      vpr::Interval val;

      val.set(21,vpr::Interval::Sec);
      assertLongsEqual(val.sec(), 21);

      val.set(21,vpr::Interval::Msec);
      assertLongsEqual(val.msec(), 21);

      val.set(21,vpr::Interval::Usec);
      assertLongsEqual(val.usec(), 21);
   }

   void testSetf()
   {
      vpr::Interval val;

      val.setf(12.75f, vpr::Interval::Sec);
      assertLongsEqual(val.msec(), 12750);

      val.setf(12.75f, vpr::Interval::Msec);
      assertLongsEqual(val.usec(), 12750);

      val.setf(12.75f, vpr::Interval::Usec);
      assertLongsEqual(val.usec(), 12);
   }

   void testSec()
   {
      vpr::Interval val;

      val.sec(21);
      assertLongsEqual(val.sec(), 21);
      assertLongsEqual(val.msec(), 21000);
      assertLongsEqual(val.usec(), 21000000);
   }

   void testSecf()
   {
      vpr::Interval val;

      val.secf(12.345678);

      //float val_sec = val.secf();
      //float val_msec = val.msecf();
      //float val_usec = val.usecf();

      assertDoublesEqual(val.secf() , 12.345678f, 0.000001f);
      assertDoublesEqual(val.msecf(), 12345.678f, 0.001f);
      assertDoublesEqual(val.usecf(), 12345678.0f, 1.0f);
   }

   void testMsec()
   {
      vpr::Interval val;

      val.msec(12345);
      assertLongsEqual(val.sec() , 12);
      assertLongsEqual(val.msec(), 12345);
      assertLongsEqual(val.usec(), 12345000);
   }

   void testMsecf()
   {
      vpr::Interval val;

      val.msecf(12345.678f);

      //float val_sec = val.secf();
      //float val_msec = val.msecf();
      //float val_usec = val.usecf();

      assertDoublesEqual(val.secf() , 12.345678f, 0.00001f);
      assertDoublesEqual(val.msecf(), 12345.678f, 0.001f);
      assertDoublesEqual(val.usecf(), 12345678.0f, 1.0f);
   }

   void testUsec()
   {
      vpr::Interval val;

      val.usecf(1234567);
      assertLongsEqual(val.sec() , 1);
      assertLongsEqual(val.msec(), 1234);
      assertLongsEqual(val.usec(), 1234567);
   }

   void testUsecf()
   {
      vpr::Interval val;

      val.usecf(12345678.0f);

      //float val_sec = val.secf();
      //float val_msec = val.msecf();
      //float val_usec = val.usecf();

      assertDoublesEqual(val.secf() , 12.345678f, 0.000001f);
      assertDoublesEqual(val.msecf(), 12345.678f, 0.001f);
      assertDoublesEqual(val.usecf(), 12345678.0f, 1.0f);
   }


   void testSetNowOverhead()
   {
      const vpr::Uint32 iters(100000);
      vpr::Uint32 loops = iters;

      vpr::Interval time_in, time_out;
      time_in.setNow();

      while(loops--)
      {
         time_out.setNow();
      }

      vpr::Interval diff = time_out - time_in;

      double per_call;      // Num ns per call
      per_call = (diff.usecf()*1000.0f) / double(iters);

      std::cout << "vpr::Interval::setNow: overhead = " << per_call << "ns per call\n" << std::flush;
   }

   static Test* suite()
   {
      TestSuite *test_suite = new TestSuite ("IntervalTest");

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

   static Test* metric_suite()
   {
      TestSuite *test_suite = new TestSuite ("IntervalTest_metric");

      test_suite->addTest( new TestCaller<IntervalTest>("testSetNowOverhead", &IntervalTest::testSetNowOverhead));

      return test_suite;
   }

};

}


#endif
