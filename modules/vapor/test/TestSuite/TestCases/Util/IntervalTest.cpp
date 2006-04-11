#include <vpr/vpr.h>
#include <vpr/Util/Interval.h>

#include <TestCases/Util/IntervalTest.h>

namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( IntervalTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( IntervalMetricTest, MySuites::metric() );

void IntervalTest::construction()
{
   vpr::Interval val1(21,vpr::Interval::Sec);
   CPPUNIT_ASSERT_EQUAL(val1.sec(), (vpr::Uint64) 21);

   vpr::Interval val2(21,vpr::Interval::Msec);
   CPPUNIT_ASSERT_EQUAL(val2.msec(), (vpr::Uint64) 21);

   vpr::Interval val3(20,vpr::Interval::Usec);
   CPPUNIT_ASSERT_EQUAL(val3.usec(), (vpr::Uint64) 20);
}

void IntervalTest::testSet()
{
   vpr::Interval val;

   val.set(21,vpr::Interval::Sec);
   CPPUNIT_ASSERT_EQUAL(val.sec(), (vpr::Uint64) 21);

   val.set(21,vpr::Interval::Msec);
   CPPUNIT_ASSERT_EQUAL(val.msec(), (vpr::Uint64) 21);

   val.set(20,vpr::Interval::Usec);
   CPPUNIT_ASSERT_EQUAL(val.usec(), (vpr::Uint64) 20);
}

void IntervalTest::testSetf()
{
   vpr::Interval val;

   val.setf(12.75f, vpr::Interval::Sec);
   CPPUNIT_ASSERT_EQUAL(val.msec(), (vpr::Uint64) 12750);

   val.setf(12.75f, vpr::Interval::Msec);
   CPPUNIT_ASSERT_EQUAL(val.usec(), (vpr::Uint64) 12750);

   val.setf(20.75f, vpr::Interval::Usec);
   CPPUNIT_ASSERT_EQUAL(val.usec(), (vpr::Uint64) 20);
}

void IntervalTest::testSetd()
{
   vpr::Interval val;

   val.setd(12.75f, vpr::Interval::Sec);
   CPPUNIT_ASSERT_EQUAL(val.msec(), (vpr::Uint64) 12750);

   val.setd(12.75f, vpr::Interval::Msec);
   CPPUNIT_ASSERT_EQUAL(val.usec(), (vpr::Uint64) 12750);

   val.setd(20.75f, vpr::Interval::Usec);
   CPPUNIT_ASSERT_EQUAL(val.usec(), (vpr::Uint64) 20);
}

void IntervalTest::testSec()
{
   vpr::Interval val;

   val.sec(21);
   CPPUNIT_ASSERT_EQUAL(val.sec(), (vpr::Uint64) 21);
   CPPUNIT_ASSERT_EQUAL(val.msec(), (vpr::Uint64) 21000);
   CPPUNIT_ASSERT_EQUAL(val.usec(), (vpr::Uint64) 21000000);
}

void IntervalTest::testSecf()
{
   vpr::Interval val;

   val.secf(12.34567);

   //float val_sec = val.secf();
   //float val_msec = val.msecf();
   //float val_usec = val.usecf();

   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.secf() , 12.34567f, 0.00001f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.msecf(), 12345.67f, 0.01f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.usecf(), 12345678.0f, 100.0f);
}

void IntervalTest::testSecd()
{
   vpr::Interval val;

   val.secd(12.34567);

   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.secd() , 12.34567f, 0.00001f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.msecd(), 12345.67f, 0.01f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.usecd(), 12345678.0f, 100.0f);
}

void IntervalTest::testMsec()
{
   vpr::Interval val;

   val.msec(12345);
   CPPUNIT_ASSERT_EQUAL(val.sec(), (vpr::Uint64) 12);
   CPPUNIT_ASSERT_EQUAL(val.msec(), (vpr::Uint64) 12345);
   CPPUNIT_ASSERT_EQUAL(val.usec(), (vpr::Uint64) 12345000);
}

void IntervalTest::testMsecf()
{
   vpr::Interval val;

   val.msecf(12345.678f);

   //float val_sec = val.secf();
   //float val_msec = val.msecf();
   //float val_usec = val.usecf();

   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.secf() , 12.345678f, 0.0001f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.msecf(), 12345.678f, 0.01f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.usecf(), 12345678.0f, 10.0f);
}

void IntervalTest::testMsecd()
{
   vpr::Interval val;

   val.msecd(12345.678f);

   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.secd() , 12.345678f, 0.0001f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.msecd(), 12345.678f, 0.01f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.usecd(), 12345678.0f, 10.0f);
}

void IntervalTest::testUsec()
{
   vpr::Interval val;

   val.usecf(1234560);
   CPPUNIT_ASSERT_EQUAL(val.sec() , (vpr::Uint64) 1);
   CPPUNIT_ASSERT_EQUAL(val.msec(), (vpr::Uint64) 1234);
   CPPUNIT_ASSERT_EQUAL(val.usec(), (vpr::Uint64) 1234560);
}

void IntervalTest::testUsecf()
{
   vpr::Interval val;

   val.usecf(12345678.0f);

   //float val_sec = val.secf();
   //float val_msec = val.msecf();
   //float val_usec = val.usecf();

   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.secf() , 12.345678f, 0.00001f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.msecf(), 12345.678f, 0.01f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.usecf(), 12345678.0f, 10.0f);
}

void IntervalTest::testUsecd()
{
   vpr::Interval val;

   val.usecd(12345678.0f);

   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.secd() , 12.345678f, 0.00001f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.msecd(), 12345.678f, 0.01f);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(val.usecd(), 12345678.0f, 10.0f);
}

void IntervalTest::testSubtract()
{
   vpr::Interval val1;
   vpr::Interval val2;
   vpr::Interval diff;

   val1.set(5000000,vpr::Interval::Usec);
   val2.set(5000100,vpr::Interval::Usec);

   diff = val2 - val1;

   CPPUNIT_ASSERT_EQUAL(diff.usec(), (vpr::Uint64) 100);
   CPPUNIT_ASSERT_DOUBLES_EQUAL(diff.secf(), .000100f, 0.0001f);
}

void IntervalTest::testLessThen()
{
   vpr::Interval val1, val2;

   val1.set(5, vpr::Interval::Sec);
   val2.set(10, vpr::Interval::Sec);
   CPPUNIT_ASSERT(val1 < val2);
   CPPUNIT_ASSERT(!(val2 < val1));

   CPPUNIT_ASSERT(val1 <= val1);
   CPPUNIT_ASSERT(!(val1 < val1));

   //
   // |--------------------|-----------------------|
   //  v3               v1   v2
   //

   vpr::Interval offset(10, vpr::Interval::Msec);
   val1 = (vpr::Interval::HalfPeriod - offset);
   val2 = (vpr::Interval::HalfPeriod + offset);

   CPPUNIT_ASSERT(val1 < val2);
   CPPUNIT_ASSERT(! (val2 < val1));
   CPPUNIT_ASSERT(val1 <= val2);

   vpr::Interval val3(3, vpr::Interval::Usec);
   CPPUNIT_ASSERT(val3 < val1);
   CPPUNIT_ASSERT(val3 < val1);

   CPPUNIT_ASSERT(val3 <= val1);
   CPPUNIT_ASSERT(val3 <= val2);
}

void IntervalMetricTest::testSetNowOverhead()
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

} // End of vprTest namespace
