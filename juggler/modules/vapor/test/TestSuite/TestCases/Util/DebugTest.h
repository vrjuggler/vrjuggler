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
   void OutputDebug()
   {
      vprDEBUG(vprDBG_ALL,0) << " Test output for vprDBG_ALL \n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ERROR,0) << " Test output for vprDBG_ERROR \n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_SIM,0) << " Test output for vprDBG_SIM \n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_VPR,0) << " Test output for vprDBG_VPR \n" << vprDEBUG_FLUSH;
   }
      
   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("DebugTest");
      test_suite->addTest( new CppUnit::TestCaller<DebugTest>("OutputDebug", &DebugTest::OutputDebug));
            
      return test_suite;
   }
};

}

#endif
