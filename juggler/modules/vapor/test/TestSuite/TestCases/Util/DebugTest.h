#ifndef _DEBUG_TEST_H
#define _DEBUG_TEST_H

#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Util/Debug.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class DebugTest : public CppUnit::TestCase
{
public:
   DebugTest() : CppUnit::TestCase ()
   {
   }

   DebugTest(std::string name) : CppUnit::TestCase (name)
   {
   }

   virtual ~DebugTest()
   {
   }

   /** Just outputs some debug info at each level */
   void OutputDebug();
      
   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("DebugTest");
      test_suite->addTest( new CppUnit::TestCaller<DebugTest>("OutputDebug", &DebugTest::OutputDebug));
            
      return test_suite;
   }
};

}

#endif
