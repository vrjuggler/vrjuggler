#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Util/Debug.h>

#include <TestCases/Util/DebugTest.h>

/*****************************************************************
 tests out the functionality expected of vpr::Debug
*******************************************************************/

namespace vprTest
{

   CPPUNIT_TEST_SUITE_REGISTRATION( DebugTest );

   /** Just outputs some debug info at each level */
   void DebugTest::OutputDebug()
   {
      vprDEBUG(vprDBG_ALL,   vprDBG_CRITICAL_LVL) << " Test output for vprDBG_ALL \n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL) << " Test output for vprDBG_ERROR \n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_SIM,   vprDBG_CRITICAL_LVL) << " Test output for vprDBG_SIM \n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_VPR,   vprDBG_CRITICAL_LVL) << " Test output for vprDBG_VPR \n" << vprDEBUG_FLUSH;
   }
      

}

