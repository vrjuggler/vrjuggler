/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef GADGET_NET_DIGITAL_H
#define GADGET_NET_DIGITAL_H

#include <gadget/gadgetConfig.h>
#include <gadget/RemoteInputManager/NetUtils.h>
#include <gadget/Type/NetInput.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/DigitalData.h>

namespace gadget{

class GADGET_CLASS_API NetDigital : public NetInput, public Digital{
protected:
   std::vector <unsigned short> mNetworkShortDigitalValues;  // holds data in network byte order
   DigitalInterface mLocalSource;  // local source of data, only used in transmitting data

public:
   // constructor for a transmitting network device
   NetDigital(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id);

   // constructor for a transmitting network proxy
   NetDigital(const std::string& src_device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id);

   // constructor for a receiving NetInput
   NetDigital(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id);
  
   // CONFIG related functions
   static std::string getChunkType() { return std::string("NetDigital"); }

   // Overriding NetInput virtual functions
   virtual void updateFromLocalSource();
   virtual void updateFromRemoteSource(char* recv_buffer, int recv_buff_len);
};

}  // end namespace gadget

#endif
