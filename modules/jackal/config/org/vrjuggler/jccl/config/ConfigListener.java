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

import java.util.EventListener;

/**
 * Interface for an observer to receive notifications of additions and removals
 * of <code>ChunkDesc</code>s and <code>ConfigChunk</code>s from a
 * <code>ConfigBroker</code>.
 *
 * @see ConfigBroker
 * @see ChunkDesc
 * @see ConfigChunk
 */
public interface ConfigListener
   extends EventListener
{
   /**
    * Gives notification that a chunk desc was added to the broker.
    *
    * @param evt     the config event
    */
   public void chunkDescAdded(ConfigEvent evt);

   /**
    * Gives notification that a chunk desc was removed from the broker.
    *
    * @param evt     the config event
    */
   public void chunkDescRemoved(ConfigEvent evt);

   /**
    * Gives notification that a config chunk was added to the broker.
    *
    * @param evt     the config event
    */
   public void configChunkAdded(ConfigEvent evt);

   /**
    * Gives notification that a config chunk was removed from the broker.
    *
    * @param evt     the config event
    */
   public void configChunkRemoved(ConfigEvent evt);
}
