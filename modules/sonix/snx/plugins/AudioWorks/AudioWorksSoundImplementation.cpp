
/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdlib.h>
#include <fstream>
#include <string>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>

#include <snx/SoundFactory.h>
#include <snx/Util/Debug.h>
#include <snx/plugins/AudioWorks/AudioWorksSoundImplementation.h>

#include <vpr/Util/Debug.h>

/////////////////////////
// plugin API:
#ifdef NO_SELF_REGISTER
extern "C"
{
XDL_EXPORT const char* getVersion() { return "sonix xx.xx.xx"; }
XDL_EXPORT const char* getName() { return "AudioWorks"; }
XDL_EXPORT snx::ISoundImplementation* newPlugin() { return new snx::AudioWorksSoundImplementation; }
XDL_EXPORT void deletePlugin( snx::ISoundImplementation* &p ) { if (NULL == p) return; delete p; p = NULL; }
}
#endif
/////////////////////////

namespace snx
{
#ifndef NO_SELF_REGISTER
   snx::SoundFactoryReg<AudioWorksSoundImplementation> audioworksRegistrator( "AudioWorks" );
#endif


   /**
    * constructor for the OpenAL implementation 
    */
   AudioWorksSoundImplementation::AudioWorksSoundImplementation() : 
         snx::SoundImplementation(), mTotalTimeElapsed( 0.0f ), mIsStarted( false )
   {
      // TODO: set up the defaults for aw...
      //mSoundAPIInfo.
   }

   /**
    * destructor for the OpenAL implementation
    */
   AudioWorksSoundImplementation::~AudioWorksSoundImplementation()
   {
      this->shutdownAPI();
   }

