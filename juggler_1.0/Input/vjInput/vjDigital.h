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
// positional tracker base class
//
////////////////////////////////////////////////////////////////////////
#ifndef _VJ_DIGITAL_H_
#define _VJ_DIGITAL_H_

#include <vjConfig.h>

//-----------------------------------------------------------------------------
//: vjDigital is the abstract base class that devices with digital data derive
//+ from.
//
//  vjDigital is the base class that digital devices must derive from.
//  vjDigital inherits from vjInput, so it has pure virtual function
//  constraints from vjInput in the following functions: StartSampling,
//  StopSampling, Sample, and UpdateData. <br>
//  vjDigital adds one new pure virtual function, GetDigitalData for
//  retreiving the digital data, similar to the addition for vjPosition and
//  vjAnalog.
//
// See also: vjInput
//!PUBLIC_API:
//-----------------------------------------------------------------------------
class vjDigital
{
public:
   //: Enum for the state of the digital buttons
   // Used in vjDigitalProxy
   enum State
   {
      OFF=0, ON=1, TOGGLE_ON=2, TOGGLE_OFF=3
   };

public:
   /* Constructor/Destructors */
   vjDigital()
   {
      //vjDEBUG(vjDBG_ALL,4)<<"*** vjDigital::vjDigital()\n"<< vjDEBUG_FLUSH;
   }

   virtual ~vjDigital()
   {
   }

   virtual bool config(vjConfigChunk* c)
   {
      //vjDEBUG(vjDBG_ALL,4)<<"*** vjDigital::config()\n"<< vjDEBUG_FLUSH;
      return true;;
   }

   //: Get the digital data for the given devNum
   //  Returns digital 0 or 1, if devNum makes sense.<BR>
   //  Returns -1 if function fails or if devNum is out of range.<BR>
   //  NOTE: If devNum is out of range, function will fail, possibly issueing
   //  an error to a log or console - but will not ASSERT.<BR>
   virtual int getDigitalData(int devNum = 0) = 0;
};

#endif   /* _VJ_DIGITAL_H_ */
