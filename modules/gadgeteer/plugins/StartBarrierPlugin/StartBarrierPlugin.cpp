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

#include <cluster/PluginConfig.h>

#include <boost/concept_check.hpp>

#include <jccl/Config/ConfigElement.h>
#include <plugins/StartBarrierPlugin/StartBarrierPlugin.h> // my header...

#include <gadget/Util/Debug.h>
#include <cluster/ClusterDepChecker.h>

#include <cluster/ClusterManager.h>
#include <cluster/ClusterNetwork/ClusterNetwork.h>
#include <cluster/ClusterNetwork/ClusterNode.h>

#include <cluster/Packets/StartBlock.h>

extern "C"
{
   GADGET_CLUSTER_PLUGIN_EXPORT(void) initPlugin(cluster::ClusterManager* mgr)
   {
      mgr->addPlugin(new cluster::StartBarrierPlugin());
   }
}

namespace cluster
{
   StartBarrierPlugin::StartBarrierPlugin()
      : mBarrierMaster(false)
      , mComplete(false)
      , mSlaveWaitingOnPlugins(false)
      , mSlowDownMaster(0)
      , mPluginGUID("566a50ff-5e73-43e0-a9a9-0fb62b76731a")
   {;}

   StartBarrierPlugin::~StartBarrierPlugin()
   {
   
   }

