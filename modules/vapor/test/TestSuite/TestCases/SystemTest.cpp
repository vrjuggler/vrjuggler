#include <vpr/System.h>

#include <SystemTest.h>


namespace vprTest
{

void SystemTest::ShortConversions ()
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

void SystemTest::LongConversions ()
{
   vpr::Uint32 start_val(0xDEAD1234);
   vpr::Uint32 net_val;
   vpr::Uint32 new_val;

   net_val = vpr::System::Htonl(start_val);
   new_val = vpr::System::Ntohl(net_val);
   CPPUNIT_ASSERT(new_val == start_val);

   CPPUNIT_ASSERT(0xCAFEBEEF == vpr::System::Ntohl(vpr::System::Htonl(0xCAFEBEEF)));
   CPPUNIT_ASSERT(0x12345678 == vpr::System::Ntohl(vpr::System::Htonl(0x12345678)));
}

void SystemTest::LongLongConversions ()
{
   vpr::Uint64 start_val(0xDEAD1234BEEF5678);
   vpr::Uint64 net_val;
   vpr::Uint64 new_val;

   net_val = vpr::System::Htonll(start_val);
   new_val = vpr::System::Ntohll(net_val);
   CPPUNIT_ASSERT(new_val == start_val);

   CPPUNIT_ASSERT(0xFACE1234CAFE5678 == vpr::System::Ntohll(vpr::System::Htonll(0xFACE1234CAFE5678)));
   CPPUNIT_ASSERT(0xADB0BD0DEC211975 == vpr::System::Ntohll(vpr::System::Htonll(0xADB0BD0DEC211975)));
}

} // End of vprTest namespace
