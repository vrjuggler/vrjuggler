/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

/**
 * Service bean for Tweek that manages loaded ConfigChunk and ChunkDesc DBs.
 */
public class ConfigManagerService
{
   public void add(ConfigChunkDB db)
   {
      mChunkDBs.add(db);
   }

   public void add(ChunkDescDB db)
   {
      mDescDBs.add(db);
   }

   public ChunkDescDB getAllChunkDescs()
   {
      ChunkDescDB result = new ChunkDescDB();
      for (Iterator itr = mDescDBs.iterator(); itr.hasNext(); )
      {
         ChunkDescDB db = (ChunkDescDB)itr.next();
         result.addAll(db);
      }
      return result;
   }

   /**
    * Finds all ConfigChunks that are defined by the ChunkDesc with the given
    * unique token.
    *
    * @param token      the token of the ChunkDesc used to search with
    *
    * @return  a list of ConfigChunks found
    */
   public List findConfigChunksByToken(String token)
   {
      System.out.println("Finding CCs in "+this);
      List found = new ArrayList();

      // Go through each ConfigChunk database and look through it for
      // ConfigChunks defined by the ChunkDesc token given
      System.out.println("Examining "+mChunkDBs.size()+" ConfigChunkDBs for token "+token);
      for (Iterator db_itr = mChunkDBs.iterator(); db_itr.hasNext(); )
      {
         ConfigChunkDB db = (ConfigChunkDB)db_itr.next();
         for (Iterator chk_itr = db.getOfDescToken(token).iterator(); chk_itr.hasNext(); )
         {
            ConfigChunk chunk = (ConfigChunk)chk_itr.next();
            System.out.println("Found chunk "+chunk.getName());
            found.add(chunk);
//            found.add(chk_itr.next());
         }
      }
      return found;
   }

   private List mChunkDBs = new ArrayList();
   private List mDescDBs = new ArrayList();
}
