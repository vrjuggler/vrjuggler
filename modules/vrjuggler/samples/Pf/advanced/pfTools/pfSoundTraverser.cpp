/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <string>
#include <Performer/pr.h>
#include <Performer/pf.h>
#include <Performer/pfutil.h>
#include <Performer/pf/pfGeode.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pr/pfMemory.h>
#include <Performer/pf/pfSwitch.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <vrj/Util/Debug.h>

#include <pfSoundNode.h>
#include <pfSoundTraverser.h>


void pfSoundTraverser::preForkInit()
{
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << "[pfSoundTraverser] pfSoundTraverser::preForkInit(): Initializing sound node class type with Performer\n"
      << vprDEBUG_FLUSH;
   pfSoundNode::init();
}

void pfSoundTraverser::replace(pfNode* node, const std::string& keyName)
{
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << clrOutNORM(clrGREEN, "pfSoundTraverser: ")
      << "Checking graph for soundnodes (nodes with the "
      << keyName << " extension...\n" << vprDEBUG_FLUSH;

   // use the performer traversal mechanism
   pfuTraverser trav;
   pfuInitTraverser(&trav);

   trav.preFunc = pfSoundTraverser::nodesReplaceWithSound;
   trav.data = const_cast<std::string*>(&keyName);

   pfuTraverse(node, &trav);
}

void pfSoundTraverser::enable(pfNode* node)
{
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << clrOutNORM(clrGREEN,"pfSoundTraverser: [enable]")
      << " Enabling sounds in subgraph.\n"
      << vprDEBUG_FLUSH;

   // use the performer traversal mechanism
   pfuTraverser trav;
   pfuInitTraverser(&trav);

   trav.preFunc = pfSoundTraverser::soundNodesEnable;
   trav.data = NULL;

   pfuTraverse(node, &trav);
}

void pfSoundTraverser::disable(pfNode* node)
{
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << clrOutNORM(clrGREEN, "pfSoundTraverser: [disable]")
      << " Disabling sounds in subgraph.\n" << vprDEBUG_FLUSH;

   // use the performer traversal mechanism
   pfuTraverser trav;
   pfuInitTraverser(&trav);

   trav.preFunc = pfSoundTraverser::soundNodesDisable;
   trav.data = NULL;

   pfuTraverse(node, &trav);
}

void pfSoundTraverser::trigger(pfNode* node)
{
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << clrOutNORM(clrGREEN, "pfSoundTraverser: [soundOn]")
      << " Triggering sounds in subgraph.\n" << vprDEBUG_FLUSH;

   // use the performer traversal mechanism
   pfuTraverser trav;
   pfuInitTraverser(&trav);

   trav.preFunc = pfSoundTraverser::soundNodesTrigger;
   trav.data = NULL;

   pfuTraverse(node, &trav);
}

void pfSoundTraverser::stop(pfNode* node)
{
   vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
      << clrOutNORM(clrGREEN,"pfSoundTraverser: [soundOff]")
      << " Stopping sounds in subgraph.\n" << vprDEBUG_FLUSH;

   // use the performer traversal mechanism
   pfuTraverser trav;
   pfuInitTraverser(&trav);

   trav.preFunc = pfSoundTraverser::soundNodesStop;
   trav.data = NULL;

   pfuTraverse(node, &trav);
}

int pfSoundTraverser::soundNodesTrigger(pfuTraverser* trav)
{
   pfNode* currentNode = trav->node;
   if (currentNode->isOfType(pfSoundNode::getClassType()))
   {
      pfSoundNode* soundNode = static_cast<pfSoundNode*>(currentNode);

      // get the name
      const char* nodeNameChar = soundNode->getName();
      std::string nodeName;
      if (nodeNameChar == NULL)
      {
         nodeName = "";
      }
      else
      {
         nodeName = nodeNameChar;
      }

      // unpause the sound, unmute it too
      snx::sonix::instance()->trigger(nodeName);
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << clrOutNORM(clrYELLOW, "[SoundTrigger] ")
         << "Setting the " << nodeName << " sound to <triggered>\n"
         << vprDEBUG_FLUSH;
   }

   return PFTRAV_CONT;      // Return continue
}

int pfSoundTraverser::soundNodesStop(pfuTraverser* trav)
{
   pfNode* currentNode = trav->node;
   if (currentNode->isOfType(pfSoundNode::getClassType()))
   {
      pfSoundNode* soundNode = static_cast<pfSoundNode*>(currentNode);

      // get the name
      const char* nodeNameChar = soundNode->getName();
      std::string nodeName;
      if (nodeNameChar == NULL)
      {
         nodeName = "";
      }
      else
      {
         nodeName = nodeNameChar;
      }

      // unpause the sound, unmute it too
      snx::sonix::instance()->stop(nodeName);

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << clrOutNORM(clrYELLOW,"[SoundStop] ")
         << "Setting the " << nodeName << " sound to <stopped>\n"
         << vprDEBUG_FLUSH;
   }

   return PFTRAV_CONT;      // Return continue
}

