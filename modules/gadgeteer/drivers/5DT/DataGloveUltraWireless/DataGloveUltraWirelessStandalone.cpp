/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vpr/Util/Assert.h> // for vprASSERT

#include <drivers/5DT/DataGloveUltraWireless/DataGloveUltraWirelessStandalone.h>

///////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////

// Destructor
DataGloveUltraWirelessStandalone::~DataGloveUltraWirelessStandalone()
{
   close();
}

bool DataGloveUltraWirelessStandalone::open(const std::string& tty_port, int baud_rate, bool port_a, bool port_b)
{
   mStartCalibration = true;
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
   mIsActive = false;
   if ( mPort != NULL )
   {
      mPort->close();
      delete mPort;
      mPort = NULL;
   }
}

#if 0
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
#endif

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

void DataGloveUltraWirelessStandalone::processGloveData(unsigned int gloveNum)
{
   const unsigned int Port_A = 0;
   const unsigned int Port_B = 1;
   const unsigned int offset = 6; // Start of first record
   unsigned int reading = 0;

   for( unsigned int i = 0; i < 14; i++ )
   {
      unsigned int cur_index = offset + (i * 3);
      reading = ( ( ( ( mDataBuffer[cur_index] & 0x000f) << 8) & 0x0f00 ) |
                  ( ( ( mDataBuffer[cur_index + 1] & 0x000f) << 4) & 0x00f0) |
                  ( ( ( mDataBuffer[cur_index + 2] & 0x000f) << 0) & 0x000f) );

      if( mStartCalibration )
      {
         if( Port_A == gloveNum )
         {
            mGloveAMin[i] = reading;
            mGloveAMax[i] = reading;
         }
         else if( Port_B == gloveNum )
         {
            mGloveBMin[i] = reading;
            mGloveBMax[i] = reading;
         }
      }
      else
      {
         if( Port_A == gloveNum )
         {
            if( reading > mGloveAMax[i] )
            {
               mGloveAMax[i] = reading;
            }
            if( reading < mGloveAMin[i] )
            {
               mGloveAMin[i] = reading;
            }
         }
         else if( Port_B == gloveNum )
         {
            if( reading > mGloveBMax[i] )
            {
               mGloveBMax[i] = reading;
            }
            if( reading < mGloveBMin[i] )
            {
               mGloveBMin[i] = reading;
            }
         }
      }

      if( Port_A == gloveNum )
      {
         // Auto Calibrate
         float top_div = (reading - mGloveAMin[i]);
         float bottom_div = (mGloveAMax[i] - mGloveAMin[i]);
         float new_reading = (top_div / bottom_div);
         mGloveAData.push_back(new_reading);
         //std::cout << new_reading << " ";
         //std::cout << reading << " ";
      }
      else if( Port_B == gloveNum )
      {
         // Auto Calibrate
         float top_div = (reading - mGloveBMin[i]);
         float bottom_div = (mGloveBMax[i] - mGloveBMin[i]);
         float new_reading = (top_div / bottom_div);
         mGloveBData.push_back(new_reading);
         //std::cout << new_reading << " ";
         //std::cout << reading << " ";
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
   #if 0
   std::cout << "going to read some gloves!" << std::endl;
   if( found_glove_a )
   {
      std::cout << "Don't need glove a" << std::endl;
   }

   if( found_glove_b )
   {
      std::cout << "Don't need glove b" << std::endl;
   }
   #endif

   while( ! (found_glove_a && found_glove_b) )
   {
      readGloveData();

      if( mDataBuffer[3] == 2 || mDataBuffer[3] == 3 )
      {
         if( mPortAEnabled )
         {
            //std::cout << "A" << std::endl;
            found_glove_a = true;
            processGloveData(0);
         }
      }
      else
      {
         if( mPortBEnabled )
         {
            //std::cout << "B" << std::endl;
            found_glove_b = true;
            processGloveData(1);
         }
      }
      mDataBuffer.clear();
      mReadBuffer.clear();
   }

   mStartCalibration = false;
   //std::cout << std::endl;

   return true;
}

void DataGloveUltraWirelessStandalone::copyGloveData(/*WIRELESS_GLOVE_PORT port,*/ std::vector<float>& data_buffer)
{
   for(unsigned int i = 0; i < mGloveAData.size(); ++i)
   {
      data_buffer[i] = mGloveAData[i];
   }
   for(unsigned int i = 0; i < mGloveBData.size(); ++i)
   {
      data_buffer[i + 14] = mGloveBData[i];
   }
   mGloveAData.clear();
   mGloveBData.clear();
}

///////////////////////////////////////////////////////////
// Private methods
///////////////////////////////////////////////////////////

