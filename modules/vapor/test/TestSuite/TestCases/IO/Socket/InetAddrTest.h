#ifndef _VPR_TEST_INET_ADDR_TEST_H_
#define _VPR_TEST_INET_ADDR_TEST_H_

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

   InetAddrTest(std::string name)
   : CppUnit::TestCase (name)
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

   void testEqual();

   void testSets();

   void testAddressLookup();

   static CppUnit::Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("NoninteractiveInetAddrTest");

      test_suite->addTest( new CppUnit::TestCaller<InetAddrTest>("testCreation", &InetAddrTest::testCreation));
      test_suite->addTest( new CppUnit::TestCaller<InetAddrTest>("testEqual", &InetAddrTest::testEqual));
      test_suite->addTest( new CppUnit::TestCaller<InetAddrTest>("testSets", &InetAddrTest::testSets));
      test_suite->addTest( new CppUnit::TestCaller<InetAddrTest>("testAddressLookup",
                                                                 &InetAddrTest::testAddressLookup));

      return test_suite;
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

#endif
