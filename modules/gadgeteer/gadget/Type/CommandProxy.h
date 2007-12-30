/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_COMMAND_PROXY_H_
#define _GADGET_COMMAND_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <vpr/Util/Assert.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/Command.h>
#include <gadget/Type/CommandProxyPtr.h>

namespace gadget
{

/** \class CommandProxy CommandProxy.h gadget/Type/CommandProxy.h
 *
 * A proxy class to command-oriented devices, used by the Input Manager.
 *
 * A command proxy always points to a command-oriented device and unit number
 * within that device.  The Input Manager can therefore keep an array of these
 * around and treat them as command devices that only return a single
 * sub-device's amount of data (one int).
 *
 * @see gagdet::Command
 */
class GADGET_CLASS_API CommandProxy : public TypedProxy<Command>
{
protected:
   CommandProxy(const std::string& deviceName = "UnknownCommand",
                const int unitNum = -1);

public:
   /** @name Construction/Destruction */
   //@{
   /**
    * Creates a CommandProxy instance and returns it wrapped in a
    * CommandProxyPtr object.
    *
    * @since 1.3.7
    */
   static CommandProxyPtr create(const std::string& deviceName = "UnknownCommand",
                                 const int unitNum = -1);

   virtual ~CommandProxy();
   //@}

   virtual void updateData();

   /** Returns the time of the last update. */
   virtual vpr::Interval getTimeStamp() const;

   /**
    * Gets the command data.
    */
   int getData() const
   {
      // If we're stupefied, return 0, return the current command value.
      return (isStupefied() ? 0 : mData.getDigital());
   }

   CommandData* getCommandData()
   {
      return &mData;
   }

   /**
    * Returns a pointer to the gadget::Command object that we are proxying.
    */
   CommandPtr getCommandPtr()
   {
      // If we're stupefied, return NULL.  Otherwise, return mTypedDevice.
      return (isStupefied() ? CommandPtr() : mTypedDevice);
   }

   /**
    * Returns the unit index into the string speech recognition device from
    * which this proxy is reading data.
    */
   int getUnit() const
   {
      return mUnitNum;
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);

private:
   int mUnitNum;     /**<  The sub-unit of the device we are working with */

   /**
    * Copy of the digital data we are dealing with.
    * @see getData()
    */
   CommandData mData;
};

} // End of gadget namespace

#endif
