#ifndef _VPRTEST_LIBRARY_TEST_H_
#define _VPRTEST_LIBRARY_TEST_H_

#include <string>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


namespace vprTest
{

class LibraryTest : public CppUnit::TestCase
{
public:
   LibraryTest(): CppUnit::TestCase()
   {
      ;
   }

   LibraryTest(std::string name): CppUnit::TestCase(name)
   {
      ;
   }

   virtual ~LibraryTest()
   {
      ;
   }

   virtual void setUp();

   void namingTest();
   void loadTest();
   void unloadTest();
   void libraryPathTest();
   void loadCSymbolTest();
   void loadCxxSymbolTest();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("LibraryTest");

      test_suite->addTest(new CppUnit::TestCaller<LibraryTest>("namingTest", &LibraryTest::namingTest));
      test_suite->addTest(new CppUnit::TestCaller<LibraryTest>("loadTest", &LibraryTest::loadTest));
      test_suite->addTest(new CppUnit::TestCaller<LibraryTest>("unloadTest", &LibraryTest::unloadTest));
//      test_suite->addTest(new CppUnit::TestCaller<LibraryTest>("libraryPathTest", &LibraryTest::libraryPathTest));
      test_suite->addTest(new CppUnit::TestCaller<LibraryTest>("loadCSymbolTest", &LibraryTest::loadCSymbolTest));
      test_suite->addTest(new CppUnit::TestCaller<LibraryTest>("loadCxxSymbolTest", &LibraryTest::loadCxxSymbolTest));

      return test_suite;
   }

private:
   std::string mCModuleName;
   std::string mCxxModuleName;
};

}


#endif /* _VPRTEST_LIBRARY_TEST_H_ */
