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
package org.vrjuggler.jccl.editors;

import java.beans.*;
import java.util.*;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.tweek.beans.BeanRegistry;

/**
 * An editor that knows how to edit ConfigChunk pointer properties in a
 * ConfigChunk.
 */
public class EnumeratedPropertyValueEditor
   extends PropertyEditorSupport
{
   /**
    * Gets the property value as text.
    */
   public String getAsText()
   {
      for (int i=0; i<propertyDesc.getNumEnums(); ++i)
      {
         DescEnum de = propertyDesc.getEnumAt(i);
         if (de.getValue().equals(this.value))
         {
            return de.getName();
         }
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
         for (int i=0; i<propertyDesc.getNumEnums(); ++i)
         {
            DescEnum de = propertyDesc.getEnumAt(i);
            if (text.equals(de.getName()))
            {
               setValue(de.getValue());
               return;
            }
         }

         // If we got here, we didn't get a valid string value
         throw new IllegalArgumentException(text);
      }
   }

   /**
    * Gets a list of known values that this property may be.
    */
   public String[] getTags()
   {
      String[] tags = new String[propertyDesc.getNumEnums()];
      for (int i=0; i<propertyDesc.getNumEnums(); ++i)
      {
         DescEnum de = propertyDesc.getEnumAt(i);
         tags[i] = de.getName();
      }
      return tags;
   }

   /**
    * Sets up the tags list based on the given description of the property.
    */
   public void setPropertyDesc(PropertyDesc desc)
   {
      this.propertyDesc = desc;
   }

   /**
    * Gets the property value.
    */
   public Object getValue()
   {
      return value;
   }

   /**
    * Sets the object that is to be edited.
    */
   public void setValue(Object value)
   {
      this.value = value;
      firePropertyChange();
   }

   /**
    * The value of the Chunk pointer being edited.
    */
   private Object value = null;

   /**
    * The list of tags supported by this value.
    */
   private PropertyDesc propertyDesc = null;
}
