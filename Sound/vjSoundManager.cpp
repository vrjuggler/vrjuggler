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
#include <Sound/vjSoundFactory.h>

//vjSoundManager* vjSoundManager::_instance = NULL;   //: The instance ptr
vjSingletonImp(vjSoundManager);

vjSoundManager::vjSoundManager() : mSoundEngine( NULL )
{

}


//: Add the chunk to the configuration
//! PRE: configCanHandle(chunk) == true
bool vjSoundManager::configAdd(vjConfigChunk* chunk)
{
   if (!configCanHandle( chunk ))
   {
      return false;
   }

   if (mSoundEngine == NULL)
   {
      // if not created yet, create it, adding the first config.
      mSoundEngine = vjSoundFactory::instance()->loadEngine(chunk);
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"vjSoundEngine:") << " created and added config" << "\n" << vjDEBUG_FLUSH;
   }
   else
   {
      // if already created, then add to it.
      mSoundEngine->config( chunk );
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"vjSoundEngine:")  << " Added config." << "\n" << vjDEBUG_FLUSH;

   }

   if(mSoundEngine == NULL)
   {
      vjDEBUG(vjDBG_ALL,vjDBG_CRITICAL_LVL)
               << clrOutNORM(clrRED,"ERROR:")
               << "Failed to load sound engine\n" << vjDEBUG_FLUSH;
      return false;
   }

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
bool vjSoundManager::configCanHandle(vjConfigChunk* chunk)
{
   return vjSoundFactory::instance()->recognizeEngine( chunk );
}

//: Enable a frame to be drawn
void vjSoundManager::update()
{
   if(mSoundEngine == NULL)
   {
      printAlert();
      mSoundEngine = new vjSoundEngine;
   }

   if(mSoundEngine != NULL)
      mSoundEngine->update();
}

//: Blocks until the end of the frame
//! POST: The frame has been drawn
void vjSoundManager::sync()
{
   if(mSoundEngine == NULL)
   {
      printAlert();
      mSoundEngine = new vjSoundEngine;
   }

   if(mSoundEngine != NULL)
      mSoundEngine->sync();
}

// given an alias, return the handle.
// TODO: if called twice with name alias, should return same pointer.
// memory managed by engine...
// returns NULL if invalid name.
vjSound* vjSoundManager::getHandle( const char* const alias )
{
   if(mSoundEngine == NULL)
   {
      printAlert();
      mSoundEngine = new vjSoundEngine;
   }

   if(mSoundEngine != NULL)
      return mSoundEngine->getHandle( alias );
   else
      return NULL;
}

//: Factory function to create a new sound.
// memory managed by engine
vjSound* vjSoundManager::newSound()
{
   if(mSoundEngine == NULL)
   {
      printAlert();
      mSoundEngine = new vjSoundEngine;
   }

   if(mSoundEngine != NULL)
      return mSoundEngine->newSound();
   else
      return NULL;
}

void vjSoundManager::printAlert()
{
   vjDEBUG(vjDBG_ALL,vjDBG_CRITICAL_LVL)
      << clrOutNORM(clrYELLOW,"[SoundEngine] ALERT:") << std::endl
      << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_ALL,vjDBG_CRITICAL_LVL)
      << "A sound engine wasn't created yet, so VR Juggler is using\n"
      << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_ALL,vjDBG_CRITICAL_LVL)
      << "stubbed out version--your app should run, but you will\n"
      << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_ALL,vjDBG_CRITICAL_LVL)
      << "not hear sound (run-time reconfiguration of the SoundEngine\n"
      << vjDEBUG_FLUSH;
   vjDEBUG_NEXT(vjDBG_ALL,vjDBG_CRITICAL_LVL)
      << "at this point may fail or crash the system).\n"
      << vjDEBUG_FLUSH;
}
