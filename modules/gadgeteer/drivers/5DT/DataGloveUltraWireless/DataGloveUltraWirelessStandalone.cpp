/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <string.h>        // for strcpy();
#include <stdio.h>
#include <stdlib.h>
#include <iostream>        // for std::cout
#include <fstream>         // for ifstream
#include <string.h>
#include <algorithm>

#include <vpr/Util/Assert.h> // for vprASSERT

#include <drivers/5DT/DataGloveUltraWireless/DataGloveUltraWirelessStandalone.h>

// Constants
#define INDEX_FINGER_BIT  0x01
#define MIDDLE_FINGER_BIT 0x02
#define RING_FINGER_BIT   0x04
#define LITTLE_FINGER_BIT 0x08

#define JOINT_OPEN 1
#define JOINT_CLOSED 0
#define JOINT_UNDEFINED -1

///////////////////////////////////////////////////////////
// Static Methods
///////////////////////////////////////////////////////////

// Return joint state for a single value with thresholds
int getSensorState(const float value, 
                   const float upper_thresh, 
                   const float lower_thresh)
{
   if(value > upper_thresh)
   {
      return JOINT_CLOSED;
   }
   else if(value < lower_thresh)
   {
      return JOINT_OPEN;
   }

   return JOINT_UNDEFINED;
}

// Return joint state from 2 sensor states
int getJointState(const int sensor1_state, const int sensor2_state)
{
   // Logic:
   // If both are undefined, joint is undefined
   // If either is closed, joint is closed (closed takes precedence)
   // If none are closed but either is open, joint is open

   if( sensor1_state == JOINT_UNDEFINED && sensor2_state == JOINT_UNDEFINED )
   {
      return JOINT_UNDEFINED;
   }
   else if( sensor1_state == JOINT_CLOSED || sensor2_state == JOINT_CLOSED )
   {
      return JOINT_CLOSED;
   }
   else if( sensor1_state == JOINT_OPEN || sensor2_state == JOINT_OPEN )
   {
      return JOINT_OPEN;
   }

   return JOINT_UNDEFINED; // undefined if the code gets to here
}

///////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////

// Constructor
DataGloveUltraWirelessStandalone::DataGloveUltraWirelessStandalone()
   : mIsActive(false),
     mGestureUpperThresh(0.65f),
     mGestureLowerThresh(0.35f),
     mGloveAGesture(-1),
     mGloveBGesture(-1),
     mAutoResetEnabled(true),
     mResetMinA(false),
     mResetMaxA(false),
     mResetMinB(false),
     mResetMaxB(false)
{
   // Initialize min/max values to the opposite of where they will likely be
   for(int i = 0; i < 14; i++)
   {
      mGloveAMin[i] = UINT_MAX;
      mGloveAMax[i] = 0;

      mGloveBMin[i] = UINT_MAX;
      mGloveBMax[i] = 0;
   }
}

// Destructor
DataGloveUltraWirelessStandalone::~DataGloveUltraWirelessStandalone()
{
   close();
}

bool DataGloveUltraWirelessStandalone::open
   (const std::string& tty_port, int baud_rate, bool port_a, bool port_b)
{
   mPortAEnabled = port_a;
   mPortBEnabled = port_b;

   // Create new serial port
   mPort = new vpr::SerialPort(tty_port);
   if( mPort != NULL )
   {
      mPort->setOpenReadWrite();
      mPort->setBlocking(false);
   }

   try
   {
      std::cout<<"\n[DataGloveUltraWireless] Connecting To 5DT Data Glove Ultra\n"<<std::flush;
      mPort->open();
   }
   catch (vpr::IOException&)
   {
      std::cout << "[DataGloveUltraWireless] Port (" << tty_port
         << ") open failed\n" << std::flush;
      mPort->close();
      return false;
   }

   std::cout << "[DataGloveUltraWireless] Port (" << tty_port
      << ") open successfully\n" << std::flush;
   mPort->clearAll();      
   mPort->setRead(true);
   mPort->setMinInputSize(1);
   mPort->setOutputBaudRate(baud_rate); // Put me before input to be safe
   mPort->setInputBaudRate(baud_rate);
   mPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
   std::cout << "[DataGloveUltraWireless] Port (" << tty_port
      << ") successfully changed the port settings\n" << std::flush;


   mIsActive = true;
   return true;
}

