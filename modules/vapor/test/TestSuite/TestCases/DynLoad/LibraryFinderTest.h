#ifndef _VPRTEST_LIBRARY_FINDER_TEST_H_
#define _VPRTEST_LIBRARY_FINDER_TEST_H_

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

namespace vprTest
{

class LibraryFinderTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(LibraryFinderTest);
CPPUNIT_TEST( scanTest );
CPPUNIT_TEST( scanAndLoadTest );
CPPUNIT_TEST_SUITE_END();

public:
   LibraryFinderTest();
      
   virtual ~LibraryFinderTest()
   {;}

   virtual void setUp();

   void scanTest();
   void scanAndLoadTest();

private:
   std::string mModuleDir;
   std::string mCModuleName;
   std::string mCxxModuleName;
};

}

#endif /* _VPRTEST_LIBRARY_FINDER_TEST_H_ */

