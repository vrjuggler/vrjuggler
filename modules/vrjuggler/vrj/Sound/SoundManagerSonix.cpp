/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <vpr/Util/FileUtils.h> // for replaceEnvVars...
#include <snx/sonix.h>
#include <jccl/Config/ConfigChunk.h>
#include <vrj/Sound/SoundManagerSonix.h>

namespace vrj
{
   /**
    * Adds the chunk to the configuration.
    * @pre configCanHandle(chunk) == true
    */
   bool SoundManagerSonix::configAdd( jccl::ConfigChunkPtr chunk )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "======================================\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "SoundManagerSonix is being configured...\n" << vprDEBUG_FLUSH;

      if (!configCanHandle( chunk ))
      {
         std::cerr << "ERROR: Wrong chunk type in SoundManagerSonix\n" << std::flush;
         return false;
      }

      std::string manager_name = chunk->getName();
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "My Name: " << manager_name
                                              << "\n" << vprDEBUG_FLUSH;
      std::string api_to_use = chunk->getProperty<std::string>( "api" );
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Use API: " << api_to_use
                                              << "\n" << vprDEBUG_FLUSH;
      float listener_position[3];
      listener_position[0] = (float)chunk->getProperty<float>( "listener_position", 0 );
      listener_position[1] = (float)chunk->getProperty<float>( "listener_position", 1 );
      listener_position[2] = (float)chunk->getProperty<float>( "listener_position", 2 );
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Listener Position: " << listener_position[0] << ","
         << listener_position[1] << "," << listener_position[2] << "\n" << vprDEBUG_FLUSH;
      std::string file_search_path = chunk->getProperty<std::string>( "file_search_path" );
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Search path: " << file_search_path << "\n" << vprDEBUG_FLUSH;

      // configure sonix
      sonix::instance()->changeAPI( api_to_use );
      gmtl::Matrix44f mat;
      gmtl::setTrans( mat, gmtl::Vec3f( listener_position[0], listener_position[1], listener_position[2] ) );
      sonix::instance()->setListenerPosition( mat );

      // read the list of sounds
      int size = chunk->getNum( "Sounds" );
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Configuring " << size
                                              << " sounds.\n" << vprDEBUG_FLUSH;
      for (int x = 0; x < size; ++x)
      {
         jccl::ConfigChunkPtr sound_chunk = chunk->getProperty<jccl::ConfigChunkPtr>( "Sounds", x );
         std::string alias = sound_chunk->getName();
         std::string filename = sound_chunk->getProperty<std::string>( "filename" );
         filename = vpr::replaceEnvVars( filename );
         bool ambient = sound_chunk->getProperty<bool>( "ambient" );
         bool retriggerable = sound_chunk->getProperty<bool>( "retriggerable" );
         int loop = sound_chunk->getProperty<int>( "loop" );
         float cutoff = sound_chunk->getProperty<float>( "cutoff" );
         float volume = sound_chunk->getProperty<float>( "volume" );
         float pitchbend = sound_chunk->getProperty<float>( "pitchbend" );
         float position[3];
         position[0] = sound_chunk->getProperty<float>( "position", 0 );
         position[1] = sound_chunk->getProperty<float>( "position", 1 );
         position[2] = sound_chunk->getProperty<float>( "position", 2 );

         // configure the sound...
         snx::SoundInfo si;
         si.datasource = snx::SoundInfo::FILESYSTEM;
         si.filename = filename;
         si.cutoff = cutoff;
         si.volume = volume;
         si.pitchbend = pitchbend;
         si.repeat = loop;
         si.ambient = ambient;
         si.retriggerable = retriggerable;
         si.position[0] = position[0];
         si.position[1] = position[1];
         si.position[2] = position[2];
         vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
            << "- Configuring " << alias << " (" << filename << ")\n"
            << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "  + pos(" << position[0] << "," << position[1] << ","
            << position[2] << ")," << "loop(" << loop << "),\n"
            << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "  + amb(" << ambient << ")," << "ct(" << cutoff << "),"
            << "vl(" << volume << "),\n" << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "  + freq(" << pitchbend << ")," << "retrig(" << retriggerable
            << ")\n" << vprDEBUG_FLUSH;

         sonix::instance()->configure( alias, si );
      }
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "======================================\n" << vprDEBUG_FLUSH;

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

} // end namespace
