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
 * File:          $RCSfile: Ether24Standalone.h,v $
 * Date modified: $Date: 2006/01/08 20:28:21 $
 * Version:       $Revision: 1.17 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef GADGET_ELEXOL_ETHER24_STANDALONE_H
#define GADGET_ELEXOL_ETHER24_STANDALONE_H

#include <vpr/vpr.h>
#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketDatagram.h>

#include <boost/algorithm/string.hpp>

#include <map>
#include <stdexcept>

namespace Elexol
{
   // ------ Elexol exceptions ------ //
   /** Base exception for Elexol errors. */
   class ElexolException : public std::runtime_error
   {
   public:
      ElexolException(const std::string& msg)
         : std::runtime_error(msg)
         , mDescription(msg)
      {
         /* Do nothing. */ ;
      }

      virtual ~ElexolException() throw ()
      {
         /* Do nothing. */ ;
      }
   
      virtual const char* what() const throw()
      {
         std::string m_full_desc = "ElexolException: " + mDescription;
         return m_full_desc.c_str();
      }
      std::string mDescription;
   };

   class ElexolTimeoutException : public ElexolException
   {
   public:
      ElexolTimeoutException(const std::string& msg)
         : ElexolException(msg)
      {
         /* Do nothing. */ ;
      }

      virtual ~ElexolTimeoutException() throw ()
      {
         /* Do nothing. */ ;
      }
   };

   struct is_iless
   {
      bool operator()(const std::string& lhs,
                      const std::string& rhs) const
      {
         std::string lhs_lower(boost::to_lower_copy(lhs));
         std::string rhs_lower(boost::to_lower_copy(rhs));

         return lhs_lower < rhs_lower;
      }
   };

   typedef std::map<std::string, vpr::InetAddr, Elexol::is_iless> device_map_t;
   typedef vpr::Uint8 CommandType;
   typedef vpr::Uint8 AddressType;
   typedef vpr::Uint8 Port;

   namespace Command
   {
      const CommandType Value(0);
      const CommandType Direction('!');
      const CommandType PullUp('@');
      const CommandType Threshold('#');
      const CommandType SchmittTrigger('$');
      const CommandType EEPROM('\'');
      const CommandType ReadWord('R');
      const CommandType WriteWord('W');
      const CommandType EraseWord('E');
      const CommandType WriteDisable('0');
      const CommandType WriteEnable('1');
      const CommandType Reset('@');
      const CommandType EchoByte('`');
      const CommandType EchoSpace('*');
      const CommandType SendHostData('%');
      const CommandType PortReadOffset('a');
      const CommandType PortWriteOffset('A');
   }

   namespace Address
   {
      // Byte Address
      const AddressType ControlBits1(10);
      const AddressType ControlBits2(11);

      // Word address
      const AddressType IpAddressA(6);
      const AddressType IpAddressB(7);

      // Presets (byte addresses)
      const AddressType ValueA(17);
      const AddressType DirectionA(16);
      const AddressType PullUpA(19);
      const AddressType ThresholdA(18);
      const AddressType DirectionB(21);
      const AddressType SchmittTriggerA(20);
      const AddressType ThresholdB(23);
      const AddressType ValueB(22);
      const AddressType SchmittTriggerB(25);
      const AddressType PullUpB(24);
      const AddressType ValueC(27);
      const AddressType DirectionC(26);
      const AddressType PullUpC(29);
      const AddressType ThresholdC(28);
      const AddressType SchmittTriggerC(30);

      // User data (word addresses)
      const AddressType FirstUserWord(48);
      const AddressType LastUserWord(63);

      // User data (byte addresses)
      const AddressType FirstUserByte(96);
      const AddressType LastUserByte(127);
   }


