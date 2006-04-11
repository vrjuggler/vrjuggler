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
package org.vrjuggler.jccl.config;

import java.util.*;
import org.jdom.Element;

/**
 * The <code>ChunkFactory</code> is responsible for the creation of new
 * <code>ConfigChunk</code>s from a specific set up <code>ChunkDesc</code>s.
 */
public class ChunkFactory
{
   /**
    * Sets the list of ChunkDescs this factory uses when it creates new
    * ConfigChunks.
    *
    * @param descs   the list of chunk descs this factory should use
    */
   public static void setDescs(List descs)
   {
      mDescs = descs;
   }

   /**
    * Gets the list of ChunkDescs this factory uses when it creates new
    * ConfigChunks.
    */
   public static List getDescs()
   {
      return mDescs;
   }

   /**
    * Adds the given ChunkDesc to this factory.
    *
    * @param desc    the ChunkDesc to add
    */
   public static void add(ChunkDesc desc)
   {
      mDescs.add(desc);
   }

   /**
    * Removes the given ChunkDesc from this factory.
    *
    * @param desc    the ChunkDesc to remove
    */
   public static void remove(ChunkDesc desc)
   {
      mDescs.remove(desc);
   }

   /**
    * Creates a new ConfigChunk using the ChunkDesc with the given token. If
    * this factory has more than one ChunkDesc identified by the given token,
    * the first one will be used.
    *
    * @param token   the unique token identifying the ChunkDesc to use when
    *                creating the ConfigChunk
    *
    * @return  the new ConfigChunk if successful; null if this factory doesn't
    *          know about a ChunkDesc with the given token
    */
   public static ConfigChunk createConfigChunk(String token)
   {
      ConfigChunk new_chunk = null;

      List matches = ConfigUtilities.getDescsWithToken(mDescs, token);
      if (matches.size() > 0)
      {
         ChunkDesc cd = (ChunkDesc)matches.get(0);
         new_chunk = new ConfigChunk(cd);
      }

      return new_chunk;
   }

   /**
    * Gets the ChunkDesc in this factory identified by the given token.
    *
    * @param token   the unique token identifying the ChunkDesc to retrieve
    *
    * @return  the ChunkDesc with the given token; null if this factory doesn't
    *          know about a ChunkDesc with the given token
    */
   protected static ChunkDesc getChunkDescByToken(String token)
   {
      List matches = ConfigUtilities.getDescsWithToken(mDescs, token);
      if (matches.size() > 0)
      {
         return (ChunkDesc)matches.get(0);
      }
      return null;
   }

   /**
    * The list of ChunkDescs this factory uses when trying to create a new
    * ConfigChunk.
    */
   private static List mDescs = new ArrayList();
}
