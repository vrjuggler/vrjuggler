
/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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




/* Generated by Together */

#ifndef SNXSOUNDFACTORY_H
#define SNXSOUNDFACTORY_H
#include <iostream>
#include <string>
#include <fstream>
#include "snx/xdl.h"
#include "snx/dirlist.h"
#include "snx/ReplaceEnvVars.h"
#include "snx/Singleton.h"
#include "snx/SoundImplementation.h"

namespace snx
{

class SoundFactory : public snx::Singleton<SoundFactory>
{
public:
   SoundFactory() : snx::Singleton<SoundFactory>()
   {
      std::vector<std::string> search_paths;
      search_paths.push_back( "${SNX_BASE_DIR}/lib/modules/sonix" );
      search_paths.push_back( "${SNX_BASE_DIR}/share/sonix/plugins" );
      search_paths.push_back( "${HOME}/.sonix/plugins" );

      std::vector<std::string> filelist;
      for (int x = 0; x < search_paths.size(); ++x)
      {
         search_paths[x] = snx::replaceEnvVars( search_paths[x] );
         std::cout << "[snx]SoundFactory| Finding plugins in: \n"
                   << " + " << search_paths[x] << std::endl;
         if (std::ifstream(search_paths[x].c_str()).good())
         {
            xdl::dirlist( search_paths[x].c_str(), filelist );
            this->filterOutPluginFileNames( filelist, filelist );
            this->loadPlugins( filelist );
         }
      }
   }

   bool isPlugin( std::string filename )
   {
      if (filename.find( ".snx" ) != std::string::npos)
      {
         xdl::Library lib;
         if (lib.open( filename.c_str(), xdl::NOW ) == false) return false;
         if (lib.lookup( "regPlugin" ) == NULL) return false;
         if (lib.lookup( "unregPlugin" ) == NULL) return false;
         if (lib.lookup( "getVersion" ) == NULL) return false;
         if (lib.lookup( "getName" ) == NULL) return false;

         // @todo give sonix an internal version number string!
         typedef std::string (*getVersionFunc)(void);
         getVersionFunc getVersion = (getVersionFunc)lib.lookup( "getVersion" );
         //if (getVersion != NULL && getVersion() != snx::version) return false;

         lib.close();
         return true;
      }
      else
      {
         return false;
      }
   }

   void filterOutPluginFileNames( std::vector<std::string> srclist,
                                  std::vector<std::string>& destlist )
   {
      for (int x = 0; x < srclist.size(); ++x)
      {
         if (this->isPlugin( srclist[x] ))
         {
            destlist.push_back( srclist[x] );
         }
      }
   }
   
   std::vector<xdl::Library> mPlugins;
   
   
   void loadPlugins( std::vector<std::string> filelist )
   {
      unloadPlugins();
      mPlugins.clear();
      mPlugins.resize( filelist.size() );
      for (int x = 0; x < filelist.size(); ++x)
      {
         typedef SoundImplementation* (*regPluginFunc)(void);
         mPlugins[x].open( filelist[x].c_str(), xdl::NOW );
         regPluginFunc regPlugin = (regPluginFunc)mPlugins[x].lookup( "regPlugin" );
         if (regPlugin != NULL)
            regPlugin();
      }
   }   
   
   void unloadPlugins()
   {
      for (int x = 0; x < mPlugins.size(); ++x)
      {
         typedef void (*unregPluginFunc)(void);
         unregPluginFunc unregPlugin = (unregPluginFunc)mPlugins[x].lookup( "unregPlugin" );
         if (unregPlugin != NULL)
            unregPlugin();
         mPlugins[x].close();
      }
      mPlugins.clear();
   }

   /**
    * @input name of api to create
    * @output an implementation is returned for you to use (new memory, you delete)
    * @postconditions if apiName is not known, then a stub implementation is returned
    * @semantics factory function used to create an implementation of a sound API 
    */
   void createImplementation( const std::string& apiName,
                              snx::SoundImplementation* &mImplementation );

   // pass valid pointer to a sound implementation with a name it's refered to
   // or... pass NULL to unregister/delete apiName...
   void reg( const std::string& apiName, snx::SoundImplementation* impl )
   {
      if (impl != NULL)
      {
         std::cout<<"[snx]SoundFactory| NOTICE: Registering sound API: "<<apiName<<" ["<<(int)impl<<"]\n"<<std::flush;
         impl->setName( apiName );
         mRegisteredImplementations[apiName] = impl;
      }
      else
      {
         mRegisteredImplementations.erase( apiName );
         std::cout<<"[snx]SoundFactory| NOTICE: UnRegistered sound API: "<<apiName<<" ["<<(int)impl<<"]\n"<<std::flush;
      }      
      
   }
   
   std::map< std::string, snx::SoundImplementation* > mRegisteredImplementations;
private:  
   /** @link dependency */
   /*#  snx::SoundImplementation lnkSoundImplementation; */

   /** @link dependency */
   /*#  snx::StubSoundImplementation lnksnx::StubSoundImplementation; */

   /** @link dependency */
   /*#  snx::AudioWorksSoundImplementation lnksnx::AudioWorksSoundImplementation; */

   /** @link dependency */
   /*#  snx::OpenALSoundImplementation lnksnx::OpenALSoundImplementation; */

   /** @link aggregation 
    * @clientCardinality 1
    * @supplierCardinality 0..*
    * @directed */
   /*#  snx::SoundImplementation lnkSoundImplementation1; */
};

template <class _type>
class SoundFactoryReg
{
public:
   _type mSoundAPI;
   std::string mNameOfAPI;
   SoundFactoryReg( const std::string& apiName ) : mSoundAPI(), mNameOfAPI( apiName )
   {
      mSoundAPI.setName( mNameOfAPI );
      snx::SoundFactory::instance()->reg( mNameOfAPI, &mSoundAPI );
   }
   virtual ~SoundFactoryReg()
   {
      mSoundAPI.setName( "unavailable/deleted/nomore" );
      snx::SoundFactory::instance()->reg( mNameOfAPI, NULL );
   }   
};

}; // end namespace

#endif //SNXSOUNDFACTORY_H
