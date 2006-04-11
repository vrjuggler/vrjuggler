#include <iostream>
#include <string.h>

#include <vpr/Thread/Signal.h>

#include <TestCases/Thread/SignalTest.h>


static RETSIGTYPE handlerSIGSEGV
#if defined(VPR_OS_IRIX) && defined(__GNUC__)
   (void)
#else
   (int signum)
#endif
{
//   std::cout << "\n\n\nSignal caught\n" << std::flush;
//   throw new CppUnit::Exception("stuff");
   return;
}

namespace vprTest
{

//CPPUNIT_TEST_SUITE_REGISTRATION( SignalTest );

void SignalTest::testSegFault ()
{
   vpr::SignalAction segv_action(handlerSIGSEGV);
   char buffer[80];
   char* bad_addr;
   vpr::ReturnStatus status;

   status = vpr::SigHandler::registerHandler(SIGSEGV, segv_action);
   CPPUNIT_ASSERT(status.success() && "Handler registration failed");

   bad_addr = (char*) 0x1;
   memcpy(buffer, bad_addr, sizeof(buffer));

   CPPUNIT_ASSERT(false);  // Execution should not reach this point
}

} // End of vprTest namespace
