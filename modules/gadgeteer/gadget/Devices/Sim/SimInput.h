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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_SIM_INPUT_H
#define _GADGET_SIM_INPUT_H
//#pragma once

#include <gadget/gadgetConfig.h>
#include <vector>
#include <boost/concept_check.hpp>
#include <jccl/Config/ConfigChunkPtr.h>
#include <gadget/Type/EventWindowInterface.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/IO/SerializableObject.h>


namespace gadget
{

/**
 * Base class for all simulated input devices.
 * Has the common routines for accessing and seting up keyboard interface.
 */
class SimInput : public vpr::SerializableObject
{
public:
   class KeyModPair
   {
   public:
      KeyModPair() : mKey(gadget::KEY_NONE), mModifier(gadget::KEY_NONE)
      {;}

      KeyModPair(gadget::Keys key, gadget::Keys modifier)
         : mKey(key), mModifier(modifier)
      {;}

      KeyModPair(const KeyModPair& in)
      {
         copy(in);
      }

      const KeyModPair& operator=(const KeyModPair& in)
      {
         copy(in);
         return *this;
      }

      void copy(const KeyModPair& in)
      {
         mKey      = in.mKey;
         mModifier = in.mModifier;
      }

   public:
      gadget::Keys mKey;
      gadget::Keys mModifier;
   };

public:
   SimInput()
   {
      //vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)<<"*** SimInput::SimInput()\n"<< vprDEBUG_FLUSH;
   }
   virtual ~SimInput() {}

   /**
    * Configures the simulated input device.
    * @post Event window proxy is configured.
    */
   virtual bool config(jccl::ConfigChunkPtr chunk);

   virtual std::string getBaseType()
   {
       return std::string("SimInput");
   }

   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer)
   {
      boost::ignore_unused_variable_warning(writer);
      return vpr::ReturnStatus::Succeed;
   }

   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader)
   {
      boost::ignore_unused_variable_warning(reader);
      return vpr::ReturnStatus::Succeed;
   }

protected:

   /**
    * Checks the given keypair.
    * @return Number of times the key was pressed with the modifier alone
    *         held down.
    */
   virtual int checkKeyPair(KeyModPair& pair);

   /**
    * Gets number of times the key was pressed.
    * @return Number of times the key was pressed.
    */
   virtual int checkKey(gadget::Keys keyId);

   /**
    * Constructs a vector of key mod pairs.
    * Takes as input a chunk that has a list of KeyModPair embeded chunks.
    *
    * @pre keyList must be full of var values containing chunks of the type
    *      "KeyModPair".  The KeyModPair chunk type must have fields name key
    *      and modKey.
    *
    * @return vector of KeyModPairs
    */
   std::vector<KeyModPair> readKeyList(std::vector<jccl::ConfigChunkPtr>& keyList);

protected:
   EventWindowInterface mEventWin; /**< The event window from which we receive events. */
};

} // End of gadget namespace


#endif
