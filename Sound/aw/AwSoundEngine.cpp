/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <vjConfig.h>

#include <unistd.h>
#include <aw.h> //audio works
#include <Sound/vjSoundEngine.h>   // base class
#include <Sound/aw/AwSound.h>       // my sound type...
#include <Config/vjParseUtil.h>
#include <Utils/vjFileIO.h>
#include <Sound/aw/AwSoundEngine.h> // my header

AwSoundEngine::AwSoundEngine() : vjSoundEngine(), mObserver( NULL ), mInitialized( false )
{
}

AwSoundEngine::~AwSoundEngine() {}

void AwSoundEngine::init()
{
   vjDEBUG_BEGIN(vjDBG_ALL,vjDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"==============================================================")  << "\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"Juggler is starting Audioworks:")  << "\n" << vjDEBUG_FLUSH;

   std::string tmpFile = "/var/tmp/";
   tmpFile += getenv( "USER" );
   tmpFile += ".AwSoundEngine.adf";


   std::string command = "${VJ_BASE_DIR}/bin/catadf.pl";
   command = vjFileIO::demangleFileName( command, "" );
   command += " -o ";
   command += tmpFile;
   command += " ";
   command += mAdfFileList;

   std::string commandRm = "rm ";
   commandRm += tmpFile;

   // removing old single adf file.
   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << "Attempting to delete: " << commandRm.c_str() << "\n" << vjDEBUG_FLUSH;
   unlink(tmpFile.c_str());

   // creating single adf file from many, using the perl script.
   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << "Creating single adf file from many, using the perl script.\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << "Using command: " << command.c_str() << "\n" << vjDEBUG_FLUSH;
   system( command.c_str() );
   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << "Done with command\n" << vjDEBUG_FLUSH;

   mInitialized = false;  //set it to true at the end of this func...

   // initialize the AudioWorks system
   vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << "** initialize the AudioWorks system\n" << vjDEBUG_FLUSH;

   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << "awOpenAWD\n" << vjDEBUG_FLUSH;
   awOpenAWD("");
   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << "awOpenEP\n" << vjDEBUG_FLUSH;
   awOpenEP(0, AWEP_SHARE);
   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << "awEPReset,awEPFlush,awCloseEP,awCloseAWD\n" << vjDEBUG_FLUSH;
   awEPReset();
   awEPFlush();
   awCloseEP();
   awCloseAWD();
   //printf( "Loading adf file [%s]\n", adfFile);

   // The three stages in setting up a AudioWorks application are
   // - Initialization
   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << "awInitSys\n" << vjDEBUG_FLUSH;
   int result = awInitSys();
   if (result == -1)
   {
      vjASSERT( result != -1 && "awInitSys failed" );
      return;
   }
   
   // - Definition
   // Call awDefineSys() with the name of an application definition file
   vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) << "[aw] Loading: " << std::flush
                                       << tmpFile.c_str() << "\n"
                                       << vjDEBUG_FLUSH;
   result = awDefineSys( tmpFile.c_str() );
   if (result == -1)
   {
      vjDEBUG(vjDBG_ALL,vjDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"[aw] FATAL: ")  << "awDefineSys failed...Audioworks couldn't initialize (or not as likely, ADF file couldn't be found)"<< "\n" << vjDEBUG_FLUSH;
      //sleep( 5 );
      vjASSERT( false && "awDefineSys failed...Audioworks system couldn't initialize (or not as likely, ADF file couldn't be found)" );
      return;
   }
   
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
   vjDEBUG(vjDBG_ALL,vjDBG_STATE_LVL) << "awConfigSys\n" << vjDEBUG_FLUSH;
   result = awConfigSys( 1 );
   if (result != 0)
   {
      vjASSERT( result == 0 && "awConfigSys failed" );
      return;
   }
   vjDEBUG( vjDBG_ALL, vjDBG_CONFIG_LVL ) << clrOutNORM(clrYELLOW,"done\n") << vjDEBUG_FLUSH;

   // use a separate process for the sound engine.
   // OFF is default
   //awProp( awGetSys(), AWSYS_MPMODE, AW_ON );

   //usleep( 3000 );

   // we need an observer to do localized sound.
   mObserver = awFindObs( "you" );
   if (mObserver == NULL)
   {
      vjDEBUG(vjDBG_ALL,vjDBG_CONFIG_LVL) <<"[aw] \n"
          <<"[aw] !!! WARNING !!!: could not find in "<<tmpFile.c_str()<<" file the \"observer\" named \"you\".  \n"
          <<"[aw] !!!         !!!: This *has* to be there, or else none of the sound localization \n"
          <<"[aw] !!!         !!!:   functions will work (like setPosition). \n"
          <<"[aw] \n"<<vjDEBUG_FLUSH;
   }

   // default...
   this->setPosition( mPosition );

   mInitialized = true;
   vjDEBUG_END(vjDBG_ALL,vjDBG_CONFIG_LVL) << clrOutNORM(clrGREEN,"==============================================================")  << "\n" << vjDEBUG_FLUSH;
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
vjSound* AwSoundEngine::newSound()
{
   // if not initialized...
   if (mInitialized == false)
   {
      this->init();         // Initialize the new puppy *woof*
   }
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
   vjSoundEngine::setPosition( position );

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
   vjSoundEngine::setPosition( x, y, z );

   if (mObserver != NULL)
   {
      float xyz[3] = { x, y, z };
      float hpr[3] = { 0.0f, 0.0f, 0.0f };
      awXYZHPR( mObserver, xyz, hpr );
   }
}

bool AwSoundEngine::config( vjConfigChunk* chunk )
{
   vjASSERT( (std::string)chunk->getType() == AwSoundEngine::getChunkType() );

   std::string adfName = (std::string)chunk->getProperty( "adfConfigFile" );

   if (adfName == "")
   {
      return false;
   }

   else
   {
      std::string demangled = demangleFileName( adfName, "" );
      mAdfFileList += demangled;
      mAdfFileList += " ";
      return true;
   }
}


