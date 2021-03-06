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
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef SNXSUBSYNTHSOUNDIMPLEMENTATION_H
#define SNXSUBSYNTHSOUNDIMPLEMENTATION_H

#include <snx/PluginConfig.h>

#include <map>
#include <string>
#include <vector>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>

#include <syn/Core/Module.h>
#include <syn/Stream/PortAudioOStream.h>
#include <syn/Module/MixerModule.h>
#include <syn/Module/InstrumentModule.h>
#include <syn/Module/WaveTableOscModule.h>
#include <syn/Module/OperatorModule.h>
#include <syn/Module/FilterModule.h>
#include <syn/Module/OStreamModule.h>
#include <syn/Runner/Runner.h>

#include <snx/SoundImplementation.h>
#include <snx/SoundInfo.h>
#include <snx/SoundAPIInfo.h>

namespace snx
{

class SubsynthSoundImplementation : public snx::SoundImplementation
{
public:
    
   /**
    * constructor for the OpenAL implementation 
    */
   SubsynthSoundImplementation();

   /**
    * destructor for the OpenAL implementation
    */
   virtual ~SubsynthSoundImplementation();

   /**
     * every implementation can return a new copy of itself
     */
   virtual void clone( snx::ISoundImplementation* &newCopy )
   {
      SubsynthSoundImplementation* temp = new SubsynthSoundImplementation;
      newCopy = temp;
      
      // copy state, so that we return a true "clone"
      temp->copy( *this );
   }
   
   /**
    * @input alias of the sound to trigger, and number of times to play
    * @preconditions alias does not have to be associated with a loaded sound.
    * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
    * @semantics Triggers a sound
    */
   virtual void trigger( const std::string& alias, const int looping = 0);

   /**
     * is the sound currently playing?
     */
   bool isPlaying(const std::string& alias) const;
   
   /**
    * @semantics stop the sound
    * @input alias of the sound to be stopped
    */
   virtual void stop( const std::string& alias );

   /**
    * pause the sound, use unpause to return playback where you left off...
    */
   virtual void pause( const std::string& alias );

   /**
    * resume playback from a paused state.  does nothing if sound was not paused.
    */
   virtual void unpause( const std::string& alias );

   /** if the sound is paused, then return true. */
   virtual bool isPaused(const std::string& alias) const;

   /*
    * when sound is already playing then you call trigger,
    * does the sound restart from beginning?
    * (if a tree falls and no one is around to hear it, does it make sound?)
    */
   virtual void setRetriggerable( const std::string& alias, bool onOff )
   {
      snx::SoundImplementation::setRetriggerable( alias, onOff );
   }

   /**
    * ambient or positional sound.
    * is the sound ambient - attached to the listener, doesn't change volume
    * when listener moves...
    * or is the sound positional - changes volume as listener nears or retreats..
    */
   virtual void setAmbient( const std::string& alias, bool setting = false );

   /** 1 is no change.  2 is really high, 0 is really low. */
   void setPitchBend( const std::string& alias, float amount );

   /** 0 - 1. */
   void setVolume( const std::string& alias, float amount );

   /** 1 is no change.  0 is total cutoff. */
   void setCutoff( const std::string& alias, float amount );

   
  
   /**
    * set sound's 3D position 
    */
   virtual void setPosition( const std::string& alias, float x, float y, float z );

   /**
    * get sound's 3D position
    * @input alias is a name that has been associate()d with some sound data
    * @output x,y,z are returned in OpenGL coordinates.
    */
   virtual void getPosition(const std::string& alias, float& x, float& y,
                            float& z) const;
   
   /**
    * set the position of the listener
    */
   virtual void setListenerPosition( const gmtl::Matrix44f& mat );

   /**
    * get the position of the listener
    */
   virtual void getListenerPosition(gmtl::Matrix44f& mat) const;
   
public:
   /**
    * start the sound API, creating any contexts or other configurations at startup
    * @postconditions sound API is ready to go.
    * @semantics this function should be called before using the other functions in the class.
    * @return value: 1 if sucess 0 otherwise
    */
   virtual int startAPI();

   /*
    * configure the sound API global settings
    */
   
   /**
    * kill the sound API, deallocating any sounds, etc...
    * @postconditions sound API is ready to go.
    * @semantics this function could be called any time, the function could be called multiple times, so it should be smart.
    */
   virtual void shutdownAPI();

   /**
     * query whether the API has been started or not
     * @semantics return true if api has been started, false otherwise.
     */
   virtual bool isStarted() const
   {
      return mIsOpen;
   }

