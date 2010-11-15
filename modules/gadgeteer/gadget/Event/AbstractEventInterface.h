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

#ifndef _GADGET_ABSTRACT_EVENT_INTERFACE_H_
#define _GADGET_ABSTRACT_EVENT_INTERFACE_H_

#include <gadget/gadgetConfig.h>

#include <string>
#include <vector>
#include <boost/noncopyable.hpp>

#include <gadget/Event/EventGeneratorPtr.h>


namespace gadget
{

/** \class AbstractEventInterface AbstractEventInterface.h gadget/Event/AbstractEventInterface.h
 *
 * @since 2.1.2
 */
class GADGET_CLASS_API AbstractEventInterface : private boost::noncopyable
{
protected:
   AbstractEventInterface();

public:
   virtual ~AbstractEventInterface();

   const std::string& getProxyName() const
   {
      return mProxyName;
   }

   virtual bool isConnected() const = 0;

   virtual void refresh() = 0;

   virtual const EventGeneratorPtr& getEventGenerator() const = 0;

   static void refreshAllInterfaces();

protected:
   void setProxyName(const std::string& proxyName)
   {
      mProxyName = proxyName;
      refresh();
   }

private:
   std::string mProxyName;

   static std::vector<AbstractEventInterface*> sAllocatedInterfaces;
};

}


#endif /* _GADGET_ABSTRACT_EVENT_INTERFACE_H_ */
