#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <IO/Socket/InetAddr.h>

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

      addr1.setAddressValue(23);
      addr3.setAddressValue(23);

      assertTest(addr1 == addr3);

      addr3.setAddressValue(17);

      assertTest(addr1 != addr3);
   }

   void testSets()
   {
      vpr::InetAddr addr1;

      addr1.setPort(23);

      assertTest(23 == addr1.getPort());

      addr1.setAddressValue(1221);

      assertTest(1221 == addr1.getAddressValue());
   }

   void testAddressLookup () {
      vpr::InetAddr addr1("192.49.3.2", 13768);
      vpr::InetAddr addr2( vpr::System::ntohl((vpr::Uint32)3224437506), 13768);
      vpr::InetAddr addr3("cruncher.vrac.iastate.edu", 13768);
      vpr::InetAddr addr4("129.186.232.58", 13768);
      vpr::InetAddr addr5("cruncher.vrac.iastate.edu:13768");

      assertTest(addr1.getAddressValue() == vpr::System::ntohl(3224437506));
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
