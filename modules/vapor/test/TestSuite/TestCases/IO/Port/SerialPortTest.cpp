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
   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testRepeatedOpen()
{
   vpr::SerialPort port(mSendPortName);

   for ( int i = 0; i < 20; ++i )
   {
      CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
      CPPUNIT_ASSERT(port.close().success() && "Port closing failed");

      // Give the OS a chance to "rest" since we're dealing with hardware
      vpr::System::msleep(10);
   }
}

void SerialPortTest::testOpenModes()
{
   vpr::SerialPort port1(mSendPortName);
   port1.setOpenReadOnly();
   CPPUNIT_ASSERT(port1.open().success() && "Failed to open read only");
   CPPUNIT_ASSERT(port1.close().success() && "Failed to close");

   vpr::SerialPort port2(mSendPortName);
   port2.setOpenWriteOnly();
   CPPUNIT_ASSERT(port2.open().success() && "Failed to open write only");
   CPPUNIT_ASSERT(port2.close().success() && "Failed to close");

   vpr::SerialPort port3(mSendPortName);
   port3.setOpenReadWrite();
   CPPUNIT_ASSERT(port3.open().success() && "Failed to open read/write");
   CPPUNIT_ASSERT(port3.close().success() && "Failed to close");
}

void SerialPortTest::testBlockingOpen()
{
   vpr::SerialPort port(mSendPortName);

   port.setBlocking(true);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
   CPPUNIT_ASSERT(port.isBlocking() && "Port should be in blocking mode");
   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testNonBlockingOpen()
{
   vpr::SerialPort port(mSendPortName);

   port.setBlocking(false);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
   CPPUNIT_ASSERT(! port.isBlocking() && "Port should not be in blocking mode");
   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testEnableBlocking()
{
   vpr::SerialPort port(mSendPortName);

   // Open the port in non-blocking mode so that we can change its blocking
   // state after opening.
   port.setBlocking(false);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
   CPPUNIT_ASSERT(port.setBlocking(true).success() && "Failed to enable blocking");
   CPPUNIT_ASSERT(port.isBlocking() && "Port should be in blocking mode");
   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testEnableNonBlocking()
{
   vpr::SerialPort port(mSendPortName);

   // Open the port in blocking mode so that we can change its blocking state
   // after opening.
   port.setBlocking(true);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
   CPPUNIT_ASSERT(port.setBlocking(false).success() && "Failed to enable non-blocking");
   CPPUNIT_ASSERT(! port.isBlocking() && "Port should not be in blocking mode");
   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeMinInputSize()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint16 buf_size, buf_size_new;

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
   CPPUNIT_ASSERT(port.getMinInputSize(buf_size).success() && "Buffer size query failed");

   buf_size++;
   CPPUNIT_ASSERT(port.setMinInputSize(buf_size).success() && "Buffer size update failed");

   port.getMinInputSize(buf_size_new);
   CPPUNIT_ASSERT(buf_size == buf_size_new && "Buffer size update failed");

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeTimeout()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint8 timeout, timeout_new;

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
   CPPUNIT_ASSERT(port.getTimeout(timeout).success() && "Timeout query failed");

   timeout++;
   CPPUNIT_ASSERT(port.setTimeout(timeout).success() && "Timeout update failed");

   port.getTimeout(timeout_new);
   CPPUNIT_ASSERT(timeout == timeout_new && "Timeout update failed");

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeCharacterSize()
{
   vpr::SerialPort port(mSendPortName);
   vpr::SerialTypes::CharacterSizeOption orig_size, new_size, mod_size;

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
   CPPUNIT_ASSERT(port.getCharacterSize(orig_size).success() && "Character size query failed");

   new_size = (orig_size == vpr::SerialTypes::CS_BITS_8) ? vpr::SerialTypes::CS_BITS_7
                                                         : vpr::SerialTypes::CS_BITS_8;

   CPPUNIT_ASSERT(port.setCharacterSize(new_size).success() && "Character size update failed");

   port.getCharacterSize(mod_size);
   CPPUNIT_ASSERT(new_size == mod_size && "Character size update failed");

   CPPUNIT_ASSERT(port.setCharacterSize(orig_size).success() && "Character size reset failed");

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeReadState()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getReadState() )
   {
      CPPUNIT_ASSERT(port.setRead(false).success() && "Failed to disable read");
      CPPUNIT_ASSERT(port.setRead(true).success() && "Failed to re-enable read");
   }
   else
   {
      CPPUNIT_ASSERT(port.setRead(true).success() && "Failed to enable read");
      CPPUNIT_ASSERT(port.setRead(false).success() && "Failed to re-disable read");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeStopBits()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint8 orig_bits, new_bits, mod_bits;

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");
   CPPUNIT_ASSERT(port.getStopBits(orig_bits).success() && "Stop bits query failed");

   CPPUNIT_ASSERT((orig_bits == 1 || orig_bits == 2) && "Invalid stop bit setting read");

   new_bits = (orig_bits == 1) ? 2 : 1;

   CPPUNIT_ASSERT(port.setStopBits(new_bits).success() && "Stop bits update failed");

   port.getStopBits(mod_bits);
   CPPUNIT_ASSERT(new_bits == mod_bits && "Stop bits update failed");

   CPPUNIT_ASSERT(port.setStopBits(orig_bits).success() && "Stop bits reset failed");

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeCanonicalInput()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getCanonicalState() )
   {
      CPPUNIT_ASSERT(port.setCanonicalInput(false).success() && "Failed to disable canonical input");
      CPPUNIT_ASSERT(port.setCanonicalInput(true).success() && "Failed to re-enable canonical input");
   }
   else
   {
      CPPUNIT_ASSERT(port.setCanonicalInput(true).success() && "Failed to enable canonical input");
      CPPUNIT_ASSERT(port.setCanonicalInput(false).success() && "Failed to re-disable canonical input");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeBadByteIgnore()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getBadByteIgnoreState() )
   {
      CPPUNIT_ASSERT(port.setBadByteIgnore(false).success() && "Failed to disable bad byte ignoring");
      CPPUNIT_ASSERT(port.setBadByteIgnore(true).success() && "Failed to re-enable bad byte ignoring");
   }
   else
   {
      CPPUNIT_ASSERT(port.setBadByteIgnore(true).success() && "Failed to enable bad byte ignoring");
      CPPUNIT_ASSERT(port.setBadByteIgnore(false).success() && "Failed to re-disable bad byte ignoring");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeInputParityCheck()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getInputParityCheckState() )
   {
      CPPUNIT_ASSERT(port.setInputParityCheck(false).success() && "Failed to disable input parity checking");
      CPPUNIT_ASSERT(port.setInputParityCheck(true).success() && "Failed to re-enable input parity checking");
   }
   else
   {
      CPPUNIT_ASSERT(port.setInputParityCheck(true).success() && "Failed to enable input parity checking");
      CPPUNIT_ASSERT(port.setInputParityCheck(false).success() && "Failed to re-disable input parity checking");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeBitStripping()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getBitStripState() )
   {
      CPPUNIT_ASSERT(port.setBitStripping(false).success() && "Failed to disable bit stripping");
      CPPUNIT_ASSERT(port.setBitStripping(true).success() && "Failed to re-enable bit stripping");
   }
   else
   {
      CPPUNIT_ASSERT(port.setBitStripping(true).success() && "Failed to enable bit stripping");
      CPPUNIT_ASSERT(port.setBitStripping(false).success() && "Failed to re-disable bit stripping");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeStartStopInput()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getStartStopInputState() )
   {
      CPPUNIT_ASSERT(port.setStartStopInput(false).success() && "Failed to disable start/stop input");
      CPPUNIT_ASSERT(port.setStartStopInput(true).success() && "Failed to re-enable start/stop input");
   }
   else
   {
      CPPUNIT_ASSERT(port.setStartStopInput(true).success() && "Failed to enable start/stop input");
      CPPUNIT_ASSERT(port.setStartStopInput(false).success() && "Failed to re-disable start/stop input");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeStartStopOutput()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getStartStopOutputState() )
   {
      CPPUNIT_ASSERT(port.setStartStopOutput(false).success() && "Failed to disable start/stop output");
      CPPUNIT_ASSERT(port.setStartStopOutput(true).success() && "Failed to re-enable start/stop output");
   }
   else
   {
      CPPUNIT_ASSERT(port.setStartStopOutput(true).success() && "Failed to enable start/stop output");
      CPPUNIT_ASSERT(port.setStartStopOutput(false).success() && "Failed to re-disable start/stop output");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeParityGeneration()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getParityGenerationState() )
   {
      CPPUNIT_ASSERT(port.setParityGeneration(false).success() && "Failed to disable parity generation");
      CPPUNIT_ASSERT(port.setParityGeneration(true).success() && "Failed to re-enable parity generation");
   }
   else
   {
      CPPUNIT_ASSERT(port.setParityGeneration(true).success() && "Failed to enable parity generation");
      CPPUNIT_ASSERT(port.setParityGeneration(false).success() && "Failed to re-disable parity generation");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeParityErrorMarking()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getParityErrorMarkingState() )
   {
      CPPUNIT_ASSERT(port.setParityErrorMarking(false).success() && "Failed to disable parity error marking");
      CPPUNIT_ASSERT(port.setParityErrorMarking(true).success() && "Failed to re-enable parity error marking");
   }
   else
   {
      CPPUNIT_ASSERT(port.setParityErrorMarking(true).success() && "Failed to enable parity error marking");
      CPPUNIT_ASSERT(port.setParityErrorMarking(false).success() && "Failed to re-disable parity error marking");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeParity()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getParity() == vpr::SerialTypes::PORT_PARITY_EVEN )
   {
      CPPUNIT_ASSERT(port.setParity(vpr::SerialTypes::PORT_PARITY_ODD).success() && "Failed to set odd parity");
      CPPUNIT_ASSERT(port.setParity(vpr::SerialTypes::PORT_PARITY_EVEN).success() && "Failed to reset even parity");
   }
   else
   {
      CPPUNIT_ASSERT(port.setParity(vpr::SerialTypes::PORT_PARITY_EVEN).success() && "Failed to reset even parity");
      CPPUNIT_ASSERT(port.setParity(vpr::SerialTypes::PORT_PARITY_ODD).success() && "Failed to set odd parity");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeInputBaudRate()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint32 orig_baud, mod_baud;

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   CPPUNIT_ASSERT(port.getInputBaudRate(orig_baud).success() && "Failed to query input baud rate");

   for ( std::vector<vpr::Uint32>::iterator i = mBaudRates.begin();
         i != mBaudRates.end();
         ++i )
   {
      CPPUNIT_ASSERT(port.setInputBaudRate(*i).success() && "Failed to set new baud rate");
      port.getInputBaudRate(mod_baud);
      CPPUNIT_ASSERT(mod_baud == *i && "Failed to set new baud rate");
   }

   CPPUNIT_ASSERT(port.setInputBaudRate(orig_baud).success() && "Failed to reset input baud rate");

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeOutputBaudRate()
{
   vpr::SerialPort port(mSendPortName);
   vpr::Uint32 orig_baud, mod_baud;

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   CPPUNIT_ASSERT(port.getOutputBaudRate(orig_baud).success() && "Failed to query output baud rate");

   for ( std::vector<vpr::Uint32>::iterator i = mBaudRates.begin();
         i != mBaudRates.end();
         ++i )
   {
      CPPUNIT_ASSERT(port.setOutputBaudRate(*i).success() && "Failed to set new baud rate");
      port.getOutputBaudRate(mod_baud);
      CPPUNIT_ASSERT(mod_baud == *i && "Failed to set new baud rate");
   }

   CPPUNIT_ASSERT(port.setOutputBaudRate(orig_baud).success() && "Failed to reset output baud rate");

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeHardwareFlowControl()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   if ( port.getHardwareFlowControlState() )
   {
      CPPUNIT_ASSERT(port.setHardwareFlowControl(false).success() && "Failed to disable hardware flow control");
      CPPUNIT_ASSERT(port.setHardwareFlowControl(true).success() && "Failed to re-enable hardware flow control");
   }
   else
   {
      CPPUNIT_ASSERT(port.setHardwareFlowControl(true).success() && "Failed to enable hardware flow control");
      CPPUNIT_ASSERT(port.setHardwareFlowControl(false).success() && "Failed to re-disable hardware flow control");
   }

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testChangeSoftwareFlowControl()
{
   vpr::SerialPort port(mSendPortName);

   CPPUNIT_ASSERT(port.open().success() && "Port opening failed");

   CPPUNIT_ASSERT(port.controlFlow(vpr::SerialTypes::OUTPUT_OFF).success() &&
                  "Failed to suspend output");
   CPPUNIT_ASSERT(port.controlFlow(vpr::SerialTypes::OUTPUT_ON).success() &&
                  "Failed to resume output");
   CPPUNIT_ASSERT(port.controlFlow(vpr::SerialTypes::INPUT_OFF).success() &&
                  "Failed to send STOP to device");
   CPPUNIT_ASSERT(port.controlFlow(vpr::SerialTypes::INPUT_ON).success() &&
                  "Failed to send START to device");

   CPPUNIT_ASSERT(port.close().success() && "Port closing failed");
}

void SerialPortTest::testSendRecv()
{
   threadAssertReset();

   mState = NOT_READY;

   vpr::ThreadMemberFunctor<SerialPortTest>* receiver_functor =
      new vpr::ThreadMemberFunctor<SerialPortTest>(this, &SerialPortTest::testSendRecv_receiver);
   vpr::Thread receiver_thread(receiver_functor);

   vpr::ThreadMemberFunctor<SerialPortTest>* sender_functor =
      new vpr::ThreadMemberFunctor<SerialPortTest>(this, &SerialPortTest::testSendRecv_sender);
   vpr::Thread sender_thread(sender_functor);

   receiver_thread.join();
   sender_thread.join();

   checkThreadAssertions();
}

void SerialPortTest::testSendRecv_receiver(void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   vpr::SerialPort recv_port(mRecvPortName);
   vpr::ReturnStatus status;

   recv_port.setOpenReadOnly();
   recv_port.setBlocking(true);

   status = recv_port.open();
   assertTestThread(status.success() && "Failed to open receiver");

   if ( status.success() )
   {
      std::string buffer;
      vpr::Uint32 bytes_read = 0;

      status = recv_port.setCharacterSize(vpr::SerialTypes::CS_BITS_8);
      assertTestThread(status.success() && "Failed to change character size");

      status = recv_port.setRead(true);
      assertTestThread(status.success() && "Failed to enable reading");

      status = recv_port.setCanonicalInput(false);
      assertTestThread(status.success() && "Failed to disable canonical input");

      status = recv_port.setMinInputSize(mDataBuffer.size() * 2);
      assertTestThread(status.success() && "Failed to change buffer size");

      mCondVar.acquire();
      {
         mState = RECEIVER_READY;
         mCondVar.signal();
      }
      mCondVar.release();

      status = recv_port.read(buffer, mDataBuffer.size(), bytes_read,
                              vpr::Interval(2, vpr::Interval::Sec));
      assertTestThread(status.success() && "Failed to read from serial port");
      assertTestThread(mDataBuffer == buffer && "Read wrong string");

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

      assertTestThread(recv_port.close().success() && "Failed to close receiver");
   }
}

void SerialPortTest::testSendRecv_sender(void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   vpr::SerialPort send_port(mSendPortName);
   vpr::ReturnStatus status;

   send_port.setOpenWriteOnly();
   send_port.setBlocking(true);

   status = send_port.open();
   assertTestThread(status.success() && "Failed to open sender");

   if ( status.success() )
   {
      vpr::Uint32 bytes_written;

      send_port.setUpdateAction(vpr::SerialTypes::NOW);

      status = send_port.setCharacterSize(vpr::SerialTypes::CS_BITS_8);
      assertTestThread(status.success() && "Failed to change character size");

      status = send_port.setCanonicalInput(false);
      assertTestThread(status.success() && "Failed to disable canonical input");

      mCondVar.acquire();
      {
         while ( mState != RECEIVER_READY )
         {
            mCondVar.wait();
         }
      }
      mCondVar.release();

      status = send_port.write(mDataBuffer, mDataBuffer.size(), bytes_written);
      assertTestThread(status.success() && "Failed to write");
      assertTestThread(mDataBuffer.size() == bytes_written && "Failed to write all bytes");

      status = send_port.flushQueue(vpr::SerialTypes::IO_QUEUES);
      assertTestThread(status.success() && "Failed to flush I/O queues");

      mCondVar.acquire();
      {
         while ( mState != RECEIVER_DONE )
         {
            mCondVar.wait();
         }
      }
      mCondVar.release();

      assertTestThread(send_port.close().success() && "Failed to close sender");
   }
}


} // End of vprTest namespace
