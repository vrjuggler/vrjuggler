#ifndef _VPR_TEST_AVERAGING_ALLOCATION_STRATEGY_TEST_H_
#define _VPR_TEST_AVERAGING_ALLOCATION_STRATEGY_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <MySuites.h>


namespace vprTest
{

class AveragingAllocationStrategyTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(AveragingAllocationStrategyTest);
CPPUNIT_TEST(testBasic);
CPPUNIT_TEST(testDefaultWindowSize);
/*
CPPUNIT_TEST(testWindowSize10);
CPPUNIT_TEST(testWindowSize1000);
*/
CPPUNIT_TEST_SUITE_END();

public:
   void testBasic();

   void testDefaultWindowSize();
/*
   void testWindowSize10();

   void testWindowSize1000();
*/
};

}


#endif /* _VPR_TEST_AVERAGING_ALLOCATION_STRATEGY_TEST_H_ */
