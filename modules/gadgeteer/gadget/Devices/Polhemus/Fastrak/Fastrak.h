/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

// ----------------------------------------------------------------------------
// Author:
//     Nicolas Tarrin <nicolas.tarrin@inria.fr>
//     February 2001
// ----------------------------------------------------------------------------

// 4 stations: from 0 to 3

#ifndef _VJ_FASTRACK_
#define _VJ_FASTRACK_

#include <vjConfig.h>

#include <iostream>
#include <string.h>

#include <Math/vjMatrix.h>
#include <Input/vjInput/vjDigital.h>
#include <Input/vjPosition/vjPosition.h>

//! might be defined somewhere else in juggler, but where ?
#define TRUE 1
#define FALSE 0

// C driver (R.E.) functions and procedures
extern "C" void trackerInit(char *configfile);
extern "C" void getNewCoords(unsigned int station, float *vecXYZ, float *vecAER);
extern "C" int getCoords(unsigned int stations, float *vecXYZ, float *vecAER);
extern "C" int getNewButtonStatus(unsigned int station);
extern "C" void trackerFinish();

class vjFastrack : public vjPosition, public vjDigital
{
public:
  vjFastrack();                                  // must call vjAnalog()? see vjAnalog.h
  virtual ~vjFastrack();
  static std::string getChunkType() { return std::string("Fastrack"); }// return what chunk type is associated with this class.
public:
  //:vjInput pure virtual functions
  virtual bool config( vjConfigChunk *fastrackChunk);
  virtual int startSampling();                 // start a new thread
  virtual int sample();                        // read data from the fastrack
  virtual void updateData();                   // swap the data and vjInput::tri-buffered indicies
  virtual int stopSampling();                  // kills off the sample thread
  virtual char* getDeviceName() { return "vjFastrack"; };// device's configchunk name (match .desc file)
public:
  //:vjDigital pure virtual function
  virtual int getDigitalData(int station = 0);
  //:vjPosition pure virtual function
  virtual vjMatrix* getPosData(int station = 0);
private:
  static void threadedSampleFunction(void* classPointer);
  int buttonState[3];                   // only one button on station 0
  float trackersPosition[4][3][3];      // 4 stations, 3 coordinates, 3 buffers
  float trackersOrientation[4][3][3];   // 4 stations, 3 orientations, 3 buffers
  vjThread *sampleThread;

  char *configFile;
};

// ************************************************************************************************
// driver R.E.

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <signal.h>

#define NSTATION 4
#define XYZ 3
// 3 first letters forced in lower case
#define LC3(x) (0x202020|(((x)[0]<<16)|((x)[1]<<8)|(x)[2]))
// 2 first letters forced in lower case
#define LC2(x) (0x2020|(((x)[0]<<8)|(x)[1]))

// bits positions of bits in flag word "config.found"
enum conf { DEV, BAUD, BUTTON,
	    REC, REC1, REC2, REC3,
	    TIP, TIP1, TIP2, TIP3,
	    INC, INC1, INC2, INC3,
	    HEM, HEM1, HEM2, HEM3,
	    ARF, ARF1, ARF2, ARF3,
	    TMF, TMF1, TMF2, TMF3,
};

// each reportable thing is identified by a bit in config.perstation[n].rec
// the following enum defines the bit positions
enum rec { Pos, Ang, Quat, But };

static int trackerfd;

static volatile unsigned char TrackerBuf[256];
static volatile int DoFlush;
static pid_t ReadPid;

struct perstation {
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

static
struct {
  int found;         // flags: one bit for each feature found in the chunk
  int len;           // total length of a message sent by the tracker
  char port[20];     // port the tracker is attached to
  int baud;          // port speed
  char button;
  char cont;
  struct perstation perstation[NSTATION];
} conf;

// end driver R.E.
// ************************************************************************************************


#endif // _VJ_FASTRACK_
