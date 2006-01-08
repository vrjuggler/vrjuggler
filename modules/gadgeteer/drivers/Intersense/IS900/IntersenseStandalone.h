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

///////////////////////////////////////////////////////////////////////////////
//             intersense.cpp is a c++ wrapper for isdriver.x.  isense.x
//        represents IS' c wrapper.  intersense.cpp is meant to supercede
//             isense.x in a c++ environment by both its OO nature and it's
//        modified funcionality.
//
//        1) the intersense object represents a single IS interface box
//
//        2) baud rate can now be specified through the wrapper
//
//        3) Support for intertrax has been disabled.  Without disabling this
//        support, the intertrax driver will sometimes load when the IS driver
//        fails to initially connect.  Intertrax is a 3DOF angular device, and
//        reported angular variables alone do not satisfy a complete tracking solution
//
//     4) port can now be specified by an exact device name  (ex. /dev/ttyd0)
//
//        Modified: 11/27/00                        Author: Chris Johanson
//                 Modified By: Ben Thompson
/////////////////////////////////////////////////////////////////////////////////


#ifndef isIntersense_h
#define isIntersense_h

#include <fstream>
#include <string>

//extern "C" {
#include <drivers/Intersense/IS900/isense.h>
//}

struct isConfig {
    bool State;
    int InertiaCube;
    int Enhancement;
    int Sensitivity;
    int Prediction;
    int AngleFormat;
    bool TimeStamped;
    bool GetButtons;
    bool GetAnalogData;
};

class IntersenseStandalone {
public:
    IntersenseStandalone() : mPortName("")
    {
       init();
    }

    ~IntersenseStandalone()
    {
       if(script != NULL) delete [] script;
    }

    bool open();
    bool close();
    void init();

   /** New data is taken from the tracker; if a parameter has been changed,
    *  such as baud rate, this will also occur during this frame.
    */
   bool updateData();

//TODO: Functions are Tracker specific and require an assert of isActive
    
    void setPortName(const std::string& portname)
    { mPortName = portname; }
    void setBaudRate(const int& baud_rate)
    { mBaudRate = baud_rate; }
    
    const std::string setPortName()
    { return mPortName; }
    int setBaudRate()
    { return mBaudRate; }

    int& rNumStations() {return mNumStations;}
    int NumStations() {return mNumStations;}

    bool& rVerbose() { return mVerbose; }
    bool Verbose() { return mVerbose; }

// Station is ON or OFF
    bool& rState(int currentStation) { return mLocalConfigData[currentStation].State; }
    bool State(int currentStation) { return mLocalConfigData[currentStation].State; }

// Inertia cubes numbered 1-4.  If none is assigned the number is -1
    int& rInertiaCube(int currentStation)  { return mLocalConfigData[currentStation].InertiaCube; }
    int InertiaCube(int currentStation) const { return mLocalConfigData[currentStation].InertiaCube; }

    int& rEnhancement(int currentStation)  { return mLocalConfigData[currentStation].Enhancement; }
    int Enhancement(int currentStation) const { return mLocalConfigData[currentStation].Enhancement; }

// Levels 1-4
    int& rSensitivity(int currentStation) { return mLocalConfigData[currentStation].Sensitivity; }
    int Sensitivity(int currentStation) const { return mLocalConfigData[currentStation].Sensitivity; }

// 0 - 50 ms
    int& rPrediction(int currentStation) { return mLocalConfigData[currentStation].Prediction; }
    int Prediction(int currentStation) const { return mLocalConfigData[currentStation].Prediction; }

// 0 == ISD_EULER; 1 == ISD_QUATERNION
    int& rAngleFormat(int currentStation) { return mLocalConfigData[currentStation].AngleFormat; }
    int AngleFormat(int currentStation) const { return mLocalConfigData[currentStation].AngleFormat; }

    bool& rTimeStamped(int currentStation) { return mLocalConfigData[currentStation].TimeStamped; }
    bool TimeStamped(int currentStation) const { return mLocalConfigData[currentStation].TimeStamped; }

    bool& rButtons(int currentStation) { return mLocalConfigData[currentStation].GetButtons; }
    bool Buttons(int currentStation) const { return mLocalConfigData[currentStation].GetButtons; }

