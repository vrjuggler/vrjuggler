#include <iostream>
#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Thread/ThreadFunctor.h>
#include <vpr/IO/Port/SerialPort.h>

#include <TestCases/IO/Port/SerialPortTest.h>

#include <boost/concept_check.hpp>


namespace vprTest
{

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SerialPortTest, vprTest::MySuites::interactive() );

std::string SerialPortTest::mSendPortName;
std::string SerialPortTest::mRecvPortName;
std::vector<vpr::Uint32> SerialPortTest::mBaudRates;

void SerialPortTest::setUp()
{
   if ( mRecvPortName.empty() )
   {
      std::cout << "Input the reader serial port name: " << std::flush;
      std::cin >> mRecvPortName;
   }

   if ( mSendPortName.empty() )
   {
      std::cout << "Input the writer serial port name: " << std::flush;
      std::cin >> mSendPortName;
   }

   // Fill in the vector of possible baud rate settings.
   if ( mBaudRates.empty() )
   {
      // Define all the supported input baud rates.  This may change based on
      // the host platform, and #ifdef's should be used for platform-specific
      // changes.
      mBaudRates.push_back(50);
      mBaudRates.push_back(75);
      mBaudRates.push_back(110);
      mBaudRates.push_back(134);
      mBaudRates.push_back(150);
      mBaudRates.push_back(200);
      mBaudRates.push_back(300);
      mBaudRates.push_back(600);
      mBaudRates.push_back(1200);
#ifndef VPR_OS_IRIX
      mBaudRates.push_back(1800);
#endif
      mBaudRates.push_back(2400);
      mBaudRates.push_back(4800);
      mBaudRates.push_back(9600);
      mBaudRates.push_back(19200);
      mBaudRates.push_back(38400);
      mBaudRates.push_back(57600);
#ifndef VPR_OS_Linux
      mBaudRates.push_back(76800);
#endif /* VPR_OS_Linux */
      mBaudRates.push_back(115200);
#ifndef VPR_OS_IRIX
#ifndef VPR_OS_Linux
      mBaudRates.push_back(230400);
#endif /* VPR_OS_Linux */
#ifdef VPR_OS_FreeBSD
      mBaudRates.push_back(460800);
      mBaudRates.push_back(912600);
#endif /* VPR_OS_FreeBSD */
#endif /* VPR_OS_IRIX */
   }
}

void SerialPortTest::tearDown()
{
   /* Do nothing. */ ;
}

void SerialPortTest::testGetName()
{
   vpr::SerialPort port1(mSendPortName), port2(mRecvPortName);
   CPPUNIT_ASSERT(port1.getName() == mSendPortName && "Incorret port name in use");
   CPPUNIT_ASSERT(port2.getName() == mRecvPortName && "Incorret port name in use");
   CPPUNIT_ASSERT(port1.getName() != port2.getName());
}

void SerialPortTest::testChangeUpdateAction()
{
   vpr::SerialPort port(mSendPortName);

   port.setUpdateAction(vpr::SerialTypes::NOW);
   CPPUNIT_ASSERT(port.getUpdateAction() == vpr::SerialTypes::NOW);
   port.setUpdateAction(vpr::SerialTypes::DRAIN);
   CPPUNIT_ASSERT(port.getUpdateAction() == vpr::SerialTypes::DRAIN);
   port.setUpdateAction(vpr::SerialTypes::FLUSH);
   CPPUNIT_ASSERT(port.getUpdateAction() == vpr::SerialTypes::FLUSH);
}

void SerialPortTest::testOpen()
{
   vpr::SerialPort port(mSendPortName);
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testRepeatedOpen()
{
   vpr::SerialPort port(mSendPortName);

   for ( int i = 0; i < 20; ++i )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());

      // Give the OS a chance to "rest" since we're dealing with hardware
      vpr::System::msleep(10);
   }
}

void SerialPortTest::testOpenModes()
{
   vpr::SerialPort port1(mSendPortName);
   port1.setOpenReadOnly();
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open read only", port1.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to close", port1.close());

   vpr::SerialPort port2(mSendPortName);
   port2.setOpenWriteOnly();
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open write only", port2.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to close", port2.close());

   vpr::SerialPort port3(mSendPortName);
   port3.setOpenReadWrite();
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open read/write", port3.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to close", port3.close());
}

