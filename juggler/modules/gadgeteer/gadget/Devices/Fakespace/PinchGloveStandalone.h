/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <gadget/gadgetConfig.h>
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

   /**
    * Call updateStringFromHardware to get the most current pinch data.
    */
   void updateStringFromHardware();

   /**
    * Gets the last sampled string.
    *
    * @note call updateStringFromHardware to get most current pinch data.
    */
   void getSampledString( std::string& gestureString );

   void setPort(const std::string& port_name)
   {
       mPortName=port_name;
   }
   const std::string& getPort()
   {
       return(mPortName);
   }
   void setBaudRate(int baud_rate)
   {
      mBaudRate = baud_rate;
   }
   int getBaudRate()
   {
      return(mBaudRate);
   }

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
   std::string         mPreviousGestureString;
   std::string         mPortName;
   int                 mBaudRate;
   vpr::SerialPort*    mPort;

   /// equal to "00000.00000"
   static const std::string    mOpenHandString;

   /* functions provided by fakespace */
   //int       mConnectToHardware( const unsigned char* const ttyPort = "/dev/ttyd3" );
   vpr::ReturnStatus     mConnectToHardware();
   int     mSendCommandToHardware( const char* const command, unsigned char *reply );

   int     mReadRecordsFromHardware( const int& rec_max_len, unsigned char *records );
   void    mGetStringFromHardware( char string[12] );

private:
   char mGestureStringTemp[12];
   int  first;
};

#endif
