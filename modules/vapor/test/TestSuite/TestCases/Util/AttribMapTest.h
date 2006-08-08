#ifndef _ATTRIB_MAP_TEST_H
#define _ATTRIB_MAP_TEST_H

#include <iostream>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <vpr/Util/AttributeMapBase.h>
#include <vpr/vpr.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class AttribMapTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(AttribMapTest);
#if defined(_MSC_VER) && _MSC_VER >= 1310  // 1310 == VC++ 7.1
CPPUNIT_TEST( testFundamentalGetSet<float> );
CPPUNIT_TEST( testFundamentalGetSet<double> );
CPPUNIT_TEST( testFundamentalGetSet<vpr::Uint8> );
CPPUNIT_TEST( testFundamentalGetSet<vpr::Uint16> );
CPPUNIT_TEST( testFundamentalGetSet<vpr::Uint32> );
CPPUNIT_TEST( testFundamentalGetSet<vpr::Int8> );
CPPUNIT_TEST( testFundamentalGetSet<vpr::Int16> );
CPPUNIT_TEST( testFundamentalGetSet<vpr::Int32> );
#endif
CPPUNIT_TEST( testExists );
CPPUNIT_TEST_SUITE_END();

public:
   /** Tests getting and setting to a type that can be set with a float value */
   template<typename TYPE>
   void testFundamentalGetSet()
   {
      TYPE test_val, ret_val; //, default_val;
      test_val = (TYPE)12.2175f;

      vpr::AttributeMapBase  attrib_map;

      attrib_map.setAttrib("test_val", test_val);
      ret_val = attrib_map.template getAttrib<TYPE>("test_val");
      CPPUNIT_ASSERT(test_val == ret_val);

      //ret_val = attrib_map.template getAttrib<TYPE>("test_val-not");
      //CPPUNIT_ASSERT(ret_val == default_val);

      CPPUNIT_ASSERT( attrib_map.attribExists("test_val"));
      CPPUNIT_ASSERT( !attrib_map.attribExists("test_val-not"));
   }

   /** Test existence tests */
   void testExists();
};

}

#endif
