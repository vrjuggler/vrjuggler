#ifndef _FILE_UTILS_TEST_H
#define _FILE_UTILS_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Util/FileUtils.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class FileUtilsTest : public CppUnit::TestCase
{
public:
   FileUtilsTest() : CppUnit::TestCase ()
   {
   }

   FileUtilsTest(std::string name) : CppUnit::TestCase (name)
   {
   }

   virtual ~FileUtilsTest()
   {
   }

   void ReplaceEnvVarsTest();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("FileUtilsTest");
      test_suite->addTest( new CppUnit::TestCaller<FileUtilsTest>("ReplaceEnvVarsTest", &FileUtilsTest::ReplaceEnvVarsTest));

      return test_suite;
   }
};

}

#endif
