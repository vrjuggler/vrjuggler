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

#include <gadget/Devices/Intersense/IntersenseAPI/IntersenseAPIStandalone.h>

void IntersenseAPIStandalone::init()
{
  mBaudRate = 9600;
  mPort = 0;
  mVerbose = false;
  mHandle = 0;
  mActive = false;
  mNumStations = 0;
  script = NULL;
}

bool IntersenseAPIStandalone::open(std::string dso_location)
{
  //if(!mPortName.empty()) {
  //    mHandle = ISD_OpenTrackerPort(mPortName.c_str(), FALSE, mVerbose, (DWORD)mBaudRate);
  //} else {
   std::cout << "IntersenseStandalone, trying to open: " << mPortName << " at " << mBaudRate << std::endl;
//      mHandle = ISD_OpenTracker(mPortName, mBaudRate, FALSE, mVerbose/*, (DWORD)mBaudRate*/);
  //mHandle = ISD_OpenTracker( (Hwnd) NULL, 0, FALSE, verbose );
   mHandle = ISD_OpenTracker( (Hwnd) NULL, 0, true, true , dso_location.c_str());
  //}
  bool tracker_connect = false;
  if (-1 != mHandle) {
    tracker_connect = true;
    //sendScript();
  }

  mActive = true;
  return tracker_connect;

}

bool IntersenseAPIStandalone::close()
{
  return (bool)ISD_CloseTracker( mHandle );
}

bool IntersenseAPIStandalone::updateData()
{

  bool is_data = false;

  if (FALSE != ISD_GetData(mHandle, &mData))
  {
      is_data = true;
  }
  
  return is_data;

}



//TODO: verify that data is in euler angle form. or quaternion
//FIX:  this is done in the Isense
float& IntersenseAPIStandalone::xPos( const int& i ) { return mData.Station[i].Position[0]; }
float& IntersenseAPIStandalone::yPos( const int& i ) { return mData.Station[i].Position[1]; }
float& IntersenseAPIStandalone::zPos( const int& i ) { return mData.Station[i].Position[2]; }

float& IntersenseAPIStandalone::xRot( const int& i ) { return mData.Station[i].Orientation[2]; }
float& IntersenseAPIStandalone::yRot( const int& i ) { return mData.Station[i].Orientation[1]; }
float& IntersenseAPIStandalone::zRot( const int& i ) { return mData.Station[i].Orientation[0]; }

float& IntersenseAPIStandalone::xQuat( const int& i ) { return mData.Station[i].Orientation[2]; }
float& IntersenseAPIStandalone::yQuat( const int& i ) { return mData.Station[i].Orientation[1]; }
float& IntersenseAPIStandalone::zQuat( const int& i ) { return mData.Station[i].Orientation[0]; }
float& IntersenseAPIStandalone::wQuat( const int& i ) { return mData.Station[i].Orientation[3]; }

int IntersenseAPIStandalone::buttonState(const int& i, const int& f)
{
  if(f < MAX_NUM_BUTTONS && i < mNumStations)
     return mData.Station[i].ButtonState[f];
  else
     return 0;
}


int IntersenseAPIStandalone::analogData(const int& i, const int& j) {
  if(j < MAX_ANALOG_CHANNELS && i < mNumStations)
     return mData.Station[i].AnalogData[j];
  else
     return 0;
}


