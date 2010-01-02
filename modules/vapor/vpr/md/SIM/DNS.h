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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SIM_DNS_H_
#define _VPR_SIM_DNS_H_

#include <vpr/vprConfig.h>

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <vpr/Util/Singleton.h>
#include <vpr/vprTypes.h> /* Uint32 */

namespace vpr
{

namespace sim
{

   /** \class DNS DNS.h vpr/md/SIM/DNS.h
    *
    * Do lookups on addresses, giving results for each unique query.
    */
   class VPR_CLASS_API DNS : public vpr::Singleton<DNS>
   {
   public:
      DNS() : mUniqueGenerator(0)
      {
         mDnsAddressLookup["localhost"] = 0x7F000001;
      }

      /**
       * Given an Internet name or dotted-decimal address, this returns the
       * IP address in network byte order.
       */
      vpr::Uint32 lookupAddress(const std::string& addr);

   protected:

      /** Use this to look up an address from a name. */
      std::map< std::string, vpr::Uint32 > mDnsAddressLookup;

      vpr::Uint32 mUniqueGenerator;
   };

} // sim namespace

} // vpr namespace

#endif
