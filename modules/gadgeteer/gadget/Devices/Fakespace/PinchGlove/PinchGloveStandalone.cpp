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

#include <gadget/Devices/DriverConfig.h>
#include <stdio.h>
#include <vpr/vpr.h>
#include <vpr/System.h>
#include <gadget/Util/Debug.h>
#include <gadget/Devices/Fakespace/PinchGlove/PinchGloveStandalone.h>

const std::string PinchGloveStandalone::mOpenHandString = "00000.00000";
   
const unsigned short START_BYTE_DATA = 0x80;
const unsigned short START_BYTE_DATA_TS = 0x81;
const unsigned short START_BYTE_TEXT = 0x82;
const unsigned short END_BYTE = 0x8F;


///////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////

// Constructor
PinchGloveStandalone::PinchGloveStandalone()
{
   mGestureString = mOpenHandString;
   //mTimeStamps = false;
}

PinchGloveStandalone::~PinchGloveStandalone()
{
   if ( mPort != NULL )
   {
      mPort->close();
      delete mPort;
      mPort = NULL;
   }
}

vpr::ReturnStatus PinchGloveStandalone::connectToHardware()
{
   std::string temp_string;

   // Create a new serial port
   mPort = new vpr::SerialPort(mPortName);    
   
   mPort->setOpenReadWrite();
   
   if ( !mPort->open().success() )
   {
      return(vpr::ReturnStatus::Fail);
   }
   else
   {
      std::cout<<"[PinchGloveStandalone] Port ("<< mPortName <<") open success\n"<<std::flush;
      mPort->setBufferSize(1);
      mPort->setOutputBaudRate(mBaudRate); // Put me before input to be safe
      mPort->setInputBaudRate(mBaudRate);
      mPort->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
      std::cout<<"[PinchGloveStandalone] Port ("<< mPortName <<") successfully changed the port settings\n"<<std::flush;
   }

   // Turn time stamps on
   mSendCommandToHardware("T1");
   vpr::System::msleep(25);
   temp_string = mGetStringFromHardware();
   if ( temp_string != "1" )
   {
      std::cout<<"[PinchGloveStandalone] Could not turn time stamps on, result should == '1' but result == '" << temp_string << "'" << std::endl;
      // We must close the serial port since we are going to quit trying now
      //mPort->close();
      return(vpr::ReturnStatus::Fail);
   }
   else
   {  
      std::cout<<"[PinchGloveStandalone] Turned time stamps on.\n";
   }
   
   
   // Version compatability
   
   mSendCommandToHardware("V1");
   vpr::System::msleep(25);
   temp_string = mGetStringFromHardware();
   if ( temp_string != "1" )
   {
      std::cout << "[PinchGloveStandaone] Could not set to version 1 formatting, result should == '1' but result == '" << temp_string <<"'" << std::endl;
      // We must close the serial port since we are going to quit trying now
      //mPort->close();
      return(vpr::ReturnStatus::Fail);
   }
   else
   {
      printf("[PinchGloveStandalone] Set to version 1 formatting\n");
   }
   
   // Get the configuration information and print it
   printf("[PinchGloveStandalone] Configuration:\n");
   
   mSendCommandToHardware("CP");
   vpr::System::msleep(25);
   std::cout << mGetStringFromHardware() << std::endl;
   

   mSendCommandToHardware("CL");
   vpr::System::msleep(25);
   std::cout << mGetStringFromHardware() << std::endl;

   mSendCommandToHardware("CR");
   vpr::System::msleep(25);
   std::cout << mGetStringFromHardware() << std::endl;

   mSendCommandToHardware("CT");
   vpr::System::msleep(25);
   std::cout << mGetStringFromHardware() << std::endl;

   return(vpr::ReturnStatus::Succeed);
}


