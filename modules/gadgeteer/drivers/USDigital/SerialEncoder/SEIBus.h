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
#ifndef SEIBUS_H
#define SEIBUS_H

#include <iostream>
#include <time.h>
#include <boost/concept_check.hpp>

#include <vpr/IO/Port/SerialPort.h>
#include <vpr/System.h>

#include "SEIConstants.h"

typedef unsigned int UInt32;
typedef unsigned char byte;

#define MAX_SEI_DEVICES 15

class SEIBus
{
public:
   SEIBus()
   {
      int i;

      port=0;
      numDevices=0;

      for ( i=0;i<MAX_SEI_DEVICES;i++ )
      {
         deviceSerialNumbers[i]=0;
         deviceAddresses[i]=-1;
      }

      checkChecksum=true;
      reportErrors=true;
   }

   ~SEIBus()
   {
      delete port;
   }

   int initializeSEI(const char *portName)
   {
      int i;
      int status=0;

      port=new vpr::SerialPort(portName);
      port->setOpenReadWrite();
      port->setBlocking(true);      

      if ( port->open()==vpr::ReturnStatus::Fail )
      {
         return -1;
      }

      port->clearAll();
      port->setLocalAttach(true);
      port->setUpdateAction(vpr::SerialTypes::NOW);
      port->setOutputBaudRate(9600);
      port->setInputBaudRate(9600);
      port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
      port->setRead(true);

//These probably won't work on windows since enableBreakByteIgnore doesn't do anything
//		port->enableBreakByteIgnore();
//		port->sendBreak(0);

#if CTS_CHECKING
      std::cout << "initializeSEI: Searching for serial numbers\n" << std::flush;
      status=searchSerialNumbers(0,0x80000000);
#else
      std::cout << "initializeSEI: CTS checking not supported.  The following operations are not supported: \n\t getNumberOfDevices \n\t getSerialNumberOfDevice \n\t getAddressOfDevice\n" << std::flush;
#endif

      if ( status )
      {
         return status;
      }

      for ( i=0;i<numDevices;i++ )
      {
         bool errors=reportErrors;

         //Don't report errors on first try, checksum is always bad (?)
         reportErrors=false;
         getAddressFromSerialNumber(deviceSerialNumbers[i],deviceAddresses[i]);
         reportErrors=errors;

         //Now try 3 more times
         for ( int t=0;t<=3;t++ )
         {
            status=getAddressFromSerialNumber(deviceSerialNumbers[i],deviceAddresses[i]);
            if ( !status )break;
         }

         if ( status )
         {
            reportError("initializeSEI: Error getting address");
            return -1;
         }
      }

      return status;
   }

   int getNumberOfDevices()
   {
      return numDevices;
   }

   int getSerialNumberOfDevice(int i)
   {
      if ( i>=0 && i<MAX_SEI_DEVICES )
      {
         return deviceSerialNumbers[i];
      }

      return -1;
   }

   int getAddressOfDevice(int i)
   {
      if ( i>=0 && i<MAX_SEI_DEVICES )
      {
         return deviceAddresses[i];
      }

      return -1;
   }

   int ED2GetPosition1(char address,float &pos)
   {
      char *posc;
      int err;

      const int size=5;
      char inData[size];

      err=singleByteCommand((char)(ED2RDPOS1 + (address & 0x0F)),inData,size);
      if ( err!=0 )
      {
         reportError("ED2GetPosition1: Error getting position");
         return -1;
      }

      posc = (char*)&pos;
      pos=0;

      //WARNING: BYTE ORDER
      if ( vpr::System::isLittleEndian() )
      {
         posc[0]=inData[4];
         posc[1]=inData[3];
         posc[2]=inData[2];
         posc[3]=inData[1];
      }
      else
      {
         posc[0]=inData[1];
         posc[1]=inData[2];
         posc[2]=inData[3];
         posc[3]=inData[4];
      }
      return 0;
   }

   int ED2SetOrigin(char address)
   {
      int err;
      char outData[2];
      char inData[3];

      outData[0]=(char)(0xF0 | address);
      outData[1]=ED2ORIG;

      err=multiByteCommand(outData,2,inData,3);

      if ( err )
      {
         reportError("ED2SetOrigin: Error setting origin");
         return err;                                     // 1 to 6
      }

      return 0;
   }

   int getAddressFromSerialNumber(UInt32 serialNumber,char &addr)
   {
      int err;
      char *pc;
      char outData[6];
      char inData[8];

      outData[0] = (char)0xFF;
      outData[1] = RADDR;
      pc = (char*)&serialNumber;
      //WARNING: BYTE ORDER
      if ( vpr::System::isLittleEndian() )
      {
         outData[2] = pc[3];
         outData[3] = pc[2];
         outData[4] = pc[1];
         outData[5] = pc[0];
      }
      else
      {
         outData[2] = pc[0];
         outData[3] = pc[1];
         outData[4] = pc[2];
         outData[5] = pc[3];
      }

      err=multiByteCommand(outData,6,inData,8);

      if ( err )
      {
         reportError("getAddress: Error getting address");
         return err;                                     // 1 to 6
      }

      addr=inData[6];

      return 0;
   }

private:
   void reportError(char *error)
   {
      if ( reportErrors )
      {
         std::cout << error << "\n" << std::flush;
      }
   }

