#include <vpr/IO/Socket/SimpleAllocationStrategies.h>

#include <TestCases/IO/Socket/SimpleAllocationStrategiesTest.h>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(SimpleAllocationStrategiesTest);

void SimpleAllocationStrategiesTest::testMinimalAlloc()
{
   size_t expected;

   expected = 0;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::minimalAllocationStrategy(0, 0));

   expected = 1;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::minimalAllocationStrategy(0, 1));
   CPPUNIT_ASSERT_EQUAL(expected, vpr::minimalAllocationStrategy(1, 0));

   expected = 2;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::minimalAllocationStrategy(1, 1));

   expected = 15;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::minimalAllocationStrategy(5, 10));
   CPPUNIT_ASSERT_EQUAL(expected, vpr::minimalAllocationStrategy(10, 5));
}

void SimpleAllocationStrategiesTest::testDoublingAlloc()
{
   size_t expected;

   expected = 0;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(0, 0));

   expected = 2;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(0, 1));
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(1, 0));

   expected = 4;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(1, 1));
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(2, 1));

   expected = 6;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(1, 2));
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(3, 1));

   expected = 8;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(1, 3));
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(4, 3));

   expected = 20;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(10, 5));
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(5, 5));

   expected = 30;
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(5, 10));
   CPPUNIT_ASSERT_EQUAL(expected, vpr::doublingAllocationStrategy(15, 5));
}

}
