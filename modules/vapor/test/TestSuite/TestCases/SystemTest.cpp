#include <vpr/System.h>

#include <TestCases/SystemTest.h>
#include <vpr/Util/Assert.h>


namespace vprTest
{

CPPUNIT_TEST_SUITE_REGISTRATION( SystemTest );

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

void SystemTest::LongLongConversions()
{
#ifdef VPR_OS_Win32
   vpr::Uint64 start_val(0xDEAD1234BEEF5678);
#else
   vpr::Uint64 start_val(0xDEAD1234BEEF5678ll);
#endif
   vpr::Uint64 net_val;
   vpr::Uint64 new_val;

   net_val = vpr::System::Htonll(start_val);
   new_val = vpr::System::Ntohll(net_val);
   CPPUNIT_ASSERT(new_val == start_val);

#ifdef VPR_OS_Win32
   CPPUNIT_ASSERT(0xFACE1234CAFE5678 == vpr::System::Ntohll(vpr::System::Htonll(0xFACE1234CAFE5678)));
   CPPUNIT_ASSERT(0xADB0BD0DEC211975 == vpr::System::Ntohll(vpr::System::Htonll(0xADB0BD0DEC211975)));
#else
   CPPUNIT_ASSERT(0xFACE1234CAFE5678ll == vpr::System::Ntohll(vpr::System::Htonll(0xFACE1234CAFE5678ll)));
   CPPUNIT_ASSERT(0xADB0BD0DEC211975ll == vpr::System::Ntohll(vpr::System::Htonll(0xADB0BD0DEC211975ll)));
#endif
}

void SystemTest::GetSetEnvTest()
{
   const std::string env_name("VPR_ENV_TEST_PROP");
   const std::string set_value("Set");
   std::string result_value;
   vpr::ReturnStatus status;

   status = vpr::System::setenv(env_name, set_value);
   CPPUNIT_ASSERT(status.success() && "setenv failed");
   status = vpr::System::getenv(env_name, result_value);
   CPPUNIT_ASSERT(status.success() && "getenv failed");
   CPPUNIT_ASSERT(set_value == result_value);
}

void SystemTest::getStackTrace()
{
   stack_trace_1();

   // Test assert stack trace
   //vprASSERT(false && "Failed on purpose to test stack trace abilities.");
}

void SystemTest::stack_trace_1()
{
   stack_trace_2();
}

void SystemTest::stack_trace_2()
{
   std::string stack_trace = vpr::System::getCallStack();
   ///*
   std::cout << "\n\nStacktrace test:\n" << stack_trace << "---------\n\n\n" << std::endl;
   //*/
}

} // End of vprTest namespace
