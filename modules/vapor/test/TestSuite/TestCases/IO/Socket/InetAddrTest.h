#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <vpr/IO/Socket/InetAddr.h>

namespace vprTest
{

class InetAddrTest : public CppUnit::TestCase
{
public:
   InetAddrTest()
   : CppUnit::TestCase ()
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
      CPPUNIT_ASSERT(any_addr.getPort() == 0);
      CPPUNIT_ASSERT(any_addr.getAddressValue() == vpr::InetAddr::AnyAddr.getAddressValue());
      //CPPUNIT_ASSERT(any_addr == vpr::InetAddr::AnyAddr);
   }

   void testEqual()
   {
      vpr::InetAddr addr1(80);
      vpr::InetAddr addr2(21);
      vpr::InetAddr addr3(80);

      CPPUNIT_ASSERT(addr1 != addr2);
      CPPUNIT_ASSERT(addr1 == addr3);

      addr1.setAddress(23, addr1.getPort());
      addr3.setAddress(23, addr3.getPort());

      CPPUNIT_ASSERT(addr1 == addr3);

      addr3.setAddress(17, addr3.getPort());

      CPPUNIT_ASSERT(addr1 != addr3);
   }

   void testSets()
   {
      vpr::InetAddr addr1;

      addr1.setPort(23);

      CPPUNIT_ASSERT(23 == addr1.getPort());

      addr1.setAddress(1221, addr1.getPort());

      CPPUNIT_ASSERT(1221 == addr1.getAddressValue());
   }

   void testAddressLookup () {
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
      CPPUNIT_ASSERT(addr3.getAddressValue() == addr4.getAddressValue());
      CPPUNIT_ASSERT(addr3.getAddressString() == addr4.getAddressString());
      CPPUNIT_ASSERT(addr3 == addr4);
      CPPUNIT_ASSERT(addr3 == addr5);
   }

   void registerTests (CppUnit::TestSuite* suite)
   {
      suite->addTest( new CppUnit::TestCaller<InetAddrTest>("testCreation", &InetAddrTest::testCreation));
      suite->addTest( new CppUnit::TestCaller<InetAddrTest>("testEqual", &InetAddrTest::testEqual));
      suite->addTest( new CppUnit::TestCaller<InetAddrTest>("testSets", &InetAddrTest::testSets));
      suite->addTest( new CppUnit::TestCaller<InetAddrTest>("testAddressLookup",
                                                        &InetAddrTest::testAddressLookup));
   }

   static CppUnit::Test* interactiveSuite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("InteractiveInetAddrTest");
      //test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
