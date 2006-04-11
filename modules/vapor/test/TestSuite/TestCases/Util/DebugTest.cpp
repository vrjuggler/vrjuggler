#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/Util/Debug.h>

#include <TestCases/Util/DebugTest.h>

/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

   CPPUNIT_TEST_SUITE_REGISTRATION( DebugTest );

   /** Just outputs some debug info at each level */
   void DebugTest::OutputDebug()
   {
      vprDEBUG(vprDBG_ALL,0) << " Test output for vprDBG_ALL \n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ERROR,0) << " Test output for vprDBG_ERROR \n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_SIM,0) << " Test output for vprDBG_SIM \n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_VPR,0) << " Test output for vprDBG_VPR \n" << vprDEBUG_FLUSH;
   }
      

}

