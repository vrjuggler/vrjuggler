#include <vjConfig.h>

#include <assert.h>
#include <unistd.h>
#include <iostream.h>
#include <aw.h> //audio works
#include <Sound/SoundEngine.h>   // base class
#include <Sound/aw/AwSound.h>       // my sound type...

#include <Sound/aw/AwSoundEngine.h> // my header

AwSoundEngine::AwSoundEngine() : mObserver( NULL )
{
}

AwSoundEngine::~AwSoundEngine() {}

void AwSoundEngine::init( const char* const adfFileName )
{
   // initialize the AudioWorks system
   awOpenAWD("");
   awOpenEP(0, AWEP_SHARE);
   awEPReset();
   awEPFlush();
   awCloseEP();
   awCloseAWD();
   //printf( "Loading adf file [%s]\n", adfFile);

   // The three stages in setting up a AudioWorks application are
   // - Initialization
   assert( awInitSys() != -1 );

   // - Definition
   // Call awDefineSys() with the name of an application definition file
   assert( awDefineSys( adfFileName ) != -1 );
   // Make explicit function calls to create instances of AudioWorks classes.

   // - Configuration
   // Finally, call awConfigSys(1) to complete setup.  
   // awConfigSys() now requires one argument called the map switch.  
   // This switch is used to allow multiple engine configurations.  
   // Passing in a value of 1 will cause the standard mapping of all 
   // waveforms to be assigned to the first sound engine that is found in 
   // the list.  Passing in a value of 0 will skip all of the mapping
   // function calls.  These functions must be called by the application 
   // for each sound and engine that will be used in the same simulation.
   assert( awConfigSys( 1 ) == 0 );

   // use a separate process for the sound engine.
   // OFF is default
   //awProp( awGetSys(), AWSYS_MPMODE, AW_ON );

   //usleep( 3000 );
   
   // we need an observer to do localized sound.
   mObserver = awFindObs( "you" );
   if (mObserver == NULL)
   {
      cout<<"[aw] \n"
          <<"[aw] !!! WARNING !!!: could not find in the .adf file the \"observer\" named \"you\".  \n"
          <<"[aw] !!!         !!!: This *has* to be there, or else none of the sound localization \n"
          <<"[aw] !!!         !!!:   functions will work (like setPosition). \n"
          <<"[aw] \n"<<flush;
   }
   
   //this->setPosition( 0.0f, 0.0f, 0.0f );
}

// lookup a filename, given the sound's alias.
// the "filename" returned can be used in a call to Sound::load()
void AwSoundEngine::aliasToFileName( const char* const alias, std::string& filename )
{
   // AW finds the name for you. :)
   filename = alias;
}



//: Factory function to create a new sound.
// memory managed by engine
Sound* AwSoundEngine::newSound()
{
   return new AwSound( *this );
}

// call this once per main loop.
//
void AwSoundEngine::update()
{      
   // set the state of our sound to on
   awSyncFrame();

   // time in seconds since awInitSys was called.
   float seconds = awGetClockSecs();

   // cause all processing to be performed for the current frame.
   // functions called after this one, will be applied next frame.
   awFrame( seconds );
}

void AwSoundEngine::kill()
{
   awExit();
}

// set observer position
void AwSoundEngine::setPosition( const vjMatrix& position )
{
   SoundEngine::setPosition( position );
   
   if (mObserver != NULL)
   {
      // vgMat is just a [4][4]
      vgPosition* vgposition = NULL;
      const vgMat& vgmat = position.mat;
      vgPosMat( vgposition, const_cast<vgMat&>( vgmat ) );
   
      awPos( mObserver, vgposition );
   }
}

// set observer position
void AwSoundEngine::setPosition( const float& x, const float& y, const float& z )
{
   SoundEngine::setPosition( x, y, z );
   
   if (mObserver != NULL)
   {
      float xyz[3] = { x, y, z };
      float hpr[3] = { 0.0f, 0.0f, 0.0f };
      awXYZHPR( mObserver, xyz, hpr );
   }
}
