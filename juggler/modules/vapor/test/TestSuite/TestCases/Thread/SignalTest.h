#ifndef _VPR_TEST_SIGNAL_TEST_H_
#define _VPR_TEST_SIGNAL_TEST_H_

#include <iostream>
#include <string.h>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

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

   SignalTest (std::string name)
      : CppUnit::TestCase(name)
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

   static CppUnit::Test* suite ()
   {
      CppUnit::TestSuite* suite = new CppUnit::TestSuite("SignalTest");

      suite->addTest(new CppUnit::TestCaller<SignalTest>("testSegFault",
                                                         &SignalTest::testSegFault));

      return suite;
   }
};

}

#endif
