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

#include <gadget/Type/NetInput.h>

#include <gadget/Type/Proxy.h>

namespace gadget{


// constructor used when a device has been requested from us (and remote id has been given)
NetInput::NetInput(const std::string& src_device_name, Input* input_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id, uint p_data_size) : DATA_SIZE(p_data_size), DATA_TIME_SIZE(p_data_size + sizeof(float)){
   mLocalId = local_device_id;
   mRemoteId = rmt_device_id;
   mDeviceName = src_device_name;
   mAckInitialization = false;
   mNeedToResendRequest = false;
}

// constructor used when a device has been requested from us (and remote id has been given)
NetInput::NetInput(const std::string& src_device_name, Proxy* proxy_ptr, VJ_NETID_TYPE local_device_id, VJ_NETID_TYPE rmt_device_id, uint p_data_size) : DATA_SIZE(p_data_size), DATA_TIME_SIZE(p_data_size + sizeof(float)){
   mLocalId = local_device_id;
   mRemoteId = rmt_device_id;
   mDeviceName = src_device_name;
   mAckInitialization = false;
   mNeedToResendRequest = false;
}

// constructor used when we are going to request a remote device and receive data from it.
// a Proxy in the input manager will point to a NetInput in the Remote Input Manager.
// the NetInput will have "_NET_" appended to its name.
NetInput::NetInput(jccl::ConfigChunkPtr chunk, VJ_NETID_TYPE local_device_id, uint p_data_size) : DATA_SIZE(p_data_size), DATA_TIME_SIZE(p_data_size + sizeof(float)){
   mLocalId = local_device_id;
   mRemoteId = 0;
   mAckInitialization = false;
   mNeedToResendRequest = false;
   mSrcName = chunk->getFullName();
   mInstName = mSrcName;
   mInstName += "_NET_";
}

}  // end namespace gadget
