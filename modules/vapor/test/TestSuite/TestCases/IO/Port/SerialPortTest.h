#ifndef _VPRTEST_SERIAL_PORT_TEST_H_
#define _VPRTEST_SERIAL_PORT_TEST_H_

#include <string>
#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <MySuites.h>

#include <cppunit/extensions/ThreadTestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <vpr/vpr.h>
#include <vpr/Sync/CondVar.h>

namespace vprTest
{

class SerialPortTest : public CppUnit::ThreadTestCase
{
CPPUNIT_TEST_SUITE(SerialPortTest);

CPPUNIT_TEST( testGetName );
CPPUNIT_TEST( testChangeUpdateAction );
CPPUNIT_TEST( testOpen );
CPPUNIT_TEST( testRepeatedOpen );
CPPUNIT_TEST( testOpenModes );
CPPUNIT_TEST( testBlockingOpen );
CPPUNIT_TEST( testNonBlockingOpen );
CPPUNIT_TEST( testEnableBlocking );
CPPUNIT_TEST( testEnableNonBlocking );
CPPUNIT_TEST( testChangeBufferSize );
CPPUNIT_TEST( testChangeTimeout );
CPPUNIT_TEST( testChangeCharacterSize );
CPPUNIT_TEST( testChangeReadState );
CPPUNIT_TEST( testChangeStopBits );
CPPUNIT_TEST( testChangeCanonicalInput );
CPPUNIT_TEST( testChangeBadByteIgnore );
CPPUNIT_TEST( testChangeInputParityCheck );
CPPUNIT_TEST( testChangeBitStripping );
CPPUNIT_TEST( testChangeStartStopInput );
CPPUNIT_TEST( testChangeStartStopOutput );
CPPUNIT_TEST( testChangeParityGeneration );
CPPUNIT_TEST( testChangeParityErrorMarking );
CPPUNIT_TEST( testChangeParity );
#ifndef VPR_OS_IRIX
CPPUNIT_TEST( testChangeInputBaudRate );
#endif
CPPUNIT_TEST( testChangeOutputBaudRate );
CPPUNIT_TEST( testChangeHardwareFlowControl );
CPPUNIT_TEST( testChangeSoftwareFlowControl );
CPPUNIT_TEST( testSendRecv );

CPPUNIT_TEST_SUITE_END();

public:
   SerialPortTest (std::string name)
      : CppUnit::ThreadTestCase(name), mDataBuffer("This is a test...")
   {
      /* Do nothing. */ ;
   }

   SerialPortTest (void)
      : CppUnit::ThreadTestCase(), mDataBuffer("This is a test...")
   {
      /* Do nothing. */ ;
   }

   virtual ~SerialPortTest (void)
   {
      /* Do nothing. */ ;
   }

   virtual void setUp(void);
   virtual void tearDown(void);

   // These tests do not require opening the serial port.
   void testGetName(void);
   void testChangeUpdateAction(void);

   // These tests require opening the serial port.
   void testOpen(void);
   void testRepeatedOpen(void);
   void testOpenModes(void);
   void testBlockingOpen(void);
   void testNonBlockingOpen(void);
   void testEnableBlocking(void);
   void testEnableNonBlocking(void);
   void testChangeBufferSize(void);
   void testChangeTimeout(void);
   void testChangeCharacterSize(void);
   void testChangeReadState(void);
   void testChangeStopBits(void);
   void testChangeCanonicalInput(void);
   void testChangeBadByteIgnore(void);
   void testChangeInputParityCheck(void);
   void testChangeBitStripping(void);
   void testChangeStartStopInput(void);
   void testChangeStartStopOutput(void);
   void testChangeParityGeneration(void);
   void testChangeParityErrorMarking(void);
   void testChangeParity(void);
   void testChangeInputBaudRate(void);
   void testChangeOutputBaudRate(void);
   void testChangeHardwareFlowControl(void);
   void testChangeSoftwareFlowControl(void);

   void testSendRecv(void);
   void testSendRecv_sender(void* arg);
   void testSendRecv_receiver(void* arg);   

private:
   static std::string mSendPortName;
   static std::string mRecvPortName;
   static std::vector<vpr::Uint32> mBaudRates;

   enum State
   {
      NOT_READY,
      RECEIVER_READY,
      RECEIVER_DONE,
      DATA_SENT
   };

   State        mState;
   vpr::CondVar mCondVar;
   std::string  mDataBuffer;
};

} // End of vprTest namespace


#endif /* _VPRTEST_SERIAL_PORT_TEST_H_ */