   int searchSerialNumbers(unsigned int serialNumber,unsigned int mask)
   {
      int bit;
      char *pc;
      int err;
      char data[10];

      for ( bit = 0; bit <= 1; bit++ )
      {          // try this bit low, then high 
         if ( bit ) serialNumber |= mask ^ (mask << 1); // set bit of SN 

         if ( waitForNotBusy(10) )
         {
            reportError("searchSerialNumbers: Timeout error 1");
            return -1;                       // busy should not timeout
         }

         data[0]=(unsigned char)0xFF;
         write(data,1);                      // multi byte command

         if ( waitForBusy(10) )
         {
            reportError("searchSerialNumbers: Timeout error 2");
            return -1;                       // busy should not timeout
         }

         wait(2);                         // wait for all encoders to sync
         data[0] = CHKSN;
         pc = (char*)&serialNumber;
         //WARNING: BYTE ORDER
         if ( vpr::System::isLittleEndian() )
         {
            data[1] = pc[3];
            data[2] = pc[2];
            data[3] = pc[1];
            data[4] = pc[0];
         }
         else
         {
            data[1] = pc[0];
            data[2] = pc[1];
            data[3] = pc[2];
            data[4] = pc[3];
         }
         pc = (char*)&mask;
         if ( vpr::System::isLittleEndian() )
         {
            data[5] = pc[3];
            data[6] = pc[2];
            data[7] = pc[1];
            data[8] = pc[0];
         }
         else
         {
            data[5] = pc[0];
            data[6] = pc[1];
            data[7] = pc[2];
            data[8] = pc[3];
         }

         write(data,9);                      // multi byte command
         wait(10);                           // KTL ver 5.03

         // if no match, busy will release	within 5 msec
         if ( waitForNotBusy(5) )
         {                 // busy stays active means match
            data[0] = WAKEUP;
            write(data,1);                   // terminate command 
            if ( mask == 0xFFFFFFFF )
            {             // all bits matched 
               deviceSerialNumbers[numDevices]=serialNumber;
               numDevices++;

               if ( numDevices>=MAX_SEI_DEVICES )
               {
                  return 1;
               }
            }
            else
            {                           // bits matched, test next one 
               err = searchSerialNumbers(serialNumber, (mask >> 1) | 0x80000000);
               if ( err ) return err;
            }
         }
      }

      return 0;
   }

   int write(char *data,int size)
   {
      vpr::Uint32 amount;
      port->write(data,size,amount);

      if ( amount==0 )
      {
         return -1;
      }

      return 0;
   }

   int read(char *data,int size,int timeout)
   {
      vpr::Uint32 amount;
      port->setTimeout(timeout/100);
      port->read(data,size,amount,vpr::Interval(timeout,vpr::Interval::Msec));

      if ( amount==0 )
      {
         return -1;
      }

      return 0;
   }

#ifdef CTS_CHECKING
   bool isBusy()
   {
      return !port->isCTSHigh();
   }
#endif

   void clearPort()
   {
      port->flushQueue(vpr::SerialTypes::IO_QUEUES);
   }

   unsigned long getTime()
   {
      return clock();
   }

   int waitForBusy(int timeout,bool inverted=false)
   {
#ifdef CTS_CHECKING
      UInt32 t;

      t = getTime() + timeout;
      do
      {
         if ( inverted && !isBusy() )
         {
            return 0;
         }
         if ( !inverted && isBusy() )
         {
            return 0;
         }
      }  while ( getTime() < t );     // check for time out
      return -1;           // there was a timeout
#else
      boost::ignore_unused_variable_warning(timeout);
      boost::ignore_unused_variable_warning(inverted);
      return 0;
#endif
   }

   int waitForNotBusy(int timeout)
   {
      return waitForBusy(timeout,true);
   }

   void wait(int time)
   {
      vpr::System::msleep(time);
   }

   int singleByteCommand(char cmd,char *inData,int inSize)
   {
      int err;

      clearPort();                   

      err=write(&cmd,1);
      if ( err!=0 )
      {
         return -1;
      }

      err=read(inData,inSize,1000);

      if ( err!=0 )
      {
         return -2;
      }

      return 0;
   }

   int multiByteCommand(char *outData,int outSize,char *inData,int inSize)
   {
      int err;

      memset(inData,0,inSize);

      clearPort();

      if ( waitForNotBusy(1000) )
      {
         reportError("multiByteCommand: Busy is active, can't send command");
         return -1;
      }

      err=write(outData,1);
      if ( err )
      {
         reportError("multiByteCommand: Error writing command start");
         return -1;
      }

      if ( waitForBusy(1000) )
      {
         reportError("multiByteCommand: Timed out waiting for busy");
         return -1;
      }

      // if all encoders command, add delay to let them sync
      if ( (outData[0] & 0xF) == 0xF )wait(2);

      err = write(outData+1,outSize-1);
      if ( err )
      {
         reportError("multiByteCommand: Error writing rest of command");
         return err;
      }

      err = read(inData,inSize,1000);

      if ( err )
      {
         reportError("multiByteCommand: Error reading result");
         return err;
      }

      if ( checkChecksum )
      {
         int checksum=0;
         int i;

         for ( i=0;i<inSize;i++ ) checksum^=inData[i];
         if ( checksum&0xFF )
         {
            reportError("multiByteCommand: Error in checksum");
            return -1;
         }
      }

      return 0;
   }

   vpr::SerialPort *port;
   int numDevices;
   UInt32 deviceSerialNumbers[MAX_SEI_DEVICES];
   char deviceAddresses[MAX_SEI_DEVICES];
   bool checkChecksum;
   bool reportErrors;
};

#endif