    bool& rAnalogData(int currentStation) { return mLocalConfigData[currentStation].GetAnalogData; }
    bool AnalogData(int currentStation) const { return mLocalConfigData[currentStation].GetAnalogData; }

// TODO: check for valid station call.
// WARNING: calls must surround a change to station configuration states
// returns number of station
    int getConfigState(int d)
    {
       ISD_GetStationConfig( mHandle, &mConfigData[d], d+1, mVerbose );

       // WARNING: the following lines should be removed.  For now this is a convenient way
       // to see what the default tracker data is (as specified by the IS interface box).
       // these numbers can then be used to initialize the configData properly
       mLocalConfigData[d].InertiaCube = mConfigData[d].InertiaCube;
       mLocalConfigData[d].Enhancement = mConfigData[d].Enhancement;
       mLocalConfigData[d].Sensitivity = mConfigData[d].Sensitivity;
       mLocalConfigData[d].Prediction = mConfigData[d].Prediction;
       mLocalConfigData[d].AngleFormat = mConfigData[d].AngleFormat;
       mLocalConfigData[d].TimeStamped = mConfigData[d].TimeStamped;
       mLocalConfigData[d].GetButtons = mConfigData[d].GetButtons;
       mLocalConfigData[d].GetAnalogData = mConfigData[d].GetAnalogData;
       mLocalConfigData[d].State = mConfigData[d].State;
       
       return d;
    }

    int setConfigState(int d)
    {
       mConfigData[d].State = mLocalConfigData[d].State;
       mConfigData[d].InertiaCube = mLocalConfigData[d].InertiaCube;
       mConfigData[d].Enhancement = mLocalConfigData[d].Enhancement;
       mConfigData[d].Sensitivity = mLocalConfigData[d].Sensitivity;
       mConfigData[d].Prediction = mLocalConfigData[d].Prediction;
       mConfigData[d].AngleFormat = mLocalConfigData[d].AngleFormat;
       mConfigData[d].TimeStamped = mLocalConfigData[d].TimeStamped;
       mConfigData[d].GetButtons = mLocalConfigData[d].GetButtons;
       mConfigData[d].GetAnalogData = mLocalConfigData[d].GetAnalogData;

       ISD_SetStationConfig( mHandle, &mConfigData[d], d+1, mVerbose );
       return d;
    } // d  i'th station of tracker

   /**
    *  sendScript
    *  filename: the name of a file containing an intersense protocol script
    */
   bool sendScript()
   {
      //return ((ISD_SendScript(mHandle, (char*) script) == 1) ? true : false);
      return false;
   }

    bool setScript(const char* inscript) {
       if(script != NULL) delete [] script;
       script = new char [strlen(inscript)+3];
       strcpy(script, inscript);
       return true;
    }

//TODO: check for euler or quat values in the configstate.

   /** Gets the x position of the i'th receiver. */
   float& xPos( const int& i );

   /** Gets the y position of the i'th receiver. */
   float& yPos( const int& i );

   /** Gets the z position of the i'th receiver. */
   float& zPos( const int& i );

   /** Gets the z rotation of the i'th receiver. */
   float& zRot( const int& i );

   /** Gets the y rotation of the i'th receiver. */
   float& yRot( const int& i );

   /** Gets the x rotation of the i'th receiver. */
   float& xRot( const int& i );

   /** Gets the x quaternion value of the i'th receiver. */
   float& xQuat( const int& i);

   /** Gets the y quaternion value of the i'th receiver. */
   float& yQuat( const int& i);

   /** Gets the z quaternion value of the i'th receiver. */
   float& zQuat( const int& i);

   /** Gets the w quaternion value of the i'th receiver. */
   float& wQuat ( const int& i );

    int buttonState(const int& i, const int &f);

    int analogData(const int& i, const int& j);

    bool isActive() {return mActive;}

private:

//fcn called to apply current mConfigData
    void configStation();

//Tracker level data
    ISD_TRACKER_HANDLE mHandle;
//    ISD_TRACKER_TYPE mType;
    int mBaudRate; //fix was implemented in all c drivers to user defined baudrate
    int mPort;
    bool mVerbose;
    bool mActive;
    std::string mPortName; //fix was implemented in all c drivers to allow precise control over the port used
    char* script;
//Station level data
    int mCurrentStation; //0-3 for current 9/2000 IS900
    ISD_STATION_INFO_TYPE mConfigData[ISD_MAX_STATIONS];
    isConfig mLocalConfigData[ISD_MAX_STATIONS];

    ISD_DATA_TYPE mData;
    int mNumStations;
};

#endif //isIntersense_h
