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

#include <string.h>        // for strcpy();
#include <stdio.h>
#include <stdlib.h>
#include <iostream>        // for std::cout
#include <fstream>         // for ifstream
#include <string.h>

#include <vpr/Util/Assert.h> // for vprASSERT

#include <gadget/Devices/5DT/DataGlove/DataGloveStandalone.h>

///////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////

// Constructor
DataGloveStandalone::DataGloveStandalone()
{
}

// Destructor
DataGloveStandalone::~DataGloveStandalone()
{
    if ( port != NULL )
    {
        port->close();
        delete port;
        port = NULL;
    }
}

int generate_gestures(float motion1/*Index flexure*/, float motion2/*Middle flexure*/, float motion3/*Ring flexure*/, float motion4/*Pinky flexure*/)
{
   // motion>0.5 --> flexed 
   // motion<0.5 --> unflexed

   // motion1 - Index finger
   // motion2 - Middle finger
   // motion3 - Ring finger
   // motion4 - Pinky finger
   
   // If anyone wants to add new gestures, it is welcome to do so.
   if (motion1>0.5 && motion2>0.5 && motion3>0.5 && motion4>0.35) {
      return 0;
   } else if (motion1<0.5 && motion2>0.5 && motion3>0.5 && motion4>0.35) {
      return 1;
   } else if (motion1>0.5 && motion2<0.5 && motion3>0.5 && motion4>0.35) {
      return 2;
   } else if (motion1<0.5 && motion2<0.5 && motion3>0.5 && motion4>0.35) {
      return 3;
   } else if (motion1>0.5 && motion2<0.5 && motion3<0.5 && motion4>0.35) {
      return 4;
   } else if (motion1<0.5 && motion2>0.5 && motion3<0.5 && motion4>0.35) {
      return 5;
   } else if (motion1>0.5 && motion2<0.5 && motion3<0.5 && motion4>0.35) {
      return 6;
   } else if (motion1<0.5 && motion2<0.5 && motion3<0.5 && motion4>0.35) {
      return 7;
   } else if (motion1>0.5 && motion2>0.5 && motion3>0.5 && motion4<0.35) {
      return 8;
   } else if (motion1<0.5 && motion2<0.5 && motion3>0.5 && motion4<0.35) {
      return 9;
   } else if (motion1>0.5 && motion2<0.5 && motion3>0.5 && motion4<0.35) {
      return 10;
   } else if (motion1<0.5 && motion2<0.5 && motion3>0.5 && motion4<0.35) {
      return 11;
   } else if (motion1>0.5 && motion2>0.5 && motion3<0.5 && motion4<0.35) {
      return 12;
   } else if (motion1<0.5 && motion2>0.5 && motion3<0.5 && motion4<0.35) {
      return 13;
   } else if (motion1>0.5 && motion2<0.5 && motion3<0.5 && motion4<0.35) {
      return 14;
   } else if (motion1<0.5 && motion2<0.5 && motion3<0.5 && motion4<0.35) {
      return 15;
   } else {
      return 16;
   }

}

// Read data from the DataGlove hardware
int DataGloveStandalone::ReadRecordsFromHardware( float *ch0, float *ch1, float *ch2, float *ch3, float *ch4, float *ch5, float *ch6 , int *ch7, int *ch8 )
{
   unsigned char buf[10];   // Define buffer
   unsigned char packet[10];   // Define packet
   vpr::Uint32 written;

   port->read(buf, 1, written);
   while(buf[0]!=0x80) // Read data untill read the header.
   {
      port->read(buf, 1, written);
   }
   // If read the start buffer, read the rest 9 data.
   port->read(&packet, 9, written);

   // Decode the report and store the values 
   *ch0 = float(packet[0])/255.0;   // Thumb
   *ch1 = float(packet[1])/255.0;   // Index
   *ch2 = float(packet[2])/255.0;   // Middle
   *ch3 = float(packet[3])/255.0;   // Ring
   *ch4 = float(packet[4])/255.0;   // Pinkie
   *ch5 = 180*float(packet[5])/255.0;   // Pitch
   *ch6 = 180*float(packet[6])/255.0;   // Roll
   // Define Gestures based on four fingers' flexures and return the value into ch7
   *ch7 = generate_gestures(*ch1,*ch2,*ch3,*ch4);
   *ch8 = int(packet[8]);   // 0:Right hand, 6:Left hand

      
   return 0;   
}

// Connect to the DataGlove hardware
bool DataGloveStandalone::connectToHardware(const std::string& ttyPort, int mBaudRate)
{
    std::cout<<"\n[dataglove] Connecting To DataGlove Hardware\n"<<std::flush;
    int result = mConnectToHardware( ttyPort , mBaudRate);
    if (result == 1)
    {
        std::cout<<"[dataglove] Connected to DataGlove hardware on port "<<ttyPort<<"\n"<<std::flush;
    }
    else
    {
        std::cout<<"[dataglove] connectToHardware(\""<<ttyPort<<"\") returned "<<(result == 1 ? "true":"false")<<"\n"<<std::flush;
    }
    std::cout<<"\n"<<std::flush;
    return result == 1 ? true:false;
}

///////////////////////////////////////////////////////////
// Private methods
///////////////////////////////////////////////////////////
int DataGloveStandalone::mConnectToHardware(const std::string& ttyPort, int baud)
{
   //const int BUFFER_LEN = 100;
   //char buf[BUFFER_LEN];
   //int cnt;
   //vpr::Uint32 written;

   // Create new serial port
   port = new vpr::SerialPort(ttyPort);
   port->setOpenReadWrite();

   if (!port->open().success()) 
   {
      std::cout<<"[dataGlove] Port ("<<ttyPort<<") open failed\n"<<std::flush;
      port->close();
      return 0;
	}
   else
   {
      std::cout<<"[dataGlove] Port ("<<ttyPort<<") open success\n"<<std::flush;
      port->clearAll();      
      baud = 9600;
      port->enableRead();
      port->setBufferSize(1);
      port->setOutputBaudRate(baud); // Put me before input to be safe
      port->setInputBaudRate(baud);
      port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
      std::cout<<"[dataGlove] Port ("<<ttyPort<<") successfully changed the port settings\n"<<std::flush;
   }

   return 1;
}
