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

#include <Sound/vjSoundFactory.h>
#include <Sound/vjSoundEngineConstructor.h>


//std::vector< vjSoundEngineConstructorBase* > vjSoundFactory::mConstructors;
//vjSoundFactory* vjSoundFactory::_instance = NULL;   //: The singleton instance ptr
vjSingletonImp(vjSoundFactory);

vjSoundFactory::vjSoundFactory()
{
}

// register a creator to the factory.
// this is intended to be used by each api's registration method

void vjSoundFactory::registerConstructor( vjSoundEngineConstructorBase* constructor )
{
   vjASSERT(constructor != NULL);
   mConstructors.push_back(constructor);     // Add the constructor to the list
   std::cout << "vjSoundFactory::registerConstructor: Sound engine registered for: "
	     << constructor->getChunkType().c_str()
	     << "   :" << (void*)constructor
	     << " type:" << typeid(*constructor).name() << std::endl;
}

bool vjSoundFactory::recognizeEngine(vjConfigChunk* chunk)
{
	if(vjSoundFactory::findConstructor(chunk) == -1)
      return false;
   else
      return true;
}


vjSoundEngine* vjSoundFactory::loadEngine(vjConfigChunk* chunk)
{
	vjASSERT(recognizeEngine(chunk));

   int index = findConstructor(chunk);

   vjSoundEngine* new_engine;
   vjSoundEngineConstructorBase* constructor = mConstructors[index];

   vjDEBUG(vjDBG_INPUT_MGR,3) << "vjSoundFactory::loadEngine: Loading device: "
              << chunk->getType() << "  with: "
              << typeid(*constructor).name() << std::endl << vjDEBUG_FLUSH;

   new_engine = constructor->createEngine(chunk);
   return new_engine;
}

int vjSoundFactory::findConstructor(vjConfigChunk* chunk)
{
   std::string chunk_type;
   chunk_type = (std::string)chunk->getType();
   
   for (unsigned int i = 0; i < mConstructors.size(); i++)
   {
      // Get next constructor
      vjSoundEngineConstructorBase* construct = mConstructors[i];
      vjASSERT(construct != NULL);

      if(construct->getChunkType() == chunk_type)
         return i;
   }

   return -1;
}
