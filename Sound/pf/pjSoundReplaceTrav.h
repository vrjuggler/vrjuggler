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

#ifndef _PERFORMER_JUGGLER_SOUND_REPLACE_TRAV_H_
#define _PERFORMER_JUGGLER_SOUND_REPLACE_TRAV_H_

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
#include <Sound/vjSoundManager.h>
#include <Kernel/vjDebug.h>


//XXX: Should be replaced by vjDEBUG filtering and levels
#define VJSOUNDTRAV_VERBOSE 1

//: This is a Performer traverser that will replace any node ending in mKeyName
//  in your scene graph with a pjSoundNode.
//
//  For example, if mKeyName == _Sound_, and your nodename == Gong_Sound_
//  then that node will be replaced with a new pjSoundNode of a sound
//  called "Gong"
//
// REQUIRED!!! You must call pjSoundReplaceTrav::preForkInit() in your juggler app's
//              preForkInit() member function.  Your app is likely to crash otherwise!!
class pjSoundReplaceTrav
{
public:
   // This function must be called pjSoundReplaceTrav::preForkInit() in your juggler app's
   // preForkInit() member function.  Your app is likely to crash otherwise!!
   static void preForkInit()
   {
      vjDEBUG(vjDBG_ALL,1)<<"[pjSoundReplaceTrav] pjSoundReplaceTrav::preForkInit(): Initializing sound node type with performer\n"<<vjDEBUG_FLUSH;
      pjSoundNode::init();
   }

   static void traverse( pfNode* node, const std::string& keyName = "_Sound_" )
   {
      //vjSoundManager::instance().engine()->setPosition( 0.0f, 0.0f, 0.0f );
      vjDEBUG(vjDBG_ALL,1)<<clrOutNORM(clrGREEN,"pjSoundReplaceTrav: ") <<"Checking graph for soundnodes (nodes with the "<<keyName.c_str()<<" extension...\n"<<vjDEBUG_FLUSH;

      // use the performer traversal mechanism
       pfuTraverser trav;
       pfuInitTraverser( &trav );

       trav.preFunc = pjSoundReplaceTrav::processNode;
       trav.data = const_cast<std::string*>( &keyName );

       pfuTraverse( node, &trav );
   }

   static void on( pfNode* node )
   {
      //vjSoundManager::instance().engine()->setPosition( 0.0f, 0.0f, 0.0f );
      vjDEBUG(vjDBG_ALL,1)<<clrOutNORM(clrGREEN,"pjSoundReplaceTrav: [soundOn]")<<" Turning sounds on in subgraph.\n"<<vjDEBUG_FLUSH;

      // use the performer traversal mechanism
       pfuTraverser trav;
       pfuInitTraverser( &trav );

       trav.preFunc = pjSoundReplaceTrav::turnSoundNodesOn;
       trav.data = NULL;

       pfuTraverse( node, &trav );
   }

   static void off( pfNode* node )
   {
      //vjSoundManager::instance().engine()->setPosition( 0.0f, 0.0f, 0.0f );
      vjDEBUG(vjDBG_ALL,1)<< clrOutNORM(clrGREEN,"pjSoundReplaceTrav: [soundOff]") <<" Turning sounds off in subgraph.\n"<<vjDEBUG_FLUSH;

      // use the performer traversal mechanism
       pfuTraverser trav;
       pfuInitTraverser( &trav );

       trav.preFunc = pjSoundReplaceTrav::turnSoundNodesOff;
       trav.data = NULL;

       pfuTraverse( node, &trav );
   }


protected:
   static int turnSoundNodesOn( pfuTraverser* trav )
   {
      pfNode* currentNode = trav->node;
      if (currentNode->isOfType( pjSoundNode::getClassType() ))
      {
         pjSoundNode* soundNode = static_cast<pjSoundNode*>( currentNode );
         soundNode->sound().trigger();
         vjDEBUG(vjDBG_ALL,0)<<clrOutNORM(clrYELLOW,"[SoundOn] ")<<"Setting the "<<soundNode->sound().getName()<<" sound node to on\n"<<vjDEBUG_FLUSH;
      }
      else
      {
         //cout<<"Not a sound Node: "<<currentNode->getName()<<" classtype="<<currentNode->getClassType()<<"!="<<pjSoundNode::getClassType()<<"\n"<<flush;
      }

      return PFTRAV_CONT;      // Return continue
   }

   static int turnSoundNodesOff( pfuTraverser* trav )
   {
      pfNode* currentNode = trav->node;
      if (currentNode->isOfType( pjSoundNode::getClassType() ))
      {
         pjSoundNode* soundNode = static_cast<pjSoundNode*>( currentNode );
         soundNode->sound().stop();
         vjDEBUG(vjDBG_ALL,0)<<clrOutNORM(clrYELLOW,"[SoundOff] ")<<"Setting the "<<soundNode->sound().getName()<<" sound node to off\n"<<vjDEBUG_FLUSH;
      }
      else
      {
         //cout<<"Not a sound Node: "<<currentNode->getName()<<" classtype="<<currentNode->getClassType()<<"!="<<pjSoundNode::getClassType()<<"\n"<<flush;
      }

      return PFTRAV_CONT;      // Return continue
   }

