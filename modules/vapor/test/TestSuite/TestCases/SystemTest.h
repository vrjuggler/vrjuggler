#ifndef _SYSTEM_TEST_H
#define _SYSTEM_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class SystemTest : public CppUnit::TestCase
{
public:
   SystemTest() : CppUnit::TestCase ()
   {
   }

   SystemTest(std::string name) : CppUnit::TestCase (name)
   {
   }

   virtual ~SystemTest()
   {
   }

   void ShortConversions();

   void LongConversions();

   void LongLongConversions();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("SystemTest");
      test_suite->addTest( new CppUnit::TestCaller<SystemTest>("ShortConversions", &SystemTest::ShortConversions));
      test_suite->addTest( new CppUnit::TestCaller<SystemTest>("LongConversions", &SystemTest::LongConversions));
      test_suite->addTest( new CppUnit::TestCaller<SystemTest>("LongLongConversions", &SystemTest::LongLongConversions));

      return test_suite;
   }
};

}

#endif
