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

#ifndef _GADGET_PINCH_GLOVE_STANDALONE_H_
#define _GADGET_PINCH_GLOVE_STANDALONE_H_

#include <gadget/Devices/DriverConfig.h>
#include <string>
#include <vpr/IO/Port/SerialPort.h>


/**
 * Software interface to Fakespace Pinchglove hardware.
 *
 *
 * @note The long function names greatly decrease the
 *       ambiguity of what the functions do ... sorry. :)
 *
 * @date 1-23-99
 */
class PinchGloveStandalone
{
public:
   /// Default constructor.
   PinchGloveStandalone();
   ~PinchGloveStandalone();

   
   /** Connects to the pinch glove hardware. */
   vpr::ReturnStatus connectToHardware();
   
   std::string getGestureFromHardware();

   void setPort(const std::string& port_name)
   { mPortName=port_name; }
   const std::string& getPort()
   { return(mPortName); }
   void setBaudRate(int baud_rate)
   { mBaudRate = baud_rate; }
   int getBaudRate()
   { return(mBaudRate); }
   
   /*
   void setTimeStamp(bool time_stamps)
   { mTimeStamps = time_stamps; }
   bool getTimeStamp()
   { return(mTimeStamps); }
   */
   

   /**
    * Uses one of these indices to index the string returned by
    * GetSampledString().
    */
   enum finger
   {
      LTHUMB = 0, LINDEX = 1, LMIDDLE = 2, LRING = 3, LPINKY = 4,
      RTHUMB = 6, RINDEX = 7, RMIDDLE = 8, RRING = 9, RPINKY = 10
   };

protected:
   std::string         mGestureString;
   std::string         mPortName;
   int                 mBaudRate;
   vpr::SerialPort*    mPort;

   /// equal to "00000.00000"
   static const std::string    mOpenHandString;

   vpr::ReturnStatus       mSendCommandToHardware( const char* command );
   std::vector<vpr::Uint8> mGetDataAndTimeStampFromHardware();
   
   std::string mGetStringFromHardware();

private:
   //bool mTimeStamps;
};

#endif
