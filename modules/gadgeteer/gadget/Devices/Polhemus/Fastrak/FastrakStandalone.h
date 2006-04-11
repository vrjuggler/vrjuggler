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

#ifndef _GADGET_FASTRAK_STANDALONE_H_
#define _GADGET_FASTRAK_STANDALONE_H_

// driver R.E.

#include <stdio.h>
#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/IO/Port/SerialPort.h>

#define NSTATION 4
//#define XYZ 3
const vpr::Uint8 XYZ = 3;
// 3 first letters forced in lower case
#define LC3(x) (0x202020|(((x)[0]<<16)|((x)[1]<<8)|(x)[2]))
// 2 first letters forced in lower case
#define LC2(x) (0x2020|(((x)[0]<<8)|(x)[1]))



struct perstation
{
   int begin;
   int posoff;
   int angoff;
   int quatoff;
   int butoff;
   int rec;
   float tip[XYZ];
   float inc;
   float hem[XYZ];
   float arf[3*XYZ];
   float tmf[XYZ];
};

struct FastrakConfig
{
   int found;         // flags: one bit for each feature found in the chunk
   int len;           // total length of a message sent by the tracker
   char port[20];     // port the tracker is attached to
   int baud;          // port speed
   char button;
   char cont;
   struct perstation perstation[NSTATION];
};

class FastrakStandalone
{
public:
   FastrakStandalone()
      : mSerialPort(NULL), mReadThread(NULL), mDoFlush(false)
   {
      ;
   }

   void setConfig(const FastrakConfig& conf)
   {
      mConf = conf;
   }

   const FastrakConfig& getConfig() const
   {
      return mConf;
   }

   /**
    * Opens the port to which the device is connected.
    *
    * @pre The configuration must have been set using setConfig().
    */
   vpr::ReturnStatus open();

   vpr::ReturnStatus trackerInit();

   void getNewCoords(unsigned int station, float *vecXYZ, float *vecAER);
   int getNewButtonStatus(unsigned int station);
   void trackerFinish();

   int getCoords(unsigned int stations, float *vecXYZ, float *vecAER);

private:
   vpr::ReturnStatus readStatus();

   int Read(int len);
   void readloop(void* unused);
   void checkchild();
   void getTrackerInfo(struct perstation* psp, unsigned char c);
   void getTrackerBuf();

   vpr::SerialPort* mSerialPort;
   vpr::Thread* mReadThread;
   unsigned char mTrackerBuf[256];
   bool mDoFlush;

   FastrakConfig mConf;
};

// bits positions of bits in flag word "config.found"
enum conf
{
   DEV, BAUD, BUTTON,
   REC, REC1, REC2, REC3,
   TIP, TIP1, TIP2, TIP3,
   INC, INC1, INC2, INC3,
   HEM, HEM1, HEM2, HEM3,
   ARF, ARF1, ARF2, ARF3,
   TMF, TMF1, TMF2, TMF3,
};

// each reportable thing is identified by a bit in config.perstation[n].rec
// the following enum defines the bit positions
enum rec
{
   Pos, Ang, Quat, But
};

// end driver R.E.


#endif /* _GADGET_ASTRACK_STANDALONE_H_ */
