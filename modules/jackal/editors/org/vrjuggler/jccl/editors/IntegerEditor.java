/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

package org.vrjuggler.jccl.editors;

import java.beans.*;

/**
 * An editor that knows how to edit Integer objects.
 */
public class IntegerEditor
   extends PropertyEditorSupport
{
   /**
    * Gets the property value as text.
    */
   public String getAsText()
   {
      return mValue.toString();
   }

   /**
    * Sets the property value to the given string.
    *
    * @param text    a valid floating point number in string form
    *
    * @throws IllegalArgumentException
    *             if text does not contain a valid number
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
         try
         {
            Integer num = new Integer(text);
            setValue(num);
         }
         catch (NumberFormatException nfe)
         {
            // Man ... don't people know what a number looks like?!
            throw new IllegalArgumentException(text + " is not an integer");
         }
      }
   }

   /**
    * Gets a list of known values that this property may be.
    */
   public String[] getTags()
   {
      return null;
   }

   /**
    * Gets the property value.
    */
   public Object getValue()
   {
      return mValue;
   }

   /**
    * Sets the object that is to be edited.
    */
   public void setValue(Object value)
   {
      this.mValue = (Integer)value;
      firePropertyChange();
   }

   /**
    * The Integer being edited.
    */
   private Integer mValue = null;
}
