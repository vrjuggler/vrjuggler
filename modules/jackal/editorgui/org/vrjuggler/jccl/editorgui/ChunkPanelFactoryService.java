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
package org.vrjuggler.jccl.editorgui;

import java.awt.Component;
import java.io.Serializable;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.editorgui.chunkdescui.*;
import org.vrjuggler.jccl.editorgui.configchunkui.*;

/**
 * Designed to be a Tweek service bean, this class is responsible for
 * instantiating editor panels for specific ConfigChunks and ChunkDescs.
 */
public class ChunkPanelFactoryService
   implements Serializable
{
   /**
    * Creates a new ChunkPanelFactoryService.
    */
   public ChunkPanelFactoryService()
   {
   }

   /**
    * Creates a new component which can edit the given ChunkDesc. In the
    * future, this may query the ChunkDesc for a custom editor.
    *
    * @return  a component serving as a ChunkDesc editor
    */
   public Component getEditor(ChunkDesc desc, ChunkDescDB descDB)
   {
      return new ChunkDescPanel(desc, descDB, null, true);
   }

   /**
    * Creates a new component which can edit the given ConfigChunk. In the
    * future, this may query the ConfigChunk for a custom editor.
    *
    * @return  a component serving as a ChunkDesc editor
    */
   public Component getEditor(ConfigChunk chunk, ConfigChunkDB chunkDB)
   {
      DefaultConfigChunkPanel pnl = new DefaultConfigChunkPanel();
      pnl.setChunk(chunk, chunkDB);
      return pnl;
   }
}