   /** TODO: Implement AutoScan
    * The AutoScan mode will allow the module to originate communication with
    * a remote device or another Ether I/O 24 module. This mode is very useful
    * as it allows your software the freedom not to have to poll the module to
    * check the state of the inputs.
    *
    * To allow the module to communicate through a router without having to
    * set up Gateway addresses and Subnet masks, the module stores the Ethernet
    * address of the target device as well as the IP address and Port number.
    * If your target device is outside your local network then this address will
    * be that of the gateway or router whereas if your target device is on your
    * local area network then this address will be that of the device itself.
    *
    * Mask bits are used to allow some of the input pins of the module to toggle
    * without generating messages from the module. Any input whose corresponding
    * mask bit is low, is ignored by the AutoScan function. The Scan Rate is a 
    * 16-bit value which is used to divide the scan rate of the AutoScan mode
    * from it's base rate of 1,000 scans per second down to a user programmed
    * rate from 500 scans per second to one scan per 65.5 seconds. When set at
    * 1 the scan rate is 1,000 scans per second, it is 500 scans per second when
    * the value is 2 and so on. You simply divide the 1,000 per second rate by
    * your desired scan rate to find the value for word 18.
    *
    * The filter value which is in the MSB of Address 17 is used to count the
    * number of identical reads that are required before a port value is
    * considered valid and sent to the target device. When set at 0 the filter
    * is turned off, when set at 1 the port must read the same for two scans to
    * be considered valid and sent to the target. Higher numbers simply increase
    * the number of identical reads required before the value is considered valid.
    */
   namespace AutoScan
   {
      // Word Address
      const AddressType AutoScanScanRate(18);
      const AddressType TargetMACAddress1(19);
      const AddressType TargetMACAddress2(20);
      const AddressType TargetMACAddress3(21);
      const AddressType TargetIPAddress1(22);
      const AddressType TargetIPAddress2(23);
      const AddressType TargetPort(24);

      // Byte Address
      const AddressType PortAMask(32);
      const AddressType PortBMask(33);
      const AddressType PortCMask(34);
      const AddressType FilterCount(35);
   }

   const Port PortA(0);
   const Port PortB(1);
   const Port PortC(2);
}

class Ether24Standalone
{
public:
   Ether24Standalone()
      : mActive(false)
      , mSocket(NULL)
   {;}

   ~Ether24Standalone()
   {
      close();
   }

   /**
    * Opens UDP socket used to communicate with Elexol Ether I/O 24 device.
    *
    * @param addr The address of the device to communicate with.
    *
    */
   void open(vpr::InetAddr& addr);

   /**
    * Closes UDP socket used to communicate with Elexol Ether I/O 24 device.
    * 
    * @pre Must have already called open to create the UDP socket.
    */
   void close();

   static Elexol::device_map_t getDevicesByMacAddress();

   /**
    * Reads the value for the specified port. The pins that are set as outputs
    * are read as though they were inputs with the specified state.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the port value for.
    * @see Elexol::Port
    */
   vpr::Uint8 getValue(const Elexol::Port port) const
   {
      return getState(port, Elexol::Command::Value);
   }

   /**
    * Reads the direction value for the specified port. For each pin a value
    * of 0 means output and 1 means input.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the port value for.
    * @see Elexol::Port
    */
   vpr::Uint8 getDirection(const Elexol::Port port) const
   {
      return getState(port, Elexol::Command::Direction);
   }

   /**
    * Reads the pullup value for the specified port. For each pin a value of 0
    * enables the pull up resistor. And a value of 1 disables the resistor.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the port value for.
    * @see Elexol::Port
    */
   vpr::Uint8 getPullUp(const Elexol::Port port) const
   {
      return getState(port, Elexol::Command::PullUp);
   }

   /**
    * Reads the threshold value for the specified port. For each pin
    * a value of 0 means to use a threshold of 2.5V. And a value of
    * 1 means to use a threshold of 1.4V.
    * 
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the port value for.
    * @see Elexol::Port
    */
   vpr::Uint8 getThreshold(const Elexol::Port port) const
   {
      return getState(port, Elexol::Command::Threshold);
   }

   /** Reads the trigger value for the specified port. For each pin a
    * value of 0 enables the Schmitt trigger. And a value of 1 disables
    * the Schmitt trigger.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the port value for.
    * @see Elexol::Port
    */
   vpr::Uint8 getSchmittTrigger(const Elexol::Port port) const
   {
      return getState(port, Elexol::Command::SchmittTrigger);
   }

   /**
    * The port value is written to the entire port with each of the
    * values bits affecting the corresponding I/O line. To change a
    * single I/O line without affecting the others it is required to
    * read its current value before writing a new value with only the
    * corresponding bits changed. This command does not affect any
    * I/O lines that are set as inputs.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the port value for.
    * @param val The new port value.
    *
    * @see Elexol::Port
    */
   void setValue(const Elexol::Port port, const vpr::Uint8 val) const
   {
      setState(port, Elexol::Command::Value, val);
   }

   /**
    * The direction value is written to the entire port with each of the
    * bits in the byte affecting the corresponding I/O line. To change a
    * single I/O line without affecting the others it is necessary to
    * read its current value before writing a new value with only the
    * corresponding bits changed. For each pin a value of 0 means output
    * and 1 means input. For example To set the entire port as outputs use
    * val = 0 to set all as inputs use val = 255 to set 0, 1, 2 and 3
    * as inputs and 4, 5, 6 and 7 as outputs use val = 15.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the direction value for.
    * @param val The new direction value.
    *
    * @see Elexol::Port
    */
   void setDirection(const Elexol::Port port, const vpr::Uint8 val) const
   {
      setState(port, Elexol::Command::Direction, val);
   }

