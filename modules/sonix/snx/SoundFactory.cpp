
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

#include <snx/snxConfig.h>

#include <iostream>
#include <fstream>
#include <string>

#include <snx/dirlist.h>
#include <snx/ReplaceEnvVars.h>
#include <snx/PluginAPI.h>
#include <snx/Util/Debug.h>
#include "snx/ISoundImplementation.h"
#include "snx/StubSoundImplementation.h" // in case lookup fails...

#include "snx/SoundFactory.h" // my header.


namespace snx
{

SoundFactory::SoundFactory() : vpr::Singleton<SoundFactory>()
{
   std::vector<std::string> search_paths;
   search_paths.push_back( "${SNX_BASE_DIR}/share/sonix/plugins" );
   search_paths.push_back( "${HOME}/.sonix/plugins" );

   std::vector<std::string> filelist;
   for (unsigned int x = 0; x < search_paths.size(); ++x)
   {
      search_paths[x] = snx::replaceEnvVars( search_paths[x] );
      vpr::DebugOutputGuard output1(snxDBG, vprDBG_CONFIG_LVL, std::string("Finding plugins in: \n"), std::string(""));
      vpr::DebugOutputGuard output2(snxDBG, vprDBG_CONFIG_LVL, search_paths[x]+ std::string("\n"), std::string(""));
      if (xdl::isDir(search_paths[x].c_str()))
      {
         xdl::dirlist( search_paths[x].c_str(), filelist );
         this->filterOutPluginFileNames( search_paths[x].c_str(), filelist, filelist );
         this->loadPlugins( search_paths[x], filelist );
         
         vpr::DebugOutputGuard output3(snxDBG, vprDBG_CONFIG_LVL, std::string("filelist: \n"), std::string(""));
         for(unsigned int i=0;i<filelist.size();i++){
            vpr::DebugOutputGuard output4(snxDBG, vprDBG_CONFIG_LVL, filelist[i]+std::string("\n"), std::string(""), false);
         }
      }
   }
}

void SoundFactory::errorOutput(std::string filename, const char* test)
{
   vpr::DebugOutputGuard output4(snxDBG, vprDBG_CONFIG_LVL, std::string("Plugin: ") + filename + std::string("\n"), std::string(""));
   vpr::DebugOutputGuard output5(snxDBG, vprDBG_CONFIG_LVL, std::string(" does not export symbol: ") + std::string(test), std::string(""));
}

bool SoundFactory::isPlugin(std::string filename)
{  
   xdl::Library lib;
   if (lib.open( filename.c_str(), xdl::NOW ) == false)
   {
      vpr::DebugOutputGuard output6(snxDBG, vprDBG_CONFIG_LVL, std::string("Plugin: " + filename + " has invalid name.\n"), std::string(""));
      return false;
   }
   if (lib.lookup( "newPlugin" ) == NULL)
   {
      errorOutput(filename, "newPlugin");
      return false;
   }  
   if (lib.lookup( "deletePlugin" ) == NULL)
   {
      errorOutput(filename, "deletePlugin");
      return false;
   }
   if (lib.lookup( "getVersion" ) == NULL)
   {
      errorOutput(filename, "getVersion");
      return false;
   }
   if (lib.lookup( "getName" ) == NULL)
   {
      errorOutput(filename, "getName");
      return false;
   }
   

   // @todo give sonix an internal version number string!
   //getVersionFunc getVersion = (getVersionFunc)lib.lookup( "getVersion" );
   //if (getVersion != NULL && getVersion() != snx::version) return false;

   lib.close();
   vpr::DebugOutputGuard output7(snxDBG, vprDBG_CONFIG_LVL, std::string("Plugin: ") + filename + std::string(" is a valid sonix plugin.\n"), std::string(""),false); 
   return true;
}

void SoundFactory::filterOutPluginFileNames(const char* dir,
                                            std::vector<std::string> srclist,
                                            std::vector<std::string>& destlist)
{
   destlist.clear();
   std::string temp;
   
   vpr::DebugOutputGuard output8(snxDBG, vprDBG_CONFIG_LVL, std::string("Checking Plugins validity...\n"), std::string(""));
   for (unsigned int x = 0; x < srclist.size(); ++x)
   {
      if(srclist[x] != "." && srclist[x] != "..")
      {
         vpr::DebugOutputGuard output9(snxDBG, vprDBG_CONFIG_LVL, srclist[x]+std::string("\n"), std::string(""), false);
         temp = dir + std::string("/") + srclist[x];
         if (this->isPlugin( temp/*srclist[x]*/ ))
         {
            destlist.push_back( srclist[x] );
         }
      }
   }
}

void SoundFactory::loadPlugins(std::string prefix,
                               std::vector<std::string> filelist)
{
   unloadPlugins();
   mPlugins.clear();
   mPlugins.resize( filelist.size() );
   for (unsigned int x = 0; x < filelist.size(); ++x)
   {
      std::string full_path = prefix + std::string( "/" ) + filelist[x];
      
      // open the library
      mPlugins[x].open( full_path.c_str(), xdl::NOW );

      // get the name..
      getNameFunc getName = (getNameFunc)mPlugins[x].lookup( "getName" );
      std::string name;
      if (getName != NULL)
      {
         name = getName();
         vpr::DebugOutputGuard output9(snxDBG, vprDBG_CONFIG_LVL, std::string("Got plugin: ")+ name+std::string(" registering...\n"), std::string(""));
         
      
         // create the implementation
         newPluginFunc newPlugin = (newPluginFunc)mPlugins[x].lookup( "newPlugin" );
         ISoundImplementation* si = NULL;
         if (newPlugin != NULL)
         {
            si = newPlugin();
            if (NULL != si)
            {
               this->reg( name, si );
            }
         }
      }
   }
}

void SoundFactory::unloadPlugins()
{
   for (unsigned int x = 0; x < mPlugins.size(); ++x)
   {
      // get the name..
      getNameFunc getName = (getNameFunc)mPlugins[x].lookup( "getName" );
      std::string name;
      ISoundImplementation* impl = NULL;
      if (getName != NULL)
      {
         name = getName();
         impl = mRegisteredImplementations[name];
          
         // unreg it.
         this->reg( name, NULL );
      }

      // delete the memory using the delete func that comes with the library...
      deletePluginFunc deletePlugin = (deletePluginFunc)mPlugins[x].lookup( "deletePlugin" );
      if (NULL != deletePlugin && NULL != impl)
         deletePlugin( impl );
      
      // close the library
      mPlugins[x].close();
   }
   mPlugins.clear();
}

void SoundFactory::reg(const std::string& apiName,
                       snx::ISoundImplementation* impl)
{
   if (impl != NULL)
   {
      vpr::DebugOutputGuard output10(snxDBG, vprDBG_CONFIG_LVL, std::string("NOTICE: Registering sound API: ")+apiName+std::string("\n"), std::string(""));
            //std::string(" [")+(int)impl+"]\n", std::string("\n"));
      impl->setName( apiName );
      mRegisteredImplementations[apiName] = impl;
   }
   else
   {
      mRegisteredImplementations.erase( apiName );
      vpr::DebugOutputGuard output11(snxDBG, vprDBG_CONFIG_LVL, std::string("NOTICE: UnRegistered sound API: ")+apiName+std::string("\n"), std::string(""));
         //   +std::string(" [")+(int)impl+std::string("]\n") std::string("\n"));
   }      
}

/**
 * @input name of api to create
 * @output an implementation is returned for you to use
 * @postconditions if apiName is not known, then a stub implementation is returned
 * @semantics factory function used to create an implementation of a sound API 
 */
void SoundFactory::createImplementation( const std::string& apiName,
                           snx::ISoundImplementation* &mImplementation )
{
   if (mRegisteredImplementations.count( apiName ) > 0)
   {
      mRegisteredImplementations[apiName]->clone( mImplementation );
   }

   else
   {
      mImplementation = new snx::StubSoundImplementation;
   }
   
   //mImplementation->setName( apiName );
}

} // end namespace
