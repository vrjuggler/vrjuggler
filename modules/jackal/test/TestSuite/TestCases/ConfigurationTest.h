#ifndef _JCCL_CONFIGURATION_TEST_H
#define _JCCL_CONFIGURATION_TEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace jcclTest
{

// Tests out the functionality of jccl::Configuration's
class ConfigurationTest : public CppUnit::TestCase
{
public:
   ConfigurationTest() : CppUnit::TestCase()
   {
   }

   ConfigurationTest(std::string name) : CppUnit::TestCase(name)
   {
   }

   virtual ~ConfigurationTest()
   {
   }

   void testLoad();
   //void testDependencySort();
   //void testDependencySortFailure();
   void testClear();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("ConfigurationTest");
      test_suite->addTest(new CppUnit::TestCaller<ConfigurationTest>("testLoad", &ConfigurationTest::testLoad));
      //test_suite->addTest(new CppUnit::TestCaller<ConfigurationTest>("testDependencySort", &ConfigurationTest::testDependencySort));
      //test_suite->addTest(new CppUnit::TestCaller<ConfigurationTest>("testDependencySortFailure", &ConfigurationTest::testDependencySortFailure));
      test_suite->addTest(new CppUnit::TestCaller<ConfigurationTest>("testClear", &ConfigurationTest::testClear));

      return test_suite;
   }
};

}

#endif
