/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <string>

#include <drivers/Intersense/IntersenseAPI/isense/isense.h>


class IntersenseAPIStandalone
{
public:
   IntersenseAPIStandalone()
      : mActive(false)
      , mHandle(-1)
      , mPort("")
      , mVerbose(false) 
   {;}

   bool open(const std::string& dsoLocation);
   bool close();

   /**
    * New data is taken from the tracker; if a parameter has been changed,
    * such as baud rate, this will also occur during this frame.
    */
   bool updateData();

   //TODO: Functions are Tracker specific and require an assert of isActive
    
   void setPort(const std::string& port)
   {
      mPort = port;
   }

   void setVerbose(const bool verb)
   {
      mVerbose = verb;
   }
   
   const std::string getPort() const
   {
      return mPort;
   }

   bool getVerbose() const
   {
      return mVerbose;
   }

   /**
    * Return if the Station is ON or OFF.
    */
   bool getState(const unsigned int currentStation) const
   {
      return mConfigData[currentStation].State;
   }

   /**
    * Inertia cubes numbered 1-4.  If none is assigned the number is -1.
    */
   int getInertiaCube(const unsigned int currentStation) const 
   {
      return mConfigData[currentStation].InertiaCube;
   }

   /**
    * Return the enhancment value.
    */
   int getEnhancement(const unsigned int currentStation) const 
   {
      return mConfigData[currentStation].Enhancement;
   }

   /**
    * Return the sensitivity level (Level 1-4).
    */
   int getSensitivity(const unsigned int currentStation) const 
   {
      return mConfigData[currentStation].Sensitivity;
   }

   /**
    * Get the prediction time (0-50 ms).
    */
   int getPrediction(const unsigned int currentStation) const 
   {
      return mConfigData[currentStation].Prediction;
   }

   /**
    * Returns the format of the angles. 
    * (0 == ISD_EULER; 1 == ISD_QUATERNION)
    */
   int getAngleFormat(const unsigned int currentStation) const 
   {
      return mConfigData[currentStation].AngleFormat;
   }

   /**
    * Return whether the station should send time stamps or not.
    */
   bool getTimeStamped(const unsigned int currentStation) const 
   {
      return mConfigData[currentStation].TimeStamped;
   }

   /**
    * Identifies whether the given station has button and joystick inputs
    * enabled.
    */
   bool hasInputs(const unsigned int currentStation) const
   {
      return mConfigData[currentStation].GetInputs;
   }

   /////////////
   //
   
   /**
    * Return if the Station is ON or OFF.
    */
   void setState(const unsigned int currentStation, const bool state)
   {
      mConfigData[currentStation].State = state;
   }

   /**
    * Inertia cubes numbered 1-4.  If none is assigned the number is -1.
    */
   void setInertiaCube(const unsigned int currentStation,
                       const int inertialCube)
   {
      mConfigData[currentStation].InertiaCube = inertialCube;
   }

   /**
    * Return the enhancment value.
    */
   void setEnhancement(const unsigned int currentStation,
                       const int enhancement)
   {
      mConfigData[currentStation].Enhancement = enhancement;
   }

   /**
    * Return the sensitivity level (Level 1-4).
    */
   void setSensitivity(const unsigned int currentStation,
                       const int sensitivity)
   {
      mConfigData[currentStation].Sensitivity = sensitivity;
   }

   /**
    * Get the prediction time (0-50 ms).
    */
   void setPrediction(const unsigned int currentStation, const int prediction)
   {
      mConfigData[currentStation].Prediction = prediction;
   }

   /**
    * Returns the format of the angles. 
    * (0 == ISD_EULER; 1 == ISD_QUATERNION)
    */
   void setAngleFormat(const unsigned int currentStation,
                       const int angleFormat)
   {
      mConfigData[currentStation].AngleFormat = angleFormat;
   }

   /**
    * Return whether the station should send time stamps or not.
    */
   void setTimeStamped(const unsigned int currentStation,
                       const bool timeStamped)
   {
      mConfigData[currentStation].TimeStamped = timeStamped;
   }

