#ifndef _VPR_TEST_MAXIMUM_ALLOCATION_STRATEGY_TEST_H_
#define _VPR_TEST_MAXIMUM_ALLOCATION_STRATEGY_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <MySuites.h>


namespace vprTest
{

class MaximumAllocationStrategyTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(MaximumAllocationStrategyTest);
CPPUNIT_TEST(test);
CPPUNIT_TEST_SUITE_END();

public:
   void test();
};

}


#endif /* _VPR_TEST_MAXIMUM_ALLOCATION_STRATEGY_TEST_H_ */
