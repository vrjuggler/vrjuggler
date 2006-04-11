#ifndef _VPRTEST_LIBRARY_TEST_H_
#define _VPRTEST_LIBRARY_TEST_H_

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

namespace vprTest
{

class LibraryTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(LibraryTest);
CPPUNIT_TEST( namingTest );
CPPUNIT_TEST( loadTest );
CPPUNIT_TEST( unloadTest );
//CPPUNIT_TEST( libraryPathTest );
CPPUNIT_TEST( loadCSymbolTest );
CPPUNIT_TEST( loadCxxSymbolTest );
CPPUNIT_TEST_SUITE_END();

public:
   virtual void setUp();

   void namingTest();
   void loadTest();
   void unloadTest();
   void libraryPathTest();
   void loadCSymbolTest();
   void loadCxxSymbolTest();

private:
   std::string mCModuleName;
   std::string mCxxModuleName;
};

}

#endif /* _VPRTEST_LIBRARY_TEST_H_ */

