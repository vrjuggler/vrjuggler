/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <iostream>
#include <sstream>
#include <cstdlib>

#include <vpr/IO/TimeoutException.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/WIN32/IO/Port/SerialPortImplWin32.h>


namespace
{

/** Helper function for error reporting within this file. */
std::string getErrorMessage(const DWORD errorCode)
{
   char* msg_buffer(NULL);
   FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                 NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                 (LPTSTR) &msg_buffer, 0, NULL);

   // Copy the contents of msg_buffer into err_msg and then release the memory
   // allocated for msg_buffer. What a hassle.
   std::string err_msg(msg_buffer);
   LocalFree(msg_buffer);

   return err_msg;
}

std::string getErrorMessageWithCode(const DWORD errorCode)
{
   std::stringstream msg_stream;
   msg_stream << getErrorMessage(errorCode) << " (error code = " << errorCode
              << ")";
   return msg_stream.str();
}

}

namespace vpr
{

// Constructor.  This sets all the default values for the given port name.
SerialPortImplWin32::SerialPortImplWin32(const std::string& portName)
   : mHandle(NULL)
   , mOpenFlag(GENERIC_READ | GENERIC_WRITE)
   , mName(portName)
   , mOpen(false)
   , mBlocking(0)
   , mParityMark(false)
   , mCurrentTimeout(0)
{
   ;
}

// Destructor.  If the file handle is non-NULL, its memory is released.
SerialPortImplWin32::~SerialPortImplWin32()
{
   if ( mOpen )
   {
      close();
   }
}

// Open the serial port and initialize its flags.
void SerialPortImplWin32::open()
{
   COMMTIMEOUTS gct;
   mHandle = CreateFile( mName.c_str(),
                         mOpenFlag,
                         0,    // comm devices must be opened w/exclusive-access
                         NULL, // no security attributes
                         OPEN_EXISTING, // comm devices must use OPEN_EXISTING
                         mBlocking,
                         NULL  // hTemplate must be NULL for comm devices
                       );

   if ( mHandle == INVALID_HANDLE_VALUE )
   {
      // Report the error.
      std::stringstream msg_stream;
      msg_stream << "CreateFile() failed for " << mName
                 << ": " << getErrorMessageWithCode(GetLastError());

      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR") << ": " << msg_stream.str()
         << std::endl << std::endl << vprDEBUG_FLUSH;

      throw IOException(msg_stream.str(), VPR_LOCATION);
   }

   gct.ReadIntervalTimeout         = 0;
   gct.ReadTotalTimeoutConstant    = 0;
   gct.ReadTotalTimeoutMultiplier  = 0;
   gct.WriteTotalTimeoutConstant   = 5000;
   gct.WriteTotalTimeoutMultiplier = 0;

   if ( ! SetCommTimeouts(mHandle, &gct) )
   {
      std::stringstream msg_stream;
      msg_stream << "Timeout initialization for " << mName << " failed: "
                 << getErrorMessageWithCode(GetLastError());

      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR") << ": " << msg_stream.str()
         << std::endl << vprDEBUG_FLUSH;

      throw IOException(msg_stream.str(), VPR_LOCATION);
   }

   DCB dcb;
   GetCommState(mHandle, &dcb);
   SetCommState(mHandle, &dcb);

   setHardwareFlowControl(false);

   mOpen = true;
}

void SerialPortImplWin32::close()
{
   if ( ! CloseHandle(mHandle) )
   {
      std::stringstream msg_stream;
      msg_stream << "[vpr::SerialPortImplWin32::close()] Could not close "
                 << mName << ": " << getErrorMessageWithCode(GetLastError());
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << msg_stream.str() << std::endl << vprDEBUG_FLUSH;

      throw IOException(msg_stream.str(), VPR_LOCATION);
   }

   mHandle = NULL;
}

void SerialPortImplWin32::setBlocking(bool blocking)
{
   if ( ! mOpen )
   {
      std::stringstream msg_stream;
      msg_stream << "Enabling blocking mode after port open is unsupported "
                 << "on Windows.";
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR") << ": " << msg_stream.str()
         << std::endl << vprDEBUG_FLUSH;
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      if ( blocking )
      {
         mBlocking &= ~FILE_FLAG_OVERLAPPED;
      }
      else
      {
         // XXX: Still not sure why this is not supported.
         std::stringstream msg_stream;
         msg_stream << "Non-blocking serial ports are not currently "
                    << "supported on Windows.";
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR") << ": " << msg_stream.str()
            << std::endl << vprDEBUG_FLUSH;
         throw IOException(msg_stream.str(), VPR_LOCATION);
//         mBlocking |= FILE_FLAG_OVERLAPPED;
      }
   }
}

vpr::SerialTypes::UpdateActionOption SerialPortImplWin32::getUpdateAction()
   const
{
   // Update Action is always NOW in Win32.
   return vpr::SerialTypes::NOW;
}

void SerialPortImplWin32::clearAll()
{
   /* XXX: Not implemented yet... */ ;
}

void SerialPortImplWin32::setUpdateAction(SerialTypes::UpdateActionOption action)
{
   // NOTE: Serial port update action is always NOW on Windows.
   /* Do Nothing */ ;
}

// Query the serial port for the maximum buffer size.
vpr::Uint16 SerialPortImplWin32::getMinInputSize() const
{
   COMMPROP lpCommProp;
   if ( ! GetCommProperties(mHandle, &lpCommProp) ||
        (int) lpCommProp.dwCurrentTxQueue == 0 )
   {
      throw IOException("Maximum buffer size is unavailable.", VPR_LOCATION);
   }

   return lpCommProp.dwCurrentTxQueue;
}

// Attempt to change the buffer size to the given argument.
void SerialPortImplWin32::setMinInputSize(const vpr::Uint8 size)
{
   if ( ! SetupComm(mHandle, (int) size, (int) size) )
   {
      std::stringstream msg_stream;
      msg_stream << "Could not set the minimum buffer size: "
                 << getErrorMessageWithCode(GetLastError());
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING") << ": " << msg_stream.str()
         << std::endl << vprDEBUG_FLUSH;
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

// Get the value of the timeout (in tenths of a second) to wait for data to
// arrive.  This is only applicable in non-canonical mode.
vpr::Uint8 SerialPortImplWin32::getTimeout() const
{
   COMMTIMEOUTS t;
   if ( ! GetCommTimeouts(mHandle, &t) )
   {
      std::stringstream msg_stream;
      msg_stream << "The timeout value is unavailable: "
                 << getErrorMessageWithCode(GetLastError());
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING") << ": " << msg_stream.str()
         << std::endl << vprDEBUG_FLUSH;
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }

   return (int) t.ReadTotalTimeoutConstant / 100;
}

// Set the value of the timeout to wait for data to arrive.  The value given
// must be in tenths of a second.  This is only applicable in non-canonical
// mode.
void SerialPortImplWin32::setTimeout(const vpr::Uint8 timeout)
{
   COMMTIMEOUTS t;
   GetCommTimeouts(mHandle, &t);

   mCurrentTimeout = timeout;

   t.ReadTotalTimeoutConstant = (int) timeout * 100;
   if ( ! SetCommTimeouts(mHandle, &t) )
   {
      std::stringstream msg_stream;
      msg_stream << "Could not set timeout value: "
                 << getErrorMessageWithCode(GetLastError());
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING") << ": " << msg_stream.str()
         << std::endl << vprDEBUG_FLUSH;
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

// Get the character size (the bits per byte).
vpr::SerialTypes::CharacterSizeOption SerialPortImplWin32::getCharacterSize()
   const
{
   vpr::SerialTypes::CharacterSizeOption size;

   DCB dcb;
   if ( GetCommState(mHandle, &dcb) )
   {
      switch ( dcb.ByteSize )
      {
         case 5:
            size = vpr::SerialTypes::CS_BITS_5;
            break;
         case 6:
            size = vpr::SerialTypes::CS_BITS_6;
            break;
         case 7:
            size = vpr::SerialTypes::CS_BITS_7;
            break;
         case 8:
            size = vpr::SerialTypes::CS_BITS_8;
            break;
      }
   }
   else
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to acquire bits/byte: "
                 << getErrorMessageWithCode(GetLastError());
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING") << ": " << msg_stream.str()
         << std::endl << vprDEBUG_FLUSH;
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }

   return size;
}

// Set the current character size (the bits per byte) to the size in the given
// value.  This is used for both reding and writing, and the size does not
// include the parity bit (if any).
void SerialPortImplWin32::setCharacterSize(const vpr::SerialTypes::CharacterSizeOption bpb)
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   switch ( bpb )
   {
      case vpr::SerialTypes::CS_BITS_5:
         dcb.ByteSize = 5;
         break;
      case vpr::SerialTypes::CS_BITS_6:
         dcb.ByteSize = 6;
         break;
      case vpr::SerialTypes::CS_BITS_7:
         dcb.ByteSize = 7;
         break;
      case vpr::SerialTypes::CS_BITS_8:
         dcb.ByteSize = 8;
         break;
   }

   if ( ! SetCommState(mHandle,&dcb) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to set bits/byte: "
                 << getErrorMessageWithCode(GetLastError());
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING") << ": " << msg_stream.str()
         << std::endl << vprDEBUG_FLUSH;
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

vpr::Uint8 SerialPortImplWin32::getStopBits() const
{
   vpr::Uint8 num_bits;
   DCB dcb;

   if ( GetCommState(mHandle, &dcb) )
   {
      switch ( dcb.StopBits )
      {
         case ONESTOPBIT:
            num_bits = 1;
            break;
         case TWOSTOPBITS:
            num_bits = 2;
            break;
      }
   }
   else
   {
      std::stringstream msg_stream;
      msg_stream << "Number of stop bits is unavailable: "
                 << getErrorMessageWithCode(GetLastError());
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING") << ": " << msg_stream.str()
         << std::endl << vprDEBUG_FLUSH;
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }

   return num_bits;
}

// Set the number of stop bits to use.  The value must be either 1 or 2.
void SerialPortImplWin32::setStopBits(const vpr::Uint8 numBits)
{
   DCB dcb;

   GetCommState(mHandle, &dcb);
   switch ( numBits )
   {
      case 1:
         dcb.StopBits = ONESTOPBIT;
         break;
      case 2:
         dcb.StopBits = TWOSTOPBITS;
         break;
   }

   if ( ! SetCommState(mHandle, &dcb) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to set stop bits: "
                 << getErrorMessageWithCode(GetLastError());
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING") << ": " << msg_stream.str()
         << std::endl << vprDEBUG_FLUSH;
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

// Get the state of parity checking for input.
bool SerialPortImplWin32::getInputParityCheckState() const
{
   DCB dcb;
   bool b;
   if ( ! GetCommState(mHandle, &dcb) )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING")
         << ": Error attaining parity checking state: "
         << getErrorMessageWithCode(GetLastError()) << std::endl
         << vprDEBUG_FLUSH;
   }
   if ( dcb.fParity == true )
   {
      b = true;
   }
   else
   {
      b = false;
   }
   return b;
}

// Enable input parity checking.
void SerialPortImplWin32::setInputParityCheck(bool flag)
{
   DCB dcb;

   if ( ! GetCommState(mHandle, &dcb) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to query serial port state: "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }

   dcb.fParity = flag;
   if ( ! SetCommState(mHandle, &dcb) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to change serial port parity setting: "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

// Get the current CLOCAL state, if the device is locally attached.
bool SerialPortImplWin32::getLocalAttachState() const
{
   // XXX: Not implemented yet...
   return false;
}

// Enable CLOCAL, that is the device is locally attached
void SerialPortImplWin32::setLocalAttach(bool flag)
{
   /* XXX: Not implemented yet... */ ;
}

// Get the current state of ignoring BREAK bytes.
bool SerialPortImplWin32::getBreakByteIgnoreState() const
{
   // XXX: Not implemented yet...
   return false;
}

// Enable ignoring of received BREAK bytes.
void SerialPortImplWin32::setBreakByteIgnore(bool flag)
{
   /* XXX: Not implemented yet... */ ;
}

// Get the current parity checking type (either odd or even).
vpr::SerialTypes::ParityType SerialPortImplWin32::getParity() const
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   if ( dcb.Parity == EVENPARITY )
   {
      return vpr::SerialTypes::PORT_PARITY_EVEN;
   }
   if ( dcb.Parity == ODDPARITY )
   {
      return vpr::SerialTypes::PORT_PARITY_ODD;
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING")
         << ": Failed to query serial port parity setting: "
         << getErrorMessageWithCode(GetLastError()) << std::endl
         << vprDEBUG_FLUSH;
      return vpr::SerialTypes::PORT_PARITY_ODD;
   }
}

// Enable odd or even parity.
void SerialPortImplWin32::setParity(const vpr::SerialTypes::ParityType& type)
{
   DCB dcb;
   GetCommState(mHandle, &dcb);

   if ( type == vpr::SerialTypes::PORT_PARITY_ODD )
   {
      dcb.Parity = ODDPARITY;
   }
   else
   {
      dcb.Parity = EVENPARITY;
   }

   SetCommState(mHandle, &dcb);
}

vpr::Uint32 SerialPortImplWin32::write_i(const void* buffer,
                                         const vpr::Uint32 length,
                                         const vpr::Interval& timeout)
{
   unsigned long bytes;

   if ( vpr::Interval::NoTimeout != timeout )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_WARNING_LVL)
         << "[vpr::SerialPortImplWin32::write_i()] Timeout not supported\n"
         << vprDEBUG_FLUSH;
   }

   if ( ! WriteFile(mHandle, buffer, length, &bytes, NULL) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to write to serial port " << mName << ": "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }

   return bytes;
}

void SerialPortImplWin32::setDataTerminalReady(bool val)
{
   const DWORD func = (val ? SETDTR : CLRDTR);

   if ( ! EscapeCommFunction(mHandle, func) )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to " << (val ? "set" : "clear")
                 << " DTR on serial port " << mName << ": "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

void SerialPortImplWin32::setRequestToSend(bool val)
{
   const DWORD func = (val ? SETRTS : CLRRTS);

   if ( ! EscapeCommFunction(mHandle, func) )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to " << (val ? "set" : "clear")
                 << " RTS on serial port " << mName << ": "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

vpr::Uint32 SerialPortImplWin32::read_i(void* buffer, const vpr::Uint32 length,
                                        const vpr::Interval& timeout)
{
   unsigned long bytes;

   // Shouldn't be setting this every read, but don't have any other way of
   // specifying the timeout.
   if ( vpr::Interval::NoTimeout != timeout )
   {
      COMMTIMEOUTS t;
      GetCommTimeouts(mHandle, &t);
      t.ReadTotalTimeoutConstant = (int)timeout.msec();
      SetCommTimeouts(mHandle, &t);
   }

   if ( ! ReadFile(mHandle, buffer, length, &bytes, NULL) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to read from serial port " << mName << ": "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }

   //Now set the timeout back
   if ( vpr::Interval::NoTimeout != timeout )
   {
      COMMTIMEOUTS t;
      GetCommTimeouts(mHandle, &t);
      t.ReadTotalTimeoutConstant = (int)mCurrentTimeout*100;
      SetCommTimeouts(mHandle, &t);
   }

   // XXX: Does reading 0 bytes really indicate a timeout?
   if ( bytes == 0 )
   {
      std::stringstream msg_stream;
      msg_stream << "Timeout while attempting to read from serial port "
                 << mName;
      throw TimeoutException(msg_stream.str(), VPR_LOCATION);
   }

   return bytes;
}

vpr::Uint32 SerialPortImplWin32::readn_i(void* buffer,
                                         const vpr::Uint32 length,
                                         const vpr::Interval& timeout)
{
   //Call read_i for now
   return read_i(buffer, length, timeout);
}

// Get the current state of ignoring bytes with framing errors (other than a
// BREAK) or parity errors.
bool SerialPortImplWin32::getBadByteIgnoreState() const
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   return dcb.fErrorChar;
}

// Enable ignoring of received bytes with framing errors or parity errors.
void SerialPortImplWin32::setBadByteIgnore(bool flag)
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   dcb.fErrorChar = flag;

   if ( ! SetCommState(mHandle, &dcb) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to set bad byte ignore state: "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

// Get the current state of parity generation for outgoing bytes and parity
// checking for incoming bytes.
bool SerialPortImplWin32::getParityGenerationState() const
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   if ( dcb.fParity = false )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "parity checking is not true\n" << vprDEBUG_FLUSH;
      return false;
   }
   else if ( dcb.Parity != NOPARITY )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "parity generation not invoked\n" << vprDEBUG_FLUSH;
      return false;
   }
   else
   {
      return true;
   }
}

// Enable parity generation for outgoing bytes and parity checking for
// incoming bytes.
void SerialPortImplWin32::setParityGeneration(bool enableParity)
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   dcb.fParity = enableParity;

   if ( enableParity )
   {
      vpr::SerialTypes::ParityType p = getParity();
      if ( dcb.Parity == NOPARITY )
      {
         dcb.Parity = ODDPARITY;
      }
      else
      {
         dcb.Parity = p;
      }
   }
   else
   {
      dcb.Parity = NOPARITY;
   }

   if ( ! SetCommState(mHandle, &dcb) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to set parity generation state: "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

// Enable marking of bytes with parity errors or framing errors (except
// BREAKs).  This is only active if input parity and framing error reporting
// is enabled (see getInputParityCheckState() for more information).  The mark
// is the three-byte sequence 0377 0 X where X is the byte received in error.
// If bit stripping is enabled, a valid 0377 byte is passed as the two-byte
// sequence 0377 0377.
bool SerialPortImplWin32::getParityErrorMarkingState() const
{
   return mParityMark;
}

// Enable parity error and framing error marking.
void SerialPortImplWin32::setParityErrorMarking(bool flag)
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   dcb.fErrorChar = flag;

   if ( ! SetCommState(mHandle, &dcb) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to set parity error marking: "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      mParityMark = flag;
   }
}

// Get the current input baud rate.
vpr::Uint32 SerialPortImplWin32::getInputBaudRate() const
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   return dcb.BaudRate;
}

// Set the current input baud rate.
void SerialPortImplWin32::setInputBaudRate(const vpr::Uint32& baud)
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   dcb.BaudRate = baud;

   if ( ! SetCommState(mHandle, &dcb) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to set input baud to " << baud << ": "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

// Get the current output baud rate.
vpr::Uint32 SerialPortImplWin32::getOutputBaudRate() const
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   return dcb.BaudRate;
}

// Set the current output baud rate.
void SerialPortImplWin32::setOutputBaudRate(const vpr::Uint32& baud)
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   dcb.BaudRate = baud;
   if ( ! SetCommState(mHandle, &dcb) )
   {
      std::stringstream msg_stream;
      msg_stream << "Failed to set output baud to " << baud << ": "
                 << getErrorMessageWithCode(GetLastError());
      throw IOException(msg_stream.str(), VPR_LOCATION);
   }
}

// Transmit a continuous stream of zero bits for the given duration.  If the
// argument is 0, the transmission will last between 0.25 and 0.5 seconds.
// Otherwise, the duration specfies the number of seconds to send the zero bit
// stream.
void SerialPortImplWin32::sendBreak(const vpr::Int32 duration)
{
   DWORD flags;

   //Send a break for .5 seconds
   SetCommBreak(mHandle);
   Sleep(500);
   ClearCommBreak(mHandle);
   Sleep(35);
   ClearCommError(mHandle,&flags,NULL); //Clear the break error
}

void SerialPortImplWin32::drainOutput()
{
   /* Do nothing. */ ;
}

void SerialPortImplWin32::controlFlow(SerialTypes::FlowActionOption opt)
{
   /* Do nothing. */ ;
}

bool SerialPortImplWin32::getHardwareFlowControlState() const
{
   DCB dcb;
   GetCommState(mHandle, &dcb);
   if ( dcb.fRtsControl == RTS_CONTROL_DISABLE &&
        dcb.fDtrControl == DTR_CONTROL_DISABLE )
   {
      return false;
   }
   else
   {
      return true;
   }
}

void SerialPortImplWin32::setHardwareFlowControl(bool enable)
{
   DCB dcb;
   GetCommState(mHandle, &dcb);

   if ( enable )
   {
      dcb.fRtsControl = RTS_CONTROL_ENABLE;
      dcb.fDtrControl = DTR_CONTROL_ENABLE;
   }
   else
   {
      dcb.fRtsControl = RTS_CONTROL_DISABLE;
      dcb.fDtrControl = DTR_CONTROL_DISABLE;
   }

   SetCommState(mHandle,&dcb);
}

void SerialPortImplWin32::flushQueue(vpr::SerialTypes::FlushQueueOption queue)
{
   if ( queue == vpr::SerialTypes::INPUT_QUEUE ||
        queue == vpr::SerialTypes::IO_QUEUES )
   {
      PurgeComm(mHandle, PURGE_RXCLEAR);
   }

   if ( queue == vpr::SerialTypes::OUTPUT_QUEUE ||
        queue == vpr::SerialTypes::IO_QUEUES )
   {
      PurgeComm(mHandle, PURGE_TXCLEAR);
   }
}

bool SerialPortImplWin32::getCanonicalState() const
{
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Canonical State not yet implemented--EOF is enabled." << std::endl
      << vprDEBUG_FLUSH;
   return false;
}

void SerialPortImplWin32::setCanonicalInput(bool flag)
{
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Canoncial State not yet implemented, EOF is enabled." << std::endl
      << vprDEBUG_FLUSH;
}

bool SerialPortImplWin32::getBitStripState() const
{
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Bit Stripping is not yet implemented on Windows." << std::endl
      << vprDEBUG_FLUSH;
   return false;
}

void SerialPortImplWin32::setBitStripping(bool flag)
{
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Bit Stripping is not yet implemented on Windows." << std::endl
      << vprDEBUG_FLUSH;
}

bool SerialPortImplWin32::getStartStopInputState() const
{
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Start/Stop Input is not yet implemented on Windows." << std::endl
      << vprDEBUG_FLUSH;
   return false;
}

bool SerialPortImplWin32::getStartStopOutputState() const
{
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Start/Stop Output is not yet implemented on Windows." << std::endl
      << vprDEBUG_FLUSH;
   return false;
}

void SerialPortImplWin32::setStartStopInput(bool flag)
{
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Start/Stop Input is not yet implemented on Win32." << std::endl
      << vprDEBUG_FLUSH;
}

void SerialPortImplWin32::setStartStopOutput(bool flag)
{
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
      << "Start/Stop output is not yet implemented on Win32." << std::endl
      << vprDEBUG_FLUSH;
}

} // End of vpr namespace
