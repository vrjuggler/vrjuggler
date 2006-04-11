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

public class ChunkDescEvent
   extends EventObject
{
   /**
    * Creates a new ChunkDesc event from the given source object for a property
    * of the desc that is not a property desc (i.e. name, token, ...).
    *
    * @param src     the object that fired this event
    * @param value   the value of the property in question
    * @param index   the index of the property if it is indexed in a list.
    *                Otherwise, set this to 0.
    */
   public ChunkDescEvent(Object src, Object value, int index)
   {
      this(src, null, index, value);
   }

   /**
    * Creates a new ChunkDesc event from the given source object for the given
    * property desc contained therein.
    */
   public ChunkDescEvent(Object src, PropertyDesc propDesc)
   {
      this(src, propDesc, 0, null);
   }

   /**
    * Creates a new ChunkDesc event from the given source object relating to
    * the given property desc, the index into that property desc, and the value
    * of such property desc.
    */
   public ChunkDescEvent(Object src, PropertyDesc propDesc, int index, Object value)
   {
      super(src);
      this.mPropertyDesc = propDesc;
      this.mIndex = index;
      this.mValue = value;
   }

   /**
    * Gets the property desc that is associated with this event (if there is
    * one).
    *
    * @return  the property desc associated with the event; null if there is no
    *          property desc associated with the event.
    */
   public PropertyDesc getPropertyDesc()
   {
      return mPropertyDesc;
   }

   /**
    * Gets the index that is associated with this event.
    */
   public int getIndex()
   {
      return mIndex;
   }

   /**
    * Gets the value that is associated with this event.
    */
   public Object getValue()
   {
      return mValue;
   }

   private PropertyDesc mPropertyDesc;
   private int mIndex;
   private Object mValue;
}