   /**
     * every implementation can return a new copy of itself
     */
   void AudioWorksSoundImplementation::clone( snx::ISoundImplementation* &newCopy )
   {
      AudioWorksSoundImplementation* temp = new AudioWorksSoundImplementation;
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
   void AudioWorksSoundImplementation::trigger( const std::string & alias, const int & looping )
   {
      snx::SoundImplementation::trigger( alias, looping );

      if (mBindTable.count( alias ) > 0)
      {
         if (this->isPaused( alias ))
         {
            this->unpause( alias );
         }
         else
         {
            awProp(mBindTable[alias].mSound, AWSND_STATE, AW_ON);
            awProp(mBindTable[alias].mSound, AWSND_ENABLE, AW_ON);
            
            // @todo, this only works with "loop points"... grrr....
            awProp(mBindTable[alias].mSound, AWSND_NLOOPS, looping );

            //if (isAmbient(alias))
            //{
            //   awProp( mBindTable[alias].mPlayer, AWSND_STATE, AW_ON );
            //}
         }         
      }
   }

    /**
     * is the sound currently playing?
     */
   bool AudioWorksSoundImplementation::isPlaying( const std::string& alias )
   {
      if (mBindTable.count( alias ) > 0)
      {
         int notpaused = (bool)awGetProp( mBindTable[alias].mSound, AWSND_ENABLE );
         int playing = (bool)awGetProp( mBindTable[alias].mSound, AWSND_STATE );
         return (notpaused == AW_ON) && (playing == AW_ON);
      }
      return false;
   }
   
   /**
    * pause the sound, use unpause to return playback where you left off...
    */
   void AudioWorksSoundImplementation::pause( const std::string& alias )
   {
      if (mBindTable.count( alias ) > 0)
      {
         // @todo  this isn't _quite_ pause.  AW didn't really seem to have one 
         //        (that I could find..)  enable does a mute...
         awProp( mBindTable[alias].mSound, AWSND_STATE, AW_OFF );
      }
   }   

   /**
    * resume playback from a paused state.  does nothing if sound was not paused.
    */
   void AudioWorksSoundImplementation::unpause( const std::string& alias )
   {
      if (mBindTable.count( alias ) > 0)
      {
         // @todo  this isn't _quite_ pause.  AW didn't really seem to have one 
         //        (that I could find..)  enable does a mute...
         awProp( mBindTable[alias].mSound, AWSND_STATE, AW_ON );
      }
   }
   
   /** if the sound is paused, then return true. */
   bool AudioWorksSoundImplementation::isPaused( const std::string& alias )
   {
      if (mBindTable.count( alias ) > 0)
      {
         return (awGetProp(mBindTable[alias].mSound, AWSND_STATE) == AW_OFF);
      }
      return false;
   }
      
   /*
    * when sound is already playing then you call trigger,
    * does the sound restart from beginning?
    * (if a tree falls and no one is around to hear it, does it make sound?)
    */
   void AudioWorksSoundImplementation::setRetriggerable( const std::string& alias, bool onOff )
   {
      snx::SoundImplementation::setRetriggerable( alias, onOff );

      if (mBindTable.count( alias ) > 0)
      {
         if (onOff == true)
            awProp( mBindTable[alias].mSound, AWSND_RETRIGGER, AW_ON );    //Set retriggering ON
         else
            awProp( mBindTable[alias].mSound, AWSND_RETRIGGER, AW_OFF );   //Set retriggering ON
      }
   }   

   /**
    * ambient or positional sound.
    * is the sound ambient - attached to the listener, doesn't change volume
    * when listener moves...
    * or is the sound positional - changes volume as listener nears or retreats..
    */
   void AudioWorksSoundImplementation::setAmbient( const std::string& alias, bool isAmbient )
   {
      snx::SoundImplementation::setAmbient( alias, isAmbient );

      if (mBindTable.count( alias ) > 0)
      {
         if (isAmbient == true)
         {
            awRemPlyrSnd( mBindTable[alias].mPlayer, mBindTable[alias].mSound );

            // attaches the player to observer
            awProp( mBindTable[alias].mPlayer, AWPLYR_CSREF, AWPLYR_OBS );
            awPlyrObsRef( mBindTable[alias].mPlayer, mObs );

            //float xyz[3] = { 0.0f, 0.0f, 0.0f }, hpr[3] = { 0.0f, 0.0f, 0.0f };
            //awXYZHPR(si.mSound, xyz, hpr);               //Set sound at origin
         }

         else
         {
            // detaches player from observer
            awProp( mBindTable[alias].mPlayer, AWPLYR_CSREF, AWPLYR_CSREFPOS );
            awAddPlyrSnd( mBindTable[alias].mPlayer, mBindTable[alias].mSound );

            float xyz[3] = { mSounds[alias].position[0], mSounds[alias].position[1], mSounds[alias].position[2] };
            float hpr[3] = { 0.0f, 0.0f, 0.0f };
            // convert xyz to performer coords...
            awPlyrCSRef( mBindTable[alias].mPlayer, xyz, hpr );
         }
      }
   }

   /**
    * @semantics stop the sound
    * @input alias of the sound to be stopped
    */
   void AudioWorksSoundImplementation::stop( const std::string& alias )
   {
      snx::SoundImplementation::stop( alias );

      if (mBindTable.count( alias ) > 0)
      {
         awProp( mBindTable[alias].mSound, AWSND_STATE, AW_OFF );
      }
   }

   /**
    * set sound's 3D position 
    */
   void AudioWorksSoundImplementation::setPosition( const std::string& alias, float x, float y, float z )
   {
      // NOTE: don't limit based on ambiance, even abient sounds can have position...
      snx::SoundImplementation::setPosition( alias, x, y, z );

      if (mBindTable.count( alias ) > 0 && mSounds.count( alias ) > 0)
      {
         float xyz[3] = { 0.0f, 0.0f, 0.0f };
         float hpr[3] = { 0.0f, 0.0f, 0.0f };
         snx::SoundImplementation::getPosition( alias, xyz[0], xyz[1], xyz[2] );

         if (this->isAmbient( alias ) == true)
            awXYZHPR( mBindTable[alias].mSound, xyz, hpr );  //Set sound at origin
         else
            awXYZHPR( mBindTable[alias].mPlayer, xyz, hpr );  //Set sound at origin
         //std::cout<<"awXYZHPR( "<<xyz[0]<<" "<<xyz[1]<<" "<<xyz[2]<<std::endl;
      }
      //std::cout<<"you asked for "<<x<<" "<<y<<" "<<z<<std::endl;
   }

   /**
    * get sound's 3D position
    * @input alias is a name that has been associate()d with some sound data
    * @output x,y,z are returned in OpenGL coordinates.
    */
   void AudioWorksSoundImplementation::getPosition( const std::string& alias, float& x, float& y, float& z )
   {
      snx::SoundImplementation::getPosition( alias, x, y, z );
   }

   /**
    * set the position of the listener
    */
   void AudioWorksSoundImplementation::setListenerPosition( const gmtl::Matrix44f& mat )
   {
      snx::SoundImplementation::setListenerPosition( mat );
      vpr::DebugOutputGuard output1(snxDBG, vprDBG_CONFIG_LVL, std::string("WARNING: setListenerPosition not implemented yet\n"), std::string("\n"));
   }

   /**
    * get the position of the listener
    */
   void AudioWorksSoundImplementation::getListenerPosition( gmtl::Matrix44f& mat )
   {
      snx::SoundImplementation::getListenerPosition( mat );
   }

   /** 1 is no change.  2 is really high, 0 is really low. */
   void AudioWorksSoundImplementation::setPitchBend( const std::string& alias, float amount )
   {
      snx::SoundImplementation::setPitchBend( alias, amount );
      if (mBindTable.count( alias ) > 0 && mSounds.count( alias ) > 0)
      {
         //if (this->isAmbient( alias ) == true)
         awProp( mBindTable[alias].mSound, AWSND_PBEND, amount );
         //else
         awProp( mBindTable[alias].mPlayer, AWSND_PBEND, amount );
      }
   }
   
   /** 0 - 1. */
   void AudioWorksSoundImplementation::setVolume( const std::string& alias, float amount )
   {
      snx::SoundImplementation::setVolume( alias, amount );
      if (mBindTable.count( alias ) > 0 && mSounds.count( alias ) > 0)
      {
         float real_amount = amount * 60.0f;
         
         //if (this->isAmbient( alias ) == true)
         awProp( mBindTable[alias].mSound, AWSND_SPL, real_amount );
         //else
         awProp( mBindTable[alias].mPlayer, AWSND_SPL, real_amount );
      }
   }
   
   /** 1 is no change.  0 is total cutoff. */
   void AudioWorksSoundImplementation::setCutoff( const std::string& alias, float amount )
   {
      snx::SoundImplementation::setCutoff( alias, amount );
      if (mBindTable.count( alias ) > 0 && mSounds.count( alias ) > 0)
      {
         float true_cutoff = 22050.0f * amount;
         
         //if (this->isAmbient( alias ) == true)
         awProp( mBindTable[alias].mSound, AWSND_LOPASS, true_cutoff );
         //else
         awProp( mBindTable[alias].mPlayer, AWSND_LOPASS, true_cutoff );
      }
   }  
      
   /**
    * start the sound API, creating any contexts or other configurations at startup
    * @postconditions sound API is ready to go.
    * @semantics this function should be called before using the other functions in the class.
    * @return value: 1 if success, 0 otherwise
    */
   int AudioWorksSoundImplementation::startAPI()
   {
      // figure out what host to connect to if any:
      std::string hostname = getenv( "SNX_AW_HOSTNAME" );

      // if no hostname set, then look for an rc file
      if (hostname == "")
      {
         hostname = "localhost";
         std::string rc_filepath = getenv( "HOME" );
         rc_filepath += "/.sonix-aw";
         vpr::DebugOutputGuard output1(snxDBG, vprDBG_CONFIG_LVL, std::string("AudioWorks| Reading "+rc_filepath+" for hostname"), std::string("\n")); 
         std::ifstream rc_file;
         rc_file.open( rc_filepath.c_str(), std::ifstream::in );
         if (rc_file.good())
         {
            rc_file>>hostname;
         }
         else
         {
            
         }
         rc_file.close();
      }
      vpr::DebugOutputGuard output2(snxDBG, vprDBG_CONFIG_LVL, std::string("AudioWorks| hostname="+hostname), std::string("\n"));
        
      
      // setup the hardware system
      awOpenAWD("");
      awOpenEP(0, AWEP_SHARE); // multiusers can use audio works at the same time...
      awEPReset();
      awEPFlush();
      awCloseEP();
      awCloseAWD();

      //initialize the AW system
      if (awInitSys() == -1) 
      {
        vprDEBUG(snxDBG, vprDBG_CONFIG_LVL) << clrOutNORM(clrYELLOW, "AudioWorks| ERROR: InitSys() failed!\n") << vprDEBUG_FLUSH;
        return 0;
      }

      //Initialize the engine
      mEngine = awNewEng();         //Define the new engine
      
      // if hostname is set, then set it in AW
      if (hostname != "localhost")
      {
         awEngAWD( mEngine, hostname.c_str() );
      }
      assert( mEngine != NULL );
      awProp(mEngine, AWENG_VOLUME, 1.0);   //Set its volume to maximum output
      int result = awAttachEng(mEngine);
      if (result < 0)      //Attach the engine to the system
      {
          vprDEBUG(snxDBG, vprDBG_CONFIG_LVL)
             << clrOutNORM(clrYELLOW, "AudioWorks| ERROR: ")
             << "failed to attach to engine (retval=" << result << ")...\n\n"
             << vprDEBUG_FLUSH;
     //     awPrint(mEngine);
          return 0;
      }

      //Set up the channel
      mChannel = awNewChan();            //Define the new channel
      assert( mChannel != NULL );
      awChanEng(mChannel, mEngine);         //Associate the channel with the engine

      awProp(mChannel, AWCHAN_MODEL, AWIF_QUAD);   //Set QUAD sound imaging model
      awProp(mChannel, AWCHAN_NVOICES, 16);      //Set 16 voices at once
      awProp(mChannel, AWCHAN_ENABLE, AW_ON);      //Enable the channel imaging


      //Set up scene object and add the sound objects to it
      mScene = awNewScene();            //Define new scene
      assert( mScene != NULL );

      //Set up environment object
      mEnv = awNewEnv();            //Define new environment
      assert( mEnv != NULL );
      awProp(mEnv, AWENV_SOS, 330.0);         //Set the speed of sound

      //Set up observer object
      mObs = awNewObs();                  //Define new observer
      assert( mObs != NULL );

      awProp(mObs, AWOBS_STATE,       AW_ON);      //Enable the observer
      awProp(mObs, AWOBS_TETHERCOORD,       AWOBS_TABSOLUTE);   //Set coords to absolute
      awProp(mObs, AWOBS_CSREF,       AWOBS_ABSOLUTE);   //Set coords to absolute
      awProp(mObs, AWOBS_SPLMIN,       40.0);         //Minimum SPL heard
      awProp(mObs, AWOBS_SPLMAX,       200.0);      //Maximum SPL heard
      awProp(mObs, AWOBS_CALCVEL,       AW_ON);      //Calculate the observer velocity
      awProp(mObs, AWOBS_ATTENUATION,       AW_ON);      //Sound drops volume at distance
      awProp(mObs, AWOBS_DELAY,       AW_ON);      //Sound takes time to travel
      awProp(mObs, AWOBS_DOPPLER,       AW_ON);      //Sound movement doppler effect
      awProp(mObs, AWOBS_PRIORITY,       AW_OFF);      //Sound playing priority
      awProp(mObs, AWOBS_WCMAP,       AW_ON);      //World Coordinate Mapping ON


      // set up the observer (audiojuggler API only supports one for now)
      awAddObsChan(mObs, mChannel);      //Associate observer with the channel
      awObsScene(mObs, mScene);      //Associate observer with the scene
      awObsEnv(mObs, mEnv);         //Associate observer with the environment

      awConfigChan( mChannel ); /// exp...

      result = awConfigSys(1);
     if (result != 0)         //Attempt to configure the system
     {    
         vpr::DebugOutputGuard output2(snxDBG, vprDBG_CONFIG_LVL, std::string("ERROR: ConfigSys() failed\n"), std::string("\n"));
   //    awPrint(mEngine);
       return 0;
     }

     mIsStarted = true; //success


     // init the state now that the engine is started...
     this->setListenerPosition( mListenerPos );
     return 1;
   }   

   /**
    * kill the sound API, deallocating any sounds, etc...
    * @postconditions sound API is ready to go.
    * @semantics this function could be called any time, the function could be called multiple times, so it should be smart.
    */
   void AudioWorksSoundImplementation::shutdownAPI()
   {
      if (mIsStarted == true)
      {
         awUnConfigChan( mChannel );

         awRemObsChan(mObs, mChannel);
         awObsScene(mObs, NULL);      
         awObsEnv(mObs, NULL);         //Associate observer with the environment
         awDelete( mObs );

         awDelete( mEnv );
         awDelete( mScene );
         awDelete( mChannel );

         awDetachEng( mEngine );
         awDelete( mEngine );

         awExit();

         mIsStarted = false;
      }
   }   

   /**
     * query whether the API has been started or not
     * @semantics return true if api has been started, false otherwise.
     */
   bool AudioWorksSoundImplementation::isStarted() const
   {
      return mIsStarted;
   }

   /**
    * configure the sound API global settings
    */
   void AudioWorksSoundImplementation::configure( const snx::SoundAPIInfo& sai )
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
   void AudioWorksSoundImplementation::configure( const std::string& alias, const snx::SoundInfo& description )
   {
      snx::SoundImplementation::configure( alias, description );
      // do nothing
   }

   /**
     * remove a configured sound, any future reference to the alias will not
     * cause an error, but will not result in a rendered sound
     */
   void AudioWorksSoundImplementation::remove( const std::string& alias )
   {
      snx::SoundImplementation::remove( alias );
      // do nothing
   }

   /**
    * clear all associate()tions.
    * @semantics any existing aliases will be stubbed. aounds will be unbind()ed
    */
   void AudioWorksSoundImplementation::clear()
   {
   }   

   /** @todo: make me work */
   void loadload( const std::string& filename, awWave& wav )
   {
      // aiff
      awName( &wav, filename.c_str() );       //Set the aifc filename
      if (awLoadWav( &wav ) != 0)               //Load the aifc file
      {
          vpr::DebugOutputGuard output3(snxDBG, vprDBG_CONFIG_LVL, std::string("ERROR: failed to open wave file\nwave dump:\n"), std::string("\n"));
          awPrint( &wav );
          return;
      }
      else
      {
         vpr::DebugOutputGuard output4(snxDBG, vprDBG_CONFIG_LVL, std::string("NOTICE: loaded: ")+filename+std::string("\n"), std::string("\n"));
      }
      
      awPrint( &wav );
          
      
      /*
      // wav:  here is the beginnings of code to load wav files ourselves.
      //   this will let us support other rates than 11025...
      awProp( wav, AWWV_NFRAMES, samples );
      awProp( wav, AWWV_FORMAT, AWWV_S16BIT );
      awProp( wav, AWWV_NCHANS, AWWV_MONO );
      awProp( wav, AWWV_LOOPTYPE, AWWV_LOOPOFF );
      awProp( wav, AWWV_SRATE, samp_rate );
      
      awPutWavSam( wav, AWWV_MONOIDX, frame_offset, num_samps, data );
      */
   }   
   
   /**
    * load/allocate the sound data this alias refers to the sound API
    * @postconditions the sound API has the sound buffered.
    */
   void AudioWorksSoundImplementation::bind( const std::string& alias )
   {
      //std::cout<<"[snx]AudioWorks| DEBUG: bind() "<<alias<<"\n"<<std::flush;

      this->unbind( alias );

      AWSoundInfo si;
      snx::SoundInfo sinfo = mSounds[alias];


      si.mWave = awNewWav();                      //Define the wave form
      loadload( sinfo.filename, *si.mWave );
      awMapWavToSE( si.mWave, mEngine );                    //Associate the wave with the engine
      awFlushWavToSE( si.mWave );                            //Flush the changes to the engine

      // Set up sounds and associate them with waves
      si.mSound = awNewSnd();                                //Define new sound
      awSndWave( si.mSound, si.mWave );                     //Associate the sound with the wave
      awMakeSnd( si.mSound);                                 //Necessary to change the wave form

      awProp( si.mSound, AWSND_ENABLE,      AW_ON );         //Enable the sound
      awProp( si.mSound, AWSND_STATE,       AW_OFF );        //Turn it off for now
      awProp( si.mSound, AWSND_RETRIGGER,   AW_ON );         //Set retriggering ON
         
      awProp( si.mSound, AWSND_ABSORPTION,  AWSND_INHERIT ); //Inherit modeling settings
      awProp( si.mSound, AWSND_ATTENUATION, AWSND_INHERIT ); //from the observer
      awProp( si.mSound, AWSND_DOPPLER,     AWSND_INHERIT ); // ...
      awProp( si.mSound, AWSND_DELAY,       AWSND_INHERIT ); // ...
      awProp( si.mSound, AWSND_WCMAP,       AWSND_INHERIT ); // ...
      awProp( si.mSound, AWSND_PRIORITY,    AWSND_INHERIT ); // ...
      awProp( si.mSound, AWSND_EXPUNGE,     AW_OFF );        //Don't delete it once it plays
      awProp( si.mSound, AWSND_PBEND,       1.0 );           //Pitch Bend
      awProp( si.mSound, AWSND_SPL,         60.0 );          //Sound Pressure Level
      awProp( si.mSound, AWSND_SPLLO,       60.0);           //SPL Fade LO
      awProp( si.mSound, AWSND_SPLHI,       160.0);          //SPL Fade HI
      awProp( si.mSound, AWSND_PBENDLO,     1.0);            //Pitch Bend LO
      awProp( si.mSound, AWSND_PBENDHI,     1.0);            //Pitch Bend HI
      awProp( si.mSound, AWSND_LOPASS,      22050.0);        //Lo Pass cutoff
      awProp( si.mSound, AWSND_LOPASSLO,    22050.0);        //Lo Pass Fade LO
      awProp( si.mSound, AWSND_LOPASSHI,    22050.0);        //Lo Pass Fade HI

      // set the player
      si.mPlayer = awNewPlyr();
      awAddPlyrSnd( si.mPlayer, si.mSound );

      //Set up scene object and add the sound objects to it
      awAddSceneSnd(mScene, si.mSound);         //Add the sounds to it

      mBindTable[alias] = si;

      this->setAmbient( alias, sinfo.ambient );
      this->setCutoff( alias, sinfo.cutoff );
      this->setPitchBend( alias, sinfo.pitchbend );
      this->setVolume( alias, sinfo.volume );
      this->setRetriggerable( alias, sinfo.retriggerable );
      
      vpr::DebugOutputGuard output5(snxDBG, vprDBG_CONFIG_LVL, std::string("bind() done...\n"), std::string("\n"));
      // was it playing?  if so, then start it up again...
      if (sinfo.triggerOnNextBind == true)
      {
         sinfo.triggerOnNextBind = false; // done...
         vpr::DebugOutputGuard output6(snxDBG, vprDBG_CONFIG_LVL, std::string("[snx]AudioWorks| NOTIFY: triggering reconfigured sound x\n"), std::string("\n"));
         this->trigger( alias, sinfo.repeat );
      }

   }

   /**
    * unload/deallocate the sound data this alias refers from the sound API
    * @postconditions the sound API no longer has the sound buffered.
    */
   void AudioWorksSoundImplementation::unbind( const std::string& alias )
   {
      if (mBindTable.count( alias ) > 0)
      {
         //awRemPlyrSnd( mBindTable[alias].mPlayer, mBindTable[alias].mSound );
         awRemSceneSnd( mScene, mBindTable[alias].mSound );  // detach from the scene
         awUnMapWavToSE( mBindTable[alias].mWave );           // detach it from the engine
         awDelete( mBindTable[alias].mSound );
         awDelete( mBindTable[alias].mWave );

         mBindTable.erase( alias );
         //std::cout<<"[snx]AudioWorks| DEBUG: unbind() "<<alias<<"\n"<<std::flush;
      }

      assert( mBindTable.count( alias ) == 0 && "should have unbound" );
   }

   /**
    * take a time step of [timeElapsed] seconds.
    * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
    * @input time elapsed since last frame
    */
   void AudioWorksSoundImplementation::step( const float & timeElapsed )
   {
      if (this->isStarted() == true)
      {
         // audioworks wants total time elapsed, not time of last frame.
         mTotalTimeElapsed += timeElapsed;

         snx::SoundImplementation::step( timeElapsed );
         double total_time_elapsed = mTotalTimeElapsed;
         //total_time_elapsed = awGetClockSecs(); // @todo remove me...
         awFrame(total_time_elapsed);

         /*
         int num = awGetNumEngWav( mEngine );       // how many wavs do i have loaded in AW
         int num2 = awGetNumFreeEngWav( mEngine );  // how many wavs can be loaded yet into AW
         int onOrOff( AW_OFF ), onOrOff2, onOrOff3;
         awGetProp( si.mSound, AWSND_EMITTING, onOrOff2 );
         awGetProp( si.mSound, AWSND_PROPAGATING, onOrOff3 );

         if (onOrOff2 == AW_ON || onOrOff3 == AW_ON)
            onOrOff = AW_ON;

         switch (onOrOff)
         {
            case AW_ON:
               break;
            default:
            case AW_OFF:
               break;
         }
         */
      }
   }


}; // end namespace;
