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

#ifndef _JCCL_CHUNKFACTORY_H_
#define _JCCL_CHUNKFACTORY_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkDesc.h>
#include <jccl/Config/ChunkDescDB.h>
#include <vpr/Util/Singleton.h>

#include <cppdom/cppdom.h>

namespace jccl
{

   /** Factory for chunks and place holder for some other system wide factory functions
   */
   class JCCL_CLASS_API ChunkFactory
   {
   public:
      //: Adds descriptions in _descdb to the factory
      /*
      void addDescs (ChunkDescDB* _descdb)
      {
         descdb.insert (_descdb);
      }
      */

      /** Adds descriptions in file 'filename' to the factory */
      bool loadDescs(const std::string& filename, const std::string& parentFile = "");

      // we actually do need this so that the EM can send the descdb to the gui...
      ChunkDescDB* getChunkDescDB()
      {
         return &mDescDB;
      }

      ChunkDescPtr getChunkDesc(const std::string& token)
      {
         return mDescDB.get(token);
      }

      /** Creates a Chunk using the named description */
      ConfigChunkPtr createChunk(const std::string& desctoken)
      {
         return createChunk(mDescDB.get(desctoken) );
      }

      //: Creates a Chunk using the given description
      ConfigChunkPtr createChunk(ChunkDescPtr d);

      /** Get the global XML context that we are using system-wide */
      cppdom::ContextPtr getXMLContext()
      {
         vprASSERT(mGlobalContext.get() != NULL);
         return mGlobalContext;
      }

      /** Creates a new (empty) XML node using global context */
      cppdom::NodePtr createXMLNode()
      {
         return cppdom::NodePtr(new cppdom::Node(getXMLContext()));
      }

      /** Creates a new (empty) XML document using global context */
      cppdom::DocumentPtr createXMLDocument()
      {
         return cppdom::DocumentPtr(new cppdom::Document(getXMLContext()));
      }

   private:
      ChunkFactory();

      ChunkDescDB             mDescDB;
      cppdom::ContextPtr      mGlobalContext;      /**< The global context to use for jccl */

      vprSingletonHeader(ChunkFactory);
   };

} // namespace jccl

#endif
