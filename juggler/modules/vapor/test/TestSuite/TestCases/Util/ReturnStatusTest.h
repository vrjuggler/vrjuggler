#ifndef _STATUS_TEST_H
#define _STATUS_TEST_H

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class ReturnStatusTest : public CppUnit::TestCase
{
public:
   ReturnStatusTest() : CppUnit::TestCase ()
   {
   }

   ReturnStatusTest(std::string name) : CppUnit::TestCase (name)
   {
   }

   virtual ~ReturnStatusTest()
   {
   }

   void basic();

   void helpers();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("ReturnStatusTest");

      test_suite->addTest( new CppUnit::TestCaller<ReturnStatusTest>("basic vpr::ReturnStatus Test", &ReturnStatusTest::basic));
      test_suite->addTest( new CppUnit::TestCaller<ReturnStatusTest>("Helpers for vpr::ReturnStatus test", &ReturnStatusTest::helpers));

      return test_suite;
   }
};

}

#endif
