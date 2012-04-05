/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _GADGET_HAT_PROXY_H_
#define _GADGET_HAT_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Hat.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/HatProxyPtr.h>
#include <vpr/Util/Assert.h>

namespace gadget
{

/** \class HatProxy HatProxy.h gadget/Type/HatProxy.h
 *
 * A proxy class to hat devices, used by the Input Manager.
 *
 * A hat proxy always points to a hat device and unit number within
 * that device.  The Input Manager can therefore keep an array of these
 * around and treat them as hat devices that only return a single
 * sub-device's amount of data (one int).
 *
 * @see gagdet::Hat
 */
class GADGET_API HatProxy
   : public TypedProxy<Hat>
{
public:
   /** @since 2.1.1 */
   typedef HatProxy::typed_proxy_ base_type;

protected:
   HatProxy(const std::string& deviceName = "UnknownHat",
                const int unitNum = -1);

public:
   /** @name Construction/Destruction */
   //@{
   /**
    * Creates a HatProxy instance and returns it wrapped in a
    * HatProxyPtr object.
    *
    * @since 1.3.7
    */
   static HatProxyPtr create(const std::string& deviceName = "UnknownHat",
                                 const int unitNum = -1);

   virtual ~HatProxy();
   //@}

   virtual void updateData();

   static HatState::State getNextState(const HatState::State prevState,
                                           const HatState::State curState);

   static std::string getElementType();
};

} // End of gadget namespace

#endif
