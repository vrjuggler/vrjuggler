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

#include <gadget/Devices/Intersense/IS900/IntersenseStandalone.h>

void IntersenseStandalone::init()
{
  mBaudRate = 9600;
  mPort = 0;
  mVerbose = false;
  mHandle = 0;
  mActive = false;
  mNumStations = 0;
  script = NULL;
}

bool IntersenseStandalone::open()
{
  //if(!mPortName.empty()) {
  //    mHandle = ISD_OpenTrackerPort(mPortName.c_str(), FALSE, mVerbose, (DWORD)mBaudRate);
  //} else {
   std::cout << "IntersenseStandalone, trying to open: " << mPortName << " at " << mBaudRate << std::endl;
      mHandle = ISD_OpenTracker(mPortName, mBaudRate, FALSE, mVerbose/*, (DWORD)mBaudRate*/);
  //}
  bool tracker_connect = false;
  if (-1 != mHandle) {
    tracker_connect = true;
    //sendScript();
  }

  mActive = true;
  return tracker_connect;

}

bool IntersenseStandalone::close()
{
  return (bool)ISD_CloseTracker( mHandle );
}

bool IntersenseStandalone::updateData()
{

  bool is_data = false;

  if (FALSE != ISD_GetTrackerData(mHandle, &mData))
  {
      is_data = true;
  }
  
  return is_data;

}



//TODO: verify that data is in euler angle form. or quaternion
//FIX:  this is done in the Isense
float& IntersenseStandalone::xPos( const int& i ) { return mData.Station[i].Position[0]; }
float& IntersenseStandalone::yPos( const int& i ) { return mData.Station[i].Position[1]; }
float& IntersenseStandalone::zPos( const int& i ) { return mData.Station[i].Position[2]; }

float& IntersenseStandalone::xRot( const int& i ) { return mData.Station[i].Orientation[2]; }
float& IntersenseStandalone::yRot( const int& i ) { return mData.Station[i].Orientation[1]; }
float& IntersenseStandalone::zRot( const int& i ) { return mData.Station[i].Orientation[0]; }

float& IntersenseStandalone::xQuat( const int& i ) { return mData.Station[i].Orientation[2]; }
float& IntersenseStandalone::yQuat( const int& i ) { return mData.Station[i].Orientation[1]; }
float& IntersenseStandalone::zQuat( const int& i ) { return mData.Station[i].Orientation[0]; }
float& IntersenseStandalone::wQuat( const int& i ) { return mData.Station[i].Orientation[3]; }

int IntersenseStandalone::buttonState(const int& i, const int& f)
{
  if(f < MAX_NUM_BUTTONS && i < mNumStations)
     return mData.Station[i].ButtonState[f];
  else
     return 0;
}


int IntersenseStandalone::analogData(const int& i, const int& j) {
  if(j < MAX_ANALOG_CHANNELS && i < mNumStations)
     return mData.Station[i].AnalogData[j];
  else
     return 0;
}