std::string PinchGloveStandalone::getGestureFromHardware()
{
   static char nges = '0';
   int touch_count;
   char gesture[12];
   
   std::vector<vpr::Uint8> testing = mGetDataAndTimeStampFromHardware();
   
   // if anything outputs from mReadRecordsFromHardware(), then...
   if ( testing.size() > 0)
   {
      strncpy( gesture, mOpenHandString.data(), 12 );
      
      // Because of timestamps
      touch_count = testing.size()/2;
      touch_count--;

      for ( int i=0; i<touch_count; ++i )
      {
         //for debug only
         //printf("DEBUG %02X.%02X\n",testing[2*i],testing[2*i+1]);
         switch ( i )
         {
            case 0: nges = '1'; break;
            case 1: nges = '2'; break;
            case 2: nges = '3'; break;
            case 3: nges = '4'; break;
            case 4: nges = '5'; break;
            default: nges = '0';
         }

         // left hand
         switch ( testing[2*i] )
         {
         case 0x01 :  gesture[4] = nges; break;   
         case 0x02 :  gesture[3] = nges; break;
         case 0x04 :  gesture[2] = nges; break;
         case 0x08 :  gesture[1] = nges; break;
         case 0x10 :  gesture[0] = nges; break;
            
         case 0x11 :  gesture[0] = nges; gesture[4] = nges; break;
         case 0x12 :  gesture[0] = nges; gesture[3] = nges; break;
         case 0x14 :  gesture[0] = nges; gesture[2] = nges; break;
         case 0x18 :  gesture[0] = nges; gesture[1] = nges; break;         
         
         case 0x13 :  gesture[0] = nges; gesture[3] = nges; gesture[4] = nges; break;
         case 0x15 :  gesture[0] = nges; gesture[2] = nges; gesture[4] = nges; break;
         case 0x16 :  gesture[0] = nges; gesture[2] = nges; gesture[3] = nges; break;
         
         case 0x19 :  gesture[0] = nges; gesture[1] = nges; gesture[4] = nges; break;
         case 0x1A :  gesture[0] = nges; gesture[1] = nges; gesture[3] = nges; break;
         case 0x1C :  gesture[0] = nges; gesture[1] = nges; gesture[2] = nges; break;

         case 0x1B :  gesture[0] = nges; gesture[1] = nges; gesture[3] = nges; gesture[4] = nges; break;
         case 0x1D :  gesture[0] = nges; gesture[1] = nges; gesture[2] = nges; gesture[4] = nges; break;
         
         case 0x03 :  gesture[3] = nges; gesture[4] = nges; break;
         case 0x05 :  gesture[2] = nges; gesture[4] = nges; break;
         case 0x06 :  gesture[2] = nges; gesture[3] = nges; break;
         case 0x09 :  gesture[1] = nges; gesture[4] = nges; break;
         case 0x0A :  gesture[1] = nges; gesture[3] = nges; break;
         case 0x0C :  gesture[1] = nges; gesture[2] = nges; break;
                                 
         case 0x07 :  gesture[2] = nges; gesture[3] = nges; gesture[4] = nges; break;
         case 0x0B :  gesture[1] = nges; gesture[3] = nges; gesture[4] = nges; break;
         case 0x0D :  gesture[1] = nges; gesture[2] = nges; gesture[4] = nges; break;
         case 0x0E :  gesture[1] = nges; gesture[2] = nges; gesture[3] = nges; break;

         case 0x17 :  gesture[0] = nges; gesture[2] = nges; gesture[3] = nges; gesture[4] = nges; break;
         case 0x1E :  gesture[0] = nges; gesture[1] = nges; gesture[2] = nges; gesture[3] = nges; break;
         case 0x0F :  gesture[1] = nges; gesture[2] = nges; gesture[3] = nges; gesture[4] = nges; break;
         
         case 0x1F :  gesture[0] = nges; gesture[1] = nges; gesture[2] = nges; gesture[3] = nges; gesture[4] = nges; break;
         default: break;
         }
         
         
         // Right hand
         switch ( testing[2*i+1] )
         {
         case 0x01 :  gesture[10] = nges; break;   
         case 0x02 :  gesture[9] = nges; break;
         case 0x04 :  gesture[8] = nges; break;
         case 0x08 :  gesture[7] = nges; break;
         case 0x10 :  gesture[6] = nges; break;
            
         case 0x11 :  gesture[6] = nges; gesture[10] = nges; break;
         case 0x12 :  gesture[6] = nges; gesture[9] = nges; break;
         case 0x14 :  gesture[6] = nges; gesture[8] = nges; break;
         case 0x18 :  gesture[6] = nges; gesture[7] = nges; break;         
         
         case 0x13 :  gesture[6] = nges; gesture[9] = nges; gesture[10] = nges; break;
         case 0x15 :  gesture[6] = nges; gesture[8] = nges; gesture[10] = nges; break;
         case 0x16 :  gesture[6] = nges; gesture[8] = nges; gesture[9] = nges; break;
         
         case 0x19 :  gesture[6] = nges; gesture[7] = nges; gesture[10] = nges; break;
         case 0x1A :  gesture[6] = nges; gesture[7] = nges; gesture[9] = nges; break;
         case 0x1C :  gesture[6] = nges; gesture[7] = nges; gesture[8] = nges; break;

         case 0x1B :  gesture[6] = nges; gesture[7] = nges; gesture[9] = nges; gesture[10] = nges; break;
         case 0x1D :  gesture[6] = nges; gesture[7] = nges; gesture[8] = nges; gesture[10] = nges; break;
         
         case 0x03 :  gesture[9] = nges; gesture[10] = nges; break;
         case 0x05 :  gesture[8] = nges; gesture[10] = nges; break;
         case 0x06 :  gesture[8] = nges; gesture[9] = nges; break;
         case 0x09 :  gesture[7] = nges; gesture[10] = nges; break;
         case 0x0A :  gesture[7] = nges; gesture[9] = nges; break;
         case 0x0C :  gesture[7] = nges; gesture[8] = nges; break;
                                 
         case 0x07 :  gesture[8] = nges; gesture[9] = nges; gesture[10] = nges; break;
         case 0x0B :  gesture[7] = nges; gesture[9] = nges; gesture[10] = nges; break;
         case 0x0D :  gesture[7] = nges; gesture[8] = nges; gesture[10] = nges; break;
         case 0x0E :  gesture[7] = nges; gesture[8] = nges; gesture[9] = nges; break;

         case 0x17 :  gesture[6] = nges; gesture[8] = nges; gesture[9] = nges; gesture[10] = nges; break;
         case 0x1E :  gesture[6] = nges; gesture[7] = nges; gesture[8] = nges; gesture[9] = nges; break;
         case 0x0F :  gesture[7] = nges; gesture[8] = nges; gesture[9] = nges; gesture[10] = nges; break;
         
         case 0x1F :  gesture[6] = nges; gesture[7] = nges; gesture[8] = nges; gesture[9] = nges; gesture[10] = nges; break;
         default: break;
         }
      } //End For Loop
   }
   return(std::string(gesture));
}


