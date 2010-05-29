#ifndef _VPR_TEST_INET_ADDR_TEST_H_
#define _VPR_TEST_INET_ADDR_TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <vpr/IO/Socket/InetAddr.h>

namespace vprTest
{

class InetAddrTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(InetAddrTest);
CPPUNIT_TEST( testEqual );
CPPUNIT_TEST( testSets );
//CPPUNIT_TEST( testAddressLookup );
CPPUNIT_TEST( testLocalAddressLookup );
CPPUNIT_TEST( testGetAllLocalAddrs );
CPPUNIT_TEST_SUITE_END();

public:
   void testCreation()
   {
      vpr::InetAddr any_addr;
      CPPUNIT_ASSERT(any_addr.getPort() == 0);
      CPPUNIT_ASSERT(any_addr.getAddressValue() == vpr::InetAddr::AnyAddr.getAddressValue());
      //CPPUNIT_ASSERT(any_addr == vpr::InetAddr::AnyAddr);
   }

   void testEqual();

   void testSets();

   void testAddressLookup();

   void testLocalAddressLookup();

   void testGetAllLocalAddrs();
};

};

#endif