void DataGloveUltraWirelessStandalone::close()
{
   std::cout << "[DataGloveUltraWireless] closing" << std::endl;

   mIsActive = false;
   if ( mPort != NULL )
   {
      mPort->close();
      delete mPort;
      mPort = NULL;
   }

   std::cout << "[DataGloveUltraWireless] closed" << std::endl;
}

// Take in vector of calibrated values & return gesture integer
int DataGloveUltraWirelessStandalone::computeGesture(const std::vector<float> &record)
{
   // Define a threshold for considering a joint to be flexed
   float upper_thresh = mGestureUpperThresh;
   float lower_thresh = mGestureLowerThresh;

   // Init gesture value
   int gesture = 0;

   // Compute gesture from sensor values, or return -1 if 
   // any fingers are undefined
   int joint_state;

   // Index finger
   joint_state =
      getJointState(getSensorState(record[3], upper_thresh, lower_thresh),
                    getSensorState(record[4], upper_thresh, lower_thresh));

   if( joint_state == JOINT_UNDEFINED )
   {
      return -1;
   }
   else if( joint_state == JOINT_OPEN )
   {
      gesture |= INDEX_FINGER_BIT;
   }

   // Middle finger
   joint_state =
      getJointState(getSensorState(record[6], upper_thresh, lower_thresh),
                    getSensorState(record[7], upper_thresh, lower_thresh));

   if( joint_state == JOINT_UNDEFINED )
   {
      return -1;
   }
   else if( joint_state == JOINT_OPEN )
   {
      gesture |= MIDDLE_FINGER_BIT;
   }

   // Ring finger
   joint_state =
      getJointState(getSensorState(record[9], upper_thresh, lower_thresh),
                    getSensorState(record[10], upper_thresh, lower_thresh));

   if( joint_state == JOINT_UNDEFINED )
   {
      return -1;
   }
   else if( joint_state == JOINT_OPEN )
   {
      gesture |= RING_FINGER_BIT;
   }

   // Little finger
   joint_state =
      getJointState(getSensorState(record[12], upper_thresh, lower_thresh),
                    getSensorState(record[13], upper_thresh, lower_thresh));

   if( joint_state == JOINT_UNDEFINED )
   {
      return -1;
   }
   else if( joint_state == JOINT_OPEN )
   {
      gesture |= LITTLE_FINGER_BIT;
   }

   return gesture;
}

void DataGloveUltraWirelessStandalone::readGloveData()
{
   mDataBuffer.clear();
   mReadBuffer.clear();

   unsigned int bytes_read = 0;
   mReadBuffer.push_back(0); //Fill in first spot for simplicity

   // Find start of a record
   while(mReadBuffer[0] != 0x3c)
   {
      bytes_read = mPort->read(mReadBuffer, 1);
   }

   mDataBuffer.push_back(  ( ((mReadBuffer[0] & 0xf0) >> 4) & 0x0f ) );
   mDataBuffer.push_back(  ( ((mReadBuffer[0] & 0x0f) >> 0) & 0x0f ) );

   bytes_read = 0;

   // Read in rest of the record
   while( bytes_read < 28 )
   {
      unsigned int bytes_this_loop = 0;
      bytes_this_loop = mPort->read(mReadBuffer, 28 - bytes_read);
      bytes_read+=bytes_this_loop;

      for( unsigned int i=0; i < bytes_this_loop; ++i )
      {
         mDataBuffer.push_back( (((mReadBuffer[i] & 0xf0) >> 4) & 0x0f ));
         mDataBuffer.push_back( (((mReadBuffer[i] & 0x0f) >> 0) & 0x0f ));
      }
   }
}

void DataGloveUltraWirelessStandalone::updateMinMax(
   const unsigned int reading,
   unsigned int &min,
   unsigned int &max)
{
   if( reading > max )
   {
      max = reading;
   }

   if( reading < min )
   {
      min = reading;
   }
}

