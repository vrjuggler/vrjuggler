#ifndef _DEBUG_TEST_H
#define _DEBUG_TEST_H

#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <vpr/Util/Debug.h>


/*****************************************************************
 tests out the functionality expected of vpr::Debug
*******************************************************************/

namespace vprTest
{

class DebugTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(DebugTest);
CPPUNIT_TEST( OutputDebug );
CPPUNIT_TEST_SUITE_END();

public:
   /** Just outputs some debug info at each level */
   void OutputDebug();
};

}

#endif
