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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <gadget/Devices/DriverConfig.h>
#include <sys/time.h>

// need stdio for sprintf
#include <stdio.h>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>
#include <drivers/VirtualTechnologies/CyberGlove/vt_types.h>
#include <drivers/VirtualTechnologies/CyberGlove/vt_globals.h>
#include <drivers/VirtualTechnologies/CyberGlove/vt_types.h>
#include <drivers/VirtualTechnologies/CyberGlove/vt_error.h>
#include <drivers/VirtualTechnologies/CyberGlove/CyberGlove.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::CyberGlove>(inputMgr);
}

}

namespace gadget
{

bool CyberGlove::config(jccl::ConfigElementPtr e)
{
   if(! (Input::config(c) && Glove::config(c) ))
      return false;


   vprASSERT(mThread == NULL);      // This should have been set by Input(c)

   mPortName = e->getProperty<std::string>("port");
   mBaudRate = e->getProperty<int>("baud");

   char* home_dir = e->getProperty("calibration_dir").cstring();
   if (home_dir != NULL)
   {
       mCalDir = new char [strlen(home_dir) + 1];
       strcpy(mCalDir,home_dir);
   }

   std::string glove_pos_proxy = e->getProperty("glove_position");    // Get the name of the pos_proxy
   if(glove_pos_proxy == std::string(""))
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:") << " Cyberglove has no posProxy."
         << std::endl << vprDEBUG_FLUSH;
      return false;
   }

   // init glove proxy interface
   /* XXX: Doesn't appear to be used
   int proxy_index = gadget::InputManager::instance()->getProxyIndex(glove_pos_proxy);
   if(proxy_index != -1)
   {
      mGlovePos[0] = gadget::InputManager::instance()->->getPosProxy(proxy_index);
   }
   else
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ERROR:")
         << " CyberGlove::CyberGlove: Can't find posProxy."
         << std::endl << std::endl << vprDEBUG_FLUSH;
   }
   */

   mGlove = new CyberGloveBasic( mCalDir, mPortName, mBaudRate );

   return true;
}

std::string
CyberGlove::getElementType()
{
   return "cyber_glove";
}

int
CyberGlove::startSampling()
{
   
   if (mThread == NULL)
   {
      resetIndexes();
      
      //Reset Exit flag in case of a shutdown and restart of the driver
      mExitFlag = false;
      // Create a new thread to handle the control
      vpr::ThreadMemberFunctor<CyberGlove>* memberFunctor =
         new vpr::ThreadMemberFunctor<CyberGlove>(this, &CyberGlove::controlLoop, NULL);

      mThread = new vpr::Thread(memberFunctor);

      if ( ! mThread->valid() )
      {
         return 0;   // Failure
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "gadget::CyberGlove is active " << std::endl << vprDEBUG_FLUSH;
         mActive = true;
         return 1;
      }
  }
  else
     return 0; // already sampling
}

// -Opens the glove port
// -Starts sampling the glove
void CyberGlove::controlLoop(void* nullParam)
{
   boost::ignore_unused_variable_warning(nullParam);

   // Open the port and run with it
   if(mGlove->open() == 0)
   {
     vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
        << clrOutNORM(clrRED, "ERROR:")
        << " Can't open Cyberglove or it is already opened.\n"
        << vprDEBUG_FLUSH;
     return;
   }

   // Spin in the sampling
   while(!mExitFlag)
   {
      sample();
   }
}

bool CyberGlove::sample()
{
   mGlove->sample();       // Tell the glove to sample

   copyDataFromGlove();                   // Copy the data across
   mTheData[0][progress].calcXforms();    // Update the xform data

   swapValidIndexes();
   return 1;
}

void CyberGlove::updateData()
{
vpr::Guard<vpr::Mutex> updateGuard(lock);
   // Copy the valid data to the current data so that both are valid
   mTheData[0][current] = mTheData[0][valid];   // ASSERT: we only have one glove

   // swap the indicies for the pointers
   swapCurrentIndexes();

  return;
}

bool CyberGlove::stopSampling()
{

   if (mThread != NULL)
   {
      //Signal for the thread to exit..
      mExitFlag = true;
      mThread->join();
      delete mThread;
      mThread = NULL;
      vpr::System::usleep(100);

      mGlove->close();
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "stopping CyberGlove.." << std::endl << vprDEBUG_FLUSH;
   }
   return 1;
}

CyberGlove::~CyberGlove ()
{
   stopSampling();      // Stop the glove
   delete mGlove;       // Delete the glove
}



void CyberGlove::copyDataFromGlove()
{
   CYBER_GLOVE_DATA* glove_data = mGlove->getData();     // Get ptr to data

   for(int i=0;i<GloveData::NUM_COMPONENTS;i++)
      for(int j=0;j<GloveData::NUM_JOINTS;j++)
         mTheData[0][progress].angles[i][j] = glove_data->joints[i][j];

   vprASSERT(mTheData[0][progress].angles[GloveData::MIDDLE][GloveData::MPJ]
             == glove_data->joints[MIDDLE][MCP]);
}


} // End of gadget namespace
