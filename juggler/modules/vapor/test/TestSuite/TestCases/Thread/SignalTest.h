#include <iostream>
#include <string.h>

#include <TestCase.h>
#include <TestSuite.h>
#include <TestCaller.h>

#include <vpr/Thread/Signal.h>

namespace vprTest
{

static RETSIGTYPE handlerSIGSEGV (int signum)
{
//   std::cout << "\n\n\nSignal caught\n" << std::flush;
//   throw new CppUnit::Exception("stuff");
   return;
}

class SignalTest : public CppUnit::TestCase
{
public:
   SignalTest ()
      : CppUnit::TestCase()
   {
      /* Do nothing. */ ;
   }

   virtual ~SignalTest (void)
   {
      /* Do nothing. */ ;
   }

   void testSegFault (void)
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

   void registerTests (CppUnit::TestSuite* suite)
   {
      suite->addTest(new CppUnit::TestCaller<SignalTest>("testSegFault",
                                                         &SignalTest::testSegFault));
   }
};

}
