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


#ifndef _VJ_PINCH_GLOVE_H_
#define _VJ_PINCH_GLOVE_H_

#include <vjConfig.h>
#include <Input/vjGlove/vjGlove.h>
#include <Input/vjGlove/fsPinchGlove.h>

//!PUBLIC_API:
class vjPinchGlove : public vjGlove
{
public:
   //: Construct
   vjPinchGlove() : mGlove(NULL), mControlThread(NULL), mCalDir(NULL)
   {;}

   //: Destroy the glove
   ~vjPinchGlove ();

   virtual bool config(vjConfigChunk* c);

   virtual char* getDeviceName() { return "vjPinchGlove";}
   static string getChunkType() { return string("PinchGlove");}

   virtual int startSampling();
   virtual int stopSampling();
   virtual int sample();
   virtual void updateData ();

protected:
	//: The main control loop for the object
   void controlLoop(void* nullParam);

   void copyDataFromGlove();

protected:
   vjThread*         mControlThread;      // The thread of control for the object
   fsPinchGlove*     mGlove;              // The actual glove

   char*             mCalDir;             // Calibration file directory

private:
    //lookup tables for the finger angles.
   static float	    mOnLookupTable[4][11];
   static float	    mOffLookupTable[4][11];
   static bool	    mLookupInitialized; // false by default, set true when lookup table has been initialized.
   static void	    mInitLookupTable();
};

#endif	/* _VJ_PINCH_GLOVE_H_ */
