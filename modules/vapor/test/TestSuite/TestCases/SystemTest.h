#ifndef _SYSTEM_TEST_H
#define _SYSTEM_TEST_H

#include <iostream>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vpr/System.h>


/*****************************************************************
 tests out the functionality expected of vpr::ReturnStatus
*******************************************************************/

namespace vprTest
{

class SystemTest : public CppUnit::TestCase
{
public:
   SystemTest() : CppUnit::TestCase ()
   {
   }


   virtual ~SystemTest()
   {
   }

   void ShortConversions()
   {
      vpr::Uint16 start_val;
      vpr::Uint16 net_val;
      vpr::Uint16 new_val;

      start_val = 0xF7C2;
      net_val = vpr::System::Htons(start_val);
      new_val = vpr::System::Ntohs(net_val);
      CPPUNIT_ASSERT(new_val == start_val);

      CPPUNIT_ASSERT(0xFACE == vpr::System::Ntohs(vpr::System::Htons(0xFACE)));
   }

   void LongConversions()
   {
      vpr::Uint32 start_val;
      vpr::Uint32 net_val;
      vpr::Uint32 new_val;

      start_val = 0xDEAD1234;
      net_val = vpr::System::Htonl(start_val);
      new_val = vpr::System::Ntohl(net_val);
      CPPUNIT_ASSERT(new_val == start_val);

      CPPUNIT_ASSERT(0xCAFEBEEF == vpr::System::Ntohl(vpr::System::Htonl(0xCAFEBEEF)));
      CPPUNIT_ASSERT(0x12345678 == vpr::System::Ntohl(vpr::System::Htonl(0x12345678)));
   }

   void LongLongConversions()
   {
      vpr::Uint64 start_val;
      vpr::Uint64 net_val;
      vpr::Uint64 new_val;

      start_val = 0xDEAD1234BEEF5678;
      net_val = vpr::System::Htonll(start_val);
      new_val = vpr::System::Ntohll(net_val);
      CPPUNIT_ASSERT(new_val == start_val);

      CPPUNIT_ASSERT(0xFACE1234CAFE5678 == vpr::System::Ntohll(vpr::System::Htonll(0xFACE1234CAFE5678)));
      CPPUNIT_ASSERT(0xADB0BD0DEC211975 == vpr::System::Ntohll(vpr::System::Htonll(0xADB0BD0DEC211975)));
   }


   void registerTests (CppUnit::TestSuite* suite)
   {
      suite->addTest( new CppUnit::TestCaller<SystemTest>("ShortConversions", &SystemTest::ShortConversions));
      suite->addTest( new CppUnit::TestCaller<SystemTest>("LongConversions", &SystemTest::LongConversions));
      suite->addTest( new CppUnit::TestCaller<SystemTest>("LongLongConversions", &SystemTest::LongLongConversions));
   }
};

}

#endif
