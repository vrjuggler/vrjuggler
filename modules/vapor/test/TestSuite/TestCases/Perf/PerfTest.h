#ifndef _VPRTEST_PERF_TEST_H_
#define _VPRTEST_PERF_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>
#include <cppunit/extensions/MetricRegistry.h>

/*****************************************************************
 tests out the functionality expected of vpr::GUID
*******************************************************************/
namespace vprTest
{

class PerfTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(PerfTest);
CPPUNIT_TEST( testConstructTree );
CPPUNIT_TEST( testReset );
CPPUNIT_TEST_SUITE_END();

public:
   void testConstructTree ();
   void testReset ();
};


class PerfMetricTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(PerfMetricTest);
CPPUNIT_TEST( testTreeOverhead );
CPPUNIT_TEST_SUITE_END();

public:
   void testTreeOverhead();
};

}

#endif

