/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_DUMMY_GLOVE_H_
#define _VJ_DUMMY_GLOVE_H_

#include <vjConfig.h>
#include <Input/vjGlove/vjGlove.h>

//: A dummy glove for the system
// This class is used only when there is a proxy and it must be
// stupified.
//
// It returns the data in it's default init.
// i.e. 0 angles, and ident matrices
//!PUBLIC_API:
class vjDummyGlove : public vjGlove
{
public:
   //: Construct using chunk
   vjDummyGlove() : vjGlove()
   {
      active = 1;
      resetIndexes();
      mTheData[0][current].calcXforms();
   }

   //: Destroy the glove
   ~vjDummyGlove ()
   {;}

   virtual char* getDeviceName() { return "vjDummyGlove";}

   virtual int startSampling() { return 1; }
   virtual int stopSampling() { return 1; }
   virtual int sample() { return 1; }
   virtual void updateData () {}

   void setGlovePosProxy(vjPosProxy* posProxy)
   { mGlovePos[0] = posProxy; }

};

#endif	/* _VJ_CYBER_GLOVE_H_ */
