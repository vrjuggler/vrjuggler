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

#include <jccl/jcclConfig.h>
#include <jccl/Util/Debug.h>
#include <jccl/Config/ChunkFactory.h>


namespace jccl
{

   ChunkFactory::ChunkFactory()
   {
      // Create global context
      mGlobalContext = cppdom::ContextPtr(new cppdom::Context);
   }

   //: Adds descriptions in file 'file_name' to the factory
   bool ChunkFactory::loadDescs(const std::string& file_name,
                                const std::string& parentFile)
   {
      bool retval = mDescDB.load(demangleFileName(file_name, parentFile));
      if ( retval )
      {
         vprDEBUG(jcclDBG_CONFIG,vprDBG_CONFIG_LVL)
            << "Loaded ChunkDesc file: '" << file_name.c_str() << "'.\n"
            << vprDEBUG_FLUSH;
      }
      else
      {
         vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
            << "Failed to load ChunkDesc file: '" << file_name.c_str()
            << "'.\n" << vprDEBUG_FLUSH;
      }
      return retval;
   }

   //: Creates a Chunk using the given description
   ConfigChunkPtr ChunkFactory::createChunk(ChunkDescPtr d)
   {
      if ( d.get() != NULL )
      {
         d->assertValid();
         return ConfigChunkPtr(new ConfigChunk(d));
      }
      else
      {
         return ConfigChunkPtr();
      }
   }

   vprSingletonImp(ChunkFactory);
} // namespace jccl
