#ifndef _VPRTEST_LIBRARY_TEST_H_
#define _VPRTEST_LIBRARY_TEST_H_

#include <string>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

namespace vprTest
{

class LibraryLoaderTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(LibraryLoaderTest);
CPPUNIT_TEST(loadAndInitTestRaw);
CPPUNIT_TEST(loadAndInitTestFunctor);
CPPUNIT_TEST(findAndInitTest);
CPPUNIT_TEST(multiLoadTest);
CPPUNIT_TEST(loadFailureTest);
CPPUNIT_TEST(lookupFailureTest);
CPPUNIT_TEST(findFailureTest);
CPPUNIT_TEST_SUITE_END();

public:
   virtual void setUp();

   void loadAndInitTestRaw();
   void loadAndInitTestFunctor();
   void findAndInitTest();
   void multiLoadTest();
   void loadFailureTest();
   void lookupFailureTest();
   void findFailureTest();

private:
   std::pair<std::string, std::string> mModules[2];
};

}


#endif /* _VPRTEST_LIBRARY_TEST_H_ */
