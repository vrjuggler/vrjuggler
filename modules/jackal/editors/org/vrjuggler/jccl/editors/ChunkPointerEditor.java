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
      for (int i=0; i<desc.getNumEnums(); ++i)
      {
         DescEnum de = desc.getEnumAt(i);

         List chunks = getChunksWithDescToken(de.getName());

         // Grab the name of each matching ConfigChunk
         for (Iterator chk_itr = chunks.iterator(); chk_itr.hasNext(); )
         {
            ConfigChunk chunk = (ConfigChunk)chk_itr.next();
            mTags.add(chunk.getName());
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

      ConfigBroker broker = new ConfigBrokerProxy();
      // XXX: ACK! We really should look only in the current context, not all resources
      for (Iterator itr = broker.getResourceNames().iterator(); itr.hasNext(); )
      {
         List chunks = broker.getChunksIn((String)itr.next());
         matches.addAll(ConfigUtilities.getChunksWithDescToken(chunks, token));
      }

      Collections.sort(matches, new Comparator()
      {
         public int compare(Object a, Object b)
         {
            if (a instanceof ConfigChunk && b instanceof ConfigChunk)
            {
               return ((ConfigChunk)a).getName().compareTo(((ConfigChunk)b).getName());
            }
            return 0;
         }
      });
      return matches;
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
