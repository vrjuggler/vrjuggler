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

/*
 * Event type for modifications to a chunkdb : insert, remove, and
 * replace chunks...
 */

package VjConfig;

import java.util.EventObject;
import VjConfig.ConfigChunk;

public class ChunkDBEvent
   extends EventObject
{
   private ConfigChunk mNewChunk;
   private ConfigChunk mOldChunk;

   int mAction;

   public final static int INSERT = 1;
   public final static int REMOVE = 2;
   public final static int REPLACE = 3;
   public final static int REMOVEALL = 4;

   public ChunkDBEvent(Object source, int action, ConfigChunk oldChunk,
                       ConfigChunk newChunk)
   {
      super(source);
      mAction = action;
      mNewChunk = newChunk;
      mOldChunk = oldChunk;
   }

   public int getAction()
   {
      return mAction;
   }

   public ConfigChunk getOldChunk()
   {
      return mOldChunk;
   }

   public ConfigChunk getNewChunk()
   {
      return mNewChunk;
   }
}