   /**
    * The pullup Value is written to the entire port with each of the bits
    * in the byte affecting the corresponding I/O line. To change a single
    * I/O line's behaviour without affecting the others it is necessary to
    * read its current value before writing a new value with only the
    * corresponding bits changed. For each pin a value of 0 enables the
    * pull up resistor. And a value of 1 disables the resistor. For example
    * To set the entire port with pull up resistors turned on use val = 0
    * to turn all the pull up resistors off use val = 255 to set 0, 1, 2
    * and 3 as on and 4, 5, 6 and 7 as off use val = 240.
    * 
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the direction value for.
    * @param val The new direction value.
    *
    * @see Elexol::Port
    */
   void setPullUp(const Elexol::Port port, const vpr::Uint8 val) const
   {
      setState(port, Elexol::Command::PullUp, val);
   }

   /**
    * The threshold value is written to the entire port with each of the bits
    * in the byte affecting the corresponding I/O line. To change a single
    * I/O line's behaviour without affecting the others it is necessary to
    * read its current value before writing a new value with only the
    * corresponding bits changed. For each pin a value of 0 means to use a
    * threshold of 2.5V. And a value of 1 means to use a threshold of 1.4V.
    * For example to set the entire port with threshold voltage of 2.5V use
    * val = 0 to set all the ports input thresholds at 1.4V use val = 255
    * to set 0, 1, 2 and 3 at 1.4V and 4, 5, 6 and 7 at 2.5V use val = 15.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the direction value for.
    * @param val The new direction value.
    *
    * @see Elexol::Port
    */
   void setThreshold(const Elexol::Port port, const vpr::Uint8 val) const
   {
      setState(port, Elexol::Command::Threshold, val);
   }

   /**
    * The trigger value is written to the entire port with each of the bits
    * in the byte affecting the corresponding I/O line. To change a single
    * I/O line's behaviour without affecting the others it is necessary
    * to read its current value before writing a new value with only the
    * corresponding bits changed. For each pin a value of 0 enables the
    * Schmitt trigger. And a value of 1 disables the Schmitt trigger. For
    * example to set the entire port with Schmitt trigger inputs turned on
    * use val = 0 to turn all the Schmitt trigger inputs off use val = 255
    * and to set 0, 1, 2 and 3 as Schmitt trigger inputs and 4, 5, 6 and 7
    * as normal threshold inputs use val = 240.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the direction value for.
    * @param val The new direction value.
    *
    * @see Elexol::Port
    */
   void setSchmittTrigger(const Elexol::Port port, const vpr::Uint8 val) const
   {
      setState(port, Elexol::Command::SchmittTrigger, val);
   }

   /**
    * Returns true if the driver is active.
    */
   bool isActive() const
   {
      return mActive;
   }

   vpr::InetAddr getFixedIpAddress();
   void setFixedIpAddress(const vpr::InetAddr& ipAddr);
   void setEnableFixedIpAddress(bool val);

   vpr::Uint8 getByteValue(const Elexol::AddressType addr);
   void setByteValue(const Elexol::AddressType addr, const vpr::Uint8 value);


private:
   /**
    * Returns the state for the specified command type.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the state value for.
    * @param command The command type.
    *
    * @throw Elexol::ElexolException if an error occured while communicating with
    *        the Elxexol Ether I/O 24 device.
    *
    * @see Elexol::Port
    */
   vpr::Uint8 getState(const Elexol::Port port,
                       const Elexol::CommandType command) const;

   /**
    * Sets the state for the specified command type.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the state for.
    * @param command The command type.
    *
    * @throw Elexol::ElexolException if an error occured while communicating with
    *        the Elxexol Ether I/O 24 device.
    *
    * @see Elexol::Port
    */
   void setState(const Elexol::Port port, const Elexol::CommandType command,
                 const vpr::Uint8 value) const;

   void enableWriting();
   void disableWriting();

   std::pair<vpr::Uint8, vpr::Uint8>
      getWordValue(const Elexol::AddressType address);

   void setWordValue(const Elexol::AddressType address,
      std::pair<vpr::Uint8, vpr::Uint8> value);

private:
   bool                 mActive;  /**< If the driver is active. */
   vpr::SocketDatagram* mSocket;  /**< UDP socket used to communicate with device. */
   vpr::InetAddr        mAddress; /**< Address of the Elexol Ether I/O device. */
};

#endif /*GADGET_ELEXOL_ETHER24_STANDALONE_H*/