///////////////////////////////////////////////////////////
// Private methods
///////////////////////////////////////////////////////////

vpr::ReturnStatus PinchGloveStandalone::mSendCommandToHardware(const char* command)
{
   // Flush the commend buffer on the PinchGlove Hardware 
   //    (In case half a command exists in its buffer)
   // Send the command byte by byte
   
   vpr::Uint32 written;
   unsigned char buf[100];

   vpr::Interval     read_timeout(500,vpr::Interval::Msec);

      /* 
      The code below is used to make sure that the PinchGlove
      does not think we are completing a previous command. 
      The PinchGlove receives commands of two bytes. The problem
      that we are addressing is, what if there is already one byte
      sitting on the hardware. We can read from the serial port to
      see if the hardware is responding to a command, and if it is
      we know we need to send our whole command again. Also it is 
      important to note that the PinchGlove returns '?' if the
      command is unknown
      */
   
   mPort->flushQueue(vpr::SerialTypes::IO_QUEUES);
   mPort->write(&command[0], 1, written);
   if (mPort->read(&buf[0], 100, written, read_timeout).success())
   {
      mPort->write(&command[0], 1, written);
   }
   if (mPort->read(&buf[0], 100, written, read_timeout).success())
   {
      mPort->write(&command[0], 1, written);
   }

   mPort->write(&command[1], 1, written);

   return(vpr::ReturnStatus::Succeed);
}

