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

#ifndef _VJ_SOUND_MANAGER_H_
#define _VJ_SOUND_MANAGER_H_

#include <Sound/vjSoundEngine.h>
#include <Sound/vjSound.h>
#include <Kernel/vjConfigChunkHandler.h>
#include <Utils/vjSingleton.h>


//: sound manager
//!PUBLIC_API:
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
   //! POST: The frame has been rendered
   virtual void sync();

   // given an alias, return the sound.
   // memory managed internal to manager...
   // returns NULL if invalid name.
   vjSound* getHandle( const char* const alias );

   //: Factory function to create a new sound.
   // memory managed by engine
   virtual vjSound* newSound();

   /*
   vjSound stubSound;
   vjSound* sound( vjSoundHandle* handle )
   {
      if (handle == NULL || handle->isStale())
      {
         return &stubSound;
      }
      else
      {

      }
   }*/

public:
   // get the manager's soundengine
   vjSoundEngine* engine()
   {
      return mSoundEngine;
   }
   vjSoundEngine* mSoundEngine;

protected:
   //: Constructor.
   vjSoundManager();

   void printAlert(void);

vjSingletonHeader(vjSoundManager);
};

#endif   /* _VJ_SOUND_MANAGER_H_ */