   // used to traverse - don't call
   static int processNode( pfuTraverser* trav )
   {
      // get the keyname, usually it is "_Sound_"
      std::string keyName = *reinterpret_cast<std::string*>(trav->data);
      pfNode* currentNode = trav->node;
      if (currentNode == NULL)
      {
         return PFTRAV_CONT;      // Return continue
      }

      std::string nodeName = currentNode->getName();
      if (nodeName.size() <= keyName.size())
      {
         // name was too short to even be a match, discard it.
         return PFTRAV_CONT;      // Return continue
      }

      // for verbose output (outputs every node's name.)
      //#ifdef VJSOUNDTRAV_VERBOSE
      vjDEBUG(vjDBG_ALL,4)<<"[SoundReplacer] Examining node in graph named \""<<nodeName.c_str()<<"\":\n"<<vjDEBUG_FLUSH;
      //#endif

      int startOfKeyWord = nodeName.size() - keyName.size();
      int endOfKeyWord = nodeName.size() - 1;
      std::string isThisOurKeyWord = nodeName.substr( startOfKeyWord, endOfKeyWord );
      if (isThisOurKeyWord == keyName)   // If node is not a Geode
      {
//#ifndef VJSOUNDTRAV_VERBOSE
         vjDEBUG(vjDBG_ALL,4)<<"[SoundReplacer] Examining node in graph named \""<<nodeName.c_str()<<"\":\n"<<vjDEBUG_FLUSH;
//#endif
         vjDEBUG(vjDBG_ALL,4)<<"[SoundReplacer]     Substring "<<keyName<<" matched, "<<vjDEBUG_FLUSH;
         pfGroup* parent = currentNode->getParent( 0 ); // FIXME?? will 0 work for all cases (instanced nodes?)
         if (parent != NULL)
         {
            std::string soundName = nodeName.substr( 0, startOfKeyWord );
            vjDEBUG_CONT(vjDBG_ALL,2)<<"extracted sound named \""<<soundName<<"\"\n"<<vjDEBUG_FLUSH;
            vjSound* sound = vjSoundManager::instance()->getHandle( soundName.c_str() );

            // replace the found node with a sound node.
            // note: only replace if the sound is valid.
            //        this way, the users can see where the sounds are.
            if (sound != NULL)
            {
               parent->removeChild( currentNode );
               bool positional_sound_true( true );
               pjSoundNode* sn = new pjSoundNode( sound, positional_sound_true );
               int resuln = sn->setName( nodeName.c_str() );
               if (resuln == 0)
               {
                  std::string newName = nodeName;
                  newName += "__sound";
                  sn->setName( newName.c_str() );
               }
               parent->addChild( sn );
               sound->trigger();
               vjDEBUG(vjDBG_ALL,1)<<clrOutNORM(clrGREEN,"[SoundReplacer]     ")<<"Replaced "<<clrOutNORM(clrGREEN, sn->getName())<<" node with a pjSoundNode referencing the "<<soundName<<" sound."<<vjDEBUG_FLUSH;
            }
            else
            {
               vjDEBUG(vjDBG_ALL,0)<<clrOutNORM(clrRED,"[SoundReplacer] !!! WARNING !!! ") <<"SOUND NOT FOUND: "<<soundName.c_str()<<"\n"<<vjDEBUG_FLUSH;
               vjDEBUG(vjDBG_ALL,0)<<clrOutNORM(clrRED,"[SoundReplacer] !!!         !!! ") <<"You need to enter \""<<soundName.c_str()<<"\" into your sound config file(s)\n"<<vjDEBUG_FLUSH;
               return PFTRAV_CONT;      // Return continue
            }
         }
         else
         {
            vjDEBUG_CONT(vjDBG_ALL,0)<<"but Parent is NULL (nowhere to hang the pjSoundNode!)\n"<<vjDEBUG_FLUSH;
         }
      }
      // for very verbose output...
#ifdef VJSOUNDTRAV_VERBOSE
            else
      {
         vjDEBUG(vjDBG_ALL,4)<<"[SoundReplacer]     Substring not matched: \""<<isThisOurKeyWord.c_str()<<"\" != \""<<keyName<<"\"\n"<<vjDEBUG_FLUSH;
      }
#endif
      return PFTRAV_CONT;      // Return continue
   }
};

#endif   /* _PERFORMER_JUGGLER_SOUND_REPLACE_TRAV_H_ */
