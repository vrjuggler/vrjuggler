/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert
 *
 ****************** <SNX heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <snx/PluginConfig.h>

#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>
#include <vpr/Util/Assert.h>

#include <snx/SoundImplementation.h>
#include <snx/SoundInfo.h>
#include <snx/SoundFactory.h>
#include "SubsynthSoundImplementation.h"

/////////////////////////
// plugin API:
#ifdef NO_SELF_REGISTER
extern "C"
{

SNX_PLUGIN_EXPORT(const char*) getVersion()
{
   return "sonix xx.xx.xx";
}

SNX_PLUGIN_EXPORT(const char*) getName()
{
   return "Subsynth";
}

SNX_PLUGIN_EXPORT(snx::ISoundImplementation*) newPlugin()
{
   return new snx::SubsynthSoundImplementation;
}

}
#endif
/////////////////////////

namespace snx
{
#ifndef NO_SELF_REGISTER
snx::SoundFactoryReg<SubsynthSoundImplementation> subsynthRegistrator( "Subsynth" );
#endif

void SubsynthSoundImplementation::step(const float timeElapsed)
{
   snx::SoundImplementation::step( timeElapsed );
}

void SubsynthSoundImplementation::remove( const std::string& alias )
{
   snx::SoundImplementation::remove( alias );
}

SubsynthSoundImplementation::SubsynthSoundImplementation() : snx::SoundImplementation(), 
                                                         mBindLookup(), mIsOpen( false )
{
}

SubsynthSoundImplementation::~SubsynthSoundImplementation()
{
   this->shutdownAPI();
}  

/**
 * @input alias of the sound to trigger, and number of times to play
 * @preconditions alias does not have to be associated with a loaded sound.
 * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
 * @semantics Triggers a sound
 */
void SubsynthSoundImplementation::trigger(const std::string& alias,
                                          const int looping)
{
   // do nothing if not started.
   if (this->isStarted() == false) return;
   
   snx::SoundImplementation::trigger( alias, looping );

   // if sound data hasn't been loaded into sound API yet, then do so
   if (mBindLookup.count( alias ) == 0)
   {
      this->bind( alias );
   }
   
   // if data is bound (bind() succeeded), then play it.
   if (mBindLookup.count( alias ) > 0)
   {
      bool retriggerable = this->isRetriggerable( alias );
      bool is_not_playing = !this->isPlaying( alias );
      bool is_paused = this->isPaused( alias );
      if (is_paused || retriggerable || is_not_playing)
      {
         
         // @todo: handle ability to loop openal sounds n number of times.  
         //        only once or infinite is supported by Subsynth.
         if (looping == -1 || looping > 1)
         {
            mBindLookup[alias].inst->setParam( "loop", true );
         }
         else
         {
            mBindLookup[alias].inst->setParam( "loop", false );
         }
         
         //std::cout << "[snx] trigger" << std::endl;
         mBindLookup[alias].inst->setParam( "trigger", true );
      }
   }
}

bool SubsynthSoundImplementation::isPlaying( const std::string& alias )
{
   if (this->isStarted() == false) return false;
   
   if (mBindLookup.count( alias ) > 0)
   {
      syn::MultivariateType state;
      state.setValue<bool>( false );
      mBindLookup[alias].inst->getParam( "trigger", state );
      return state.getValue<bool>();
   }

   return false;
}

/** if the sound is paused, then return true. */
bool SubsynthSoundImplementation::isPaused( const std::string& alias )
{
   if (this->isStarted() == false) return false;
   
   if (mBindLookup.count( alias ) > 0)
   {
      syn::MultivariateType state;
      state.setValue<bool>( false );
      mBindLookup[alias].inst->getParam( "release", state );
      return state.getValue<bool>();
   }

   return false;
}

/**
 * pause the sound, use unpause to return playback where you left off...
 */
void SubsynthSoundImplementation::pause( const std::string& alias )
{
   if (this->isStarted() == false) return;
   
   if (mBindLookup.count( alias ) > 0)
   {
      mBindLookup[alias].inst->setParam( "trigger", false );
   }
}

/**
 * resume playback from a paused state.  does nothing if sound was not paused.
 */
void SubsynthSoundImplementation::unpause( const std::string& alias )
{
   if (this->isStarted() == false) return;
   
   if (mBindLookup.count( alias ) > 0)
   {
      if (this->isPaused( alias ))
      {
         mBindLookup[alias].inst->setParam( "trigger", true );
      }
   }
}

/**
 * @semantics stop the sound
 * @input alias of the sound to be stopped
 */
void SubsynthSoundImplementation::stop( const std::string& alias )
{
   if (this->isStarted() == false) return;
   
   snx::SoundImplementation::stop( alias );

   if (mBindLookup.count( alias ) > 0)
   {
      mBindLookup[alias].inst->setParam( "trigger", false );
   }
}

/**
 * ambient or positional sound.
 * is the sound ambient - attached to the listener, doesn't change volume
 * when listener moves...
 * or is the sound positional - changes volume as listener nears or retreats..
 */
void SubsynthSoundImplementation::setAmbient( const std::string& alias, bool ambient )
{
   snx::SoundImplementation::setAmbient( alias, ambient );
   if (mBindLookup.count( alias ) > 0 && mSounds.count( alias ) > 0)
   {
      // if positional
      if (ambient == false)
      {
         // @todo, implement spacialization in subsynth
      }
      // if ambient
      else
      {
         // @todo, implement spacialization in subsynth
      }      
   }
}

/**
 * associate a name (alias) to the description
 * @preconditions provide an alias and a SoundInfo which describes the sound
 * @postconditions alias will point to loaded sound data
 * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
 */
void SubsynthSoundImplementation::configure( const std::string& alias, const snx::SoundInfo& description )
{
   snx::SoundImplementation::configure( alias, description );
}

/**
 * set sound's 3D position 
 */
void SubsynthSoundImplementation::setPosition( const std::string& alias, float x, float y, float z )
{
   if (this->isStarted() == false) return;
   snx::SoundImplementation::setPosition( alias, x, y, z );

   if (mBindLookup.count( alias ) > 0)
   {
      if (this->isAmbient( alias ) == false)
      {
         // @todo, implement spacialization in subsynth
      }
   }
}

/**
 * get sound's 3D position
 * @input alias is a name that has been associate()d with some sound data
 * @output x,y,z are returned in OpenGL coordinates.
 */
void SubsynthSoundImplementation::getPosition( const std::string& alias, float& x, float& y, float& z )
{
   snx::SoundImplementation::getPosition( alias, x, y, z );
}

/**
 * set the position of the listener
 */
void SubsynthSoundImplementation::setListenerPosition( const gmtl::Matrix44f& mat )
{
   if (this->isStarted() == false) return;
   
   snx::SoundImplementation::setListenerPosition( mat );

   // @todo, implement spacialization in subsynth
}

/**
 * get the position of the listener
 */
void SubsynthSoundImplementation::getListenerPosition( gmtl::Matrix44f& mat )
{
   snx::SoundImplementation::getListenerPosition( mat );
}

/** 1 is no change.  2 is really high, 0 is really low. */
void SubsynthSoundImplementation::setPitchBend( const std::string& alias, float amount )
{
   snx::SoundImplementation::setPitchBend( alias, amount );
   if (mBindLookup.count( alias ) > 0 && mSounds.count( alias ) > 0)
   {
      // zero or less is not legal.
      if (amount <= 0.0f) 
         amount = 0.001f;
      amount *= 22050.0f;
      
      mBindLookup[alias].inst->setParam( "freq", amount );
   }
}

/** 0 - 1.  use 0 for mute, use 1 for unmute... */
void SubsynthSoundImplementation::setVolume( const std::string& alias, float amount )
{
   // only set the vol if in range...
   if (amount < 0) return;
   
   snx::SoundImplementation::setVolume( alias, amount );
   if (mBindLookup.count( alias ) > 0 && mSounds.count( alias ) > 0)
   {
      mBindLookup[alias].inst->setParam( "velocity", amount );
   }
}

/** 1 is no change.  0 is total cutoff. */
void SubsynthSoundImplementation::setCutoff( const std::string& alias, float amount )
{
   snx::SoundImplementation::setCutoff( alias, amount );
   if (mBindLookup.count( alias ) > 0 && mSounds.count( alias ) > 0)
   {
      mBindLookup[alias].inst->setParam( "cutoff", amount );
   }
}  


/**
 * start the sound API, creating any contexts or other configurations at startup
 * @postconditions sound API is ready to go.
 * @semantics this function should be called before using the other functions in the class.
 * @return value: 1 if success 0 otherwise
 */
int SubsynthSoundImplementation::startAPI()
{
   if (this->isStarted() == true) return;

   bool result = false;
   
   // output
   syn::OStreamModule* sink = new syn::OStreamModule;
   syn::AudioOStreamPtr ostream( new syn::PortAudioOStream );
   sink->setStream( ostream );
   sink->setInstanceName( "sink" );
   mSink = syn::ModulePtr( sink );
   
   // open the audio port
   result = sink->open();
   if (result == false) 
   { 
      vpr::DebugOutputGuard output1(snxDBG, vprDBG_CONFIG_LVL, std::string("[snx]Subsynth| sink couldn't open\n"), std::string("\n"));
      return 0;
   }
   
   // mixer
   syn::MixerModule* mixer = new syn::MixerModule;
   mixer->setInstanceName( "mix" );
   mMixer = syn::ModulePtr( mixer );
   
   // open the mixer
   result = mixer->open();
   if (result == false) 
   { 
      vpr::DebugOutputGuard output2(snxDBG, vprDBG_CONFIG_LVL, std::string("mix couldn't open\n"), std::string("\n"));
      return 0;
   }

   vpr::DebugOutputGuard output3(snxDBG, vprDBG_CONFIG_LVL, std::string("make connections\n"), std::string("\n"));
   syn::TerminalPtr output, input;

   vpr::DebugOutputGuard output4(snxDBG, vprDBG_CONFIG_LVL, std::string(" mixer -> audioport connection\n"), std::string("\n"));
   result = mixer->getOutput( "output", output );
   if (result == false) 
   { 
      vpr::DebugOutputGuard output5(snxDBG, vprDBG_CONFIG_LVL, std::string("couldn't get mixer out-term\n"), std::string("\n"));
      return 0; 
   }
   result = sink->getInput( "mono audio", input );
   if (result == false) 
   { 
      vpr::DebugOutputGuard output6(snxDBG, vprDBG_CONFIG_LVL, std::string("couldn't get audioport in-term\n"), std::string("\n"));
      return 0; 
   }
   syn::Terminal::connect( input, output );
//   syn::SampleBufferRepos::instance()->setBlockSize( blocksize );
      
         
   // init the listener...
   this->setListenerPosition( mListenerPos );

   mRunner.stop();
   this->reschedule();
   mRunner.run( 1 );
     
   mIsOpen = true;
   return 1;
}

/**
 * kill the sound API, deallocating any sounds, etc...
 * @postconditions sound API is ready to go.
 * @semantics this function could be called any time, the function could be called multiple times, so it should be smart.
 */
void SubsynthSoundImplementation::shutdownAPI()
{
   if (this->isStarted() == false)
   {
      std::cerr << "[snx]Subsynth| WARNING: API not started, nothing to shutdown\n" << std::flush;
      return;
   }
   mIsOpen = false;
   
   this->unbindAll();
   mRunner.stop();
   mMixer->close();
   mSink->close();
   
   std::cerr<<"[snx]Subsynth| NOTICE: Subsynth API closed\n"<<std::flush;
}   

/**
 * clear all associate()tions.
 * @semantics any existing aliases will be stubbed. sounds will be unbind()ed
 */
void SubsynthSoundImplementation::clear()
{
   snx::SoundImplementation::clear();
}

void SubsynthSoundImplementation::reschedule()
{
   // rescan all available modules...
   mRunner.modules().clear();
   std::map< std::string, ImplSoundInfo >::iterator it;
   for (it = mBindLookup.begin(); it != mBindLookup.end(); ++it)
   {
      mRunner.modules().insert( mRunner.modules().begin(),
                                (*it).second.inst->getChildren().begin(), 
                                (*it).second.inst->getChildren().end() );
   }
   /*
   std::map< std::string, ImplSoundInfo >::iterator it;
   for (it = mBindLookup.begin(); it != mBindLookup.end(); ++it)
   {
      mRunner.modules().push_back( (*it).second.inst );
   }
   */
   mRunner.modules().push_back( mMixer );
   mRunner.modules().push_back( mSink );
   
   // redo the schedule
   mRunner.reschedule( 1 );
}

/**
 * load/allocate the sound data this alias refers to the sound API
 * @postconditions the sound API has the sound buffered.
 */
void SubsynthSoundImplementation::bind( const std::string& alias )
{
   if (this->isStarted() == false)
   {
      std::cerr << "[snx]Subsynth| ERROR: API not started, bind() failed\n" << std::flush;
      return;
   }
   
   snx::SoundInfo& soundInfo = this->lookup( alias );

   // if alias is already bound, then unbind it...
   // TODO: we want a way to force a rebind, but do we _always_ want to force it?
   if (mBindLookup.count( alias ) > 0)
   {
      this->unbind( alias );
   }

   // load the data 
   switch (soundInfo.datasource)
   {
      default:
      case snx::SoundInfo::FILESYSTEM:
      {

         // Test for the existence of soundInfo.filename before proceeding
         // any further.
         try
         {
            boost::filesystem::path file_path(
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
               soundInfo.filename
#else
               soundInfo.filename, boost::filesystem::native
#endif
            );

            if ( ! boost::filesystem::exists(file_path) )
            {
               vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
                  << clrOutNORM(clrRED, "ERROR") << ": Subsynth| alias '"
                  << alias << "',\n" << vprDEBUG_FLUSH;
               vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
                  << "file '" << soundInfo.filename << "'\n"
                  << vprDEBUG_FLUSH;
               vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
                  << "does not exist\n" << vprDEBUG_FLUSH;
               break;
            }
         }
         catch (boost::filesystem::filesystem_error& ex)
         {
            vprDEBUG(snxDBG, vprDBG_WARNING_LVL)
               << clrOutNORM(clrYELLOW, "WARNING")
               << ": OpenAL| File system exception while testing for the "
               << "existence of file '" << soundInfo.filename << "'"
               << std::endl << vprDEBUG_FLUSH;
            vprDEBUG_NEXT(snxDBG, vprDBG_WARNING_LVL)
               << ex.what() << std::endl << vprDEBUG_FLUSH;
            break;
         }

         mRunner.pause();
         {
            // set up the subsynth instrument...
            mBindLookup[alias].inst->setInstanceName( alias );
            vpr::DebugOutputGuard output7(snxDBG, vprDBG_CONFIG_LVL, std::string("NOTIFY: loading: ")+soundInfo.filename+std::string("... \n"), std::string("\n"));
            mBindLookup[alias].inst->setParam( "filename", soundInfo.filename );
            mBindLookup[alias].inst->setParam( "loop", false );
            mBindLookup[alias].inst->setParam( "samplebased", true );
            mBindLookup[alias].inst->setParam( "basefreq", 22050.0f );
            this->setPitchBend( alias, soundInfo.pitchbend );
            this->setCutoff( alias, soundInfo.cutoff );
            this->setVolume( alias, soundInfo.volume );
            this->setAmbient( alias, soundInfo.ambient );
            this->setPosition( alias, soundInfo.position[0], soundInfo.position[1], soundInfo.position[2] );

            // connect the instrument to the mixer...
            syn::TerminalPtr output, input;
            bool result = false;
            result = mBindLookup[alias].inst->getOutput( "mono audio", output );
            if (result == false) 
            { 
               vpr::DebugOutputGuard output8(snxDBG, vprDBG_CONFIG_LVL, std::string("ERORR: couldn't get inst out-term\n"), std::string("\n")); 
               return; 
            }
            if (mMixer->isInput( alias ))
               mMixer->removeInput( alias );
            input = mMixer->createInput( alias );
            
            syn::Terminal::connect( input, output );
            this->reschedule();
         }
         mRunner.unpause();
         break;
      }
   }

   // was it playing?  if so, then start it up again...
   if (soundInfo.triggerOnNextBind == true)
   {
      soundInfo.triggerOnNextBind = false; // done...
      vpr::DebugOutputGuard output9(snxDBG, vprDBG_CONFIG_LVL, std::string("NOTIFY: triggering reconfigured sound\n"), std::string("\n"));
      this->trigger( alias, soundInfo.repeat );
   }
}   

/**
 * unload/deallocate the sound data this alias refers from the sound API
 * @postconditions the sound API no longer has the sound buffered.
 */
void SubsynthSoundImplementation::unbind( const std::string& alias )
{
   if (this->isStarted() == false)
   {
      std::cerr << "[snx]Subsynth| ERROR: API not started, unbind() failed\n" << std::flush;
      return;
   }
 
   // if alias is bound, then unbind it...
   if (mBindLookup.count( alias ) > 0)
   {
      // is it currently playing?  if so, stop it
      if (this->isPlaying( alias ) == true)
      {
         this->stop( alias );
         
         // should trigger next time, since we just unbound it
         mSounds[alias].triggerOnNextBind = true;    
      }

      mRunner.pause();          
      {
         mMixer->removeInput( alias );
         mBindLookup.erase( alias );
         this->reschedule();
      }
      mRunner.unpause();
   }
   
   vprASSERT(mBindLookup.count( alias ) == 0 && "unbind failed");
}

} // end namespace
