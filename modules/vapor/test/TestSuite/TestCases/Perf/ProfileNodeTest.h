#ifndef _VPRTEST_PROFILE_NODE_TEST_H_
#define _VPRTEST_PROFILE_NODE_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>
#include <cppunit/extensions/MetricRegistry.h>

namespace vprTest
{

class ProfileNodeTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ProfileNodeTest);
CPPUNIT_TEST( testConstructionAndBasicInterface );
CPPUNIT_TEST( testSamplingInterface );
CPPUNIT_TEST_SUITE_END();

public:
   void testConstructionAndBasicInterface();
   void testSamplingInterface();
};


class ProfileNodeMetricTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ProfileNodeMetricTest);
CPPUNIT_TEST( testOverhead );
CPPUNIT_TEST_SUITE_END();

public:
   void testOverhead();
};

}

#endif

