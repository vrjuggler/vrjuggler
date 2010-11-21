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

package org.vrjuggler.jccl.config;

/**
 * Defines a particular property value by giving it a label and a default value.
 * It is used by PropertyDefinitions to define what values are required for a
 * particular property.
 *
 * @see PropertyDefinition
 */
public class PropertyValueDefinition
{
   /**
    * Creates a new property value definition with the given label and default
    * value.
    */
   public PropertyValueDefinition(String label, Object defaultValue)
   {
      mLabel = label;
      mDefaultValue = defaultValue;
   }

   /**
    * Creates a new property value definition with the given label and a null
    * default value.
    */
   public PropertyValueDefinition(String label)
   {
      this(label, null);
   }

   /**
    * Gets the label for this value.
    */
   public String getLabel()
   {
      return mLabel;
   }

   /**
    * Sets the label for this value.
    */
   public synchronized void setLabel(String label)
   {
      mLabel = label;
   }

   /**
    * Gets the default value of this value.
    */
   public Object getDefaultValue()
   {
      return mDefaultValue;
   }

   /**
    * Sets the default value of this value.
    */
   public synchronized void setDefaultValue(Object value)
   {
      mDefaultValue = value;
   }

   /**
    * Tests if this property value definition is equal to the given one.
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof PropertyValueDefinition)
      {
         PropertyValueDefinition d = (PropertyValueDefinition)obj;
         if (mLabel.equals(d.mLabel))
         {
            if (mDefaultValue != null && d.mDefaultValue != null )
            {
               return mDefaultValue.equals(d.mDefaultValue);
            }
            else
            {
               return true;
            }
         }
      }
      return false;
   }

   /** The label used for this value. */
   private String mLabel;

   /** The default value for this value. */
   private Object mDefaultValue;
}
