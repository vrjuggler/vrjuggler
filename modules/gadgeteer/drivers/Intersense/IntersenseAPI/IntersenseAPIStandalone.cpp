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

#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <drivers/Intersense/IntersenseAPI/IntersenseAPIStandalone.h>

bool IntersenseAPIStandalone::open(const std::string& dso_location)
{
   if(!mActive)
   {
      int port_num = convertPort(mPort); 
      mHandle = ISD_OpenTracker( (Hwnd) NULL, port_num, true, true , dso_location.c_str());
      if (-1 != mHandle) 
      {
         mActive = true;
         //sendScript();
      }
   }
   return mActive;
}

int IntersenseAPIStandalone::convertPort(const std::string& port)
{
   std::string result;
   
   for(unsigned int i = 0 ; i < port.size() ; i++)
   {
      if(isdigit(port[i]))
      {
         //std::cout << port[i] << std::endl;
         result += port[i];
      }
   }
   // If we did not find a valid number, autodetect
   if("" == result)
   {
      result = "0";
   }
   std::cout << "Converted port: " << port << " to port #" << result << std::endl;
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

int IntersenseAPIStandalone::buttonState(const int& i, const int& f)
{
   if(f < MAX_NUM_BUTTONS && i < mNumStations)
   {
      return mData.Station[i].ButtonState[f];
   }
   return 0;
}


int IntersenseAPIStandalone::analogData(const int& i, const int& j)
{
   if(j < MAX_ANALOG_CHANNELS && i < mNumStations)
   {
      return mData.Station[i].AnalogData[j];
   }
   return 0;
}