   virtual void configure( const snx::SoundAPIInfo& sai )
   {
      snx::SoundImplementation::configure( sai );
      // TODO: configure the engine based on the settings!!
   }

   /**
     * configure/reconfigure a sound
     * configure: associate a name (alias) to the description if not already done
     * reconfigure: change properties of the sound to the descriptino provided.
     * @preconditions provide an alias and a SoundInfo which describes the sound
     * @postconditions alias will point to loaded sound data
     * @semantics associate an alias to sound data.  later this alias can be used to operate on this sound data.
     */
   virtual void configure( const std::string& alias, const snx::SoundInfo& description );

   /**
     * remove a configured sound, any future reference to the alias will not
     * cause an error, but will not result in a rendered sound
     */
   virtual void remove( const std::string& alias );

   /**
    * clear all associate()tions.
    * @semantics any existing aliases will be stubbed. sounds will be unbind()ed
    */
   virtual void clear();
   
   /**
    * load/allocate the sound data this alias refers to the sound API
    * @postconditions the sound API has the sound buffered.
    */
   virtual void bind( const std::string& alias );

   /**
    * unload/deallocate the sound data this alias refers from the sound API
    * @postconditions the sound API no longer has the sound buffered.
    */
   virtual void unbind( const std::string& alias );

   /**
    * take a time step of [timeElapsed] seconds.
    * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
    * @input time elapsed since last frame
    */
   virtual void step(const float timeElapsed);

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * snx::ISoundImplementation::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

private:
    /** @link dependency */
    /*#  snx::SoundInfo lnkSoundInfo; */
   
   struct ImplSoundInfo
   {
      ImplSoundInfo()
      {
         this->init();
      }
      
      void init()
      {
         syn::InstrumentModule* i = new syn::InstrumentModule;
         inst = syn::ModulePtr( i );
         
         syn::ModulePtr source = syn::ModulePtr( new syn::WaveTableOscModule );
         syn::ModulePtr velocity = syn::ModulePtr( new syn::MultModule );
         syn::ModulePtr filter = syn::ModulePtr( new syn::FilterModule );
         source->setInstanceName( "source" );
         velocity->setInstanceName( "velocity" );
         filter->setInstanceName( "filter" );
         
         // connect them like so...
         // source --> gain --> filter -->
         syn::TerminalPtr output, input;
         bool result = false;
         vpr::DebugOutputGuard output1(snxDBG, vprDBG_CONFIG_LVL, std::string("source -> velocity connection\n"), std::string("\n"));
         result = source->getOutput( "mono audio", output );
         result = velocity->getInput( "mono audio0", input );
         syn::Terminal::connect( input, output );

         vpr::DebugOutputGuard output2(snxDBG, vprDBG_CONFIG_LVL, std::string("velocity -> filter connection\n"), std::string("\n"));
         result = velocity->getOutput( "mono audio", output );
         result = filter->getInput( "mono audio", input );
         syn::Terminal::connect( input, output );
         
         vpr::DebugOutputGuard output3(snxDBG, vprDBG_CONFIG_LVL, std::string("add source/velocity/filter to instrument\n"), std::string("\n"));
         i->addModule( source );
         i->addModule( velocity );
         i->addModule( filter );

         // these are the params we care about
         i->exposeParam( "freq", "source", "freq" );
         i->exposeParam( "loop", "source", "loop" );
         i->exposeParam( "samplebased", "source", "samplebased" );
         i->exposeParam( "basefreq", "source", "basefreq" );
         i->exposeParam( "retrig", "source", "retrig" );
         i->exposeParam( "interp", "source", "interp" );
         i->exposeParam( "freqcontrol", "source", "freqcontrol" );
         i->exposeParam( "trigger", "source", "trigger" );
         i->exposeParam( "release", "source", "release" );
         i->exposeParam( "filename", "source", "filename" );
         i->exposeParam( "velocity", "velocity", "constant" );
         i->exposeParam( "cutoff", "filter", "cutoff" );
         i->exposeParam( "cutoffLo", "filter", "cutoffLo" );
         i->exposeParam( "cutoffHi", "filter", "cutoffHi" );
         i->exposeOutput( "mono audio", "filter", "mono audio" );
      }

      syn::ModulePtr inst;
   };
   std::map< std::string, ImplSoundInfo > mBindLookup;

   syn::ModulePtr mSink;
   syn::ModulePtr mMixer;
   void reschedule();
   syn::Runner mRunner;
   bool mIsOpen;

   /** @link dependency */
   /*#  snx::SoundAPIInfo lnksnx::SoundAPIInfo; */
};

} // end namespace

#endif //SNXSUBSYNTHSOUNDIMPLEMENTATION_H
