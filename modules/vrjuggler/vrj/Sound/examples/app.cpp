
#include <stdlib.h>   //for NULL
#include <unistd.h>   //for sleep
#include <iostream.h> // for cout
#include "SoundFactory.h"
#include "fileIO.h"

SoundEngine* gSoundEngine = NULL;
Sound* longPan = NULL, *fastPan = NULL, *shortTriggeredSound = NULL;

void usage(char** argv)
{
   cout<<"Usage:\n";
      cout<<"      "<<argv[0]<<" sl  (for plib's SL library)\n";
      cout<<"      "<<argv[0]<<" aw  (for audioworks library)\n"<<flush;
      cout<<"      "<<argv[0]<<" nosound  (to stub out all sound functionality)\n"<<flush;
}

bool initSoundEngine( const std::string& arg, SoundEngine* &engine )
{
   std::string soundConfigFile;
   if (arg == "sl") // some hacky logic.
   {
      soundConfigFile = "sound.dat";
   }
   
   else if (arg == "aw")
   {
      soundConfigFile = "sound.adf";
   }
   
   else if (arg == "nosound")
   {
      soundConfigFile = "sound.dat";
   }   
   else
   {
      engine = NULL;
      return false; //user didn't specify an option
   }
   
   std::string soundConfigFileWithPath;
   if (fileIO::fileExistsResolvePath( soundConfigFile, soundConfigFileWithPath ))
   {
      engine = SoundFactory::newEngine( arg, soundConfigFileWithPath.c_str() );
      
      if ( engine == NULL)
      {
         cout<<"NULL engine\n"<<flush;
         return false;
      }      
   }
      
   else
   {
      cout<<"Couldn't find "<<soundConfigFile<<"\n"<<flush;
      engine = NULL;
      return false;
   }    
   
   cout<<"Good engine\n"<<flush;
   return true; //user specified an option
}

void main(int argc, char** argv)
{
   std::string arg;
   
   if (argc > 1)
   {
      arg = argv[1];
   }   
   
   // get the sound engine...
   bool success = initSoundEngine( arg, gSoundEngine );
   if (!success)
   {
      ::usage(argv);
      cout<<"\n"<<flush;
      cout<<"!!! WARNING !!!: Option \""<<arg.c_str()<<"\" not recognized, Defaulting to the \"nosound\" option\n"<<flush;
      cout<<"\n"<<flush;
      arg = "nosound"; //default arg
      success = initSoundEngine( arg, gSoundEngine );
      sleep( 1 );
   }   
   
   assert( gSoundEngine != NULL );
   assert( success == true );
   
   longPan = gSoundEngine->getHandle( "longPan" );
   longPan->print();
   
   fastPan = gSoundEngine->getHandle( "fastPan" );
   fastPan->print();
   
   shortTriggeredSound = gSoundEngine->getHandle( "shortTriggeredSound" );
   shortTriggeredSound->print();

   // Find players
   //pendulumplayer = awFindPlyr("pendulumplayer");

   // Find objects
   //you = awFindObs("you");
 
   /*gSoundEngine->setPosition( 10, 0, 0 );
   longPan->trigger();
   gSoundEngine->update();
   usleep( 1 );
   
   gSoundEngine->setPosition( 0, 5, 0 );
   fastPan->trigger();
   gSoundEngine->update();
   usleep( 1 );
   
   gSoundEngine->setPosition( 0, 5, 5 );
   shortTriggeredSound->trigger();
   gSoundEngine->update();
   usleep( 1 );
   
   gSoundEngine->setPosition( 0, 0, 20 );
   fastPan->trigger();
   gSoundEngine->update();
   sleep( 1 );
   
   gSoundEngine->setPosition( 0, 0, 10 );
   longPan->trigger();
   gSoundEngine->update();
   sleep( 1 );
   
   gSoundEngine->setPosition( 0, 0, 5 );
   shortTriggeredSound->trigger();
   gSoundEngine->update();
   sleep( 1 );
   
   gSoundEngine->setPosition( -5, -5, -4 );
   longPan->trigger();
   gSoundEngine->update();
   sleep( 1 );
   
   gSoundEngine->setPosition( 30, 0, 0 );
   fastPan->trigger();
   gSoundEngine->update();
   sleep( 1 );
   */

   gSoundEngine->setPosition( 0, 0, 0 );
  
   // NOTES:  AW messes up when you call trigger, and setPosition every frame.
   //         if that happens, then you'll only hear sound when it's at 0,0,0
   //         this shouldn't be a problem though, since in a real app
   //         sounds are triggered infrequently compared to the number 
   //         of frames occuring.
   
   // fastPan positional test.
   // real fast panning
   cout<<"Fast panning\n"<<flush;
   for (int yy = 0; yy < 5; ++yy)
   {
      fastPan->trigger();
      for ( int x = 0; x < 100; ++x )  
      {
         cout<<"Position: "<<x - 50<<"\n"<<flush;
         //longPan->stop();
         //gSoundEngine->update();
         fastPan->setPosition( x - 50, 0, 0 );
         //gSoundEngine->update();
         gSoundEngine->update();
         usleep( 25000 );
      }
   }
   sleep( 1 );

   // positional test, for when longPan has looppoints.
   // nice long panning.
   cout<<"Slow panning left to right, back to front\n"<<flush;
   for ( int x = 0; x < 5; ++x)
   {
      longPan->trigger(); // just to test out triggering, 
                         // and to have a predictable beginning.
   
      // pan back ...
      for ( int z = 0; z < 100; ++z )  
      {
         longPan->setPosition( z - 50, 0, x * 10 - 50 );
         gSoundEngine->update();
         usleep( 50000 );
      }
      
      // ... and forth
      for ( z = 100; z > 0; --z )  
      {
         longPan->setPosition( z - 50, 0, x * 10 - 50 );
         gSoundEngine->update();
         usleep( 50000 );
      }
   }
  
   sleep( 1 );
   
   // or just trigger a sound, no panning...
   cout<<"Triggering a short sound....\n"<<flush;
   shortTriggeredSound->trigger();
   gSoundEngine->update();
   
   sleep( 3 );
  
   gSoundEngine->update();
   longPan->stop();
   fastPan->stop();
   shortTriggeredSound->stop();
   gSoundEngine->update();
  
   usleep( 30000 );
   
   cout<<"Sound should have stopped....\n"<<flush;
   cout<<"Done...\n"<<flush;
   gSoundEngine->kill();
   
   
   delete gSoundEngine;
   delete longPan;
   
   
   usleep( 1 );
}
