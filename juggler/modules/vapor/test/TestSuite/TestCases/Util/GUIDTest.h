#ifndef _VPRTEST_GUID_TEST_H_
#define _VPRTEST_GUID_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>
#include <cppunit/extensions/MetricRegistry.h>

/*****************************************************************
 tests out the functionality expected of vpr::GUID
*******************************************************************/
namespace vprTest
{

class GUIDTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(GUIDTest);
CPPUNIT_TEST( testConstructor );
CPPUNIT_TEST( testCompare );
CPPUNIT_TEST_SUITE_END();

public:
   void testConstructor ();
   void testCompare ();
};


class GUIDMetricTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(GUIDMetricTest);
CPPUNIT_TEST( testCreationOverhead );
CPPUNIT_TEST( testDebugOutput );
CPPUNIT_TEST_SUITE_END();

public:
   void testCreationOverhead();
   void testDebugOutput();  
};

}

#endif

