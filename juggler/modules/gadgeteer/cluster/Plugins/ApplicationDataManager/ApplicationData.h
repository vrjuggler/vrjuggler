/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _CLUSTER_APPLICATION_DATA_H
#define _CLUSTER_APPLICATION_DATA_H

#include <cluster/Plugins/PluginConfig.h>
#include <vpr/IO/SerializableObject.h>

#include <cluster/ClusterManager.h>
#include <cluster/Plugins/ApplicationDataManager/ApplicationDataManager.h>
#include <gadget/Util/Debug.h>

namespace cluster
{

class ApplicationData : public vpr::SerializableObject
{
public:
   /**
    * Construct a new ApplicationData object.
    *
    * @param guid - The GUID used to reference this object.
    * @param host_name - The hostname of the node that should be responsible for updating this object.
    */
   ApplicationData(const vpr::GUID& guid, const std::string& host_name) : mIsLocal(false), mId(guid), mHostname(host_name)
   {
      ClusterPlugin* app_data_mgr = ClusterManager::instance()->getPluginByGUID(vpr::GUID("cc6ca39f-03f2-4779-aa4b-048f774ff9a5"));
      if (NULL != app_data_mgr)
      {
         app_data_mgr->addSerializableObject(this);
      }
      else
      {
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL) << clrOutBOLD(clrRED,"[ApplicationData] WARNING:")
            << "Can not register ApplicationData with non existent ApplicationDataManager. "
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG(gadgetDBG_RIM,vprDBG_WARNING_LVL)
            << "In order to syncronize ApplicationData across a cluster you must load the ApplicationDataManager ClusterPlugin."
            << std::endl << vprDEBUG_FLUSH;
      }
   }

   ApplicationData()
   {;}

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
   { mIsLocal = local; }
   
   /**
    * Return the GUID for this object.
    */
   vpr::GUID getId()
   {
      return mId;
   }

   /**
    * Return the hostname of the node that should update this object
    */
   std::string getHostname()
   {
      return mHostname;
   }

   /**
    * Return the hostname of the node that should update this object
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
