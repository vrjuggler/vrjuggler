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

#ifndef GADGET_INTERSENSE_API_STANDALONE_H
#define GADGET_INTERSENSE_API_STANDALONE_H

#include <fstream>
#include <string>

#include <gadget/Devices/Intersense/IntersenseAPI/isense.h>

/*
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
*/

class IntersenseAPIStandalone {
public:
   IntersenseAPIStandalone() : mActive(false), 
                               mBaudRate(9600),
                               mCurrentStation(0),
                               mHandle(-1), 
                               mNumStations(0),
                               mPort(""), 
                               mVerbose(false) 
   {;}

   /*
   ~IntersenseAPIStandalone()
   {
      //if(script != NULL)
      //{
      //   delete [] script;
      //}
   }
   */

   bool open(const std::string& dso_location);
   bool close();

   /**
    * New data is taken from the tracker; if a parameter has been changed,
    * such as baud rate, this will also occur during this frame.
    */
   bool updateData();

   //TODO: Functions are Tracker specific and require an assert of isActive
    
   void setPort(const std::string& port)
   { mPort = port; }
   void setBaudRate(const int& baud_rate)
   { mBaudRate = baud_rate; }
   void setNumStations(int num)
   { mNumStations = num; }
   void setVerbose(bool verb)
   { mVerbose = verb; }
   
   const std::string getPort()
   { return mPort; }
   int getBaudRate()
   { return mBaudRate; }
   int getNumStations() 
   { return mNumStations; }
   bool getVerbose() 
   { return mVerbose; }

   /**
    * Return if the Station is ON or OFF
    */
   bool getState(int currentStation)
   //{ return mLocalConfigData[currentStation].State; }
   { return mConfigData[currentStation].State; }

   /**
    * Inertia cubes numbered 1-4.  If none is assigned the number is -1
    */
   int getInertiaCube(int currentStation) const 
   //{ return mLocalConfigData[currentStation].InertiaCube; }
   { return mConfigData[currentStation].InertiaCube; }

   /**
    * Return the enhancment value
    */
   int getEnhancement(int currentStation) const 
   //{ return mLocalConfigData[currentStation].Enhancement; }
   { return mConfigData[currentStation].Enhancement; }

   /**
    * Return the sensitivity level. (Level 1-4)
    */
   int getSensitivity(int currentStation) const 
   //{ return mLocalConfigData[currentStation].Sensitivity; }
   { return mConfigData[currentStation].Sensitivity; }

   /**
    * Get the prediction time. (0-50 ms)
    */
   int getPrediction(int currentStation) const 
   //{ return mLocalConfigData[currentStation].Prediction; }
   { return mConfigData[currentStation].Prediction; }

   /**
    * Returns the format of the angles. 
    * (0 == ISD_EULER; 1 == ISD_QUATERNION)
    */
   int getAngleFormat(int currentStation) const 
   //{ return mLocalConfigData[currentStation].AngleFormat; }
   { return mConfigData[currentStation].AngleFormat; }

   /**
    * Return whether the station should send time stamps or not.
    */
   bool getTimeStamped(int currentStation) const 
   //{ return mLocalConfigData[currentStation].TimeStamped; }
   { return mConfigData[currentStation].TimeStamped; }

   /**
    * Returns whether the station should send button values or not.
    */
   bool getButtons(int currentStation) const
   //{ return mLocalConfigData[currentStation].GetButtons; }
   { return mConfigData[currentStation].GetButtons; }

   /**
    * Returns whether the station should send analog values or not.
    */
   bool getAnalogData(int currentStation) const 
   //{ return mLocalConfigData[currentStation].GetAnalogData; }
   { return mConfigData[currentStation].GetAnalogData; }



   /////////////
   //
   
   /**
    * Return if the Station is ON or OFF
    */
   void setState(int currentStation, bool state)
   //{ mLocalConfigData[currentStation].State = state; }
   { mConfigData[currentStation].State = state; }

   /**
    * Inertia cubes numbered 1-4.  If none is assigned the number is -1
    */
   void setInertiaCube(int currentStation, int inertial_cube)
   //{ mLocalConfigData[currentStation].InertiaCube = inertial_cube; }
   { mConfigData[currentStation].InertiaCube = inertial_cube; }

   /**
    * Return the enhancment value
    */
   void setEnhancement(int currentStation, int enhancement)
   //{ mLocalConfigData[currentStation].Enhancement = enhancement; }
   { mConfigData[currentStation].Enhancement = enhancement; }

   /**
    * Return the sensitivity level. (Level 1-4)
    */
   void setSensitivity(int currentStation, int sensitivity) 
   //{ mLocalConfigData[currentStation].Sensitivity = sensitivity; }
   { mConfigData[currentStation].Sensitivity = sensitivity; }

   /**
    * Get the prediction time. (0-50 ms)
    */
   void setPrediction(int currentStation, int prediction)
   //{ mLocalConfigData[currentStation].Prediction = prediction; }
   { mConfigData[currentStation].Prediction = prediction; }

   /**
    * Returns the format of the angles. 
    * (0 == ISD_EULER; 1 == ISD_QUATERNION)
    */
   void setAngleFormat(int currentStation, int angle_format)
   //{ mLocalConfigData[currentStation].AngleFormat = angle_format; }
   { mConfigData[currentStation].AngleFormat = angle_format; }

   /**
    * Return whether the station should send time stamps or not.
    */
   void setTimeStamped(int currentStation, bool time_stamped)
   //{ mLocalConfigData[currentStation].TimeStamped = time_stamped; }
   { mConfigData[currentStation].TimeStamped = time_stamped; }

