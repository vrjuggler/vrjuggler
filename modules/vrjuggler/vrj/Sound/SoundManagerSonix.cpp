/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <vrj/vrjConfig.h>
#include <jccl/Config/ConfigChunk.h>
#include <vrj/Sound/SoundManagerSonix.h>
#include <snx/sonix.h>
#include <gmtl/Matrix.h>

namespace vrj
{
   /**
    * Adds the chunk to the configuration.
    * @pre configCanHandle(chunk) == true
    */
   bool SoundManagerSonix::configAdd( jccl::ConfigChunkPtr chunk )
   {
      if (!configCanHandle( chunk ))
      {
         std::cerr << "ERROR: Wrong chunk type\n" << std::flush;
         return false;
      }

      std::string manager_name = chunk->getName();
      std::string api_to_use = chunk->getProperty<std::string>( "api" );
      float listener_position[3];
      listener_position[0] = (float)chunk->getProperty<float>( "listener_position", 0 );
      listener_position[1] = (float)chunk->getProperty<float>( "listener_position", 1 );
      listener_position[2] = (float)chunk->getProperty<float>( "listener_position", 2 );
      std::string file_search_path = chunk->getProperty<std::string>( "file_search_path" );

      // configure sonix
      sonix::instance()->changeAPI( api_to_use );
      gmtl::Matrix44f mat;
      gmtl::setTrans( mat, gmtl::Vec3f( listener_position[0], listener_position[1], listener_position[2] ) );
      sonix::instance()->setListenerPosition( mat );

      // read the list of sounds
      int size = chunk->getNum( "Sound" );
      for (int x = 0; x < size; ++x)
      {
         jccl::ConfigChunkPtr sound_chunk = chunk->getProperty<jccl::ConfigChunkPtr>( "Sound", x );
         std::string alias = (std::string)sound_chunk->getName();
         std::string filename = (std::string)sound_chunk->getProperty<std::string>( "filename" );
         bool ambient = (bool)sound_chunk->getProperty<bool>( "ambient" );
         bool retriggerable = (bool)sound_chunk->getProperty<bool>( "retriggerable" );
         int loop = (int)sound_chunk->getProperty<int>( "loop" );
         float position[3];
         position[0] = (float)chunk->getProperty<float>( "position", 0 );
         position[1] = (float)chunk->getProperty<float>( "position", 1 );
         position[2] = (float)chunk->getProperty<float>( "position", 2 );

         // configure the sound...
         snx::SoundInfo si;
         si.datasource = snx::SoundInfo::FILESYSTEM;
         si.filename = filename;
         si.repeat = loop;
         si.ambient = ambient;
         si.retriggerable = retriggerable;
         si.position[0] = position[0];
         si.position[1] = position[1];
         si.position[2] = position[2];
         sonix::instance()->configure( alias, si );
      }

      return true;
   }

   /**
    * Removes the chunk from the current configuration.
    * @pre configCanHandle(chunk) == true
    */
   bool SoundManagerSonix::configRemove(jccl::ConfigChunkPtr chunk)
   {
      // remove any specified sounds...
      int size = chunk->getNum( "Sound" );
      for (int x = 0; x < size; ++x)
      {
         jccl::ConfigChunkPtr sound_chunk = chunk->getProperty<jccl::ConfigChunkPtr>( "Sound", x );
         std::string alias = (std::string)sound_chunk->getName();
         sonix::instance()->remove( alias );
      }

      return true;
   }

   /**
    * Can the handler handle the given chunk?
    * @return true if we can handle it; false if not.
    */
   bool SoundManagerSonix::configCanHandle( jccl::ConfigChunkPtr chunk )
   {
      std::string chunk_type = (std::string)chunk->getDescToken();
      if(std::string("juggler_audio_manager") == chunk_type)
         return true;
      else
         return false;
   }

   /** Enables a frame to be drawn. */
   void SoundManagerSonix::update()
   {
      float time_delta = 0.1; // TODO: get real time since last frame...
      sonix::instance()->step( time_delta );
   }

   /**
    * Blocks until the end of the frame.
    * @post The frame has been drawn.
    */
   void SoundManagerSonix::sync()
   {
      // whatever...
   }

}; // end namespace
