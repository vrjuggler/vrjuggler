/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _CLUSTER_PLUGIN_H
#define _CLUSTER_PLUGIN_H

#include <gadget/gadgetConfig.h>
#include <gadget/PacketHandler.h>

#include <string> 
#include <boost/concept_check.hpp>

#include <vpr/Util/Assert.h>

#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl/Config/ConfigElementPtr.h>

namespace vpr
{
   class GUID;
   class SerializableObject;
}
   
namespace cluster
{
   class Packet;

/** \class ClusterPlugin ClusterPlugin.h cluster/ClusterPlugin.h
 *
 * Interface for cluster plugins.
 */
class GADGET_CLASS_API ClusterPlugin 
   : public jccl::ConfigElementHandler, public gadget::PacketHandler
{
public:
   ClusterPlugin();
   virtual ~ClusterPlugin();

   void setActive(bool active);
   bool isActive();
   
   virtual void preDraw() = 0;
   virtual void postPostFrame() = 0;
   virtual std::string getPluginName() = 0;

   virtual void sendRequests()
   {;}

   virtual void addSerializableObject(vpr::SerializableObject* object)
   {
      boost::ignore_unused_variable_warning(object);
      vprASSERT(false && "Adding a serializableObject is not supported by this plugin.");
   }

   virtual bool isPluginReady()
   {
      return true;
   }

   virtual bool createBarrier()
   {
      return false;
   }
   
   //virtual bool isPluginReady() = 0;
   virtual bool configCanHandle(jccl::ConfigElementPtr element) = 0;
   virtual bool configAdd(jccl::ConfigElementPtr element) = 0;
   virtual bool configRemove(jccl::ConfigElementPtr element) = 0;

private:   
   bool  mActive;
};

} // end namespace gadget

#endif