   /**
    * Returns whether the station should send button values or not.
    */
   void setButtons(int currentStation, bool get_buttons)
   //{ mLocalConfigData[currentStation].GetButtons = get_buttons; }
   { mConfigData[currentStation].GetButtons = get_buttons; }

   /**
    * Returns whether the station should send analog values or not.
    */
   void setAnalogData(int currentStation, bool get_analog_data)
   //{ mLocalConfigData[currentStation].GetAnalogData = get_analog_data; }
   { mConfigData[currentStation].GetAnalogData = get_analog_data; }

   //
   ////////////

// TODO: check for valid station call.
// WARNING: calls must surround a change to station configuration states
// returns number of station

   /**
    * Load the current configuration that is stored on the physical tracker
    * device.
    */
   void loadConfigState(int d)
   {
      ISD_GetStationConfig( mHandle, &mConfigData[d], d+1, mVerbose );

      // WARNING: the following lines should be removed.  For now this is a convenient way
      // to see what the default tracker data is (as specified by the IS interface box).
      // these numbers can then be used to initialize the configData properly
      /*
      mLocalConfigData[d].InertiaCube = mConfigData[d].InertiaCube;
      mLocalConfigData[d].Enhancement = mConfigData[d].Enhancement;
      mLocalConfigData[d].Sensitivity = mConfigData[d].Sensitivity;
      mLocalConfigData[d].Prediction = mConfigData[d].Prediction;
      mLocalConfigData[d].AngleFormat = mConfigData[d].AngleFormat;
      mLocalConfigData[d].TimeStamped = mConfigData[d].TimeStamped;
      mLocalConfigData[d].GetButtons = mConfigData[d].GetButtons;
      mLocalConfigData[d].GetAnalogData = mConfigData[d].GetAnalogData;
      mLocalConfigData[d].State = mConfigData[d].State;
      */
   }

   /**
    * Save the current configuration state to the physical tracker device.
    */
   void saveConfigState(int d)
   {
      /*
      mConfigData[d].State = mLocalConfigData[d].State;
      mConfigData[d].InertiaCube = mLocalConfigData[d].InertiaCube;
      mConfigData[d].Enhancement = mLocalConfigData[d].Enhancement;
      mConfigData[d].Sensitivity = mLocalConfigData[d].Sensitivity;
      mConfigData[d].Prediction = mLocalConfigData[d].Prediction;
      mConfigData[d].AngleFormat = mLocalConfigData[d].AngleFormat;
      mConfigData[d].TimeStamped = mLocalConfigData[d].TimeStamped;
      mConfigData[d].GetButtons = mLocalConfigData[d].GetButtons;
      mConfigData[d].GetAnalogData = mLocalConfigData[d].GetAnalogData;
      */
      ISD_SetStationConfig( mHandle, &mConfigData[d], d+1, mVerbose );
   }

   /**
    *  Send the script located in the mHandle file descriptor to the physical
    *  tracker.
    */
   /*
   bool sendScript()
   {
      //return ((ISD_SendScript(mHandle, (char*) script) == 1) ? true : false);
      return false;
   }
   */

   /*
   bool setScript(const char* inscript) 
   {
      if(script != NULL)
      {
         delete [] script;
      }
      script = new char [strlen(inscript)+3];
      strcpy(script, inscript);
      return true;
   }
   */

   /**
    * Get the x position of the i'th receiver. 
    */
   float xPos( const int& i )
   { return mData.Station[i].Position[0]; }
   /**
    * Gets the y position of the i'th receiver. 
    */
   float yPos( const int& i )
   { return mData.Station[i].Position[1]; }
   /**
    * Gets the z position of the i'th receiver. 
    */
   float zPos( const int& i )
   { return mData.Station[i].Position[2]; }

   /**
    * Gets the y rotation of the i'th receiver. 
    */
   float xRot( const int& i )
   { return mData.Station[i].Orientation[2]; }
   /**
    * Gets the y rotation of the i'th receiver. 
    */
   float yRot( const int& i )
   { return mData.Station[i].Orientation[1]; }
   /**
    * Gets the y rotation of the i'th receiver. 
    */
   float zRot( const int& i )
   { return mData.Station[i].Orientation[0]; }

   /**
    * Gets the x quaternion value of the i'th receiver. 
    */
   float xQuat( const int& i ) 
   { return mData.Station[i].Orientation[2]; }
   /**
    * Gets the y quaternion value of the i'th receiver. 
    */
   float yQuat( const int& i ) 
   { return mData.Station[i].Orientation[1]; }
   /**
    * Gets the z quaternion value of the i'th receiver. 
    */
   float zQuat( const int& i ) 
   { return mData.Station[i].Orientation[0]; }
   /**
    * Gets the w quaternion value of the i'th receiver. 
    */
   float wQuat( const int& i ) 
   { return mData.Station[i].Orientation[3]; }

   int buttonState(const int& i, const int &f);

   int analogData(const int& i, const int& j);

   bool isActive() 
   { return mActive; }

private:
   int convertPort(const std::string& port);
private:
   bool                 mActive;
   int                  mBaudRate; 
   int                  mCurrentStation; //0-3 for current 9/2000 IS900
   ISD_TRACKER_HANDLE   mHandle;    /**< The handle used to communicate with the Intersense tracker.*/
   int                  mNumStations;
   std::string          mPort;
   bool                 mVerbose;
   //char*                script;

   //Station level data
   
   ISD_STATION_INFO_TYPE   mConfigData[ISD_MAX_STATIONS];
   
//   isConfig                mLocalConfigData[ISD_MAX_STATIONS];

   
   ISD_TRACKER_DATA_TYPE   mData;
};

#endif //GADGET_INTERSENSE_API_STANDALONE_H