void SerialPortTest::testBlockingOpen()
{
   vpr::SerialPort port(mSendPortName);

   port.setBlocking(true);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
   CPPUNIT_ASSERT(port.isBlocking() && "Port should be in blocking mode");
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testNonBlockingOpen()
{
   vpr::SerialPort port(mSendPortName);

   port.setBlocking(false);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
   CPPUNIT_ASSERT(! port.isBlocking() && "Port should not be in blocking mode");
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testEnableBlocking()
{
   vpr::SerialPort port(mSendPortName);

   // Open the port in non-blocking mode so that we can change its blocking
   // state after opening.
   port.setBlocking(false);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable blocking", port.setBlocking(true));
   CPPUNIT_ASSERT(port.isBlocking() && "Port should be in blocking mode");
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testEnableNonBlocking()
{
   vpr::SerialPort port(mSendPortName);

   // Open the port in blocking mode so that we can change its blocking state
   // after opening.
   port.setBlocking(true);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable non-blocking", port.setBlocking(false));
   CPPUNIT_ASSERT(! port.isBlocking() && "Port should not be in blocking mode");
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeMinInputSize()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint16 buf_size, buf_size_new;

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Buffer size query failed", port.getMinInputSize(buf_size));

   buf_size++;
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Buffer size update failed", port.setMinInputSize(buf_size));

   port.getMinInputSize(buf_size_new);
   CPPUNIT_ASSERT(buf_size == buf_size_new && "Buffer size update failed");

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeTimeout()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint8 timeout, timeout_new;

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Timeout query failed", port.getTimeout(timeout));

   timeout++;
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Timeout update failed", port.setTimeout(timeout));

   port.getTimeout(timeout_new);
   CPPUNIT_ASSERT(timeout == timeout_new && "Timeout update failed");

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeCharacterSize()
{
   vpr::SerialPort port(mSendPortName);
   vpr::SerialTypes::CharacterSizeOption orig_size, new_size, mod_size;

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Character size query failed", port.getCharacterSize(orig_size));

   new_size = (orig_size == vpr::SerialTypes::CS_BITS_8) ? vpr::SerialTypes::CS_BITS_7
                                                         : vpr::SerialTypes::CS_BITS_8;

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Character size update failed", port.setCharacterSize(new_size));

   port.getCharacterSize(mod_size);
   CPPUNIT_ASSERT(new_size == mod_size && "Character size update failed");

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Character size reset failed", port.setCharacterSize(orig_size));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeReadState()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getReadState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable read", port.setRead(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable read", port.setRead(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable read", port.setRead(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable read", port.setRead(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeStopBits()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint8 orig_bits, new_bits, mod_bits;

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Stop bits query failed", port.getStopBits(orig_bits));

   CPPUNIT_ASSERT((orig_bits == 1 || orig_bits == 2) && "Invalid stop bit setting read");

   new_bits = (orig_bits == 1) ? 2 : 1;

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Stop bits update failed", port.setStopBits(new_bits));

   port.getStopBits(mod_bits);
   CPPUNIT_ASSERT(new_bits == mod_bits && "Stop bits update failed");

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Stop bits reset failed", port.setStopBits(orig_bits));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeCanonicalInput()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getCanonicalState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable canonical input", port.setCanonicalInput(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable canonical input", port.setCanonicalInput(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable canonical input", port.setCanonicalInput(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable canonical input", port.setCanonicalInput(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeBadByteIgnore()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getBadByteIgnoreState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable bad byte ignoring", port.setBadByteIgnore(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable bad byte ignoring", port.setBadByteIgnore(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable bad byte ignoring", port.setBadByteIgnore(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable bad byte ignoring", port.setBadByteIgnore(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeInputParityCheck()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getInputParityCheckState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable input parity checking", port.setInputParityCheck(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable input parity checking", port.setInputParityCheck(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable input parity checking", port.setInputParityCheck(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable input parity checking", port.setInputParityCheck(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeBitStripping()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getBitStripState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable bit stripping", port.setBitStripping(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable bit stripping", port.setBitStripping(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable bit stripping", port.setBitStripping(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable bit stripping", port.setBitStripping(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeStartStopInput()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getStartStopInputState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable start/stop input", port.setStartStopInput(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable start/stop input", port.setStartStopInput(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable start/stop input", port.setStartStopInput(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable start/stop input", port.setStartStopInput(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeStartStopOutput()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getStartStopOutputState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable start/stop output", port.setStartStopOutput(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable start/stop output", port.setStartStopOutput(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable start/stop output", port.setStartStopOutput(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable start/stop output", port.setStartStopOutput(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeParityGeneration()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getParityGenerationState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable parity generation", port.setParityGeneration(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable parity generation", port.setParityGeneration(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable parity generation", port.setParityGeneration(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable parity generation", port.setParityGeneration(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeParityErrorMarking()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getParityErrorMarkingState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable parity error marking", port.setParityErrorMarking(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable parity error marking", port.setParityErrorMarking(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable parity error marking", port.setParityErrorMarking(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable parity error marking", port.setParityErrorMarking(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeParity()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getParity() == vpr::SerialTypes::PORT_PARITY_EVEN )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to set odd parity", port.setParity(vpr::SerialTypes::PORT_PARITY_ODD));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to reset even parity", port.setParity(vpr::SerialTypes::PORT_PARITY_EVEN));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to reset even parity", port.setParity(vpr::SerialTypes::PORT_PARITY_EVEN));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to set odd parity", port.setParity(vpr::SerialTypes::PORT_PARITY_ODD));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeInputBaudRate()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint32 orig_baud, mod_baud;

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to query input baud rate", port.getInputBaudRate(orig_baud));

   for ( std::vector<vpr::Uint32>::iterator i = mBaudRates.begin();
         i != mBaudRates.end();
         ++i )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to set new baud rate", port.setInputBaudRate(*i));
      port.getInputBaudRate(mod_baud);
      CPPUNIT_ASSERT(mod_baud == *i && "Failed to set new baud rate");
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to reset input baud rate", port.setInputBaudRate(orig_baud));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeOutputBaudRate()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint32 orig_baud, mod_baud;

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to query output baud rate", port.getOutputBaudRate(orig_baud));

   for ( std::vector<vpr::Uint32>::iterator i = mBaudRates.begin();
         i != mBaudRates.end();
         ++i )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to set new baud rate", port.setOutputBaudRate(*i));
      port.getOutputBaudRate(mod_baud);
      CPPUNIT_ASSERT(mod_baud == *i && "Failed to set new baud rate");
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to reset output baud rate", port.setOutputBaudRate(orig_baud));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeHardwareFlowControl()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   if ( port.getHardwareFlowControlState() )
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable hardware flow control", port.setHardwareFlowControl(false));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-enable hardware flow control", port.setHardwareFlowControl(true));
   }
   else
   {
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable hardware flow control", port.setHardwareFlowControl(true));
      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to re-disable hardware flow control", port.setHardwareFlowControl(false));
   }

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testChangeSoftwareFlowControl()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port opening failed", port.open());

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to suspend output", port.controlFlow(vpr::SerialTypes::OUTPUT_OFF));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to resume output", port.controlFlow(vpr::SerialTypes::OUTPUT_ON));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to send STOP to device", port.controlFlow(vpr::SerialTypes::INPUT_OFF));
   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to send START to device", port.controlFlow(vpr::SerialTypes::INPUT_ON));

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Port closing failed", port.close());
}

void SerialPortTest::testSendRecv()
{
   mState = NOT_READY;

   vpr::ThreadMemberFunctor<SerialPortTest>* receiver_functor =
      new vpr::ThreadMemberFunctor<SerialPortTest>(this, &SerialPortTest::testSendRecv_receiver);
   vpr::Thread receiver_thread(receiver_functor);

   vpr::ThreadMemberFunctor<SerialPortTest>* sender_functor =
      new vpr::ThreadMemberFunctor<SerialPortTest>(this, &SerialPortTest::testSendRecv_sender);
   vpr::Thread sender_thread(sender_functor);

   receiver_thread.join();
   sender_thread.join();
}

void SerialPortTest::testSendRecv_receiver(void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   vpr::SerialPort recv_port(mRecvPortName);
   //vpr::ReturnStatus status;

   recv_port.setOpenReadOnly();
   recv_port.setBlocking(true);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open receiver", recv_port.open());

   //if ( status )
   {
      std::string buffer;
      vpr::Uint32 bytes_read = 0;

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to change character size", recv_port.setCharacterSize(vpr::SerialTypes::CS_BITS_8));

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to enable reading", recv_port.setRead(true));

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable canonical input", recv_port.setCanonicalInput(false));

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to change buffer size", recv_port.setMinInputSize(mDataBuffer.size() * 2));

      mCondVar.acquire();
      {
         mState = RECEIVER_READY;
         mCondVar.signal();
      }
      mCondVar.release();

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to read from serial port", 
         recv_port.read(buffer, mDataBuffer.size(), bytes_read, vpr::Interval(2, vpr::Interval::Sec)));
      CPPUNIT_ASSERT(mDataBuffer == buffer && "Read wrong string");

      if ( mDataBuffer != buffer )
      {
         std::cout << "Expected '" << mDataBuffer << "'" << std::endl;
         std::cout << "Got '" << buffer << "'" << std::endl;
      }

      mCondVar.acquire();
      {
         mState = RECEIVER_DONE;
         mCondVar.signal();
      }
      mCondVar.release();

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to close receiver", recv_port.close());
   }
}

void SerialPortTest::testSendRecv_sender(void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   vpr::SerialPort send_port(mSendPortName);
   //vpr::ReturnStatus status;

   send_port.setOpenWriteOnly();
   send_port.setBlocking(true);

   CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to open sender", send_port.open());

   //if ( status )
   {
      vpr::Uint32 bytes_written;

      send_port.setUpdateAction(vpr::SerialTypes::NOW);

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to change character size", send_port.setCharacterSize(vpr::SerialTypes::CS_BITS_8));

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to disable canonical input", send_port.setCanonicalInput(false));

      mCondVar.acquire();
      {
         while ( mState != RECEIVER_READY )
         {
            mCondVar.wait();
         }
      }
      mCondVar.release();

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to write", send_port.write(mDataBuffer, mDataBuffer.size(), bytes_written));
      CPPUNIT_ASSERT(mDataBuffer.size() == bytes_written && "Failed to write all bytes");

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to flush I/O queues", send_port.flushQueue(vpr::SerialTypes::IO_QUEUES));

      mCondVar.acquire();
      {
         while ( mState != RECEIVER_DONE )
         {
            mCondVar.wait();
         }
      }
      mCondVar.release();

      CPPUNIT_ASSERT_NO_THROW_MESSAGE("Failed to close sender", send_port.close());
   }
}


} // End of vprTest namespace
