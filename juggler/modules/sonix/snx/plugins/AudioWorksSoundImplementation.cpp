
#include "AudioWorksSoundImplementation.h"

#include "aj/SoundFactory.h"
namespace aj
{
aj::SoundFactoryReg<AudioWorksSoundImplementation> audioworksRegistrator( "AudioWorks" );


/**
 * constructor for the OpenAL implementation 
 */
AudioWorksSoundImplementation::AudioWorksSoundImplementation() : aj::SoundImplementation(), mTotalTimeElapsed( 0.0f )
{
   // TODO: set up the defaults for aw...
   //mSoundAPIInfo.
}

/**
 * destructor for the OpenAL implementation
 */
AudioWorksSoundImplementation::~AudioWorksSoundImplementation()
{
}

/**
  * every implementation can return a new copy of itself
  */
void AudioWorksSoundImplementation::clone( aj::SoundImplementation* &newCopy )
{
   newCopy = new AudioWorksSoundImplementation;

   // copy state, so that we return a true "clone"
   newCopy->copy( *this );
}

/**
 * @input alias of the sound to trigger, and number of times to play
 * @preconditions alias does not have to be associated with a loaded sound.
 * @postconditions if it is, then the loaded sound is triggered.  if it isn't then nothing happens.
 * @semantics Triggers a sound
 */
void AudioWorksSoundImplementation::trigger( const std::string & alias, const unsigned int & looping )
{
   aj::SoundImplementation::trigger( alias, looping );

   if (mBindTable.count( alias ) > 0)
   {
      awProp(mBindTable[alias].mySound, AWSND_STATE, AW_ON);
   }
}

/*
 * when sound is already playing then you call trigger,
 * does the sound restart from beginning?
 * (if a tree falls and no one is around to hear it, does it make sound?)
 */
void AudioWorksSoundImplementation::setRetriggerable( const std::string& alias, bool onOff )
{
   aj::SoundImplementation::setRetriggerable( alias, onOff );

   if (mBindTable.count( alias ) > 0)
   {
      if (onOff == true)
         awProp(mBindTable[alias].mySound, AWSND_RETRIGGER, AW_ON);    //Set retriggering ON
      else
         awProp(mBindTable[alias].mySound, AWSND_RETRIGGER, AW_OFF);   //Set retriggering ON
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
   aj::SoundImplementation::setAmbient( alias, isAmbient );

   if (mBindTable.count( alias ) > 0)
   {
      if (isAmbient == true)
      {
         // attaches the player to observer
         awProp( mBindTable[alias].mPlayer, AWPLYR_CSREF, AWPLYR_OBS );
         awPlyrObsRef( mBindTable[alias].mPlayer, myObs );

         //float xyz[3] = { 0.0f, 0.0f, 0.0f }, hpr[3] = { 0.0f, 0.0f, 0.0f };
         //awXYZHPR(si.mySound, xyz, hpr);               //Set sound at origin
      }

      else
      {
         // detaches player from observer
         awProp( mBindTable[alias].mPlayer, AWPLYR_CSREF, AWPLYR_CSREFPOS );

         float xyz[3] = { mSounds[alias].position[0], mSounds[alias].position[1], mSounds[alias].position[2] };
         float hpr[3] = { 0.0f, 0.0f, 0.0f };
         // convert xyz to performer coords...
         awPlyrCSRef( mBindTable[alias].mPlayer, xyz, hpr );
      }
   }
}


/*
 * mute, sound continues to play, but you can't hear it...
 */
void AudioWorksSoundImplementation::mute( const std::string& alias )
{
   aj::SoundImplementation::mute( alias );

   if (mBindTable.count( alias ) > 0)
   {
      awProp( mBindTable[alias].mySound, AWSND_ENABLE, AW_OFF ); //Enable the sound
   }
}

/*
 * unmute, let the muted-playing sound be heard again
 */
void AudioWorksSoundImplementation::unmute( const std::string& alias )
{
   aj::SoundImplementation::unmute( alias );

   if (mBindTable.count( alias ) > 0)
   {
      awProp( mBindTable[alias].mySound, AWSND_ENABLE, AW_ON ); // Disable the sound
   }
}

/**
 * @semantics stop the sound
 * @input alias of the sound to be stopped
 */
void AudioWorksSoundImplementation::stop( const std::string& alias )
{
   aj::SoundImplementation::stop( alias );

   if (mBindTable.count( alias ) > 0)
   {
      awProp(mBindTable[alias].mySound, AWSND_STATE, AW_ON);
   }
}

/**
 * set sound's 3D position 
 */
void AudioWorksSoundImplementation::setPosition( const std::string& alias, float x, float y, float z )
{
   // NOTE: don't limit based on ambiance, even abient sounds can have position...
   aj::SoundImplementation::setPosition( alias, x, y, z );

   if (mBindTable.count( alias ) > 0 && mSounds.count( alias ) > 0)
   {
      float hpr[3] = { 0.0f, 0.0f, 0.0f };
      awXYZHPR( mBindTable[alias].mySound, mSounds[alias].position, hpr );  //Set sound at origin
   }
}

/**
 * get sound's 3D position
 * @input alias is a name that has been associate()d with some sound data
 * @output x,y,z are returned in OpenGL coordinates.
 */
void AudioWorksSoundImplementation::getPosition( const std::string& alias, float& x, float& y, float& z )
{
   aj::SoundImplementation::getPosition( alias, x, y, z );
}

/**
 * set the position of the listener
 */
void AudioWorksSoundImplementation::setListenerPosition( const aj::Matrix44& mat )
{
   aj::SoundImplementation::setListenerPosition( mat );
}

/**
 * get the position of the listener
 */
void AudioWorksSoundImplementation::getListenerPosition( aj::Matrix44& mat )
{
   aj::SoundImplementation::getListenerPosition( mat );
}

/**
 * start the sound API, creating any contexts or other configurations at startup
 * @postconditions sound API is ready to go.
 * @semantics this function should be called before using the other functions in the class.
 */
void AudioWorksSoundImplementation::startAPI()
{
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
     std::cerr << "InitSys() failed!\n" << std::flush;
     return;
   }

   //Initialize the engine
   myEngine = awNewEng();         //Define the new engine
   awProp(myEngine, AWENG_VOLUME, 1.0);   //Set its volume to maximum output
   if (awAttachEng(myEngine) != 0)      //Attach the engine to the system
   {
       std::cerr << "\nfailed to attach to engine\nengine dump:\n" << std::flush;
       awPrint(myEngine);
       return;
   }

   //Set up the channel
   myChannel = awNewChan();            //Define the new channel
   awChanEng(myChannel, myEngine);         //Associate the channel with the engine

   awProp(myChannel, AWCHAN_MODEL, AWIF_QUAD);   //Set QUAD sound imaging model
   awProp(myChannel, AWCHAN_NVOICES, 16);      //Set 16 voices at once
   awProp(myChannel, AWCHAN_ENABLE, AW_ON);      //Enable the channel imaging


   //Set up scene object and add the sound objects to it
   myScene = awNewScene();            //Define new scene

   //Set up environment object
   myEnv = awNewEnv();            //Define new environment
   awProp(myEnv, AWENV_SOS, 330.0);         //Set the speed of sound

   //Set up observer object
   myObs = awNewObs();                  //Define new observer

   awProp(myObs, AWOBS_STATE,       AW_ON);      //Enable the observer
   awProp(myObs, AWOBS_TETHERCOORD,       AWOBS_TABSOLUTE);   //Set coords to absolute
   awProp(myObs, AWOBS_CSREF,       AWOBS_ABSOLUTE);   //Set coords to absolute
   awProp(myObs, AWOBS_SPLMIN,       40.0);         //Minimum SPL heard
   awProp(myObs, AWOBS_SPLMAX,       200.0);      //Maximum SPL heard
   awProp(myObs, AWOBS_CALCVEL,       AW_ON);      //Calculate the observer velocity
   awProp(myObs, AWOBS_ATTENUATION,       AW_ON);      //Sound drops volume at distance
   awProp(myObs, AWOBS_DELAY,       AW_ON);      //Sound takes time to travel
   awProp(myObs, AWOBS_DOPPLER,       AW_ON);      //Sound movement doppler effect
   awProp(myObs, AWOBS_PRIORITY,       AW_OFF);      //Sound playing priority
   awProp(myObs, AWOBS_WCMAP,       AW_ON);      //World Coordinate Mapping ON


   // set up the observer (audiojuggler API only supports one for now)
   awAddObsChan(myObs, myChannel);      //Associate observer with the channel
   awObsScene(myObs, myScene);      //Associate observer with the scene
   awObsEnv(myObs, myEnv);         //Associate observer with the environment

//   float xyz[3] = { 0.0f, 0.0f, 0.0f };
//   float hpr[3] = { 0.0f, 0.0f, 0.0f };
//   awXYZHPR(myObs, xyz, hpr);      //Set the observer at the origin
   // init the listener...
   this->setListenerPosition( mListenerPos );
  //End observer setup

  if (awConfigSys(1) != 0)         //Attempt to configure the system
  {    
    std::cout << "ConfigSys() failed!\n" << std::flush;
    awPrint(myEngine);
    return;
  }
}   

/**
 * kill the sound API, deallocating any sounds, etc...
 * @postconditions sound API is ready to go.
 * @semantics this function could be called any time, the function could be called multiple times, so it should be smart.
 */
void AudioWorksSoundImplementation::shutdownAPI()
{
   awExit();
}   

/**
  * query whether the API has been started or not
  * @semantics return true if api has been started, false otherwise.
  */
bool AudioWorksSoundImplementation::isStarted() const
{
   return false;
}

/**
 * configure the sound API global settings
 */
void AudioWorksSoundImplementation::configure( const aj::SoundAPIInfo& sai )
{
   aj::SoundImplementation::configure( sai );
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
void AudioWorksSoundImplementation::configure( const std::string& alias, const aj::SoundInfo& description )
{
   aj::SoundImplementation::configure( alias, description );
   // do nothing
}

/**
  * remove a configured sound, any future reference to the alias will not
  * cause an error, but will not result in a rendered sound
  */
void AudioWorksSoundImplementation::remove( const std::string alias )
{
   aj::SoundImplementation::remove( alias );
   // do nothing
}

/**
 * clear all associate()tions.
 * @semantics any existing aliases will be stubbed. aounds will be unbind()ed
 */
void AudioWorksSoundImplementation::clear()
{
}   

/**
 * bind: load (or reload) all associate()d sounds
 * @postconditions all sound associations are buffered by the sound API
 */
void AudioWorksSoundImplementation::bindAll()
{
}   

/**
 * unbind: unload/deallocate all associate()d sounds.
 * @postconditions all sound associations are unbuffered by the sound API
 */
void AudioWorksSoundImplementation::unbindAll()
{
}

/**
 * load/allocate the sound data this alias refers to the sound API
 * @postconditions the sound API has the sound buffered.
 */
void AudioWorksSoundImplementation::bind( const std::string& alias )
{
   this->unbind( alias );

   AWSoundInfo si;
   aj::SoundInfo sinfo = mSounds[alias];


   // Set up waves and load files
   si.myWave = awNewWav();                      //Define the wave form
   awName( si.myWave, sinfo.filename.c_str() ); //Set the aifc filename
   if (awLoadWav(si.myWave) != 0)               //Load the aifc file
   {
       std::cout << "\nfailed to open wave file\nwave dump:\n" << std::flush;
       awPrint( si.myWave );
       return;
   }
   awMapWavToSE( si.myWave, myEngine );                    //Associate the wave with the engine
   awFlushWavToSE( si.myWave );                            //Flush the changes to the engine

   // Set up sounds and associate them with waves
   si.mySound = awNewSnd();                                //Define new sound
   awSndWave( si.mySound, si.myWave );                     //Associate the sound with the wave
   awMakeSnd( si.mySound);                                 //Necessary to change the wave form

   awProp( si.mySound, AWSND_ENABLE,      AW_ON );         //Enable the sound
   awProp( si.mySound, AWSND_STATE,       AW_OFF );        //Turn it off for now
   awProp( si.mySound, AWSND_RETRIGGER,   AW_ON );         //Set retriggering ON

   awProp( si.mySound, AWSND_ABSORPTION,  AWSND_INHERIT ); //Inherit modeling settings
   awProp( si.mySound, AWSND_ATTENUATION, AWSND_INHERIT ); //from the observer
   awProp( si.mySound, AWSND_DOPPLER,     AWSND_INHERIT ); // ...
   awProp( si.mySound, AWSND_DELAY,       AWSND_INHERIT ); // ...
   awProp( si.mySound, AWSND_WCMAP,       AWSND_INHERIT ); // ...
   awProp( si.mySound, AWSND_PRIORITY,    AWSND_INHERIT ); // ...
   awProp( si.mySound, AWSND_EXPUNGE,     AW_OFF );        //Don't delete it once it plays
   awProp( si.mySound, AWSND_PBEND,       1.0 );           //Pitch Bend
   awProp( si.mySound, AWSND_SPL,         60.0 );          //Sound Pressure Level
   awProp( si.mySound, AWSND_SPLLO,       60.0);           //SPL Fade LO
   awProp( si.mySound, AWSND_SPLHI,       160.0);          //SPL Fade HI
   awProp( si.mySound, AWSND_PBENDLO,     1.0);            //Pitch Bend LO
   awProp( si.mySound, AWSND_PBENDHI,     1.0);            //Pitch Bend HI
   awProp( si.mySound, AWSND_LOPASS,      22050.0);        //Lo Pass cutoff
   awProp( si.mySound, AWSND_LOPASSLO,    22050.0);        //Lo Pass Fade LO
   awProp( si.mySound, AWSND_LOPASSHI,    22050.0);        //Lo Pass Fade HI

   // set the player
   si.mPlayer = awNewPlyr();
   awAddPlyrSnd( si.mPlayer, si.mySound );

   //Set up scene object and add the sound objects to it
   awAddSceneSnd(myScene, si.mySound);         //Add the sounds to it

   mBindTable[alias] = si;

   this->setAmbient( alias, sinfo.ambient );
}

/**
 * unload/deallocate the sound data this alias refers from the sound API
 * @postconditions the sound API no longer has the sound buffered.
 */
void AudioWorksSoundImplementation::unbind( const std::string& alias )
{
   if (mBindTable.count( alias ) > 0)
   {
      AWSoundInfo si;
      si = mBindTable[alias];

      awRemSceneSnd( myScene, si.mySound );  // detach from the scene
      //awUnMapWavToSE( si.myWave, myEngine ); // detach it from the engine
      awUnMapWavToSE( si.myWave );           // detach it from the engine
      awDelete( si.mySound );
      awDelete( si.myWave );
   }
}

/**
 * take a time step of [timeElapsed] seconds.
 * @semantics call once per sound frame (doesn't have to be same as your graphics frame)
 * @input time elapsed since last frame
 */
void AudioWorksSoundImplementation::step( const float & timeElapsed )
{
   // audioworks wants total time elapsed, not time of last frame.
   mTotalTimeElapsed += timeElapsed;

   aj::SoundImplementation::step( timeElapsed );
   double total_time_elapsed = mTotalTimeElapsed;
   //total_time_elapsed = awGetClockSecs();
   awFrame(total_time_elapsed);

   /*
   int num = awGetNumEngWav( myEngine );       // how many wavs do i have loaded in AW
   int num2 = awGetNumFreeEngWav( myEngine );  // how many wavs can be loaded yet into AW
   int onOrOff( AW_OFF ), onOrOff2, onOrOff3;
   awGetProp( si.mySound, AWSND_EMITTING, onOrOff2 );
   awGetProp( si.mySound, AWSND_PROPAGATING, onOrOff3 );

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


}; // end namespace;
