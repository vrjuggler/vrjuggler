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

import org.vrjuggler.jccl.config.ValType;

/**
 * An editor that knows how to edit ValType objects.
 */
public class ValTypeEditor
   extends PropertyEditorSupport
{
   /**
    * Gets the property value as text.
    */
   public String getAsText()
   {
      if (value == ValType.STRING)
      {
         return "String";
      }
      else if (value == ValType.FLOAT)
      {
         return "Float";
      }
      else if (value == ValType.INT)
      {
         return "Integer";
      }
      else if (value == ValType.BOOL)
      {
         return "Boolean";
      }
      else if (value == ValType.CHUNK)
      {
         return "Chunk Pointer";
      }
      else if (value == ValType.EMBEDDEDCHUNK)
      {
         return "Embedded Chunk";
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
         setValue(ValType.STRING);
      }
      else if (text.equals("Float"))
      {
         setValue(ValType.FLOAT);
      }
      else if (text.equals("Integer"))
      {
         setValue(ValType.INT);
      }
      else if (text.equals("Boolean"))
      {
         setValue(ValType.BOOL);
      }
      else if (text.equals("Chunk Pointer"))
      {
         setValue(ValType.CHUNK);
      }
      else if (text.equals("Embedded Chunk"))
      {
         setValue(ValType.EMBEDDEDCHUNK);
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
                          , "Chunk Pointer"
                          , "Embedded Chunk" };
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
      this.value = (ValType)value;
      firePropertyChange();
   }

   /**
    * The ValType being edited.
    */
   private ValType value = null;
}
