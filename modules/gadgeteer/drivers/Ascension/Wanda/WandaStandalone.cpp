/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <sstream>
#include <vector>

#include <vpr/System.h>
#include <vpr/Thread/Thread.h>
#include <vpr/IO/Port/SerialPort.h>
#include <vpr/Util/Debug.h>

#include <drivers/Ascension/Wanda/WandaExceptions.h>
#include <drivers/Ascension/Wanda/WandaStandalone.h>


namespace wanda
{

const vpr::Uint8 SYNC_MASK(1 << 6);
const vpr::Uint8 BUTTON0_MASK(1 << 5);  // Left button mask
const vpr::Uint8 BUTTON2_MASK(1 << 4);  // Right button mask

const vpr::Uint8 X_AXIS_MASK_BYTE0(0x03);
const vpr::Uint8 X_AXIS_MASK_BYTE1(0x3F);
const vpr::Uint8 Y_AXIS_MASK_BYTE0(0x0C);
const vpr::Uint8 Y_AXIS_MASK_BYTE2(0x3F);

// XXX: This use of magic numbers stinks.
const float WandaStandalone::ANALOG_MIN(-34.0f);
const float WandaStandalone::ANALOG_MAX(34.0f);

WandaStandalone::WandaStandalone(const std::string& portName)
   : mPortName(portName)
   , mPort(NULL)
   , mRunning(false)
   , mTimeout(vpr::Interval::NoWait)
   , mButtons(3, 0)
   , mXAxis(0)
   , mYAxis(0)
{
   /* Do nothing. */ ;
}

WandaStandalone::~WandaStandalone()
{
   if ( isActive() || isOpen() )
   {
      stop();
   }

   if ( NULL != mPort )
   {
      delete mPort;
      mPort = NULL;
   }
}

bool WandaStandalone::isOpen() const
{
   if ( NULL != mPort )
   {
      return mPort->isOpen();
   }

   return false;
}

void WandaStandalone::setPortName(const std::string& portName)
{
   mPortName = portName;
}

void WandaStandalone::start()
{
   if ( mRunning )
   {
      // Throw an exception.
   }

   mPort = new vpr::SerialPort(mPortName);
   mPort->open();

   mPort->setUpdateAction(vpr::SerialTypes::NOW);
   mPort->clearAll();
   mPort->setBlocking(true);
   mPort->setCanonicalInput(false);

//   mPort->setTimeout(10);
   mPort->setRead(true);
   mPort->setLocalAttach(true);
   mPort->setBreakByteIgnore(true);

   // The Wanda uses 7 bits per byte, 1 stop bit, 1 start bit, and no parity.
   // It operates at 1200 baud.
   mPort->setCharacterSize(vpr::SerialTypes::CS_BITS_7);
   mPort->setStopBits(1);
   mPort->setInputBaudRate(1200);
   mPort->setOutputBaudRate(1200);
   mPort->setHardwareFlowControl(false);
   mPort->setParityGeneration(false);

   // Setting RTS to low and then back to high will evoke a response from
   // the device. See the mouse(4) man page on Linux.
   mPort->setRequestToSend(false);
   vpr::System::msleep(100);
   mPort->setRequestToSend(true);
   vpr::System::msleep(100);

   // The first two bytes from the device should be the characters 'M' and
   // '3'. If that is not the case, then we have a problem.
   std::vector<vpr::Uint8> buffer(2);
   vpr::Uint32 bytes_read(0);
   mPort->readn(buffer, 2, bytes_read);

   if ( bytes_read == 2 )
   {
      if ( buffer[0] == 'M' && buffer[1] == '3' )
      {
         mRunning = true;
      }
      else
      {
         std::ostringstream msg_stream;
         msg_stream << "Wanda communication startup failed.\n"
                    << "Expected \"M3\" from device but got \""
                    << buffer[0] << buffer[1] << "\"";
         throw wanda::ProtocolException(msg_stream.str(), VPR_LOCATION);
      }
   }
   else
   {
      std::ostringstream msg_stream;
      msg_stream << "Should have read 2 bytes to start Wanda protocol but "
                 << "got " << bytes_read << " bytes instead";
      throw wanda::ProtocolException(msg_stream.str(), VPR_LOCATION);
   }
}

void WandaStandalone::sample()
{
   // We try to read up to four bytes within a timeout period. The Wanda
   // does not return data unless the user is pressing a button or moving
   // the "joystick" nodule. Because of that, this driver could end up
   // blocking forever. Instead of having that happen, we make an effort to
   // read up to four bytes but allow for a timeout if not all four bytes
   // are available to be read.
   try
   {
      std::vector<vpr::Uint8> packet(4);
      vpr::Uint32 bytes_read(0);

      mPort->read(packet, packet.size(), bytes_read, mTimeout);

      // Append whatever was read to mDataBuffer. This has the effect of
      // concatenating every read operation onto mDataBuffer. A key part of
      // this is that the normal packet from the device will be three bytes
      // long. It is only four bytes if the middle button is pressed.
      for ( vpr::Uint32 i = 0; i < bytes_read; ++i )
      {
         mDataBuffer.push_back(packet[i]);
      }
   }
   catch (vpr::TimeoutException&)
   {
      /* Ignore the exception. */ ;
   }

   // The high bit of the first byte should always be 1. This ensures that the
   // first byte of mDataBuffer is always the start of a packet from the
   // device.
   while ( ! mDataBuffer.empty() && ! (mDataBuffer[0] & SYNC_MASK) )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "Re-synchronizing with Wanda" << std::endl << vprDEBUG_FLUSH;
      mDataBuffer.pop_front();
   }

