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

#ifndef _CLUSTER_APPLICATION_DATA_H
#define _CLUSTER_APPLICATION_DATA_H

#include <cluster/PluginConfig.h>
#include <vpr/IO/SerializableObject.h>

#include <cluster/ClusterManager.h>
#include <plugins/ApplicationDataManager/ApplicationDataManager.h>
#include <gadget/Util/Debug.h>

namespace cluster
{

class ApplicationData : public vpr::SerializableObject
{
public:
   /**
    * Construct a new ApplicationData object.
    */
   ApplicationData()
      : mIsLocal(false)
   {;}

   /**
    * Initialize the application data for the given GUID and host name.
    *
    * @param guid      The GUID used to reference this object.
    * @param hostName  The hostname of the node that should be responsible for
    *                  updating this object.
    */
   void init(const vpr::GUID& guid, const std::string& hostName)
   {
      mId = guid;
      mHostname = hostName;

      ClusterPlugin* app_data_mgr =
         ClusterManager::instance()->getPluginByGUID(vpr::GUID("cc6ca39f-03f2-4779-aa4b-048f774ff9a5"));

      if (NULL != app_data_mgr)
      {
         app_data_mgr->addSerializableObject(this);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING:")
            << " Can not register ApplicationData with non-existent "
            << "Application Data Manager." << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_RIM, vprDBG_WARNING_LVL)
            << "In order to synchronize ApplicationData across a cluster, "
            << "you must load the ApplicationDataManager cluster plug-in."
            << std::endl << vprDEBUG_FLUSH;

         // If we are not using the cluster, then we want the data to be local,
         // even if the hostnames do not match. This will allow us to run the
         // apps anywhere without recompiling.
         mIsLocal = true;
      }
   }

   virtual ~ApplicationData()
   {
      ;
   }

   /**
    * Returns true if this object should be updated by the local node.
    */
   bool isLocal()
   {
      return mIsLocal;
   }

   /**
    * Set the boolean value that determines if the local node
    * should be responsible for updating this object.
    */
   void setIsLocal(bool local)
   {
      mIsLocal = local;
   }

   /**
    * Return the GUID for this object.
    */
   const vpr::GUID& getId() const
   {
      return mId;
   }

   /**
    * Return the hostname of the node that should update this object.
    */
   const std::string& getHostname() const
   {
      return mHostname;
   }

   /**
    * Return the hostname of the node that should update this object.
    */
   void setHostname(const std::string& hostname)
   {
      mHostname = hostname;
   }

private:
   bool        mIsLocal;   /**< True if this object is to be updated by the local node. */
   vpr::GUID   mId;        /**< GUID for this object */
   std::string mHostname;  /**< Hostname of the machine that should update this object */
};

} // end namespace gadget

#endif
