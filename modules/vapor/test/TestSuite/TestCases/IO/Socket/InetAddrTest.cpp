#include <TestCases/IO/Socket/InetAddrTest.h>


namespace vprTest
{
CPPUNIT_TEST_SUITE_REGISTRATION( InetAddrTest );

void InetAddrTest::testEqual()
{
   vpr::InetAddr addr1;  addr1.setPort(80);
   vpr::InetAddr addr2;  addr2.setPort(21);
   vpr::InetAddr addr3;  addr3.setPort(80);

   CPPUNIT_ASSERT(addr1 != addr2);
   CPPUNIT_ASSERT(addr1 == addr3);

   addr1.setAddress(23, addr1.getPort());
   addr3.setAddress(23, addr3.getPort());

   CPPUNIT_ASSERT(addr1 == addr3);

   addr3.setAddress(17, addr3.getPort());

   CPPUNIT_ASSERT(addr1 != addr3);
}

void InetAddrTest::testSets()
{
   vpr::InetAddr addr1;

   addr1.setPort(23);

   CPPUNIT_ASSERT(23 == addr1.getPort());

   addr1.setAddress(1221, addr1.getPort());

   CPPUNIT_ASSERT(1221 == addr1.getAddressValue());
}

void InetAddrTest::testAddressLookup ()
{
   vpr::InetAddr addr1;
   vpr::InetAddr addr2;
   vpr::InetAddr addr3;
   vpr::InetAddr addr4;
   vpr::InetAddr addr5;
   vpr::InetAddr local_addr;

   CPPUNIT_ASSERT(addr1.setAddress("192.49.3.2", 13768).success());
   CPPUNIT_ASSERT(addr2.setAddress((vpr::Uint32)3224437506u, 13768).success());
   CPPUNIT_ASSERT(addr3.setAddress("cruncher.vrac.iastate.edu", 13768).success());
   CPPUNIT_ASSERT(addr4.setAddress("129.186.232.58", 13768).success());
   CPPUNIT_ASSERT(addr5.setAddress("cruncher.vrac.iastate.edu:13768").success());
   CPPUNIT_ASSERT(local_addr.setAddress("localhost", 0).success());

   CPPUNIT_ASSERT(local_addr.getAddressValue() > 0);
   CPPUNIT_ASSERT(addr1.getAddressValue() == 3224437506u);
   CPPUNIT_ASSERT(addr1 == addr2);
#ifndef VPR_SIMULATOR
   CPPUNIT_ASSERT(addr3.getAddressValue() == addr4.getAddressValue());
   CPPUNIT_ASSERT(addr3.getAddressString() == addr4.getAddressString());
   CPPUNIT_ASSERT(addr3 == addr4);
   CPPUNIT_ASSERT(addr3.getHostname() == addr4.getHostname());
   CPPUNIT_ASSERT(addr3 == addr5);
#endif
}

void InetAddrTest::testLocalAddressLookup ()
{
   vpr::InetAddr empty_addr, local_addr;
   vpr::ReturnStatus status;

   CPPUNIT_ASSERT(empty_addr == local_addr && "Default addresses not equal");
   status = vpr::InetAddr::getLocalHost(local_addr);
   CPPUNIT_ASSERT(status.success() && "Local host lookup failed");
   CPPUNIT_ASSERT(empty_addr != local_addr && "Default addresses not equal");
}

} // End of vprTest namespace
