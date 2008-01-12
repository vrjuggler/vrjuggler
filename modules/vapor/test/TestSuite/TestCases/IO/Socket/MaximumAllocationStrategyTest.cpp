#include <vpr/IO/Socket/MaximumAllocationStrategy.h>

#include <TestCases/IO/Socket/MaximumAllocationStrategyTest.h>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(MaximumAllocationStrategyTest);

void MaximumAllocationStrategyTest::test()
{
   size_t expected;

   vpr::MaximumAllocationStrategy s;

   expected = 0;
   CPPUNIT_ASSERT_EQUAL(expected, s(0, 0));
   CPPUNIT_ASSERT_EQUAL(expected, s(0, 0));

   expected = 1;
   CPPUNIT_ASSERT_EQUAL(expected, s(0, 1));
   CPPUNIT_ASSERT_EQUAL(expected, s(1, 0));

   expected = 2;
   CPPUNIT_ASSERT_EQUAL(expected, s(1, 1));

   expected = 15;
   CPPUNIT_ASSERT_EQUAL(expected, s(5, 10));
   CPPUNIT_ASSERT_EQUAL(expected, s(10, 5));
}

}
