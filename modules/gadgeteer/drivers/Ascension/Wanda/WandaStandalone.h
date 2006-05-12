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

#ifndef _GADGET_ASCENSION_WANDA_STANDALONE_H_
#define _GADGET_ASCENSION_WANDA_STANDALONE_H_

#include <string>
#include <deque>
#include <vector>

#include <vpr/vpr.h>
#include <vpr/vprDomain.h>
#include <vpr/Util/Interval.h>


namespace wanda
{

class WandaStandalone
{
public:
   WandaStandalone(const std::string& portName);

   ~WandaStandalone();

   /**
    * Indicates whether this device is fully active (meaning that the serial
    * port is open and communication has been initiated).
    */
   bool isActive() const
   {
      return mRunning;
   }

   /**
    * Indicates whether the serial port is open. If isActive() returns
    * \c true, then this method will also return \c true.
    */
   bool isOpen() const;

   void setPortName(const std::string& portName);

   void setTimeout(const vpr::Interval& timeout)
   {
      mTimeout = timeout;
   }

   /**
    * Opens the serial port to which the Wanda is connected and starts the
    * communication protocol with the device.
    *
    * @post If everything completes successfully, then the serial port is
    *       open, and the Wanda is ready to be sampled. isActive() and
    *       isOpen() will return \c true.
    *
    * @throw vpr::IOException is thrown if there is a problem communciating
    *        with the serial port.
    * @throw wanda::ProtocolException is thrown if the communication protocol
    *        initiation fails.
    *
    * @note It is possible for the serial port to be opened without this
    *       device being fully activated. This depends on whether the
    *       protocol is initialized correctly after the serial port is
    *       opened.
    *
    * @see setPortName()
    * @see isActive()
    * @see isOpen()
    */
   void start();

   /**
    * Attempts to read from the device and update the input state. It is
    * possible that no sample will be available to read, and in that case,
    * the input state for this object will not change. If there is a sample
    * ready to be handled, then the button and axis states are updated
    * accordingly.
    *
    * @see getButton()
    * @see getXAxis()
    * @see getYAxis()
    */
   void sample();

   /**
    * Closes the serial port and ends communication with the Wanda device.
    *
    * @post The serial port is closed. isActive() and isOpen() will return
    *       \c false.
    */
   void stop();

   /**
    * Returns the state of the given button identifier. If button \p buttonNum
    * is pressed, then 1 is returned. Otherwise, 0 is returned.
    *
    * @param buttonNum The integer identifier of the button whose state will
    *                  be returned.
    *
    * @return If button \p buttonNum is pressed, then 1 is returned;
    *         otherwise, 0 is returned.
    *
    * @throw wanda::InvalidBUttonException is thrown if \p buttonNum is not a
    *        valid button identifier for this device.
    */
   vpr::Int8 getButton(const size_t buttonNum) const;

   /**
    * Returns the current value of the X-axis data. This value is always in
    * the range [ANALOG_MIN, ANALOG_MAX].
    */
   float getXAxis() const
   {
      return mXAxis;
   }

   /**
    * Returns the current value of the Y-axis data. This value is always in
    * the range [ANALOG_MIN, ANALOG_MAX].
    */
   float getYAxis() const
   {
      return mYAxis;
   }

   /** The minimum value of an analog axis. */
   static const float ANALOG_MIN;

   /** The maximum value of an analog axis. */
   static const float ANALOG_MAX;

private:
   /**
    * Attempts to read \p length bytes from \c mPort before the given
    * timeout expires. The number of bytes read is returned to the caller. If
    * the read cannot complete before the timeout expires, then a
    * vpr::TimeoutException is thrown.
    *
    * @param length  The number of bytes to try to read from \c mPort.
    * @param timeout The amount of time to spend trying to read \p length
    *                bytes. This parameter is optional, and it defaults to
    *                vpr::Interval::NoTimeout if not given.
    *
    * @return The number of bytes read is returned.
    *
    * @throw vpr::TimeoutException is thrown if the read operation could not
    *        complete before the given timeout expired.
    */
   vpr::Uint32
      readBytes(const vpr::Uint32 length,
                const vpr::Interval& timeout = vpr::Interval::NoTimeout);

   /** @name Serial Port Members */
   //@{
   std::string      mPortName;  /**< The port name (COM1, /dev/ttyS1, etc.) */
   vpr::SerialPort* mPort;      /**< The serial port object */
   //@}

   bool mRunning;

   vpr::Interval mTimeout;      /**< Timeout used for non-blocking reads */

   /**
    * The current data buffer as read (possibly in pieces) from the hardware.
    *
    * @see sample()
    */
   std::deque<vpr::Uint8> mDataBuffer;

   /** @name Input State */
   //@{
   std::vector<vpr::Uint8> mButtons; /**< The state of the digital buttons */
   float                   mXAxis;   /**< The current X-axis value */
   float                   mYAxis;   /**< The current Y-axis value */
   //@}
};

}


#endif /* _GADGET_ASCENSION_WANDA_STANDALONE_H_ */
