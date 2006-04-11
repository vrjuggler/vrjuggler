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

import java.util.EventObject;

/**
 * This provides detailed information to ConfigBroker observers as to how the
 * ConfigChunks and ChunkDescs therein have changed.
 */
public class ConfigEvent
   extends EventObject
{
   /**
    * Constructs a new ConfigEvent fired from the given source object
    * relating to the given resource and ChunkDesc.
    *
    * @param source     the source of the event
    * @param resource   the resource that changed
    * @param desc       the chunk desc affected by the change
    */
   public ConfigEvent(Object source, String resource, ChunkDesc desc)
   {
      super(source);
      this.resource = resource;
      this.desc = desc;
   }

   /**
    * Constructs a new ConfigEvent fired from the given source object
    * relating to the given resource and ConfigChunk.
    *
    * @param source     the source of the event
    * @param resource   the resource that changed
    * @param chunk      the config chunk affected by the change
    */
   public ConfigEvent(Object source, String resource, ConfigChunk chunk)
   {
      super(source);
      this.resource = resource;
      this.chunk = chunk;
   }

   /**
    * Gets the resource associated with this context event.
    */
   public String getResource()
   {
      return resource;
   }

   /**
    * Gets the ChunkDesc associated with this event if there was one.
    *
    * @return  the affected chunk desc; null if this event does not relate to a
    *          chunk desc
    */
   public ChunkDesc getChunkDesc()
   {
      return desc;
   }

   /**
    * Gets the ConfigChunk associated with this event if there was one.
    *
    * @return  the affected config chunk; null if this event does not relate to
    *          a config chunk
    */
   public ConfigChunk getConfigChunk()
   {
      return chunk;
   }

   /**
    * The resource associated with this event.
    */
   private String resource;

   /**
    * The ChunkDesc associated with this event.
    */
   private ChunkDesc desc;

   /**
    * The ConfigChunk associated with this event.
    */
   private ConfigChunk chunk;
}
