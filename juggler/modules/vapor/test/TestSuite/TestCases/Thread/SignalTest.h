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
   std::cout << "Signal caught\n" << std::flush;
   throw new std::exception();
}

class SignalTest : public TestCase
{
public:
   SignalTest (std::string name)
      : TestCase(name)
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
      assertTest(status.success() && "Handler registration failed");

      bad_addr = (char*) 0x1;
      memcpy(buffer, bad_addr, sizeof(buffer));

      assertTest(false);  // Execution should not reach this point
   }

   static Test* suite(void)
   {
      TestSuite* test_suite = new TestSuite ("SignalTest");
      test_suite->addTest(new TestCaller<SignalTest>("testSegFault",
                                                     &SignalTest::testSegFault));

      return test_suite;
   }
};

}
