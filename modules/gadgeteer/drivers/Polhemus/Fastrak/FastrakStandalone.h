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

#ifndef _GADGET_FASTRAK_STANDALONE_H_
#define _GADGET_FASTRAK_STANDALONE_H_

#include <stdio.h>
#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/IO/Port/SerialPort.h>

#include <gmtl/Matrix.h>
#include <boost/lexical_cast.hpp>

namespace Fastrak
{
   enum HEMISPHERE
   {
      FORWARD_HEM, BACK_HEM, RIGHT_HEM, LEFT_HEM, LOWER_HEM, UPPER_HEM, AUTO_HEM
   };

   enum UNITS
   {
      INCHES, CENTIMETERS
   };

   inline std::string getHemiString(HEMISPHERE hemi)
   {
      if(FORWARD_HEM == hemi)
      { return "Forward (+X)"; }
      else if(BACK_HEM == hemi)
      { return "Back (-X)"; }
      else if(RIGHT_HEM == hemi)
      { return "Right (+Y)"; }
      else if(LEFT_HEM == hemi)
      { return "Left (-Y)"; }
      else if(LOWER_HEM == hemi)
      { return "Lower (+Z)"; }
      else if(UPPER_HEM == hemi)
      { return "Upper (-Z)"; }
      else if(AUTO_HEM == hemi)
      { return "Automatic"; }
      else
      { return "Unknown"; }
   }

   inline std::string getHemiData(const int station, HEMISPHERE hemi)
   {
      std::string data = boost::lexical_cast<std::string>(station) + ",";

      if(FORWARD_HEM == hemi)
      { data += "1,0,0\r"; }
      else if(BACK_HEM == hemi)
      { data += "-1,0,0\r"; }
      else if(RIGHT_HEM == hemi)
      { data += "0,1,0\r"; }
      else if(LEFT_HEM == hemi)
      { data += "0,-1,0\r"; }
      else if(LOWER_HEM == hemi)
      { data += "0,0,1\r"; }
      else if(UPPER_HEM == hemi)
      { data += "0,0,-1\r"; }
      else if(AUTO_HEM == hemi)
      { data += "0,0,0\r"; }
      else
      {
         throw vpr::Exception("Invalid hemisphere specified.", VPR_LOCATION);
      }

      return data;
   }
   namespace Command
   {
      const vpr::Uint8 Point('P');
      const vpr::Uint8 BinaryMode('f');
      const vpr::Uint8 AsciiMode('F');
      const vpr::Uint8 EnableContinuous('C');
      const vpr::Uint8 DisableContinuous('c');
      const vpr::Uint8 SetOutputList('O');
      const vpr::Uint8 StationStatus('l');
      const vpr::Uint8 SetHemisphere('H');
      const vpr::Uint8 SetInches('U');
      const vpr::Uint8 SetCentimeters('u');
      const vpr::Uint8 SystemStatus('S');
      const vpr::Uint8 SetStylusButton('e');
   };
}


class FastrakStandalone
{
public:
   FastrakStandalone(const std::string& port, const int baud)
      : mPort(port)
      , mBaud(baud)
      , mSerialPort(NULL)
      , mHemisphere(Fastrak::FORWARD_HEM)
      , mUnits(Fastrak::INCHES)
   {
      mReadTimeout = vpr::Interval(2,vpr::Interval::Sec);
      mStationStatus.resize(4, false);
      mStationData.resize(4, gmtl::Matrix44f());
   }

   /**
    * Opens the port to which the device is connected.
    *
    * @pre The configuration must have been set using setConfig().
    */
   bool open();

   /**
    * Initialize the tracker with default settings.
    *
    * This sets the output mode to binary and asks the tracker to return position
    * and quaternion data.
    */
   void init();

   /**
    * Sets the port to use.
    * This will be a string in the form of the native OS descriptor.<BR>
    * ex: unix - "/dev/ttyS0", win32 - "COM3"
    */
   void setPort(const std::string& serialPort)
   {
      mPort = serialPort;
   }

   /**
    * Gets the port used.
    * @see setPort for a description of the string format
    */
   const std::string& getPort() const
   {
      return mPort;
   }

   /** Sets the baud rate. */
   void setBaudRate(const int& baud)
   {
      mBaud = baud;
   }

   /** Gets the baud rate. */
   const int& getBaudRate() const
   {
      return mBaud;
   }

   /**
    * Read a single data sample.
    *
    * @post The latest tracker data has been placed into mStationData.
    */
   void readData();

   /**
    * Tell the tracker to return data in binary or ascii mode.
    */
   void setBinaryMode(const bool binary);

   /**
    * Tell the tracker which hemisphere the recevier for the given station will be in.
    *
    * @param station Index of the station to set hemisphere of operation for.
    * @param hemi The hemisphere that the receiver is in.
    * @note If hemi is AUTO_HEM, then the receiver must be in the hemisphere
    *       of the previously set hemisphere.
    */
   void setHemisphere(const vpr::Uint16 station, Fastrak::HEMISPHERE hemi);

   /**
    * Set the units to use for input/output.
    */
   void setUnits(Fastrak::UNITS units);

   /**
    * Set wether the stylus button should be enabled or not.
    */
   void setStylusButtonEnabled(const vpr::Uint16 station, bool enabled);

   /**
    * Tell the tracker what data we want to receive.
    */
   void setOutputDataList(const vpr::Uint16 station, const std::string& list);

   /**
    * @brief Return the current status of the given station.
    *
    * @param station The index (1-4) of the station to get status for.
    * @returns True if the given station is enabled.
    */
   bool getStationStatus(const vpr::Uint16 station);

   /**
    * Print the current status information for the tracker.
    */
   void printStatus();

   /**
    * Return the position of the given station.
    */
   gmtl::Matrix44f& getStationPosition(unsigned int station)
   {
      vprASSERT(station >= 1 && station <= 4 && "Invalid station index.");
      return mStationData[station];
   }

private:
   /**
    * Send a command to the tracker.
    */
   void sendCommand(vpr::Uint8 cmd, std::string data = std::string(""));

   /**
    * Get a 32-bit floating point value from the front of the given buffer.
    */
   float getFloatValue(vpr::Uint8* buff);

   /**
    * Process tracker data read from serial port.
    */
   void processDataRecord(std::vector<vpr::Uint8>& dataRecord);
private:
   std::string       mPort;         /**< Port name to open for serial port connection */
   int               mBaud;         /**< Baud rate to use for connection */
   vpr::SerialPort*  mSerialPort;   /**< Serial port object connected to the bird */

   vpr::Interval        mReadTimeout;   /**< Standard timeout for all reads */
   std::vector<bool>    mStationStatus; /**< Active status of each station. */
   vpr::Uint32          mNumActiveStations;
   std::vector<gmtl::Matrix44f> mStationData;
   Fastrak::HEMISPHERE          mHemisphere;
   Fastrak::UNITS               mUnits;
};

#endif //_GADGET_FASTRAK_STANDALONE_H_
