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

#ifndef _CLUSTER_USER_DATA_H
#define _CLUSTER_USER_DATA_H

#include <gadget/gadgetConfig.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationDataManager.h>
#include <cluster/SerializableData.h>
#include <vpr/IO/SerializableObject.h>
#include <vpr/Util/AttributeMapBase.h>

namespace cluster
{

template <class DEV_TYPE>
class UserData : public SerializableData
{
public:
   typedef DEV_TYPE DevType;
   UserData(std::string name, std::string hostname = "")
   {
      this->setAttrib("cluster.userdata.name", name);
      this->setAttrib("cluster.userdata.hostname", hostname);
      this->setAttrib("cluster.userdata.local", false);
      ApplicationDataManager::instance()->addUserData(this);

      mData = new DEV_TYPE;
   }

   virtual ~UserData()
   {
      ApplicationDataManager::instance()->removeUserData(this);
      delete mData;
   }

/*   vpr::ReturnStatus writeObject(vpr::BufferObjectWriter* writer)
   {
      ;
   }
   
   vpr::ReturnStatus readObject(vpr::BufferObjectReader* reader)
   {
      ;
   }
*/
   bool isLocal()
   {
      return(getAttrib<bool>("cluster.userdata.local"));
   }
   
   DEV_TYPE* operator->()
   { return mData; }
   
   DEV_TYPE& operator*()
   { return *(mData); }

private:
   DEV_TYPE*    mData;
};
   
// Default Specilizations
template<>
vpr::ReturnStatus UserData<vpr::Uint16>::writeObject(vpr::BufferObjectWriter* writer)
{
   writer->writeUint16(*mData);
}

template<>
vpr::ReturnStatus UserData<vpr::Uint16>::readObject(vpr::BufferObjectReader* reader)
{
   (*mData) = reader->readUint16();
}

template<>
vpr::ReturnStatus UserData<vpr::Uint32>::writeObject(vpr::BufferObjectWriter* writer)
{
   writer->writeUint32(*mData);
}

template<>
vpr::ReturnStatus UserData<vpr::Uint32>::readObject(vpr::BufferObjectReader* reader)
{
   (*mData) = reader->readUint32();
}

// Vector Impl
/*template<>
template<class DEV>
vpr::ReturnStatus UserData< std::vector<DEV> >::writeObject(vpr::BufferObjectWriter* writer)
{
   std::cout << "Write vpr::Uint32: " << *mData << std::endl;
}

template<>
template<class DEV>
vpr::ReturnStatus UserData< std::vector<DEV> >::readObject(vpr::BufferObjectReader* reader)
{
   std::cout << "Read vpr::Uint32:  " << *mData << std::endl;
}
*/

} // end namespace gadget

#endif
