#ifndef _JCCL_CONFIGURATION_TEST_H
#define _JCCL_CONFIGURATION_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace jcclTest
{

// Tests out the functionality of the class jccl::Configuration.
class ConfigurationTest : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE(ConfigurationTest);
   CPPUNIT_TEST(Load);
   CPPUNIT_TEST(Clear);
   CPPUNIT_TEST_SUITE_END();

public:
   virtual ~ConfigurationTest()
   {
   }

   void Load();
   //void testDependencySort();
   //void testDependencySortFailure();
   void Clear();
};

}

#endif
