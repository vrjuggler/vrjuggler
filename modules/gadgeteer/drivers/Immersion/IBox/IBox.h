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

/////////////////////////////////////////////////////////////////////////
//
// ibox tracking class
//
////////////////////////////////////////////////////////////////////////

#ifndef _VRJ_IBOX_H_
#define _VRJ_IBOX_H_

#include <vrj/vrjConfig.h>
#include <gad/Type/Input.h>
#include <gad/Type/Digital.h>
#include <gad/Type/Analog.h>
#include <gad/Devices/Immersion/hci.h>
#include <gad/Devices/Immersion/iboxStandalone.h>

namespace vrj
{

struct IBOX_DATA {
   int button[4];
   int analog[4];
};

//----------------------------------------------------------------------------
//: The Immersion Box input class.
//
//  The Immersion Box is a 4 Digital, 4 Analog input device, the Ibox class
//  therefore must inherit from both Digital and Analog.  The class uses
//  the HCI library for a simple interface to the IBox.
//
//-----------------------------------------------------------------------------
//!PUBLIC_API:
class IBox : public Input, public Digital, public Analog
{
public:
   //: Construction/Destruction
   IBox() : Input(), Digital(), Analog()
   {
      // re-set Analog min/max values to ibox defaults.
      this->setMin( 0.0f );
      this->setMax( 255.0f );
   }
   ~IBox();

   virtual bool config( ConfigChunk* c );

   // Input Pure Virtual Functions
   int startSampling();
   int stopSampling();
   int sample();
   void updateData();

   static std::string getChunkType() { return std::string( "IBox" ); }

   int getDigitalData( int d = 0 );

   //: Return "analog data"..
   //  Gee, that's ambiguous especially on a discrete system such as a digital computer....
   //
   //! PRE: give the device number you wish to access.
   //! POST: returns a value that ranges from 0.0f to 1.0f
   //! NOTE: for example, if you are sampling a potentiometer, and it returns reading from
   //        0, 255 - this function will normalize those values (using Analog::normalizeMinToMax())
   //        for another example, if your potentiometer's turn radius is limited mechanically to return
   //        say, the values 176 to 200 (yes this is really low res), this function will still return
   //        0.0f to 1.0f.
   //! NOTE: to specify these min/max values, you must set in your Analog (or analog device) config
   //        file the field "min" and "max".  By default (if these values do not appear),
   //        "min" and "max" are set to 0.0f and 1.0f respectivly.
   //! NOTE: TO ALL ANALOG DEVICE DRIVER WRITERS, you *must* normalize your data using
   //        Analog::normalizeMinToMax()
   float getAnalogData( int d = 0 );

private:
   // juggler ibox data in the range of [0..255]
   IBOX_DATA theData[3];
   // ibox native data in the range of [0..255]
   hci_rec thingie;
};

};

#endif
