/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.config.event;

import java.util.EventObject;

import org.vrjuggler.jccl.config.ConfigElement;


/**
 * The event type fired by instances of
 * <code>org.vrjuggler.jccl.config.ConfigElement</code> when the name or a
 * property of the source config elment changes in some way.
 *
 * @see org.vrjuggler.jccl.config.ConfigElement
 * @see ConfigElementListener
 */
public class ConfigElementEvent
   extends EventObject
{
   /**
    * Creates a new ConfigElement event from the given source eelemnt relating
    * to the given property, the index into that property, and the value of
    * such property.  For cases of config element name changes, property value
    * removals, and property value chagnes, <code>value</code> represents the 
    * old value.  For property value additions, <code>value</code> is a
    * reference to the newly added value.
    */
   public ConfigElementEvent(ConfigElement srcElt, String property, int index,
                             Object value)
   {
      super(srcElt);
      this.property = property;
      this.index = index;
      this.value = value;
   }

   /**
    * Creates a new ConfigElement event from the given source element
    * relating to a change in the ordering of the values of the named
    * property.  The "index0" and "index1" values do not have to be the
    * start and end of the range of reordered values within the property.
    *
    * @param srcElt     the element containing the property whose values were
    *                   reordered
    * @param property   the name of the property whose values were reordered
    * @param index0     the "index0" of the range of values that were chagend
    * @param index1     the "index1" of the range of values that were chagend
    *
    * @see #getIndex0()
    * @see #getIndex1()
    *
    * @since 0.92.11
    */
   public ConfigElementEvent(ConfigElement srcElt, String property,
                             int index0, int index1)
   {
      super(srcElt);
      this.property = property;
      this.index0   = index0;
      this.index1   = index1;
   }

   /**
    * Returns the name of the property in the source config element that
    * changed or null if it was the name of the source config element that
    * changed.
    */
   public String getProperty()
   {
      return property;
   }

   /**
    * Returns the value index (0 or greater) of the property that changed.  If
    * 0 is returned, this may be an indication that it was the name of the
    * source config element that changed.
    *
    * @see #getProperty()
    */
   public int getIndex()
   {
      return index;
   }

   /**
    * Returns the "index0" of the range of property values that were
    * reordered as a result of changing the index of a property value.  If
    * this event is not related to changing the index of a property value,
    * then -1 is returned.  The returned value is not necessarily the start
    * of the range of changed values, but together with the "index1" value,
    * the range can be determined.
    *
    * @see #getIndex1()
    *
    * @since 0.92.11
    */
   public int getIndex0()
   {
      return index0;
   }

   /**
    * Returns the "index1" of the range of property values that were
    * reordered as a result of changing the index of a property value.  If
    * this event is not related to changing the index of a property value,
    * then -1 is returned.  The returned value is not necessarily the start
    * of the range of changed values, but together with the "index0" value,
    * the range can be determined.
    *
    * @see #getIndex0()
    *
    * @since 0.92.11
    */
   public int getIndex1()
   {
      return index1;
   }

   /**
    * Gets the value associated with this event.  For the case of config
    * element name changes, the old name of the config element is returned.
    * For the cases of property value removals and property value changes, a
    * reference to the old value is returned.  For property value additions, a
    * reference to the newly added value is returned.
    */
   public Object getValue()
   {
      return value;
   }

   private String property;
   private int index  = -1;
   private int index0 = -1;
   private int index1 = -1;
   private Object value;
}
