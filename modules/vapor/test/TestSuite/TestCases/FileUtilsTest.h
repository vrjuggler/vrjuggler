#ifndef _FILE_UTILS_TEST_H
#define _FILE_UTILS_TEST_H

#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <vpr/Util/FileUtils.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class FileUtilsTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(FileUtilsTest);
CPPUNIT_TEST( ReplaceEnvVarsTest );
CPPUNIT_TEST_SUITE_END();

public:
   virtual ~FileUtilsTest()
   {
   }

   void ReplaceEnvVarsTest();
};

}

#endif
