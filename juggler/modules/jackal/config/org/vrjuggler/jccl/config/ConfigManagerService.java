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

import java.io.*;
import java.util.*;
import org.vrjuggler.tweek.beans.BeanRegistry;
import org.vrjuggler.tweek.services.EnvironmentService;

/**
 * Service bean for Tweek that manages loaded ConfigChunk and ChunkDesc DBs.
 */
public class ConfigManagerService
{
   /**
    * Creates a new ConfigManager service. This will try to load the VRJuggler
    * chunk desc file by default upon initialization.
    *
    * <b>TODO: This is the wrong place to be auto-loading the VRJuggler chunk
    * desc file. Look to put this elsewhere.</b>
    */
   public ConfigManagerService()
   {
      // Attempt to load the VRJuggler chunk description file
      ConfigService config_service = getConfigService();
      try
      {
         String desc_filename = expandEnvVars("${VJ_BASE_DIR}/share/vrjuggler/data/vrj-chunks.desc");
         File desc_file = new File(desc_filename);
         if (desc_file.exists() && desc_file.canRead())
         {
            // File exists and is readable. Lets see if we can load it.
            System.out.println("Trying to load "+desc_filename);
            InputStream in = new BufferedInputStream(new FileInputStream(desc_file));
            ChunkDescDB desc_db = config_service.loadChunkDescs(in);

            // Register the DB with ourselves
            this.add(desc_db);
         }
      }
      catch (IOException ioe)
      {
         System.err.println("Failed to load base VRJuggler Chunk Descriptions: "+ioe.getMessage());
      }
   }

   /**
    * Registers the given ConfigChunk collection with this manager.
    */
   public void add(ConfigChunkDB db)
   {
      mChunkDBs.add(db);
   }

   /**
    * Registers the given ChunkDesc collection with this manager.
    */
   public void add(ChunkDescDB db)
   {
      mDescDBs.add(db);
   }

   /**
    * Removes the given ConfigChunk collection from this manager.
    */
   public void remove(ConfigChunkDB db)
   {
      mChunkDBs.remove(db);
   }

   /**
    * Removes the given ChunkDesc collection from this manager.
    */
   public void remove(ChunkDescDB db)
   {
      mDescDBs.remove(db);
   }

   /**
    * Sets the active configuration to the given configuration.
    *
    * @param chunkDB    the configuration to make active
    */
   public void setActiveConfig(ConfigChunkDB chunkDB)
   {
      activeConfig = chunkDB;
   }

   /**
    * Gets the currently active configuration.
    *
    * @return  the active config, null if no config is active
    */
   public ConfigChunkDB getActiveConfig()
   {
      return activeConfig;
   }

   /**
    * Gets a collection of all the ChunkDescs in all the ChunkDesc collections
    * registered with this manager.
    */
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
         List matches = ConfigUtilities.getChunksWithDescToken(db.getAll(), token);
         for (Iterator chk_itr = matches.iterator(); chk_itr.hasNext(); )
         {
            ConfigChunk chunk = (ConfigChunk)chk_itr.next();
            System.out.println("Found chunk "+chunk.getName());
            found.add(chunk);
//            found.add(chk_itr.next());
         }
      }
      return found;
   }

   /**
    * This helper tries to get the Config service.
    */
   private ConfigService getConfigService()
   {
      return (ConfigService)BeanRegistry.instance().getBean("Config").getBean();
   }

   /**
    * Expands environment variables in the given string using Tweek's
    * EnvironmentService bean.
    *
    * @return  a post-processed version of the string with the environment
    *          variables expanded
    */
   private String expandEnvVars(String str)
   {
      return EnvironmentService.expandEnvVars(str);
   }

   /**
    * The currently active configuration in this manager.
    */
   private ConfigChunkDB activeConfig = null;

   private List mChunkDBs = new ArrayList();
   private List mDescDBs = new ArrayList();
}
