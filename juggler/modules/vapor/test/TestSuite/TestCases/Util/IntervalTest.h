#ifndef _INTERVAL_TEST_H_
#define _INTERVAL_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

/*****************************************************************
 tests out the functionality expected of vpr::Interval
*******************************************************************/
namespace vprTest
{

class IntervalTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(IntervalTest);
CPPUNIT_TEST( construction );
CPPUNIT_TEST( testSet );
CPPUNIT_TEST( testSetf );
CPPUNIT_TEST( testSetd );
CPPUNIT_TEST( testSec );
CPPUNIT_TEST( testSecf );
CPPUNIT_TEST( testSecd );
CPPUNIT_TEST( testMsec );
CPPUNIT_TEST( testMsecf );
CPPUNIT_TEST( testMsecd );
CPPUNIT_TEST( testUsec );
CPPUNIT_TEST( testUsecf );
CPPUNIT_TEST( testUsecd );
CPPUNIT_TEST( testSubtract );
CPPUNIT_TEST( testLessThen );
CPPUNIT_TEST_SUITE_END();

public:
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
};

class IntervalMetricTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(IntervalMetricTest);
CPPUNIT_TEST( testSetNowOverhead );
CPPUNIT_TEST_SUITE_END();

public:
   void testSetNowOverhead();
};


}


#endif
