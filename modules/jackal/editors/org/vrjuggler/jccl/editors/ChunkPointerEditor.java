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
package org.vrjuggler.jccl.editors;

import java.beans.*;
import java.util.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;

/**
 * An editor that knows how to edit ConfigChunk pointer properties in a
 * ConfigChunk.
 */
public class ChunkPointerEditor
   extends PropertyEditorSupport
{
   /**
    * Gets the property value as text.
    */
   public String getAsText()
   {
      return value;
   }

   /**
    * Sets the property value by parsing the given String.
    *
    * @param text    any valid string
    *
    * @throws IllegalArgumentException
    *             if the string is null
    */
   public void setAsText(String text)
      throws IllegalArgumentException
   {
      if (text == null)
      {
         // Ack ... we got a bad string
         throw new IllegalArgumentException(text);
      }
      else
      {
         setValue(text);
      }
   }

   /**
    * Gets a list of known values that this property may be.
    */
   public String[] getTags()
   {
      return (String[])mTags.toArray(new String[0]);
   }

   /**
    * Sets up the tags list based on the given description of the property.
    */
   public void setPropertyDesc(PropertyDesc desc)
   {
      // For each ChunkDesc token this desc can point to, look for matching
      // ConfigChunks we can use.
      mTags.clear();
      for (Iterator at_itr = desc.getAllowedTypes().iterator(); at_itr.hasNext(); )
      {
         String type = (String)at_itr.next();

         List chunks = getChunksWithDescToken(type);

         // Grab the name of each matching ConfigChunk
         for (Iterator chk_itr = chunks.iterator(); chk_itr.hasNext(); )
         {
            ConfigChunk chunk = (ConfigChunk)chk_itr.next();
            mTags.add(chunk.getFullName());
         }
      }
   }

   /**
    * Gets a list of the config chunks in the current configuration that have
    * the description with the given token.
    *
    * XXX: This is a complete hack. We need a better way to propogate the
    * current configuration context all the way down to here.
    */
   private List getChunksWithDescToken(String token)
   {
      List matches = new ArrayList();
      List all_chunks = new ArrayList();

      ConfigBroker broker = new ConfigBrokerProxy();
      // XXX: ACK! We really should look only in the current context, not all resources
      for (Iterator itr = broker.getResourceNames().iterator(); itr.hasNext(); )
      {
         List chunks = broker.getChunksIn((String)itr.next());

         // Get the chunks embedded within the current chunk
         for (Iterator chk_itr = chunks.iterator(); chk_itr.hasNext(); )
         {
            ConfigChunk chunk = (ConfigChunk)chk_itr.next();
            all_chunks.add(chunk);
            all_chunks.addAll(getEmbeddedChunksRecursive(chunk));
         }

         // Cull out the chunks that use the given desc token
         matches.addAll(ConfigUtilities.getChunksWithDescToken(all_chunks, token));
      }

      // Sort alphabetically
      Collections.sort(matches, new Comparator()
      {
         public int compare(Object a, Object b)
         {
            if (a instanceof ConfigChunk && b instanceof ConfigChunk)
            {
               return ((ConfigChunk)a).getFullName().compareTo(((ConfigChunk)b).getFullName());
            }
            return 0;
         }
      });
      return matches;
   }

   /**
    * Gets all embedded chunks within the given chunk recursively.
    *
    * @param srcChunk      the chunk in which to retrieve embedded chunks
    */
   private List getEmbeddedChunksRecursive(ConfigChunk srcChunk)
   {
      List results = new ArrayList();

      List emb_chunks = srcChunk.getEmbeddedChunks();
      for (Iterator itr = emb_chunks.iterator(); itr.hasNext(); )
      {
         ConfigChunk chunk = (ConfigChunk)itr.next();
         results.add(chunk);

         // Look for embedded chunks within the current config chunk
         results.addAll(getEmbeddedChunksRecursive(chunk));
      }

      return results;
   }

   /**
    * Gets the property value.
    */
   public Object getValue()
   {
      return value;
   }

   /**
    * Sets the object that is to be edited.
    */
   public void setValue(Object value)
   {
      this.value = (String)value;
      firePropertyChange();
   }

   /**
    * The String name of the Chunk pointer being edited.
    */
   private String value = null;

   /**
    * The list of tags supported by this value.
    */
   private List mTags = new ArrayList();
}
