#ifndef _VPRTEST_LIBRARY_FINDER_TEST_H_
#define _VPRTEST_LIBRARY_FINDER_TEST_H_

#include <string>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace vprTest
{

class LibraryFinderTest : public CppUnit::TestCase
{
public:
   LibraryFinderTest();

   LibraryFinderTest(std::string name);

   virtual ~LibraryFinderTest()
   {
      ;
   }

   virtual void setUp();

   void scanTest();
   void scanAndLoadTest();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite =
         new CppUnit::TestSuite("LibraryFinderTest");

      test_suite->addTest(new CppUnit::TestCaller<LibraryFinderTest>("scanTest", &LibraryFinderTest::scanTest));
      test_suite->addTest(new CppUnit::TestCaller<LibraryFinderTest>("scanAndLoadTest", &LibraryFinderTest::scanAndLoadTest));

      return test_suite;
   }

private:
   std::string mModuleDir;
   std::string mCModuleName;
   std::string mCxxModuleName;
};

}


#endif /* _VPRTEST_LIBRARY_FINDER_TEST_H_ */
