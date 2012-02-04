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

#ifndef _CLUSTER_PLUGIN_H
#define _CLUSTER_PLUGIN_H

#include <gadget/gadgetConfig.h>
#include <gadget/PacketHandler.h>

#include <string> 
#include <boost/concept_check.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <vpr/Util/Assert.h>

#include <jccl/RTRC/ConfigElementHandler.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <cluster/ClusterPluginPtr.h>

namespace vpr
{
   class SerializableObject;
}
   
namespace cluster
{

/** \class ClusterPlugin ClusterPlugin.h cluster/ClusterPlugin.h
 *
 * Interface for cluster plugins.
 */
class GADGET_API ClusterPlugin 
   : public gadget::PacketHandler,
     public boost::enable_shared_from_this<ClusterPlugin>
{
protected:
   ClusterPlugin();
public:
   virtual ~ClusterPlugin();

   void setActive(bool active);
   bool isActive();
   
   virtual void preDraw() = 0;
   virtual void postPostFrame() = 0;
   virtual std::string getPluginName() = 0;

   virtual void addSerializableObject(vpr::SerializableObject* object)
   {
      boost::ignore_unused_variable_warning(object);
      vprASSERT(false && "Adding a serializableObject is not supported by this plugin.");
   }

   virtual bool isPluginReady()
   {
      return true;
   }

   virtual bool configCanHandle(jccl::ConfigElementPtr elm)
   {
      boost::ignore_unused_variable_warning(elm);
      return false;
   }

   virtual bool configAdd(jccl::ConfigElementPtr elm)
   {
      boost::ignore_unused_variable_warning(elm);
      return false;
   }

   virtual bool configRemove(jccl::ConfigElementPtr elm)
   {
      boost::ignore_unused_variable_warning(elm);
      return false;
   }

private:   
   bool  mActive;
};

} // end namespace gadget

#endif
