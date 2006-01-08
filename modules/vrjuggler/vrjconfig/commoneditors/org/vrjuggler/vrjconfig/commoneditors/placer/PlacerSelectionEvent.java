/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

package org.vrjuggler.vrjconfig.commoneditors.placer;

import java.util.EventObject;

/**
 * Encapsulates information describing changes to a placer's selection used to
 * notify placer selection listeners of that change.
 */
public class PlacerSelectionEvent
   extends EventObject
{
   /**
    * The index of the selected item.
    */
   private int index;

   /**
    * The value of the selected item.
    */
   private Object value;

   /**
    * Creates a new event fired from the given source that has the given index
    * and value selected.
    */
   public PlacerSelectionEvent(Object source, int index, Object value)
   {
      super(source);
      this.index = index;
      this.value = value;
   }

   /**
    * Gets the index of the item that is selected.
    *
    * @return  the selected index or -1 if nothing is selected
    */
   public int getIndex()
   {
      return index;
   }

   /**
    * Gets the value of the item that is selected.
    */
   public Object getValue()
   {
      return value;
   }
}
