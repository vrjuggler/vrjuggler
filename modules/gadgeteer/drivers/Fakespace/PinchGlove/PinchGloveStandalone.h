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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_PINCH_GLOVE_STANDALONE_H_
#define _GADGET_PINCH_GLOVE_STANDALONE_H_

#include <gadget/Devices/DriverConfig.h>
#include <string>
#include <vector>
#include <vpr/IO/Port/SerialPort.h>

namespace PinchGlove
{
   // ------ PinchGlove exceptions ------ //
   /** Base exception for PinchGlove errors */
   class PinchGloveException : public std::exception
   {
   public:
      PinchGloveException(const std::string& msg) : mMsg(msg)
      {;}

      virtual ~PinchGloveException() throw()
      {;}

      const std::string& getMessage() const
      { return mMsg; }

      virtual const char* what() const throw()
      { return mMsg.c_str();  }

   protected:
      std::string mMsg;
   };

   /** Thrown when there are timeouts reading or writing */
   class TimeoutException : public PinchGloveException
   {
   public:
      TimeoutException(const std::string& msg="Time out exception") : PinchGloveException(msg)
      {;}
      virtual ~TimeoutException() throw()
      {;}
   };

   // ---------------------------------------------------------------------------------- //

   namespace Command
   {
      const vpr::Uint8 QueryHardware(0x43);
      const vpr::Uint8 TimeStamp(0x54);
      const vpr::Uint8 DataFormatVersion(0x56);
   };

   namespace Parameter
   {
      // Query Parameters
      const vpr::Uint8 LeftVersion(0x4C);
      const vpr::Uint8 RightVersion(0x52);
      const vpr::Uint8 ProcessorCode(0x50);
      const vpr::Uint8 TickLength(0x54);
      
      // Command Parameters
      const vpr::Uint8 TimeStampsOn(0x31);
      const vpr::Uint8 TimeStampsOff(0x30);
      const vpr::Uint8 DataFormatVersion1(0x31);
   }
   
   namespace Control
   {
      const vpr::Uint8 START_BYTE_DATA = 0x80;
      const vpr::Uint8 START_BYTE_DATA_TS = 0x81;
      const vpr::Uint8 START_BYTE_INFO = 0x82;
      const vpr::Uint8 END_BYTE = 0x8F;
   }
} // End PinchGlove namespace

namespace gadget
{
/**
 * Software interface to Fakespace Pinchglove hardware.
 */
class PinchGloveStandalone
{
public:
   /**
    * Constructor.
    */
   PinchGloveStandalone();

   /**
    * Deconstructor.
    */
   ~PinchGloveStandalone();

   /**
    * Set proper serial port settings and open serial port.
    */
   bool connect(const std::string& port_name, int baud);

   /**
    * Query the PinchGlove device for various information and print it to the
    * screen.
    *
    * @note Version of left glove.
    *       Version of right glove.
    *       Processor code.
    *       Length of tick length returned in timestamp.
    */
   bool printHardwareInformation(std::vector<std::string>& info);
   
   /**
    * Informs the PinchGlove hardware if it shoulf include timestamps with 
    * data packet. Timestamps are on by default. (Pinch Glove Manual p. 8)
    */
   bool setTimestampsOn(const bool val);
   
   /**
    * To support different backwardly compatible interfaces, this sets the
    * version of the data format recieved from the PinchGlove. The only version
    * to date is version 1. But in the future it will be possible to set the
    * version and have new hardware emulate this data packet format version.
    */
   bool setDataFormatVersion(const vpr::Uint8& format);
  
   /**
    * Return the next data packet waiting in the serial port buffer.
    *
    * @note This method is asynchronous because the PinchGlove only returns
    *       data when a "pinch" occurs. And in order to eliminate data
    *       possibility of a deadlock this method must not be blocking.
    */
   bool sample(std::vector<int>& data, int& timestamp);

private:
   /**
    * Send a command to the PinchGlove.
    *
    * @param first  First byte to send, the command.
    * @param second Second byte to send, the parameter.
    */
   void sendCommand(const vpr::Uint8& first, const vpr::Uint8& second);

   /**
    * Read any data from the PinchGlove into result that is of the correct
    * type. Errors are reported by throwing exceptions.
    *
    * @throw PinchGlove::TimeoutException is thrown if any I/O exception
    *        occurs while tryign to read from the device.
    */
   void readData(std::vector<vpr::Uint8>& result,
                 const vpr::Uint8& data_type);

private:
   vpr::SerialPort*     mPort; /**< Serial port used to communicate with device. */
   bool                 mTimeStampsOn; /**< Are timestamps on? */
};
} // End gadget namespace
#endif
