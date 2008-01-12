#ifndef _VPR_TEST_SIMPLE_ALLOCATION_STRATEGIES_TEST_H_
#define _VPR_TEST_SIMPLE_ALLOCATION_STRATEGIES_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <MySuites.h>


namespace vprTest
{

class SimpleAllocationStrategiesTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SimpleAllocationStrategiesTest);
CPPUNIT_TEST(testMinimalAlloc);
CPPUNIT_TEST(testDoublingAlloc);
CPPUNIT_TEST_SUITE_END();

public:
   void testMinimalAlloc();

   void testDoublingAlloc();
};

}


#endif /* _VPR_TEST_SIMPLE_ALLOCATION_STRATEGIES_TEST_H_ */