void DataGloveUltraWirelessStandalone::processGloveData(unsigned int gloveNum)
{
   const unsigned int Port_A = 0;
   const unsigned int Port_B = 1;
   const unsigned int offset = 6; // Start of first record
   unsigned int reading = 0;
   float scaled_reading;

   for( unsigned int i = 0; i < 14; i++ )
   {
      unsigned int cur_index = offset + (i * 3);
      reading = ( ( ( ( mDataBuffer[cur_index] & 0x000f) << 8) & 0x0f00 ) |
                  ( ( ( mDataBuffer[cur_index + 1] & 0x000f) << 4) & 0x00f0) |
                  ( ( ( mDataBuffer[cur_index + 2] & 0x000f) << 0) & 0x000f) );

      // Update minimum & maximum
      if( Port_A == gloveNum )
      {
         if( mResetMinA )
         {
            mGloveAMin[i] = reading;
         }

         if( mResetMaxA )
         {
            mGloveAMax[i] = reading;
         }

         updateMinMax(reading, mGloveAMin[i], mGloveAMax[i]);
      }
      else if( Port_B == gloveNum )
      {
         if( mResetMinB )
         {
            mGloveBMin[i] = reading;
         }

         if( mResetMaxB )
         {
            mGloveBMax[i] = reading;
         }

         updateMinMax(reading, mGloveBMin[i], mGloveBMax[i]);
      }

      // Calibrate: scale the reading to between 0 and 1
      if( Port_A == gloveNum )
      {
         scaled_reading =
            (float) (reading - mGloveAMin[i]) /
            (float) (mGloveAMax[i] - mGloveAMin[i]);
         mGloveAData.push_back(scaled_reading);
         //std::cout << scaled_reading << " ";
         //std::cout << reading << " ";
      }
      else if( Port_B == gloveNum )
      {
         scaled_reading =
            (float) (reading - mGloveBMin[i]) /
            (float) (mGloveBMax[i] - mGloveBMin[i]);
         mGloveBData.push_back(scaled_reading);
         //std::cout << scaled_reading << " ";
         //std::cout << reading << " ";
      }
      //std::cout << std::endl;
   }

   // Compute gestures
   if ( Port_A == gloveNum )
   {
      mGloveAGesture = computeGesture(mGloveAData);
      //std::cout << "Port A Gesture = " 
      //          << mGloveAGesture << std::endl;
   }
   else if ( Port_B == gloveNum )
   {
      mGloveBGesture = computeGesture(mGloveBData);
      //std::cout << "Port B Gesture = " 
      //          << mGloveBGesture << std::endl;
   }

   // Reset ranges on all-open & all-closed gestures
   if(mAutoResetEnabled)
   {
      mResetMinA = false;
      mResetMaxA = false;
      mResetMinB = false;
      mResetMaxB = false;

      if ( Port_A == gloveNum )
      {
         if( mGloveAGesture == 0 )
         {
            mResetMinA = true;
         }
         else if( mGloveAGesture == 15 )
         {
            mResetMaxA = true;
         }
      }
      else if ( Port_B == gloveNum )
      {
         if( mGloveBGesture == 0 )
         {
            mResetMinB = true;
         }
         else if( mGloveBGesture == 15 )
         {
            mResetMaxB = true;
         }
      }
   }
}

// Read data from the DataGloveUltraWireless hardware
bool DataGloveUltraWirelessStandalone::updateData()
{
   bool found_glove_a = !mPortAEnabled;
   bool found_glove_b = !mPortBEnabled;

   // Read a glove
   // ID glove
   // go through that glove
   // read another if needed

   while( ! found_glove_a  ||  ! found_glove_b )
   {
      readGloveData();

      if( mDataBuffer[3] == 2 || mDataBuffer[3] == 3 )
      {
         if( mPortAEnabled )
         {
            found_glove_a = true;
            processGloveData(0);
         }
      }
      else
      {
         if( mPortBEnabled )
         {
            found_glove_b = true;
            processGloveData(1);
         }
      }
      mDataBuffer.clear();
      mReadBuffer.clear();
   }

   return true;
}

const std::vector<float> DataGloveUltraWirelessStandalone::getGloveData(/*WIRELESS_GLOVE_PORT port,*/)
{
   std::vector<float> glove_data;

   std::copy(mGloveAData.begin(), mGloveAData.end(), std::back_inserter(glove_data));
   std::copy(mGloveBData.begin(), mGloveBData.end(), std::back_inserter(glove_data));
#if 0
   for(unsigned int i = 0; i < mGloveAData.size(); ++i)
   {
      data_buffer[i] = mGloveAData[i];
   }
   for(unsigned int i = 0; i < mGloveBData.size(); ++i)
   {
      data_buffer[i + 14] = mGloveBData[i];
   }
#endif
   mGloveAData.clear();
   mGloveBData.clear();
   return glove_data;
}

void DataGloveUltraWirelessStandalone::setGestureThresholds
   (float gesture_upper_thresh, float gesture_lower_thresh)
{
   mGestureUpperThresh = gesture_upper_thresh;
   mGestureLowerThresh = gesture_lower_thresh;
}

///////////////////////////////////////////////////////////
// Private methods
///////////////////////////////////////////////////////////

