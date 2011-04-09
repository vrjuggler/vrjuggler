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

#include <cluster/PluginConfig.h>

#include <gadget/Util/Debug.h>

#include <vpr/IO/BufferObjectWriter.h>
#include <cluster/Packets/DataPacket.h>
#include <plugins/ApplicationBarrierManager/ApplicationBarrier.h>
#include <plugins/ApplicationBarrierManager/ApplicationBarrierServer.h>

namespace cluster
{

ApplicationBarrierServer::ApplicationBarrierServer(const vpr::GUID& guid,  ApplicationBarrier* userData, const vpr::GUID& pluginGuid)
   : mApplicationBarrier(userData)
   , mDataPacket()
{
   // Create a DataPacket
   mDataPacket = DataPacket::create(pluginGuid, guid);
}

ApplicationBarrierServerPtr ApplicationBarrierServer::create(const vpr::GUID& guid,
                                                       ApplicationBarrier* userData,
                                                       const vpr::GUID& pluginGuid)
{
   return ApplicationBarrierServerPtr(new ApplicationBarrierServer(guid, userData, pluginGuid));
}

ApplicationBarrierServer::~ApplicationBarrierServer()
{
   // User is responsible to clean up mApplicationBarrier
}

void ApplicationBarrierServer::serializeAndSend()
{
   mDataPacket->serialize(*mApplicationBarrier);

   cluster::ClusterManager::instance()->getNetwork()->sendToAll(mDataPacket);
}

void ApplicationBarrierServer::debugDump(int debug_level)
{
   vpr::DebugOutputGuard dbg_output(gadgetDBG_RIM,debug_level,
                              std::string("-------------- ApplicationBarrierServer --------------\n"),
                              std::string("------------------------------------------\n"));

   vprDEBUG(gadgetDBG_RIM,debug_level) << "Name:     " << getId().toString() << std::endl << vprDEBUG_FLUSH;
}

vpr::GUID ApplicationBarrierServer::getId() 
{ 
   return(mApplicationBarrier->getId()); 
}

} // End of cluster namespace