   // If we do not have a complete data packet yet, then we do not have any
   // data to sample.
   if ( mDataBuffer.empty() || mDataBuffer.size() < 3 )
   {
      vpr::Thread::yield();
      return;
   }

   // The joystick data is encoded into all three bytes. According to the
   // Linux mouse(4) man page, this information is a relative change. We
   // treat it as an absolute value in the range [ANALOG_MIN, ANALOG_MAX].
   vpr::Int8 dx =
      static_cast<vpr::Int8>(((mDataBuffer[0] & X_AXIS_MASK_BYTE0) << 6) |
                                (mDataBuffer[1] & X_AXIS_MASK_BYTE1));
   vpr::Int8 dy =
      static_cast<vpr::Int8>(((mDataBuffer[0] & Y_AXIS_MASK_BYTE0) << 4) |
                                (mDataBuffer[2] & Y_AXIS_MASK_BYTE2));

   // The X-axis value seems to come back inverted relative to what the
   // Linux mouse(4) man page says.
   mXAxis = static_cast<float>(-dx);
   mYAxis = static_cast<float>(dy);

   // Clamp the X-axis value.
   if ( mXAxis > ANALOG_MAX )
   {
      mXAxis = ANALOG_MAX;
   }
   else if ( mXAxis < ANALOG_MIN )
   {
      mXAxis = ANALOG_MIN;
   }

   // Clamp the Y-axis value.
   if ( mYAxis > ANALOG_MAX )
   {
      mYAxis = ANALOG_MAX;
   }
   else if ( mYAxis < ANALOG_MIN )
   {
      mYAxis = ANALOG_MIN;
   }

   // Extract the left and right button values. On the Wanda, these appear to
   // be flipped such that the rightmost button is Button 0 and the leftmost
   // is Button 2. We will not reinterpret that state here. Instead, we follow
   // the serial mouse protocol and leave it up to the user to set up the
   // digital proxies as desired.
   mButtons[0] = (mDataBuffer[0] & BUTTON0_MASK) ? 1 : 0;
   mButtons[2] = (mDataBuffer[0] & BUTTON2_MASK) ? 1 : 0;

   // Discard the current three-byte packet.
   mDataBuffer.pop_front();
   mDataBuffer.pop_front();
   mDataBuffer.pop_front();

   // If there is still data in mDataBuffer that is not the beginning of a new
   // packet, then it is the byte for the middle button.
   if ( ! mDataBuffer.empty() && ! (mDataBuffer[0] & SYNC_MASK) )
   {
      mButtons[1] = (mDataBuffer[0] != 0) ? 1 : 0;
      mDataBuffer.pop_front();
   }
}

void WandaStandalone::stop()
{
   // Only attempt to close the serial port if we are running or if the port
   // is open. The port could be open when mRunning is true if the protocol
   // initialization failed in start().
   if ( isActive() || isOpen() )
   {
      mPort->close();
      mRunning = false;
   }
   else
   {
      throw wanda::WandaException("Not running", VPR_LOCATION);
   }
}

vpr::Int8 WandaStandalone::getButton(const size_t buttonNum) const
{
   if ( buttonNum > mButtons.size() )
   {
      std::ostringstream msg_stream;
      msg_stream << "Invalid button number " << buttonNum << " requested.\n"
                 << "Button nubmers must be in the range [0,"
                 << mButtons.size() << ")";
      throw wanda::InvalidButtonException(buttonNum, msg_stream.str(),
                                          VPR_LOCATION);
   }

   return mButtons[buttonNum];
}

}
