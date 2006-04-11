#ifndef _CONFIG_CHUNK_DB_TEST_H
#define _CONFIG_CHUNK_DB_TEST_H

#include <string>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

namespace jcclTest
{

// Tests out the functionality of ConfigChunkDBs
class ConfigChunkDBTest : public CppUnit::TestCase
{
public:
   ConfigChunkDBTest (void) : CppUnit::TestCase()
   {
   }

   ConfigChunkDBTest (std::string name) : CppUnit::TestCase(name)
   {
   }

   virtual ~ConfigChunkDBTest (void)
   {
   }

   void testLoad(void);
   //void testDependencySort(void);
   //void testDependencySortFailure(void);
   void testClear(void);

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("ConfigChunkDBTest");
      test_suite->addTest(new CppUnit::TestCaller<ConfigChunkDBTest>("testLoad", &ConfigChunkDBTest::testLoad));
      //test_suite->addTest(new CppUnit::TestCaller<ConfigChunkDBTest>("testDependencySort", &ConfigChunkDBTest::testDependencySort));
      //test_suite->addTest(new CppUnit::TestCaller<ConfigChunkDBTest>("testDependencySortFailure", &ConfigChunkDBTest::testDependencySortFailure));
      test_suite->addTest(new CppUnit::TestCaller<ConfigChunkDBTest>("testClear", &ConfigChunkDBTest::testClear));

      return test_suite;
   }
};

}

#endif
