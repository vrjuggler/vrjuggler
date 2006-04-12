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
#include <vpr/Util/Exception.h>

#include <boost/algorithm/string.hpp>

#include <map>

namespace Elexol
{
   // ------ Elexol exceptions ------ //
   /** Base exception for Elexol errors. */
   class ElexolException : public vpr::Exception
   {
   public:
      ElexolException(const std::string& msg, const std::string& location = "")
         : vpr::Exception(msg, location)
      {
         /* Do nothing. */ ;
      }

      virtual ~ElexolException() throw ()
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

   namespace Command
   {
      const vpr::Uint8 Value(0);
      const vpr::Uint8 Direction('!');
      const vpr::Uint8 PullUp('@');
      const vpr::Uint8 Threshold('#');
      const vpr::Uint8 SchmittTrigger('$');
      const vpr::Uint8 EEPROM('\'');
      const vpr::Uint8 ReadWord('R');
      const vpr::Uint8 WriteWord('W');
      const vpr::Uint8 EraseWord('E');
      const vpr::Uint8 WriteDisable('0');
      const vpr::Uint8 WriteEnable('1');
      const vpr::Uint8 Reset('@');
      const vpr::Uint8 EchoByte('`');
      const vpr::Uint8 EchoSpace('*');
      const vpr::Uint8 SendHostData('%');
      const vpr::Uint8 PortReadOffset('a');
      const vpr::Uint8 PortWriteOffset('A');
      const vpr::Uint8 ControlBitsAddress(5);
      const vpr::Uint8 IpMSBytesAddress(6);
      const vpr::Uint8 IpLSBytesAddress(7);
   }
   
   typedef vpr::Uint8 CommandType;
   typedef vpr::Uint8 Port;
   const Port PortA(0);
   const Port PortB(1);
   const Port PortC(2);
}

class Ether24Standalone
{
public:
   Ether24Standalone()
      : mActive(false)
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
    * @throw vpr::SocketException is thrown if the UDP socket could
    *        not be created.
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

private:
   /**
    * Returns the state for the specified command type.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the direction value for.
    * @param command The command type.
    *
    * @throw vpr::IOException if an error occured while communicating with
    *        the Elxexol Ether I/O 24 device.
    * @see Elexol::Port
    */
   vpr::Uint8 getState(const Elexol::Port port,
                       const Elexol::CommandType command) const;

   /**
    * Sets the state for the specified command type.
    *
    * @pre This driver must be active (open has been called)
    * @param port The port that you wish to change the direction value for.
    * @param command The command type.
    *
    * @throw vpr::IOException if an error occured while communicating with
    *        the Elxexol Ether I/O 24 device.
    * @see Elexol::Port
    */
   void setState(const Elexol::Port port, const Elexol::CommandType command,
                 const vpr::Uint8 value) const;

   void enableWriting();
   void disableWriting();

   std::pair<vpr::Uint8, vpr::Uint8>
      getMemoryValue(const Elexol::CommandType address);

   void setMemoryValue(const Elexol::CommandType address,
      std::pair<vpr::Uint8, vpr::Uint8> value);

private:
   bool                 mActive;  /**< If the driver is active. */
   vpr::SocketDatagram* mSocket;  /**< UDP socket used to communicate with device. */
   vpr::InetAddr        mAddress; /**< Address of the Elexol Ether I/O device. */
};

#endif /*GADGET_ELEXOL_ETHER24_STANDALONE_H*/
