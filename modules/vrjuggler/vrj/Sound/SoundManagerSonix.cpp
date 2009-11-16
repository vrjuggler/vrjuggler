/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#include <vrj/vrjConfig.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <snx/sonix.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/FileUtils.h> // for replaceEnvVars...
#include <jccl/Config/ConfigElement.h>
#include <vrj/Sound/SoundManagerSonix.h>

namespace vrj
{
   SoundManagerSonix::SoundManagerSonix()
      : vrj::SoundManager()
      , mLastFrameTime(0, vpr::Interval::Sec)
   {
      /* Do nothing. */ ;
   }

   /**
    * Adds the element to the configuration.
    * @pre configCanHandle(element) == true
    */
   bool SoundManagerSonix::configAdd(jccl::ConfigElementPtr element)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "======================================\n" << vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "SoundManagerSonix is being configured...\n" << vprDEBUG_FLUSH;

      if (!configCanHandle( element ))
      {
         std::cerr << "ERROR: Wrong element type in SoundManagerSonix\n"
                   << std::flush;
         return false;
      }

      std::string manager_name = element->getName();
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "My Name: " << manager_name
                                              << "\n" << vprDEBUG_FLUSH;
      std::string api_to_use = element->getProperty<std::string>( "api" );
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Use API: " << api_to_use
                                              << "\n" << vprDEBUG_FLUSH;

      float listener_position[3];
      listener_position[0] = element->getProperty<float>("listener_position", 0);
      listener_position[1] = element->getProperty<float>("listener_position", 1);
      listener_position[2] = element->getProperty<float>("listener_position", 2);
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "Listener Position: " << listener_position[0] << ","
         << listener_position[1] << "," << listener_position[2] << std::endl
         << vprDEBUG_FLUSH;

      std::vector<std::string> file_search_path(element->getNum("file_search_path"));

      for ( unsigned int i = 0; i < file_search_path.size(); ++i )
      {
         file_search_path[i] =
            vpr::replaceEnvVars(element->getProperty<std::string>("file_search_path", i));
      }

//      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
//         << "Search path: " << file_search_path << "\n" << vprDEBUG_FLUSH;

      // configure sonix
      snx::sonix::instance()->changeAPI(api_to_use);
      gmtl::Matrix44f mat;
      gmtl::setTrans( mat, gmtl::Vec3f( listener_position[0], listener_position[1], listener_position[2] ) );
      snx::sonix::instance()->setListenerPosition(mat);

      // read the list of sounds
      int size = element->getNum( "sound" );
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL) << "Configuring " << size
                                              << " sounds.\n" << vprDEBUG_FLUSH;
      for (int x = 0; x < size; ++x)
      {
         jccl::ConfigElementPtr sound_element =
            element->getProperty<jccl::ConfigElementPtr>("sound", x);
         std::string alias = sound_element->getName();
         std::string filename = sound_element->getProperty<std::string>("filename");
         filename = vpr::replaceEnvVars( filename );
         bool ambient = sound_element->getProperty<bool>("ambient");
         bool retriggerable = sound_element->getProperty<bool>("retriggerable");
         bool streaming = sound_element->getProperty<bool>("streaming");
         int loop = sound_element->getProperty<int>("loop");
         float cutoff = sound_element->getProperty<float>("cutoff");
         float volume = sound_element->getProperty<float>("volume");
         float pitchbend = sound_element->getProperty<float>("pitch_bend");
         float position[3];
         position[0] = sound_element->getProperty<float>("position", 0);
         position[1] = sound_element->getProperty<float>("position", 1);
         position[2] = sound_element->getProperty<float>("position", 2);

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
         si.streaming = streaming;
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

         snx::sonix::instance()->configure(alias, si);
      }
      vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
         << "======================================\n" << vprDEBUG_FLUSH;

      return true;
   }

   /**
    * Removes the element from the current configuration.
    * @pre configCanHandle(element) == true
    */
   bool SoundManagerSonix::configRemove(jccl::ConfigElementPtr element)
   {
      // remove any specified sounds...
      int size = element->getNum("sound");
      for (int x = 0; x < size; ++x)
      {
         jccl::ConfigElementPtr sound_element =
            element->getProperty<jccl::ConfigElementPtr>("sound", x);
         std::string alias(sound_element->getName());
         snx::sonix::instance()->remove(alias);
      }

      return true;
   }

   /**
    * Can the handler handle the given element?
    * @return true if we can handle it; false if not.
    */
   bool SoundManagerSonix::configCanHandle(jccl::ConfigElementPtr element)
   {
      return std::string("sound_manager_sonix") == element->getID();
   }

   /** Enables a frame to be drawn. */
   void SoundManagerSonix::update()
   {
      const vpr::Interval cur_time(vpr::Interval::now());
      const vpr::Interval delta(cur_time - mLastFrameTime);
      float delta_sec(0.0f);

      if ( cur_time > mLastFrameTime )
      {
         delta_sec = delta.secf();

         if ( delta_sec > 1.0f )
         {
            delta_sec = 1.0f;
         }
      }

      mLastFrameTime = cur_time;

      snx::sonix::instance()->step(delta_sec);
   }

   /**
    * Blocks until the end of the frame.
    * @post The frame has been drawn.
    */
   void SoundManagerSonix::sync()
   {
      // whatever...
   }

   void SoundManagerSonix::closeAPI()
   {
      snx::sonix::instance()->shutdownAPI();
   }

} // end namespace
