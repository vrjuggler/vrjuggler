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

//#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>

#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>


namespace cluster
{

Packet::Packet(const vpr::GUID& pluginId)
   : mPluginId(pluginId)
{
   //mData = new std::vector<vpr::Uint8>(RIM_PACKET_HEAD_SIZE);
   mPacketReader = new vpr::BufferObjectReader(&mData);
   mPacketWriter= new vpr::BufferObjectWriter(&mData);
}
Packet::Packet(std::vector<vpr::Uint8>* data) : mPluginId()
{
   // Copy data.
   mData = *(data);
   mPacketReader = new vpr::BufferObjectReader(&mData);
   mPacketWriter= new vpr::BufferObjectWriter(&mData);
}

Packet::~Packet()
{
   delete mPacketReader;
   delete mPacketWriter;
   //delete mData;
}

void Packet::dump() const
{
   std::cout << "Dumping Packet(" << mData.size() << " bytes): ";
   for ( std::vector<vpr::Uint8>::const_iterator i = mData.begin();
         i!= mData.end(); i++ )
   {
      std::cout << (int)*i << " ";
   }
   std::cout << std::endl;
}

vpr::Uint16 Packet::getPacketType() const
{
   return mHeader->getPacketType();
}

void Packet::printData(int debugLevel) const
{
   if (NULL != mHeader.get())
   {
      mHeader->printData(debugLevel);
      /*
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrCYAN,"\n====== Packet Header ======")
         << "\nRIMCode:    " << mHeader->getRIMCode()
         << "\nPacketType: " << mHeader->getPacketType()
         << "\nFrame #:    " << mHeader->getFrame()
         << "\nLength:     " << mHeader->getPacketLength() << std::endl
         << vprDEBUG_FLUSH;
      */
   }
   else
   {
   vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
      << clrOutBOLD(clrRED,"[Packet::printData] Header is still NULL, can not print data!")
      << vprDEBUG_FLUSH;
   }
}

}   // end namespace gadget
