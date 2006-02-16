#ifndef _VPRTEST_PERF_TEST_H_
#define _VPRTEST_PERF_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>
#include <cppunit/extensions/MetricRegistry.h>
#include <vpr/Sync/Mutex.h>

/*****************************************************************
 tests out the functionality expected of vapor's PerfMon lib
*******************************************************************/
namespace vprTest
{

class PerfTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(PerfTest);
CPPUNIT_TEST( testConstructTree );
CPPUNIT_TEST( testNamedLookupSample );
CPPUNIT_TEST( testReset );
CPPUNIT_TEST( testMultithreading );
CPPUNIT_TEST_SUITE_END();

public:
   void testNamedLookupSample();
   void testConstructTree();
   void testReset();
   void testMultithreading();

   void createSamples();

public:
   vpr::Mutex  mOutputLock;
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

