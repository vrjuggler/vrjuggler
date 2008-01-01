/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <math.h>

#include <vpr/Util/Debug.h>
#include <gadget/Util/Debug.h>

#include <drivers/EssentialReality/P5Glove/P5GloveStandalone.h>

///////////////////////////////////////////////////////////
// Public Methods
///////////////////////////////////////////////////////////

P5GloveStandalone::Record::Record()
   : buttonA(false)
   , buttonB(false)
   , buttonC(false)
   , thumb(0.0)
   , index(0.0)
   , middle(0.0)
   , ring(0.0)
   , pinky(0.0)
   , position(3, 0.0)
   , rotationAxis(3, 0.0)
   , rotationAngle(0.0)
{
   /* Do nothing. */ ;
}

// Constructor
P5GloveStandalone::P5GloveStandalone()
   : mGlove(NULL)
{
   /* Do nothing. */ ;
}

// Destructor
P5GloveStandalone::~P5GloveStandalone()
{
   if ( NULL != mGlove )
   {
      p5glove_close(mGlove);
   }
}

// Connect to the P5Glove hardware
bool P5GloveStandalone::connectToHardware(const int gloveNumber)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "[P5Glove] Connecting To P5Glove Hardware number " << gloveNumber
      << std::endl << vprDEBUG_FLUSH;

   for ( unsigned int i = 0 ; i < 4 ; ++i )
   {
      mGlove = p5glove_open(gloveNumber);
      if ( mGlove == NULL )
      {
         break;
      }

      if ( p5glove_sample(mGlove, -1) >= 0 )
      {
         break;
      }

      p5glove_close(mGlove);
      mGlove = NULL;
   }

   if ( mGlove == NULL )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "[P5Glove] Cannot connect to Hardware glove!\n" << vprDEBUG_FLUSH;
      return false;
   }

//   p5g_reset_prev(mGlove);
   return true;
}

bool P5GloveStandalone::readRecordsFromHardware(Record& rec)
{
   const int mask = p5glove_sample(mGlove, -1);

   if ( mask < 0 )
   {
      return false;
   }

   if ( mask & P5GLOVE_DELTA_BUTTONS )
   {
      uint32_t buttons;
      p5glove_get_buttons(mGlove, &buttons);
      /* Buttons */
      rec.buttonA = buttons & P5GLOVE_BUTTON_A;
      rec.buttonB = buttons & P5GLOVE_BUTTON_B;
      rec.buttonC = buttons & P5GLOVE_BUTTON_C;
   }

   if ( mask & P5GLOVE_DELTA_FINGERS )
   {
      p5glove_get_finger(mGlove, P5GLOVE_FINGER_THUMB, &rec.thumb);
      p5glove_get_finger(mGlove, P5GLOVE_FINGER_INDEX, &rec.index);
      p5glove_get_finger(mGlove, P5GLOVE_FINGER_MIDDLE, &rec.middle);
      p5glove_get_finger(mGlove, P5GLOVE_FINGER_RING, &rec.ring);
      p5glove_get_finger(mGlove, P5GLOVE_FINGER_PINKY, &rec.pinky);
   }

   if ( mask & P5GLOVE_DELTA_POSITION )
   {
      p5glove_get_position(mGlove, &rec.position[0]);
   }

   if ( mask & P5GLOVE_DELTA_ROTATION )
   {
      p5glove_get_rotation(mGlove, &rec.rotationAngle, &rec.rotationAxis[0]);
   }

   return true;
}
