#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <vpr/Util/AttributeMapBase.h>
#include <TestCases/Util/AttribMapTest.h>


/*****************************************************************
 tests out the functionality expected of vpr::AttibuteMapBase
*******************************************************************/

namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION( AttribMapTest );

/** Test existence tests */
void AttribMapTest::testExists()
{
   vpr::AttributeMapBase  attrib_map;
   
   CPPUNIT_ASSERT( !attrib_map.attribExists("test_val"));
   
   attrib_map.setAttrib("test_val", 12.21f);

   CPPUNIT_ASSERT( attrib_map.attribExists("test_val"));
}

}

