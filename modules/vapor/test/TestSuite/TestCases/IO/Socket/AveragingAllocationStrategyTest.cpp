#include <cstdlib>
#include <algorithm>
#include <vector>
#include <boost/bind.hpp>

#include <vpr/IO/Socket/AveragingAllocationStrategy.h>

#include <TestCases/IO/Socket/AveragingAllocationStrategyTest.h>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION(AveragingAllocationStrategyTest);

template<typename T>
inline T maxValue(const T v1, const T v2)
{
   return v1 > v2 ? v1 : v2;
}

size_t genValue(const int minValue, const int maxValue)
{
   const float scaled_value(static_cast<float>(std::rand()) / RAND_MAX);
   return (scaled_value * (maxValue - minValue)) + minValue;
}

void AveragingAllocationStrategyTest::testBasic()
{
   // Generate 100 random input write sizes in the range [25,500].
   std::vector<size_t> inputs(100, 0);
   std::generate(inputs.begin(), inputs.end(), boost::bind(genValue, 25, 500));

   vpr::AveragingAllocationStrategy s;
   size_t cur_total(0);

   for ( unsigned int i = 0; i < inputs.size(); ++i )
   {
      // Get the allocation size for the case of a buffer that is currently
      // empty.
      const size_t result = s(0, inputs[i]);
      cur_total += result;

      // Keep track of the current average of allocation sizes.
      const size_t average(cur_total / (i + 1));

      // The allocation size for this strategy is the maximum of the requested
      // size and the current average.
      CPPUNIT_ASSERT_EQUAL(result, maxValue(inputs[i], average));
   }
}

void AveragingAllocationStrategyTest::testDefaultWindowSize()
{
   size_t buffer_size(0);

   vpr::AveragingAllocationStrategy s;

   std::vector<size_t> expected(10);
   expected[0] = 50;
   expected[1] = 100;
   expected[2] = 200;
   expected[3] = 400;
   expected[4] = 800;
   expected[5] = 1600;
   expected[6] = 3200;
   expected[7] = 6400;
   expected[8] = 12800;
   expected[9] = 25600;

   size_t writes(0);

   for ( unsigned int i = 0; i < 10; ++i, ++writes )
   {
      const size_t result = s(buffer_size, 50);
      CPPUNIT_ASSERT_EQUAL(result, expected[i]);
      buffer_size += result;
   }

   const size_t average(buffer_size / writes);
   const size_t result = s(0, 50);
   CPPUNIT_ASSERT_EQUAL(result, average);
}
/*
void AveragingAllocationStrategyTest::testWindowSize10()
{
   vpr::AveragingAllocationStrategy s(10);
}

void AveragingAllocationStrategyTest::testWindowSize1000()
{
   vpr::AveragingAllocationStrategy s(1000);
}
*/
}
