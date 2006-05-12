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

#ifndef GADGET_VIRTUAL_DEVICE_H
#define GADGET_VIRTUAL_DEVICE_H

#include <gadget/gadgetConfig.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/Input.h>

#include <vpr/IO/BufferObjectReader.h>

namespace gadget
{
   /** \class VirtualDevice VirtualDevice.h gadget/VirtualDevice.h
    *
    * Virtual input device type.
    */
   class GADGET_CLASS_API VirtualDevice
   {
   public:
      VirtualDevice(const std::string& name, const vpr::GUID& local_id,
                    const std::string& base_type, const std::string& hostname,
                    gadget::Input* device);

      ~VirtualDevice();

      void debugDump(int debug_level);

      std::string getName()
      {
         return mName;
      }

      void setName(std::string name)
      {
         mName = name;
      }
      
      std::string getRemoteHostname()
      {
         return mRemoteHostname;
      }
      
      vpr::GUID getId()
      {
         return mId;
      }
      
      void setLocalId(vpr::GUID id)
      {
         mId = id;
      }

      gadget::Input* getDevice()
      {
         return mDevice;
      }
      
      void setDevice(gadget::Input* device)
      {
         mDevice = device;
      }

//      vpr::BufferObjectReader* getBufferObjectReader() { return mBufferObjectReader; }
//      void setBufferObjectReader(vpr::BufferObjectReader* object_reader) { mBufferObjectReader = object_reader; }
private:
         std::string          mName;                     /**< VirtualDevice name */
         std::string          mBaseType;
         std::string          mRemoteHostname;
         vpr::GUID            mId;
         gadget::Input*       mDevice;
         vpr::BufferObjectReader*   mBufferObjectReader;      
   };

} // end namespace gadget

#endif
