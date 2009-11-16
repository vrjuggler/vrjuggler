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

import java.beans.*;
import java.util.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;

/**
 * An editor that knows how to edit enumerated values in a configuration element.
 */
public class EnumeratedPropertyValueEditor
   extends PropertyEditorSupport
{
   /**
    * Gets the property value as text.
    */
   public String getAsText()
   {
      Map enums = mPropertyDef.getEnums();
      for (Iterator itr = enums.keySet().iterator(); itr.hasNext(); )
      {
         String label = (String)itr.next();
         if (enums.get(label).equals(mValue))
         {
            return label;
         }
      }

      // If execution reached this point, mValue is not in the enumerated
      // values.  If the enumeration for this property is editable, however,
      // then mValue is not required to be in that list, so we can just
      // return it as-is.
      if ( mPropertyDef.enumIsEditable() )
      {
         return (String) mValue;
      }

      return null;
   }

   /**
    * Sets the property value by parsing the given String.
    *
    * @param text    any valid string
    *
    * @throws IllegalArgumentException
    *             if the string is null
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
         // If the enumeration is not editable, then we have to verify that
         // the given value is allowed.
         if ( ! mPropertyDef.enumIsEditable() )
         {
            Map enums = mPropertyDef.getEnums();

            if ( ! enums.containsKey(text) )
            {
               // If we got here, we didn't get a valid string value
               throw new IllegalArgumentException(text);
            }

            setValue(enums.get(text));
         }
         else
         {
            setValue(text);
         }
      }
   }

   /**
    * Gets a list of known values that this property may be.
    */
   public String[] getTags()
   {
      List tags = new ArrayList(mPropertyDef.getEnums().keySet());
      Collections.sort(tags);
      return (String[])tags.toArray(new String[] {});
   }

   /**
    * Sets up the tags list based on the given definition of the property.
    */
   public void setPropertyDefinition(PropertyDefinition propDef)
   {
      mPropertyDef = propDef;
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
      mValue = value;
      firePropertyChange();
   }

   /**
    * The value of the property being edited.
    */
   private Object mValue;

   /**
    * The property definition for the value being edited.
    */
   private PropertyDefinition mPropertyDef;
}
