/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

//----------------------------------------------------------------
// radTrav.C
//----------------------------------------------------------------
// PURPOSE:
//	Defines radiosity scene traversal routines
//----------------------------------------------------------------

//   Radiosity Traversal routines
#include <iostream.h>
//#include <fstream.h>
#include <Performer/pr.h>
#include <Performer/pf.h>
#include <Performer/pfutil.h>
#include <Performer/pf/pfGeode.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pr/pfMemory.h>
#include <Performer/pf/pfSwitch.h>
#include <unistd.h>
#include <string>
#include <assert.h>

//: This is a Performer traverser that will replace any node ending in mKeyName
//  in your scene graph with a pjSoundNode.
//
//  For example, if mKeyName == _Sound_, and your nodename == Gong_Sound_
//  then that node will be replaced with a new pjSoundNode of a sound 
//  called "Gong"
class pjSoundReplaceTrav
{
public:
   static void traverse( pfNode* node, SoundEngine* engine, const std::string& keyName = "_Sound_" )
   {
      assert( engine != NULL && node != NULL );
      pjSoundReplaceTrav::mSoundEnginePtr = engine;
      engine->setPosition( 0.0f, 0.0f, 0.0f );
      
      // use the performer traversal mechanism
       pfuTraverser trav;
       pfuInitTraverser( &trav );

       trav.preFunc = pjSoundReplaceTrav::processNode;
       trav.data = const_cast<std::string*>( &keyName );

       pfuTraverse( node, &trav );
   }

   
protected:
   // used to traverse - don't call
   static int processNode( pfuTraverser* trav )
   {
      // get the keyname, usually it is "_Sound_"
      std::string keyName = *reinterpret_cast<std::string*>(trav->data);
      pfNode* currentNode = trav->node;
      if (currentNode == NULL)
      {
         return PFTRAV_CONT;	    // Return continue 
      }
      
      // for verbose output (outputs every node's name.)
      //cout<<"[SoundReplacer] Examining node in graph named \""<<nodeName.c_str()<<"\":\n"<<flush;
         
      std::string nodeName = currentNode->getName();
      if (nodeName.size() <= keyName.size())
      {
         // name was too short to even be a match, discard it.
         return PFTRAV_CONT;	    // Return continue 
      }
      
      int startOfKeyWord = nodeName.size() - keyName.size();
      int endOfKeyWord = nodeName.size() - 1;
      std::string isThisOurKeyWord = nodeName.substr( startOfKeyWord, endOfKeyWord );
      if (isThisOurKeyWord == keyName)   // If node is not a Geode
      {
         cout<<"[SoundReplacer] Examining node in graph named \""<<nodeName.c_str()<<"\":\n"<<flush;
         cout<<"[SoundReplacer]     Substring "<<keyName<<" matched, "<<flush;
         pfGroup* parent = currentNode->getParent( 0 ); // FIXME?? will 0 work for all cases (instanced nodes?)
         if (parent != NULL)
         {
            std::string soundName = nodeName.substr( 0, startOfKeyWord );
            cout<<"extracted sound named \""<<soundName<<"\"\n"<<flush;
            assert( mSoundEnginePtr != NULL );
            Sound* sound = mSoundEnginePtr->getHandle( soundName.c_str() );
            
            // replace the found node with a sound node.
            parent->removeChild( currentNode );
            if (sound != NULL)
            {
               parent->addChild( new pjSoundNode( sound ) );
               sound->trigger();
            }
            else
            {
               cout<<"[SoundReplacer] !!! WARNING !!! SOUND NOT FOUND: "<<soundName.c_str()<<"\n"<<flush;
               cout<<"[SoundReplacer] !!!         !!! You need to enter \""<<soundName.c_str()<<"\" into your sound config file(s)\n"<<flush;
               return PFTRAV_CONT;	    // Return continue                
            }
         }
         else
         {
            cout<<"but Parent is NULL (nowhere to hang the Sound node!)\n"<<flush;
         }         
      }
      // for very verbose output...
      //else
      //{
      //   cout<<"[SoundReplacer]     Substring not matched: \""<<isThisOurKeyWord.c_str()<<"\" != \""<<keyName<<"\"\n"<<flush;
      //}      
      return PFTRAV_CONT;	    // Return continue 
   }
   
   static SoundEngine* mSoundEnginePtr;
};

SoundEngine* pjSoundReplaceTrav::mSoundEnginePtr = NULL;

