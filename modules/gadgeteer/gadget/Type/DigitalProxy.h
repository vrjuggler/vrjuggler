/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_DIGITAL_PROXY_H_
#define _GADGET_DIGITAL_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/DigitalProxyPtr.h>
#include <vpr/Util/Assert.h>

namespace gadget
{

/** \class DigitalProxy DigitalProxy.h gadget/Type/DigitalProxy.h
 *
 * A proxy class to digital devices, used by the Input Manager.
 *
 * A digital proxy always points to a digital device and unit number within
 * that device.  The Input Manager can therefore keep an array of these
 * around and treat them as digital devices that only return a single
 * sub-device's amount of data (one int).
 *
 * @see gagdet::Digital
 */
class GADGET_CLASS_API DigitalProxy : public TypedProxy<Digital>
{
public:
   /** @since 2.1.1 */
   typedef TypedProxy<Digital> base_type;

protected:
   DigitalProxy(const std::string& deviceName = "UnknownDigital",
                const int unitNum = -1);

public:
   /** @name Construction/Destruction */
   //@{
   /**
    * Creates a DigitalProxy instance and returns it wrapped in a
    * DigitalProxyPtr object.
    *
    * @since 1.3.7
    */
   static DigitalProxyPtr create(const std::string& deviceName = "UnknownDigital",
                                 const int unitNum = -1);

   virtual ~DigitalProxy();
   //@}

   virtual void updateData();

   static std::string getElementType();
};

} // End of gadget namespace

#endif
