
#include <stdlib.h>   //for NULL
#include <unistd.h>   //for sleep
#include <iostream.h> // for cout
#include "SoundFactory.h"

SoundEngine* gSoundEngine = NULL;
Sound* sound1 = NULL, *sound2 = NULL, *sound3 = NULL;

void usage(char** argv)
{
   cout<<"Usage:\n";
      cout<<"      "<<argv[0]<<" sl  (for plib's SL library)\n";
      cout<<"      "<<argv[0]<<" aw  (for audioworks library)\n"<<flush;
      cout<<"      "<<argv[0]<<" nosound  (to stub out all sound functionality)\n"<<flush;
}

void main(int argc, char** argv)
{
   std::string arg;
   
   if (argc > 1)
   {
      arg = argv[1];
   }   
   
   if (arg == "sl")
   {
      gSoundEngine = SoundFactory::newEngine( SoundFactory::SL, "sound.dat" );
   }
   
   else if (arg == "aw")
   {
      gSoundEngine = SoundFactory::newEngine( SoundFactory::AW, "sound.adf" );
   }
   
   else if (arg == "nosound")
   {
      gSoundEngine = SoundFactory::newEngine( SoundFactory::NOSOUND, "nofileneeded" );
   }   
   
   else
   {
      ::usage(argv);
      cout<<"\n"<<flush;
      cout<<"!!! WARNING !!!: Option \""<<arg.c_str()<<"\" not recognized, Defaulting to the \"nosound\" option\n"<<flush;
      cout<<"\n"<<flush;
      sleep( 1 );
      gSoundEngine = SoundFactory::newEngine( SoundFactory::NOSOUND, "nofileneeded" );
      //exit( 0 );
   }   
   
   sound1 = gSoundEngine->getHandle( "sound1" );
   sound1->print();
   
   sound2 = gSoundEngine->getHandle( "sound2" );
   sound2->print();
   
   sound3 = gSoundEngine->getHandle( "sound3" );
   sound3->print();

   // Find players
   //pendulumplayer = awFindPlyr("pendulumplayer");

   // Find objects
   //you = awFindObs("you");
 
   gSoundEngine->setPosition( 10, 0, 0 );
   sound1->trigger();
   gSoundEngine->update();
   usleep( 1 );
   
   gSoundEngine->setPosition( 0, 5, 0 );
   sound2->trigger();
   gSoundEngine->update();
   usleep( 1 );
   
   gSoundEngine->setPosition( 0, 5, 5 );
   sound3->trigger();
   gSoundEngine->update();
   usleep( 1 );
   
   gSoundEngine->setPosition( 0, 0, 20 );
   sound2->trigger();
   gSoundEngine->update();
   sleep( 1 );
   
   gSoundEngine->setPosition( 0, 0, 10 );
   sound1->trigger();
   gSoundEngine->update();
   sleep( 1 );
   
   gSoundEngine->setPosition( 0, 0, 5 );
   sound3->trigger();
   gSoundEngine->update();
   sleep( 1 );
   
   gSoundEngine->setPosition( -5, -5, -4 );
   sound1->trigger();
   gSoundEngine->update();
   sleep( 1 );
   
   gSoundEngine->setPosition( 30, 0, 0 );
   sound2->trigger();
   gSoundEngine->update();
   sleep( 1 );
   
   sound1->stop();
   gSoundEngine->update();
  
   usleep( 30000 );
   
   cout<<"Sound should have stopped....\n"<<flush;
   cout<<"Done...\n"<<flush;
   gSoundEngine->kill();
   
   
   delete gSoundEngine;
   delete sound1;
   
   
   usleep( 1 );
}
