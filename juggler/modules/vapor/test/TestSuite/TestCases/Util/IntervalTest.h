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

class IntervalTest : public CppUnit::TestCase
{
public:
   IntervalTest() : CppUnit::TestCase ()
   {
   }


   virtual ~IntervalTest()
   {
   }

   void construction()
   {
      vpr::Interval val1(21,vpr::Interval::Sec);
      CppUnit::TestAssert::assertEquals<long>(val1.sec(), 21);

      vpr::Interval val2(21,vpr::Interval::Msec);
      CppUnit::TestAssert::assertEquals<long>(val2.msec(), 21);

      vpr::Interval val3(21,vpr::Interval::Usec);
      CppUnit::TestAssert::assertEquals<long>(val3.usec(), 21);
   }

   void testSet()
   {
      vpr::Interval val;

      val.set(21,vpr::Interval::Sec);
      CppUnit::TestAssert::assertEquals<long>(val.sec(), 21);

      val.set(21,vpr::Interval::Msec);
      CppUnit::TestAssert::assertEquals<long>(val.msec(), 21);

      val.set(21,vpr::Interval::Usec);
      CppUnit::TestAssert::assertEquals<long>(val.usec(), 21);
   }

   void testSetf()
   {
      vpr::Interval val;

      val.setf(12.75f, vpr::Interval::Sec);
      CppUnit::TestAssert::assertEquals<long>(val.msec(), 12750);

      val.setf(12.75f, vpr::Interval::Msec);
      CppUnit::TestAssert::assertEquals<long>(val.usec(), 12750);

      val.setf(12.75f, vpr::Interval::Usec);
      CppUnit::TestAssert::assertEquals<long>(val.usec(), 12);
   }

   void testSec()
   {
      vpr::Interval val;

      val.sec(21);
      CppUnit::TestAssert::assertEquals<long>(val.sec(), 21);
      CppUnit::TestAssert::assertEquals<long>(val.msec(), 21000);
      CppUnit::TestAssert::assertEquals<long>(val.usec(), 21000000);
   }

   void testSecf()
   {
      vpr::Interval val;

      val.secf(12.345678);

      //float val_sec = val.secf();
      //float val_msec = val.msecf();
      //float val_usec = val.usecf();

      CppUnit::TestAssert::assertEquals<double>(val.secf() , 12.345678f, 0.000001f);
      CppUnit::TestAssert::assertEquals<double>(val.msecf(), 12345.678f, 0.001f);
      CppUnit::TestAssert::assertEquals<double>(val.usecf(), 12345678.0f, 1.0f);
   }

   void testMsec()
   {
      vpr::Interval val;

      val.msec(12345);
      CppUnit::TestAssert::assertEquals<long>(val.sec() , 12);
      CppUnit::TestAssert::assertEquals<long>(val.msec(), 12345);
      CppUnit::TestAssert::assertEquals<long>(val.usec(), 12345000);
   }

   void testMsecf()
   {
      vpr::Interval val;

      val.msecf(12345.678f);

      //float val_sec = val.secf();
      //float val_msec = val.msecf();
      //float val_usec = val.usecf();

      CppUnit::TestAssert::assertEquals<double>(val.secf() , 12.345678f, 0.00001f);
      CppUnit::TestAssert::assertEquals<double>(val.msecf(), 12345.678f, 0.001f);
      CppUnit::TestAssert::assertEquals<double>(val.usecf(), 12345678.0f, 1.0f);
   }

   void testUsec()
   {
      vpr::Interval val;

      val.usecf(1234567);
      CppUnit::TestAssert::assertEquals<long>(val.sec() , 1);
      CppUnit::TestAssert::assertEquals<long>(val.msec(), 1234);
      CppUnit::TestAssert::assertEquals<long>(val.usec(), 1234567);
   }

   void testUsecf()
   {
      vpr::Interval val;

      val.usecf(12345678.0f);

      //float val_sec = val.secf();
      //float val_msec = val.msecf();
      //float val_usec = val.usecf();

      CppUnit::TestAssert::assertEquals<double>(val.secf() , 12.345678f, 0.000001f);
      CppUnit::TestAssert::assertEquals<double>(val.msecf(), 12345.678f, 0.001f);
      CppUnit::TestAssert::assertEquals<double>(val.usecf(), 12345678.0f, 1.0f);
   }

   void testSubtract()
   {
      vpr::Interval val1;
      vpr::Interval val2;
      vpr::Interval diff;

      val1.set(5000000,vpr::Interval::Usec);
      val2.set(5000100,vpr::Interval::Usec);

      diff = val2 - val1;

      CppUnit::TestAssert::assertEquals<long>(diff.usec(), 100);
      CppUnit::TestAssert::assertEquals<double>(diff.secf(), .000100f, 0.0001f);
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

   void registerTests (CppUnit::TestSuite* suite)
   {
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("construction", &IntervalTest::construction));
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testSet", &IntervalTest::testSet));
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testSetf", &IntervalTest::testSetf));
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testSec", &IntervalTest::testSec));
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testSecf", &IntervalTest::testSecf));
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testMsec", &IntervalTest::testMsec));
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testMsecf", &IntervalTest::testMsecf));
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testUsec", &IntervalTest::testUsec));
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testUsecf", &IntervalTest::testUsecf));
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testSubtract", &IntervalTest::testSubtract));
   }

   void registerMetricsTests (CppUnit::TestSuite* suite)
   {
      suite->addTest( new CppUnit::TestCaller<IntervalTest>("testSetNowOverhead", &IntervalTest::testSetNowOverhead));
   }

};

}


#endif