   void StartBarrierPlugin::handlePacket(Packet* packet, ClusterNode* node)
   {
      // On Barrier Recv
      // -If Master
      //   -Remove Pending slave
      //   -If all recved
      //     -Send responce to all nodes
      //     -Set Running TRUE
      // -Else
      //   -Set Running TRUE
      if ( NULL != packet && NULL != node )
      {

         if(packet->getPacketType() == cluster::Header::RIM_START_BLOCK)
         {
            //We are not actually using any data in this packet for now.
            
            //StartBlock* temp_start_block = dynamic_cast<StartBlock*>(packet);
            //vprASSERT(NULL != temp_start_block && "Dynamic cast failed!");
            if (isBarrierMaster())
            {
               removePendingBarrierSlave(node->getHostname());
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                  << "handlePacket() Master has received a START signal.\n" << vprDEBUG_FLUSH;         
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                  << "handlePacket() Slave has finished start barrier\n" << vprDEBUG_FLUSH;         
               mComplete = true;
               ClusterManager::instance()->setClusterReady(true);
            }      
         }
         else
         {
            vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"StartBarrierPlugin::handlePacket() ERROR: ")
               << "We do not handle this type of packet.\n" << vprDEBUG_FLUSH;         
         }
      }
   }
   
   /** Add the pending element to the configuration.
   *  @pre configCanHandle (element) == true.
   *  @return true iff element was successfully added to configuration.
   */
   bool StartBarrierPlugin::configAdd(jccl::ConfigElementPtr element)
   {
      // -If the cluster manager has been configured.

      if(!ClusterManager::instance()->isClusterActive())
      {
         // XXX: This could be made into a dependancy also.
         return false;
      }

      mPendingSlaves = ClusterManager::instance()->getClusterNodes();
      mSlaves = ClusterManager::instance()->getClusterNodes();


      /////////////////////////////////////////
      //  Starting Barrier Stuff
      //
      // -Set flag we have started configuring the cluster
      // -Get Sync Machine element Name
      // -Get ElementPtr to this element
      // -Get the Hostname of this node

      mBarrierMachineElementName = element->getProperty<std::string>(std::string("start_master"));
      jccl::ConfigElementPtr barrier_machine_element =
         ClusterManager::instance()->getConfigElementPointer(mBarrierMachineElementName);
      vprASSERT(NULL != barrier_machine_element.get() && "StartBarrierPlugin element must have a start_master.");
      mBarrierMasterHostname = barrier_machine_element->getProperty<std::string>(std::string("host_name"));

      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
         << "Start Master element Name is: "
         << mBarrierMachineElementName << std::endl << vprDEBUG_FLUSH;         
      vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_LVL)
         << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
         << "Start Master Hostname is: " << mBarrierMasterHostname
         << std::endl << vprDEBUG_FLUSH;         

      // Starting Barrier Stuff
      /////////////////////////////////////         

      if (ClusterNetwork::isLocalHost(mBarrierMasterHostname))
      {
         mBarrierMaster = true;
      }
      else
      {
         mBarrierMaster = false;
      }
      
      return true;
   }
   
   
   /** Remove the pending element from the current configuration.
   *  @pre configCanHandle (element) == true.
   *  @return true iff the element (and any objects it represented)
   *          were successfully removed.
   */
   bool StartBarrierPlugin::configRemove(jccl::ConfigElementPtr element)
   {
      boost::ignore_unused_variable_warning(element);
      return false;
   }
   
   /** Checks if this handler can process element.
   *  Typically, an implementation of handler will check the element's
   *  description name/token to decide if it knows how to deal with
   *  it.
   *  @return true iff this handler can process element.
   */
   bool StartBarrierPlugin::configCanHandle(jccl::ConfigElementPtr element)
   {
      return recognizeStartBarrierPluginConfig(element);
   }
      
   bool StartBarrierPlugin::recognizeStartBarrierPluginConfig(jccl::ConfigElementPtr element)
   {
      return element->getID() == getElementType();
   }

   void StartBarrierPlugin::preDraw()
   {;
   }
   void StartBarrierPlugin::postPostFrame()
   {
      // -If we are not complete
      //   -If all other plugins are ready
      //     -If Slave
      //       -Find the barrier master
      //       -If connected
      //         -Send a start block
      //       -Else
      //         -Add barrier node to pending list
      // -Else
      //   -If number of pending start nodes is 0
      //     -Send a start block to all of them
      
      //This is where all the real work gets done
      if (!mComplete)
      {
         if (ClusterManager::instance()->pluginsReady())
         {
            if (!mBarrierMaster)
            {
               ClusterNode* start_master = ClusterNetwork::instance()->getClusterNodeByName(mBarrierMachineElementName);
               if (NULL == start_master)
               {
                  vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) 
                     << clrOutBOLD(clrRED,"[StartBarrierPlugin] Barrier machine configuration element not yet loaded.")
                     << std::endl << vprDEBUG_FLUSH;
               }
               else if (start_master->isConnected())
               {
                  //Send packet to server machine
                  StartBlock temp_start_block(getPluginGUID(), 0);
                  start_master->send(&temp_start_block);
                  vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                     << "Sending signal to start master: " << mBarrierMasterHostname << std::endl << vprDEBUG_FLUSH;
               }
               else
               {
                  //If we are not connected and we are not in pending list, add to the pending list
                  if (NULL == ClusterNetwork::instance()->getPendingNode(start_master->getHostname()))
                  {
                     ClusterNetwork::instance()->addPendingNode(start_master);
                  }
               }
            }
            else
            {
               vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                  << "Barrier Master waiting...\n" << vprDEBUG_FLUSH;         
               int num_pending_nodes = getPendingBarrierSlaves().size();
               if (0 == num_pending_nodes)
               {
                  mComplete = true;
                  ClusterManager::instance()->setClusterReady(true);
                  std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX DONE - list=0 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
                  
                  StartBlock temp_start_block(getPluginGUID(), 0);
   
                  //Send responce to all nodes
                  for (std::vector<std::string>::iterator i = mSlaves.begin();
                       i != mSlaves.end() ; i++)
                  {
                     vprDEBUG(gadgetDBG_RIM,vprDBG_CONFIG_STATUS_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                        << "Sending start signal to slave: " << (*i) << std::endl << vprDEBUG_FLUSH;              
                     // Dead lock since we are actually in a recursion of ClusterNodes
                     ClusterNode* node = ClusterNetwork::instance()->getClusterNodeByHostname(*i);
                     if(NULL == node)
                     {
                        vprDEBUG(gadgetDBG_RIM,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrCYAN,"[StartBarrierPlugin] ")
                           << "Error, could not find a ClusterNode by the name of: " << (*i) << std::endl << vprDEBUG_FLUSH;              
                     }
                     else
                     {
                        node->send(&temp_start_block);
                     }
                  }
               }//End (0==num_pending_nodes)
            }//End (mBarrierMaster)
         }//End (Plugins Ready)
      }//End (!mComplete)
   }

   bool StartBarrierPlugin::isPluginReady()
   {
      return true;
   }
} // End of gadget namespace
