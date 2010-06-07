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

#ifndef _GADGET_DATA_GLOVE_STANDALONE_H_
#define _GADGET_DATA_GLOVE_STANDALONE_H_

#include <gadget/Devices/DriverConfig.h>
#include <string>
#include <vpr/IO/Port/SerialPort.h>
#include <algorithm>


/**
 * Software interface to wireless 5DT Data Glove Ultra hardware.
 *
 * @note The 5DT wireless DataGloveUltraWireless is a transmit-only device.
 * @note The glove continuously emits a glove data packet.
 *
 */

class DataGloveUltraWirelessStandalone
{
public:


   /** Default constructor. */
   DataGloveUltraWirelessStandalone()
      : mIsActive(false),
        mGestureUpperThresh(0.65f),
        mGestureLowerThresh(0.35f),
        mGloveAGesture(-1),
        mGloveBGesture(-1)
   {;}

   /** Default destructor. */
   ~DataGloveUltraWirelessStandalone();

   /** Connects to the Data Glove Ultra */
   bool open( const std::string& tty_port, int baud_rate, 
              bool port_a, bool port_b );

   /** Closes connection to the Data Glove Ultra */
   void close();

   /** Reads data from the Data Glove Ultra Ultra */
   bool updateData();

   /** Copies data from the Data Glove Ultra */
   const std::vector<float> getGloveData(/*WIRELESS_GLOVE_PORT port,*/);

   bool isActive() { return mIsActive; }

   /** Sets upper & lower thresholds for gesture detection */
   void setGestureThresholds(float gesture_upper_thresh, float gesture_lower_thresh);
   
   /** Returns current gesture integer (refer to 5DT documentation for meaning)
     * or -1 for glove on Port A
     */
   const int getGloveAGesture()
   {
      return mGloveAGesture;
   }
   
   /** Returns current gesture integer (refer to 5DT documentation for meaning)
     * or -1 for glove on Port B
     */
   const int getGloveBGesture()
   {
      return mGloveBGesture;
   }

protected:

   void readGloveData();
   void processGloveData(unsigned int gloveNum);
   int computeGesture(const std::vector<float> &record);

   // Port pointer
   vpr::SerialPort     *mPort;

   std::vector<vpr::Uint8>    mReadBuffer;
   std::vector<vpr::Uint16>   mDataBuffer;

   bool  mStartCalibration;

   bool  mPortAEnabled;
   bool  mPortBEnabled;

   std::vector<float>   mGloveAData;
   std::vector<float>   mGloveBData;
   vpr::Uint16          mGloveAID;
   vpr::Uint16          mGloveBID;
   int                  mGloveAGesture;
   int                  mGloveBGesture;

   unsigned int  mGloveAMin[14];
   unsigned int  mGloveAMax[14];
   unsigned int  mGloveBMin[14];
   unsigned int  mGloveBMax[14];
   bool          mIsActive;

   float mGestureUpperThresh;
   float mGestureLowerThresh;

};

#endif