std::vector<vpr::Uint8> PinchGloveStandalone::mGetDataAndTimeStampFromHardware()
{
   // Read the serial device until we get to a START_BYTE_DATA_TS
   // Read 1 char while it is not equal to END_BYTE
   //   - Add char to buffer
   // end while
   // return buffer

   std::vector<vpr::Uint8>  ret_val;
   unsigned char temp_char;
   vpr::Uint32 bytes_read;
   
   mPort->readn(&temp_char,1,bytes_read);
   while (temp_char != START_BYTE_DATA_TS)
   {
      mPort->readn(&temp_char,1,bytes_read);
   }
   mPort->readn(&temp_char,1,bytes_read);
   while (temp_char != END_BYTE)
   {
      ret_val.push_back(temp_char);
      mPort->readn(&temp_char,1,bytes_read);
   }
   
   return(ret_val);
}

std::string PinchGloveStandalone::mGetStringFromHardware()
{
   // Read the serial device until we get to a START_BYTE_TEXT
   // Read 1 char while it is not equal to END_BYTE
   //   - Add char to buffer
   // end while
   // return buffer

   std::string ret_val;
   unsigned char temp_char;
   vpr::Uint32 bytes_read;
   vpr::Interval     read_timeout(500,vpr::Interval::Msec);

   mPort->read(&temp_char,1,bytes_read,read_timeout);
   while (temp_char != START_BYTE_TEXT)
   {
      mPort->read(&temp_char,1,bytes_read,read_timeout);
   }
   
   mPort->read(&temp_char,1,bytes_read,read_timeout);
   while (temp_char != END_BYTE)
   {
      ret_val += temp_char;
      mPort->read(&temp_char,1,bytes_read,read_timeout);
   }

   return(ret_val);
}

/*std::vector<vpr::Uint8> PinchGloveStandalone::getDataFromHardware()
{
   // Read the serial device until we get to a START_BYTE_DATA
   // Read 1 char while it is not equal to END_BYTE
   //   - Add char to buffer
   // end while
   // return buffer

   std::vector<vpr::Uint8>  ret_val;
   unsigned char temp_char;
   vpr::Uint32 bytes_read;
   vpr::Interval     read_timeout(500,vpr::Interval::Msec);
   int num=0;

   mPort->read(&temp_char,1,bytes_read,read_timeout);
   std::cout << "CHAR: " << temp_char << " = " << (int)temp_char << std::endl;
   while (temp_char != START_BYTE_DATA)
   {
      if (temp_char==START_BYTE_DATA_TS)
      {
         std::cout << "ERROR" << std::endl;
      }
      mPort->read(&temp_char,1,bytes_read,read_timeout);
      std::cout << "CHAR: " << temp_char << " = " << (int)temp_char << std::endl;
   }
   std::cout << "We have a START_BYTE" << std::endl;
   mPort->read(&temp_char,1,bytes_read,read_timeout);
   std::cout << "CHAR: " << temp_char << " = " << (int)temp_char << std::endl;
   while (temp_char != END_BYTE)
   {
      ret_val.push_back(temp_char);
      mPort->read(&temp_char,1,bytes_read,read_timeout);
      std::cout << "CHAR: " << temp_char << " = " << (int)temp_char << std::endl;
   }
   std::cout << "We have a END_BYTE" << std::endl;

   return(ret_val);
}
*/


