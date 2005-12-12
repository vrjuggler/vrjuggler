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

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("", addr1.setAddress("192.49.3.2", 13768));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("", addr2.setAddress((vpr::Uint32)3224437506u, 13768));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("", addr3.setAddress("cruncher.vrac.iastate.edu", 13768));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("", addr4.setAddress("129.186.232.58", 13768));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("", addr5.setAddress("cruncher.vrac.iastate.edu:13768"));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("", local_addr.setAddress("localhost", 0));

   CPPUNIT_ASSERT(local_addr.getAddressValue() > 0);
   CPPUNIT_ASSERT(addr1.getAddressValue() == 3224437506u);
   CPPUNIT_ASSERT(addr1 == addr2);
#ifndef VPR_SIMULATOR
   CPPUNIT_ASSERT(addr3.getAddressValue() == addr4.getAddressValue());
   CPPUNIT_ASSERT(addr3.getAddressString() == addr4.getAddressString());
   CPPUNIT_ASSERT(addr3 == addr4);
   std::string addr3_hn, addr4_hn;
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("", addr3_hn = addr3.getHostname());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("", addr4_hn = addr4.getHostname());
   CPPUNIT_ASSERT(addr3_hn == addr4_hn);
   CPPUNIT_ASSERT(addr3 == addr5);
#endif
}

void InetAddrTest::testLocalAddressLookup ()
{
   vpr::InetAddr empty_addr, local_addr;

   CPPUNIT_ASSERT(empty_addr == local_addr && "Default addresses not equal");
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Local host lookup failed",
                                   local_addr = vpr::InetAddr::getLocalHost());
   CPPUNIT_ASSERT(empty_addr != local_addr && "Default addresses not equal");
}

void InetAddrTest::testGetAllLocalAddrs()
{
   // Set up a localhost address with port 0. This will be used below for
   // determining whether the loopback address is included with the addresses
   // returned by vpr::InetAddr::getAllLocalAdddrs().
   vpr::InetAddr localhost;
   localhost.setAddress("127.0.0.1");
   localhost.setPort(0);

   vpr::InetAddr default_addr = vpr::InetAddr::getLocalHost();

   // Get all local addresses without loopback.
   try
   {
      std::vector<vpr::InetAddr>::iterator a;

      std::vector<vpr::InetAddr> addrs = vpr::InetAddr::getAllLocalAddrs();

      a = std::find(addrs.begin(), addrs.end(), localhost);
      CPPUNIT_ASSERT(a == addrs.end() && "Loopback should not be present");

      if ( ! addrs.empty() )
      {
         a = std::find(addrs.begin(), addrs.end(), default_addr);
         CPPUNIT_ASSERT(a != addrs.end() && "Default address not in addrs");
      }
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false && "Caught vpr::Exception");
   }

   // Get all local addresses with loopback.
   try
   {
      std::vector<vpr::InetAddr>::iterator a;

      std::vector<vpr::InetAddr> addrs = vpr::InetAddr::getAllLocalAddrs(true);

      a = std::find(addrs.begin(), addrs.end(), localhost);
      CPPUNIT_ASSERT(a != addrs.end() && "Loopback should be present");

      a = std::find(addrs.begin(), addrs.end(), default_addr);
      CPPUNIT_ASSERT(a != addrs.end() && "Default address not in addrs");
   }
   catch (vpr::Exception&)
   {
      CPPUNIT_ASSERT(false && "Caught vpr::Exception");
   }
}

} // End of vprTest namespace
