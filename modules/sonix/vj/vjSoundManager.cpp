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

#include <vjConfig.h>
#include <Sound/vjSoundManager.h>
#include <aj/AudioJuggler.h>

vjSoundManager::vjSoundManager()
{

}


//: Add the chunk to the configuration
//! PRE: configCanHandle(chunk) == true
bool vjSoundManager::configAdd( vjConfigChunk* chunk )
{
   if (!configCanHandle( chunk ))
   {
      return false;
   }

   // TODO: configure audiojuggler based on chunk...

   return true;
}

//: Remove the chunk from the current configuration
//! PRE: configCanHandle(chunk) == true
bool vjSoundManager::configRemove(vjConfigChunk* chunk)
{
   return false;
}

//: Can the handler handle the given chunk?
//! RETURNS: true - Can handle it
//+          false - Can't handle it
bool vjSoundManager::configCanHandle( vjConfigChunk* chunk )
{
   // TODO: do I handle this kind of chunk?
   return false;
}

//: Enable a frame to be drawn
void vjSoundManager::update()
{
   float time_elapsed += 0.1; // TODO: get real time since last frame...
   AudioJuggler::instance().step( time_elapsed );
}

//: Blocks until the end of the frame
//! POST: The frame has been drawn
void vjSoundManager::sync()
{
   // whatever...
}
