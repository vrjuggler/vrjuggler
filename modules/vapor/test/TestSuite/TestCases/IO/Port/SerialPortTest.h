#ifndef _VPRTEST_SERIAL_PORT_TEST_H_
#define _VPRTEST_SERIAL_PORT_TEST_H_

#include <string>
#include <vector>
#include <cppunit/ThreadTestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <vpr/vpr.h>
#include <vpr/Sync/CondVar.h>


namespace vprTest
{

class SerialPortTest : public CppUnit::ThreadTestCase
{
public:
   SerialPortTest (std::string name)
      : CppUnit::ThreadTestCase(name), mDataBuffer("This is a test...\n")
   {
      /* Do nothing. */ ;
   }

   SerialPortTest (void)
      : CppUnit::ThreadTestCase(), mDataBuffer("This is a test...\n")
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

   static CppUnit::Test* suite (void)
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite("SerialPortTest");

      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testGetName", &SerialPortTest::testGetName));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeUpdateAction", &SerialPortTest::testChangeUpdateAction));

      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testOpen", &SerialPortTest::testOpen));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testRepeatedOpen", &SerialPortTest::testRepeatedOpen));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testOpenModes", &SerialPortTest::testOpenModes));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testBlockingOpen", &SerialPortTest::testBlockingOpen));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testNonBlockingOpen", &SerialPortTest::testNonBlockingOpen));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testEnableBlocking", &SerialPortTest::testEnableBlocking));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testEnableNonBlocking", &SerialPortTest::testEnableNonBlocking));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeBufferSize", &SerialPortTest::testChangeBufferSize));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeTimeout", &SerialPortTest::testChangeTimeout));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeCharacterSize", &SerialPortTest::testChangeCharacterSize));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeReadState", &SerialPortTest::testChangeReadState));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeStopBits", &SerialPortTest::testChangeStopBits));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeCanonicalInput", &SerialPortTest::testChangeCanonicalInput));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeBadByteIgnore", &SerialPortTest::testChangeBadByteIgnore));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeInputParityCheck", &SerialPortTest::testChangeInputParityCheck));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeBitStripping", &SerialPortTest::testChangeBitStripping));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeStartStopInput", &SerialPortTest::testChangeStartStopInput));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeStartStopOutput", &SerialPortTest::testChangeStartStopOutput));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeParityGeneration", &SerialPortTest::testChangeParityGeneration));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeParityErrorMarking", &SerialPortTest::testChangeParityErrorMarking));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeParity", &SerialPortTest::testChangeParity));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeInputBaudRate", &SerialPortTest::testChangeInputBaudRate));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeOutputBaudRate", &SerialPortTest::testChangeOutputBaudRate));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeHardwareFlowControl", &SerialPortTest::testChangeHardwareFlowControl));
      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testChangeSoftwareFlowControl", &SerialPortTest::testChangeSoftwareFlowControl));
//      test_suite->addTest(new CppUnit::TestCaller<SerialPortTest>("testSendRecv", &SerialPortTest::testSendRecv));

      return test_suite;
   }

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
