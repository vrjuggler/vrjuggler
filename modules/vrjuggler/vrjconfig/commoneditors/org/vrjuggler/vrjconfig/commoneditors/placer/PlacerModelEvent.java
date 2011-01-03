/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.commoneditors.placer;

import java.util.EventObject;

/**
 * Encapsulates information describing changes to a placer data model used to
 * notify placer model listeners of that change.
 */
public class PlacerModelEvent
   extends EventObject
{
   /**
    * The indicies of the items changed.
    */
   private int[] indicies;

   /**
    * The values of the items changed.
    */
   private Object[] values;

   /**
    * Creates a new event fired from the given source. This signifies that all
    * items in the model have changed.
    */
   public PlacerModelEvent(Object source)
   {
      this(source, null, null);
   }

   /**
    * Creates a new event fired from the given source that affects the items
    * at the given indices/values in the source.
    */
   public PlacerModelEvent(Object source, int[] indicies, Object[] values)
   {
      super(source);
      this.indicies = indicies;
      this.values = values;
   }

   /**
    * Gets the indicies of the items that have changed.
    */
   public int[] getIndicies()
   {
      return indicies;
   }

   /**
    * Gets the values of the items that have changed.
    */
   public Object[] getValues()
   {
      return values;
   }
}