   /**
    * Sets whether the digital and joystick inputs on the given station
    * should be used.
    */
   void setInputs(const unsigned int currentStation, const bool hasInputs)
   {
      mConfigData[currentStation].GetInputs = hasInputs;
   }

   /**
    * Returns an identifier (ISD_SYSTEM_MODEL) describing the type of system.
    */
   int getSystemModel() const
   {
      return mInfo.TrackerModel;
   }

   /**
    * Returns an identifier (ISD_SYSTEM_TYPE) describing 3DOF or 6DOF.
    */
   int getSystemType() const
   {
      return mInfo.TrackerType;
   }

   /**
    * Returns an identifier (ISD_INTERFACE_TYPE) describing the communication
    * interface.
    */
   int getInterfaceType() const
   {
      return mInfo.Interface;
   }

   //
   ////////////

// TODO: check for valid station call.
// WARNING: calls must surround a change to station configuration states
// returns number of station

   /**
    * Load the current configuration that is stored on the physical tracker
    * device.
    */
   void loadConfigState(const int d)
   {
      ISD_GetStationConfig( mHandle, &mConfigData[d], d+1, mVerbose );
   }

   /**
    * Save the current configuration state to the physical tracker device.
    */
   void saveConfigState(const int d)
   {
      ISD_SetStationConfig( mHandle, &mConfigData[d], d+1, mVerbose );
   }

   /**
    * Get the x position of the i'th receiver. 
    */
   float xPos(const unsigned int i) const
   {
      return mData.Station[i].Position[0];
   }

   /**
    * Gets the y position of the i'th receiver. 
    */
   float yPos(const unsigned int i) const
   {
      return mData.Station[i].Position[1];
   }

   /**
    * Gets the z position of the i'th receiver. 
    */
   float zPos(const unsigned int i) const
   {
      return mData.Station[i].Position[2];
   }

   /**
    * Gets the y rotation of the i'th receiver. 
    */
   float xRot(const unsigned int i) const
   {
      return mData.Station[i].Orientation[2];
   }

   /**
    * Gets the y rotation of the i'th receiver. 
    */
   float yRot(const unsigned int i) const
   {
      return mData.Station[i].Orientation[1];
   }

   /**
    * Gets the y rotation of the i'th receiver. 
    */
   float zRot(const unsigned int i) const
   {
      return mData.Station[i].Orientation[0];
   }

   /**
    * Gets the x quaternion value of the i'th receiver. 
    */
   float xQuat(const unsigned int i) const
   {
      return mData.Station[i].Orientation[2];
   }

   /**
    * Gets the y quaternion value of the i'th receiver. 
    */
   float yQuat(const unsigned int i) const
   {
      return mData.Station[i].Orientation[1];
   }

   /**
    * Gets the z quaternion value of the i'th receiver. 
    */
   float zQuat(const unsigned int i) const
   {
      return mData.Station[i].Orientation[0];
   }

   /**
    * Gets the w quaternion value of the i'th receiver. 
    */
   float wQuat(const unsigned int i) const
   {
      return mData.Station[i].Orientation[3];
   }

   /**
    * Gets the time stamp of the i'th receiver. 
    */
   float timeStamp(const unsigned int i) const
   {
      return mData.Station[i].TimeStamp;
   }

   int buttonState(const unsigned int i, const unsigned int f);

   int analogData(const unsigned int i, const unsigned int j);

   bool isActive() const
   {
      return mActive;
   }

private:
   int convertPort(const std::string& port);

   bool                 mActive;
   ISD_TRACKER_HANDLE   mHandle;    /**< The handle used to communicate with the Intersense tracker.*/
   std::string          mPort;
   bool                 mVerbose;

   //Station level data
   
   ISD_STATION_INFO_TYPE   mConfigData[ISD_MAX_STATIONS];
   ISD_TRACKER_DATA_TYPE   mData;
   ISD_TRACKER_INFO_TYPE   mInfo;
};

#endif //GADGET_INTERSENSE_API_STANDALONE_H
