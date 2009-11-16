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
package org.vrjuggler.jccl.editors;

import java.beans.PropertyEditorSupport;
import org.vrjuggler.jccl.config.ConfigElement;
import org.vrjuggler.jccl.config.ConfigElementPointer;

/**
 * An editor that knows how to edit valuetypes for configuration definitions.
 */
public class ValuetypeEditor
   extends PropertyEditorSupport
{
   /**
    * Gets the property value as text.
    */
   public String getAsText()
   {
      if (mValue == String.class)
      {
         return "String";
      }
      else if (mValue == Float.class)
      {
         return "Float";
      }
      else if (mValue == Integer.class)
      {
         return "Integer";
      }
      else if (mValue == Boolean.class)
      {
         return "Boolean";
      }
      else if (mValue == ConfigElementPointer.class)
      {
         return "Element Pointer";
      }
      else if (mValue == ConfigElement.class)
      {
         return "Embedded Element";
      }
      return "Invalid";
   }

   /**
    * Sets the property value by parsing the given String.
    *
    * @param text    must be one of the provided Tags
    *
    * @throws IllegalArgumentException
    *             if the string is badly formatted
    */
   public void setAsText(String text)
      throws IllegalArgumentException
   {
      if (text.equals("String"))
      {
         setValue(String.class);
      }
      else if (text.equals("Float"))
      {
         setValue(Float.class);
      }
      else if (text.equals("Integer"))
      {
         setValue(Integer.class);
      }
      else if (text.equals("Boolean"))
      {
         setValue(Boolean.class);
      }
      else if (text.equals("Element Pointer"))
      {
         setValue(ConfigElementPointer.class);
      }
      else if (text.equals("Embedded Element"))
      {
         setValue(ConfigElement.class);
      }
      else
      {
         // Ack ... we got a bad string
         throw new IllegalArgumentException(text);
      }
   }

   /**
    * Gets a list of known values that this property may be.
    */
   public String[] getTags()
   {
      return new String[] { "String"
                          , "Float"
                          , "Integer"
                          , "Boolean"
                          , "Element Pointer"
                          , "Embedded Element" };
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
      mValue = (Class)value;
      firePropertyChange();
   }

   /**
    * The ValType being edited.
    */
   private Class mValue;
}
