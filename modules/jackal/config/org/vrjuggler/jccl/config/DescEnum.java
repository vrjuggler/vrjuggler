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

import java.beans.*;
import org.jdom.Element;
import org.jdom.output.XMLOutputter;

/** DescEnum -- an entry in the enum vector of a PropertyDesc */
public class DescEnum
   implements Cloneable
            , ConfigTokens
{
   /**
    * Creates a new, empty enumeration.
    */
   public DescEnum()
   {
      this("", ValType.STRING);
   }

   /**
    * Creates a new enumeration with the given name and type with a default
    * value.
    */
   public DescEnum(String name, ValType type)
   {
      this(name, type, ConfigUtilities.makeProperty("", type));
   }

   /**
    * Creates a new enumeration with the given name and value.
    */
   public DescEnum(String name, ValType type, Object value)
   {
      this(new Element(prop_enum_TOKEN), type);
      setName(name);
      setValue(value);
   }

   /**
    * Creates an enumeration object for the given DOM element.
    *
    * @param element    the DOM element containing the enumeration in question
    */
   public DescEnum(Element element, ValType type)
   {
      // Make sure we don't try to set a value for an embedded chunk
      if (type == ValType.EMBEDDEDCHUNK || type == ValType.CHUNK)
      {
         throw new IllegalArgumentException("Embedded chunks and chunk pointers may not have enums.");
      }

      mDomElement = element;
      changeSupport = new PropertyChangeSupport(this);
      mValType = type;
   }

   /**
    * Makes a deep copy of this enumeration.
    */
   public Object clone()
      throws CloneNotSupportedException
   {
      DescEnum de = (DescEnum)super.clone();
      de.mDomElement = (Element)mDomElement.clone();
      return de;
   }

   /**
    * Sets the name of this enumeration to the given value.
    *
    * @param name    the new name for this enumeration.
    */
   public void setName(String name)
   {
      String old = getName();
      mDomElement.setAttribute("name", name);
      changeSupport.firePropertyChange("name,", old, name);
   }

   /**
    * Gets the name of this enumeration. If it does not have a name, it is
    * lazily initialized to the empty string.
    *
    * @return     the name of this enumeration
    */
   public String getName()
   {
      String name = "";
      if (mDomElement.getAttribute("name") != null)
      {
         name = mDomElement.getAttribute("name").getValue();
      }
      return name;
   }

   /**
    * Sets the value of this enumeration to the given value.
    *
    * @param value      the new value for this enumeration
    */
   public void setValue(Object value)
   {
      Object old = getValue();
      mDomElement.setAttribute("value", value.toString());
      changeSupport.firePropertyChange("value", old, value);
   }

   /**
    * Gets the value of this enumeration. If it does not have a value, null is
    * returned.
    *
    * @return  the value of this enumeration, null if it has no value
    */
   public Object getValue()
   {
      Object value = null;
      if (mDomElement.getAttribute("value") != null)
      {
         value = ConfigUtilities.getAttributeValue(mDomElement,
                                                   "value",
                                                   mValType);
      }
      return value;
   }

   /**
    * Gets the type of the value stored in this enumeration.
    */
   public ValType getValType()
   {
      return mValType;
   }

   /**
    * Allows access to the internal DOM node. This should generally only be used
    * by the PropertyDesc class.
    */
   protected Element getNode()
   {
      return mDomElement;
   }

   /**
    * Tests if the given enumeration is equal to this enumeration.
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof DescEnum)
      {
         return mDomElement == ((DescEnum)obj).mDomElement;
      }
      return false;
   }

   /**
    * Registers the given listener to receive notifications when this object's
    * properties change.
    */
   public void addPropertyChangeListener(PropertyChangeListener listener)
   {
      changeSupport.addPropertyChangeListener(listener);
   }

   /**
    * Unregisters the given listener so that it no longer receives notifications
    * when this object's properties change.
    */
   public void removePropertyChangeListener(PropertyChangeListener listener)
   {
      changeSupport.removePropertyChangeListener(listener);
   }

   /**
    * Gets the string representation of this enumeration.
    */
   public String toString()
   {
      XMLOutputter outputter = new XMLOutputter("   ", true);
      outputter.setLineSeparator(System.getProperty("line.separator"));
      return outputter.outputString(mDomElement);
   }

   private Element mDomElement;
   private ValType mValType;

   /**
    * Helper object for manager property changes.
    */
   private PropertyChangeSupport changeSupport;
}
