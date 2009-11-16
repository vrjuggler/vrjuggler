/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include <iostream>
#include <stdlib.h>
#include <ctype.h>

#include <vpr/Util/Debug.h>

#include <drivers/Intersense/IntersenseAPI/IntersenseAPIStandalone.h>


static std::string getTrackerTypeString(DWORD type)
{
   std::string type_str;

   switch (type)
   {
      case ISD_PRECISION_SERIES:
         type_str = "Precision Series";
         break;
      case ISD_INTERTRAX_SERIES:
         type_str = "InterTrax Series";
         break;
      default:
         type_str = "None";
         break;
   }

   return type_str;
}

static std::string getTrackerModelString(DWORD model)
{
   std::string model_str;

   switch (model)
   {
      case ISD_IS300:
         model_str = "IS-300";
         break;
      case ISD_IS600:
         model_str = "IS-600";
         break;
      case ISD_IS900:
         model_str = "IS-900";
         break;
      case ISD_INTERTRAX:
         model_str = "InterTrax";
         break;
      default:
         model_str = "Unknown";
         break;
   }

   return model_str;
}

static void printTrackerInfo(const ISD_TRACKER_INFO_TYPE& info)
{
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "InterSense tracker information:" << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "* InterSense Library version: " << info.LibVersion << std::endl
      << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "* Tracker type: " << getTrackerTypeString(info.TrackerType)
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "* Tracker model: " << getTrackerModelString(info.TrackerModel)
      << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "* Port number: " << info.Port << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "* Sync frequency: " << info.SyncRate << std::endl << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "* Sync phase: " << info.SyncPhase << "%" << std::endl
      << vprDEBUG_FLUSH;

   if ( info.TrackerModel == ISD_IS900 )
   {
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "* Ultrasonic timeout: " << info.UltTimeout << std::endl
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "* Ultrasonic speaker volume: " << info.UltVolume << std::endl
         << vprDEBUG_FLUSH;
   }

   vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "* Firmware revision: " << info.FirmwareRev << std::endl
      << vprDEBUG_FLUSH;
}

bool IntersenseAPIStandalone::open(const std::string& dsoLocation)
{
   if (!mActive)
   {
      int port_num = convertPort(mPort); 
      mHandle = ISD_OpenTracker((Hwnd) NULL, port_num, true, true,
                                dsoLocation.c_str());
      if (-1 != mHandle) 
      {
         mActive = true;
         const Bool got_config = ISD_GetTrackerConfig(mHandle, &mInfo, true);

         if ( got_config )
         {
/*
            if ( tracker_info.LibVersion < 3.58 )
            {
               vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
                  << clrOutBOLD(clrYELLOW, "WARNING")
                  << ": This driver only works with version 3.58 or newer of "
                  << "the InterSense Library." << std::endl << vprDEBUG_FLUSH;
               vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
                  << "Version " << tracker_info.LibVersion << " was loaded.\n"
                  << vprDEBUG_FLUSH;
            }
*/

            printTrackerInfo(mInfo);
         }
      }
   }

   return mActive;
}

int IntersenseAPIStandalone::convertPort(const std::string& port)
{
   std::string result;

   for (unsigned int i = 0 ; i < port.size() ; i++)
   {
      if (isdigit(port[i]))
      {
         result += port[i];
      }
   }
   
   // If we did not find a valid number, autodetect
   if ("" == result)
   {
      result = "0";
   }
   return(atoi(result.c_str()));
}

bool IntersenseAPIStandalone::close()
{
   // ISD_CloseTracker() returns true if the tracker identified by mHandle
   // was closed. If it is closed, then mActive needs to be false.
   const bool closed = static_cast<bool>(ISD_CloseTracker(mHandle));
   mActive = ! closed;
   return closed;
}

bool IntersenseAPIStandalone::updateData()
{
   if (FALSE != ISD_GetData(mHandle, &mData))
   {
      return true; 
   }
   return false;
}

int IntersenseAPIStandalone::buttonState(const unsigned int i,
                                         const unsigned int f)
{
   if (f < ISD_MAX_BUTTONS && i < ISD_MAX_STATIONS)
   {
      return mData.Station[i].ButtonState[f];
   }
   return 0;
}


int IntersenseAPIStandalone::analogData(const unsigned int i,
                                        const unsigned int j)
{
   if (j < ISD_MAX_CHANNELS && i < ISD_MAX_STATIONS)
   {
      return mData.Station[i].AnalogData[j];
   }
   return 0;
}
