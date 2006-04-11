#ifndef _VPRTEST_LIBRARY_TEST_H_
#define _VPRTEST_LIBRARY_TEST_H_

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

namespace vprTest
{

class DsoLoaderTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(DsoLoaderTest);
CPPUNIT_TEST(loadingConstructorTest);
CPPUNIT_TEST(loadTest);
CPPUNIT_TEST(setInitAndLoadTest);
CPPUNIT_TEST(multiLoadTest);
CPPUNIT_TEST(loadFailureTest);
CPPUNIT_TEST(lookupFailureTest);
//CPPUNIT_TEST(loadFromDirectoryTest);
CPPUNIT_TEST_SUITE_END();

public:
   virtual void setUp();

   void loadingConstructorTest();
   void loadTest();
   void setInitAndLoadTest();
   void multiLoadTest();
   void loadFailureTest();
   void lookupFailureTest();
//   void loadFromDirectoryTest();

private:
   std::string mModuleName[2];
};

}


#endif /* _VPRTEST_LIBRARY_TEST_H_ */
