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
#include <Config/vjConfigChunk.h>
#include <Sound/ajSoundManager.h>
#include <aj/AudioJuggler.h>
#include <aj/Matrix44.h>

namespace vrj
{
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   bool ajSoundManager::configAdd( vjConfigChunk* chunk )
   {
      if (!configCanHandle( chunk ))
      {
         std::cerr << "ERROR: Wrong chunk type\n" << std::flush;
         return false;
      }

      std::string manager_name = chunk->getProperty( "Name" );
      std::string api_to_use = chunk->getProperty( "api" );
      float listener_position[3];
      listener_position[0] = (float)chunk->getProperty( "listener_position", 0 );
      listener_position[1] = (float)chunk->getProperty( "listener_position", 1 );
      listener_position[2] = (float)chunk->getProperty( "listener_position", 2 );
      std::string file_search_path = chunk->getProperty( "file_search_path" );

      // configure audiojuggler
      AudioJuggler::instance().changeAPI( api_to_use );
      aj::Matrix44 mat;
      mat.setTrans( listener_position[0], listener_position[1], listener_position[2] );
      AudioJuggler::instance().setListenerPosition( mat );

      // read the list of sounds
      int size = chunk->getNum( "Sound" );
      for (int x = 0; x < size; ++x)
      {
         vjConfigChunk* sound_chunk = chunk->getProperty( "Sound", x );
         std::string alias = (std::string)sound_chunk->getProperty( "Name" );
         std::string filename = (std::string)sound_chunk->getProperty( "filename" );
         bool ambient = (bool)sound_chunk->getProperty( "ambient" );
         bool retriggerable = (bool)sound_chunk->getProperty( "retriggerable" );
         int loop = (int)sound_chunk->getProperty( "loop" );
         float position[3];
         position[0] = (float)chunk->getProperty( "position", 0 );
         position[1] = (float)chunk->getProperty( "position", 1 );
         position[2] = (float)chunk->getProperty( "position", 2 );

         // configure the sound...
         aj::SoundInfo si;
         si.datasource = aj::SoundInfo::FILESYSTEM;
         si.filename = filename;
         si.repeat = loop;
         si.ambient = ambient;
         si.retriggerable = retriggerable;
         si.position[0] = position[0];
         si.position[1] = position[1];
         si.position[2] = position[2];
         AudioJuggler::instance().configure( alias, si );
      }

      return true;
   }

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   bool ajSoundManager::configRemove(vjConfigChunk* chunk)
   {
      // remove any specified sounds...
      int size = chunk->getNum( "Sound" );
      for (int x = 0; x < size; ++x)
      {
         vjConfigChunk* sound_chunk = chunk->getProperty( "Sound", x );
         std::string alias = (std::string)sound_chunk->getProperty( "Name" );
         AudioJuggler::instance().remove( alias );
      }

      return true;
   }

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   bool ajSoundManager::configCanHandle( vjConfigChunk* chunk )
   {
      std::string chunk_type = (std::string)chunk->getType();

      if(std::string("juggler_audio_manager") == chunk_type)
         return true;
      else
         return false;
   }

   //: Enable a frame to be drawn
   void ajSoundManager::update()
   {
      float time_delta = 0.1; // TODO: get real time since last frame...
      AudioJuggler::instance().step( time_delta );
   }

   //: Blocks until the end of the frame
   //! POST: The frame has been drawn
   void ajSoundManager::sync()
   {
      // whatever...
   }

}; // end namespace
