#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vpr/IO/Socket/InetAddr.h>

namespace vprTest
{

class InetAddrTest : public TestCase
{
public:
   InetAddrTest( std::string name )
   : TestCase (name)
   {;}

   virtual ~InetAddrTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testCreation()
   {
      vpr::InetAddr any_addr;
      assertTest(any_addr.getPort() == 0);
      assertTest(any_addr.getAddressValue() == vpr::InetAddr::AnyAddr.getAddressValue());
      //assertTest(any_addr == vpr::InetAddr::AnyAddr);
   }

   void testEqual()
   {
      vpr::InetAddr addr1(80);
      vpr::InetAddr addr2(21);
      vpr::InetAddr addr3(80);

      assertTest(addr1 != addr2);
      assertTest(addr1 == addr3);

      addr1.setAddress(23, addr1.getPort());
      addr3.setAddress(23, addr3.getPort());

      assertTest(addr1 == addr3);

      addr3.setAddress(17, addr3.getPort());

      assertTest(addr1 != addr3);
   }

   void testSets()
   {
      vpr::InetAddr addr1;

      addr1.setPort(23);

      assertTest(23 == addr1.getPort());

      addr1.setAddress(1221, addr1.getPort());

      assertTest(1221 == addr1.getAddressValue());
   }

   void testAddressLookup () {
      vpr::InetAddr addr1;
      vpr::InetAddr addr2;
      vpr::InetAddr addr3;
      vpr::InetAddr addr4;
      vpr::InetAddr addr5;
      vpr::InetAddr local_addr;

      assertTest(addr1.setAddress("192.49.3.2", 13768).success());
      assertTest(addr2.setAddress((vpr::Uint32)3224437506u, 13768).success());
      assertTest(addr3.setAddress("cruncher.vrac.iastate.edu", 13768).success());
      assertTest(addr4.setAddress("129.186.232.58", 13768).success());
      assertTest(addr5.setAddress("cruncher.vrac.iastate.edu:13768").success());
      assertTest(local_addr.setAddress("localhost", 0).success());

      assertTest(local_addr.getAddressValue() > 0);
      assertTest(addr1.getAddressValue() == 3224437506u);
      assertTest(addr1 == addr2);
      assertTest(addr3.getAddressValue() == addr4.getAddressValue());
      assertTest(addr3.getAddressString() == addr4.getAddressString());
      assertTest(addr3 == addr4);
      assertTest(addr3 == addr5);
   }

   static Test* suite()
   {
      TestSuite* test_suite = new TestSuite ("InetAddrTest");
      test_suite->addTest( new TestCaller<InetAddrTest>("testCreation", &InetAddrTest::testCreation));
      test_suite->addTest( new TestCaller<InetAddrTest>("testEqual", &InetAddrTest::testEqual));
      test_suite->addTest( new TestCaller<InetAddrTest>("testSets", &InetAddrTest::testSets));
      test_suite->addTest( new TestCaller<InetAddrTest>("testAddressLookup",
                                                        &InetAddrTest::testAddressLookup));

      return test_suite;
   }

   static Test* interactiveSuite()
   {
      TestSuite* test_suite = new TestSuite ("InteractiveInetAddrTest");
      //test_suite->addTest( new TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
