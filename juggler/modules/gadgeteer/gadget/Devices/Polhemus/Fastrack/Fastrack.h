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

#include <gadget/Devices/Polhemus/FastrackStandalone.h>

//! might be defined somewhere else in juggler, but where ?
#define TRUE 1
#define FALSE 0

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


#endif // _VJ_FASTRACK_
