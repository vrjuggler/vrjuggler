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

#include <Sound/vjSoundEngine.h>
#include <Sound/vjSound.h>
#include <Kernel/vjConfigChunkHandler.h>

class vjSoundManager : public vjConfigChunkHandler
{
public:     // --- Config stuff -- //
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   virtual bool configAdd(vjConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   virtual bool configRemove(vjConfigChunk* chunk);

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk);

public:
   //: Enable a frame to be drawn
   virtual void update();

   //: Blocks until the end of the frame
   //! POST: The frame has been drawn
   virtual void sync();
  
   // given an alias, return the handle.
   // TODO: if called twice with name alias, should return same pointer.
   // memory managed by engine...
   // returns NULL if invalid name.
   vjSound* getHandle( const char* const alias );
   
   //: Factory function to create a new sound.
   // memory managed by engine
   virtual vjSound* newSound();
   
   vjSoundEngine* mSoundEngine;
};
