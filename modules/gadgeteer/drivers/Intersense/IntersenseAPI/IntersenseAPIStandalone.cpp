/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
#include <drivers/Intersense/IntersenseAPI/IntersenseAPIStandalone.h>

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
      }

      ISD_GetTrackerConfig(mHandle, &mInfo, true);
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
   return (bool)ISD_CloseTracker( mHandle );
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
