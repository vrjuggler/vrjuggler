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


#include <vjConfig.h>
#include <Input/Multi/aTrackdController.h>
#include <Input/Multi/vjTrackdController.h>

#include <Kernel/vjDebug.h>



 // ------------------------------------------------------------------------
 //: Constructor.
 // ------------------------------------------------------------------------
 vjTrackdController::vjTrackdController(): mTrackdController(NULL)
 {;}

 // ------------------------------------------------------------------------
 //: Destructor.
 //
 //! PRE: None.
 //! POST: Shared memory is released
 // ------------------------------------------------------------------------
 vjTrackdController::~vjTrackdController()
 {
    delete mTrackdController;
 }

// ------------------------------------------------------------------------
//: Configure the trackd sensor with the given config chunk.
//
// -Create the trackdSensor based on config info
// -set to active
// -grow the vector to however many values we need
bool vjTrackdController::config(vjConfigChunk* c)
{
   vjDigital::config(c);
   vjAnalog::config(c);

   // Create sensor
   int shm_key = (int)c->getProperty("shm_key");
   if (shm_key == 0)
   {
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << "vjTrackdSensor::config: Bad shm_key sent: Had value of 0.\n" << vjDEBUG_FLUSH;
      return false;
   }
   mTrackdController = new aTrackdController(shm_key);

   // set to active
   active = 1;

   // grow vector
   mCurValuators.resize(mTrackdController->numValuators());
   mCurButtons.resize(mTrackdController->numButtons());

   return true;
}


// ------------------------------------------------------------------------
//: Update to the sampled data.
//
//! PRE: None.
//! POST: Most recent value is copied over to temp area
// ------------------------------------------------------------------------
void vjTrackdController::updateData()
{
   vjASSERT(mTrackdController != NULL && "Make sure that trackd controller has been initialized");
   vjASSERT((unsigned)mTrackdController->numButtons() <= mCurButtons.size());
   vjASSERT((unsigned)mTrackdController->numValuators() <= mCurValuators.size() );

   for (int i=0;i<mTrackdController->numButtons();i++)
   {
      mCurButtons[i] = mTrackdController->getButton(i);
   }

   for (int j=0;j<mTrackdController->numValuators();j++)
   {
      mCurValuators[j] = mTrackdController->getValuator(j);
   }
}