int pfSoundTraverser::soundNodesEnable(pfuTraverser* trav)
{
   pfNode* currentNode = trav->node;
   if (currentNode->isOfType(pfSoundNode::getClassType()))
   {
      pfSoundNode* soundNode = static_cast<pfSoundNode*>(currentNode);

      // get the name
      const char* nodeNameChar = soundNode->getName();
      std::string nodeName;
      if (nodeNameChar == NULL)
      {
         nodeName = "";
      }
      else
      {
         nodeName = nodeNameChar;
      }

      // unpause the sound, unmute it too
      snx::sonix::instance()->unpause(nodeName);

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << clrOutNORM(clrYELLOW,"[SoundEnable] ")
         << "Setting the " << nodeName << " sound to <enabled>\n"
         << vprDEBUG_FLUSH;
   }

   return PFTRAV_CONT;      // Return continue
}

int pfSoundTraverser::soundNodesDisable(pfuTraverser* trav)
{
   pfNode* currentNode = trav->node;
   if (currentNode->isOfType(pfSoundNode::getClassType()))
   {
      pfSoundNode* soundNode = static_cast<pfSoundNode*>(currentNode);

      // get the name
      const char* nodeNameChar = soundNode->getName();
      std::string nodeName;
      if (nodeNameChar == NULL)
      {
         nodeName = "";
      }
      else
      {
         nodeName = nodeNameChar;
      }

      // pause the sound, mute it too in case it is triggered again
      snx::sonix::instance()->pause(nodeName);

      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << clrOutNORM(clrYELLOW,"[SoundDisable] ")
         << "Setting the " << nodeName << " sound to <disabled>\n"
         << vprDEBUG_FLUSH;
   }

   return PFTRAV_CONT;      // Return continue
}

int pfSoundTraverser::nodesReplaceWithSound(pfuTraverser* trav)
{
   // get the keyname, usually it is "_Sound_"
   std::string keyName = *reinterpret_cast<std::string*>(trav->data);
   pfNode* currentNode = trav->node;
   if (currentNode == NULL)
   {
      return PFTRAV_CONT;      // Return continue
   }

   const char* nodeNameChar = currentNode->getName();
   std::string nodeName;
   if (nodeNameChar == NULL)
   {
      nodeName = "";
   }
   else
   {
      nodeName = nodeNameChar;
   }

   if (nodeName.size() <= keyName.size())
   {
      // name was too short to even be a match, discard it.
      return PFTRAV_CONT;      // Return continue
   }

   // for verbose output (outputs every node's name.)
   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "[SoundReplacer] Examining node in graph named \""
      << nodeName << "\":\n" << vprDEBUG_FLUSH;

   int startOfKeyWord = nodeName.size() - keyName.size();
   int endOfKeyWord = nodeName.size() - 1;
   std::string isThisOurKeyWord = nodeName.substr(startOfKeyWord, endOfKeyWord);
   if (isThisOurKeyWord == keyName)   // If node is not a Geode
   {
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "[SoundReplacer] Found node in graph named \"" << nodeName
         << "\":\n" <<vprDEBUG_FLUSH;
      vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
         << "[SoundReplacer]     Substring " << keyName << " matched, "
         << vprDEBUG_FLUSH;
      pfGroup* parent = currentNode->getParent(0); // FIXME?? will 0 work for all cases (instanced nodes?)
      if (parent != NULL)
      {
         std::string soundName = nodeName.substr(0, startOfKeyWord);
         vprDEBUG_CONT(vprDBG_ALL, vprDBG_STATE_LVL)
            << "extracted sound named \"" << soundName << "\"\n"
            << vprDEBUG_FLUSH;

         // replace the found node with a sound node.
         // note: only replace if the sound is valid.
         //        this way, the users can see where the sounds are (in case
         //        the geoset is a sphere)
         if (soundName != "")
         {
            parent->removeChild(currentNode);
            bool positional_sound_true(true);
            pfSoundNode* sn = new pfSoundNode(soundName, positional_sound_true);
            int resuln = sn->setName(nodeName.c_str());
            if (resuln == 0)
            {
               std::string newName = nodeName;
               newName += "__sound";
               sn->setName(newName.c_str());
            }
            parent->addChild(sn);
            snx::sonix::instance()->trigger(soundName);

            vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_LVL)
                << clrOutNORM(clrGREEN, "[SoundReplacer]     ")
                << "Replaced " << clrOutNORM(clrGREEN, sn->getName())
                << " node with a pfSoundNode referencing the "
                << soundName << " sound.\n" << vprDEBUG_FLUSH;
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               <<clrOutNORM(clrRED, "[SoundReplacer] !!! WARNING !!! ")
               << "SOUND NOT FOUND: " << soundName << "\n" << vprDEBUG_FLUSH;
            vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
               << clrOutNORM(clrRED, "[SoundReplacer] !!!         !!! ")
               << "You need to enter \"" << soundName
               << "\" into your sound config file(s)\n" << vprDEBUG_FLUSH;
            return PFTRAV_CONT;      // Return continue
         }
      }
      else
      {
         vprDEBUG_CONT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "but Parent is NULL (nowhere to hang the pfSoundNode!)\n"
            << vprDEBUG_FLUSH;
      }
   }
   // for very verbose output...
   else
   {
      vprDEBUG(vprDBG_ALL,vprDBG_STATE_LVL)
         << "[SoundReplacer]     Substring not matched: \""
         << isThisOurKeyWord << "\" != \"" << keyName << "\"\n"
         << vprDEBUG_FLUSH;
   }

   return PFTRAV_CONT;      // Return continue
}
