/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
import java.util.*;
import java.io.*;
import org.jdom.Element;


/** Individual PropertyDesc of a ChunkDesc.
 *  The PropertyDesc represents a single property, with its own name and type
 *  and with one or more values.  The PropertyDesc can include help text,
 *  individual labels for each value of the Property, and an enumeration of
 *  possible values.
 *
 *  Note that PropertyDesc doesn't have any inherent synchronization; anyone
 *  modifying a PropertyDesc needs to guarantee that no one else is looking
 *  at it at the same time.
 *
 *  @version $Revision$
 */
public class PropertyDesc
   implements Cloneable
            , ConfigTokens
{
   private int enumval = 0; // for assigning numeric defaults to enum entries

   // items for assisting in GUI displays of chunks.

   /** Creates an "empty" PropertyDesc */
   public PropertyDesc (Element root)
   {
      mDomElement = root;
      changeSupport = new PropertyChangeSupport(this);
   }

   public PropertyDesc ()
   {
      mDomElement = new Element(property_desc_TOKEN);
      changeSupport = new PropertyChangeSupport(this);
      this.setName("");
      this.setToken("");
   }

   public Object clone ()
      throws CloneNotSupportedException
   {
      PropertyDesc p = (PropertyDesc)super.clone();
      p.mDomElement  = (Element) mDomElement.clone();
      return p;
   }

   public void setName (String newName)
   {
      String old = getName();
      mDomElement.setAttribute("name", newName);
      changeSupport.firePropertyChange("name", old, newName);
   }

   /**
    * Gets the name of this property description, if it has one.
    *
    * @return The empty string will be returned if this property description
    *         has no name.
    */
   public String getName ()
   {
      String name = "";

      if ( mDomElement.getAttribute("name") != null )
      {
         name = mDomElement.getAttribute("name").getValue();
      }

      return name;
   }

   public void setToken (String newToken)
   {
      String old = getToken();
      mDomElement.setAttribute("token", newToken);
      changeSupport.firePropertyChange("token", old, newToken);
   }

   /**
    * Gets the token (type) of this property description, if it has one.
    *
    * @return The empty string will be returned if this property description
    *         has no token/type set yet.
    */
   public String getToken()
   {
      String token = "";

      if ( mDomElement.getAttribute("token") != null )
      {
         token = mDomElement.getAttribute("token").getValue();
      }

      return token;
   }

   public void setHelp (String helpText)
   {
      Element help_child = mDomElement.getChild("help");

      if ( null == help_child )
      {
         help_child = new Element("help");
         mDomElement.addContent(help_child);
      }

      String old = getHelp();
      help_child.setText(helpText);
      changeSupport.firePropertyChange("help", old, helpText);
   }

   public String getHelp()
   {
      Element help_child = mDomElement.getChild("help");
      String help_text = "";

      if ( null != help_child )
      {
         help_text = help_child.getText();
      }

      return help_text;
   }

   public void setValType (ValType t)
   {
      ValType old = getValType();
      mDomElement.setAttribute("type", t.toString());
      changeSupport.firePropertyChange("type", old, t);
   }

   public ValType getValType()
   {
      if ( mDomElement.getAttribute("type") == null )
      {
         System.err.println("Invalid PropertyDesc named " + getName());
         return ValType.INVALID;
      }
      else
      {
         return ValType.getValType(mDomElement.getAttribute("type").getValue());
      }
   }

   /**
    * Adds the given Item object as a child of this property description.
    */
   public void addItem (Item item)
   {
      mDomElement.addContent(item.getNode());
      changeSupport.firePropertyChange("item", null, null);
   }

   /**
    * Removes the given Item object from this property description.
    */
   public void removeItem(Item item)
   {
      mDomElement.removeContent(item.getNode());
      changeSupport.firePropertyChange("item", null, null);
   }

   public Vector getItems ()
   {
      Vector items = new Vector();

      Iterator i = mDomElement.getChildren(item_TOKEN).iterator();

      while ( i.hasNext() )
      {
         Element item_elem = (Element) i.next();
         items.add(new Item(item_elem, this.getValType()));
      }

      return items;
   }

   /**
    * Gets the number of items in this property description.
    */
   public int getItemsSize()
   {
      return mDomElement.getChildren(item_TOKEN).size();
   }

   public void setItem (int index, Item item)
   {
      if ( index >= mDomElement.getChildren(item_TOKEN).size() )
      {
         mDomElement.addContent(item.getNode());
      }
      else
      {
         Element item_elem =
            (Element) mDomElement.getChildren(item_TOKEN).get(index);

         // XXX: Not sure if this is right... There may be an issue with the
         // default value setting not being copied into item_elem.
         item_elem.setAttribute(item_label_TOKEN, item.getLabel());
         item_elem.setAttribute(default_value_TOKEN,
                                item.getDefaultValue().toString());
      }
      changeSupport.firePropertyChange("item", null, null);
   }

   /**
    * Returns a reference to the default value at the given index.  If there
    * is no such value, an empty VarValue object is returned.
    */
   public VarValue getDefaultValue (int index)
   {
      ValType val_type = this.getValType();
      VarValue val = new VarValue(val_type);

      List children = mDomElement.getChildren(item_TOKEN);

      if ( index < children.size() )
      {
         Element item_child = (Element) children.get(index);

         if ( item_child.getAttribute(default_value_TOKEN) != null )
         {
            val.set(item_child.getAttribute(default_value_TOKEN).getValue().trim());
         }
         // XXX: Default values for embedded chunks are not yet supported.
         else if ( ValType.EMBEDDEDCHUNK == val_type )
         {
            System.out.println("PropertyDesc.getDefaultValue("+index+"): Creating new default embedded chunk");
            String chunk_type = this.getEnumAt(0).getName();
            ChunkDesc desc = ChunkFactory.getChunkDescByToken(chunk_type);
            ConfigChunk emb_chunk = new ConfigChunk(desc);
            emb_chunk.setName(item_child.getAttribute("label").getValue());
            val.set(emb_chunk);
         }
      }
      // Support embedded chunks for property descs that don't have items
      else if (ValType.EMBEDDEDCHUNK == val_type)
      {
         System.out.println("PropertyDesc.getDefaultValue("+index+"): Creating new default embedded chunk");
         String chunk_type = this.getEnumAt(0).getName();
         System.out.println("Getting desc for chunk type: "+chunk_type);
         ChunkDesc desc = ChunkFactory.getChunkDescByToken(chunk_type);
         ConfigChunk emb_chunk = new ConfigChunk(desc);
         emb_chunk.setName(this.getName());
         val.set(emb_chunk);
      }

      return val;
   }

   /**
    * Returns a copy of the default value at the given index.  If there is
    * no such default value, null is returned.
    */
   public VarValue getDefaultValueCopy (int index)
   {
      VarValue val = getDefaultValue(index);
      VarValue val_copy = null;

      // If we do have a default value at the given index, make the copy.
      if ( val != null )
      {
         val_copy = new VarValue(val);
      }

      return val_copy;
   }

   public void setHasVariableNumberOfValues (boolean allowsVariable)
   {
      boolean old = hasVariableNumberOfValues();
      if ( allowsVariable )
      {
         mDomElement.setAttribute("variable", "1");
      }
      else
      {
         mDomElement.setAttribute("variable", "0");
      }
      changeSupport.firePropertyChange("variable", old, allowsVariable);
   }

   /**
    * Determines if this property description allows a variable number of
    * values.
    */
   public boolean hasVariableNumberOfValues()
   {
      boolean result;

      // If, for some weird reason, this property description does not have a
      // variable attribute, this is a good place to add it. The default is 
      // 0 (false), so that's the value we'll use.
      if ( mDomElement.getAttribute("variable") == null )
      {
         result = false;
         mDomElement.setAttribute("variable", "0");
      }
      // Otherwise, we test the attribute value against the variable number
      // token.
      else
      {
         String value = mDomElement.getAttribute("variable").getValue();
         result = value.equals("1");
      }

      return result;
   }

   public void setUserLevel(int level)
   {
      int old = getUserLevel();
      mDomElement.setAttribute(user_level_TOKEN,
                               String.valueOf(level));
      changeSupport.firePropertyChange("level", old, level);
   }

   public int getUserLevel()
   {
      int level = 0;

      // XXX: The following code will probably not be needed in the long run.
      // (PH 5/10/2002)
      if ( mDomElement.getAttribute(user_level_TOKEN) != null )
      {
         String value =
            mDomElement.getAttribute(user_level_TOKEN).getValue();

         if ( value.equals(beginner_TOKEN) )
         {
            level = 0;
         }
         else if ( value.equals(expert_TOKEN) )
         {
            level = 1;
         }
         else
         {
            level = Integer.valueOf(value.trim()).intValue();
         }
      }

      return level;
   }

   public int getValueLabelsSize ()
   {
      return mDomElement.getChildren(item_TOKEN).size();
   }

   public String getValueLabel (int i)
   {
      if ( i < getValueLabelsSize() )
      {
         Element item_elem =
            (Element) mDomElement.getChildren(item_TOKEN).get(i);
         return item_elem.getAttribute(item_label_TOKEN).getValue();
      }
      else
      {
         return "";
      }
   }

   /**
    * Replaces the current <item> children with the given list without setting
    * default values.  In other words, this wipes out any default values that
    * were set previously.  To avoid this, use addItem().
    *
    * @see addItem()
    */
   public void setValueLabels (List labels)
   {
      mDomElement.removeChildren(item_TOKEN);
      Iterator i = labels.iterator();

      while ( i.hasNext() )
      {
         Element new_elem = new Element(item_TOKEN);
         new_elem.setAttribute(item_label_TOKEN,
                               (String) i.next());
         mDomElement.addContent(new_elem);
      }
      changeSupport.firePropertyChange("item", null, null);
   }

   /**
    * Appends the given value label to the current list of property items
    * without setting a default value.  To set a default value, too, use
    * addItem().
    *
    * @see addItem()
    */
   public void appendValueLabel (String label)
   {
      Element new_item = new Element(item_TOKEN);
      new_item.setAttribute(item_label_TOKEN, label);
      mDomElement.addContent(new_item);
      changeSupport.firePropertyChange("item", null, null);
   }

   /**
    * Adds the given enumeration to this PropertyDesc at the end of the list of
    * enumerations.
    *
    * @param newEnum    the enumeration to add
    */
   public void addEnum(DescEnum newEnum)
   {
      mDomElement.addContent(newEnum.getNode());
      newEnum.addPropertyChangeListener(enumChangeListener);
      changeSupport.firePropertyChange("enumeration", null, null);
   }

   /**
    * Removes the given enumeration from this PropertyDesc.
    *
    * @param @targetEnum   the enumeration to remove
    */
   public void removeEnum(DescEnum targetEnum)
   {
      mDomElement.removeContent(targetEnum.getNode());
      targetEnum.removePropertyChangeListener(enumChangeListener);
      changeSupport.firePropertyChange("enumeration", null, null);
   }

   /**
    * Gets the number of enumerations assocated with this PropertyDesc.
    */
   public int getNumEnums()
   {
      return mDomElement.getChildren(prop_enum_TOKEN).size();
   }

   /**
    * Gets the i'th enumeration associated with this PropertyDesc.
    *
    * @param idx     the index of the enumeration to retrieve
    *
    * @throws ArrayIndexOutOfBoundsException
    *             if the index is invalid
    */
   public DescEnum getEnumAt(int idx)
      throws ArrayIndexOutOfBoundsException
   {
      // Verify that idx is a valid index
      if ((idx < 0) || (idx >= getNumEnums()))
      {
         throw new ArrayIndexOutOfBoundsException("Index, "+idx+", is out of bounds");
      }

      // Construct a new DescEnum object for the enum we care about
      Element desc_elt =
         (Element)mDomElement.getChildren(prop_enum_TOKEN).get(idx);
      DescEnum desc_enum = new DescEnum(desc_elt, getValType());
      desc_enum.addPropertyChangeListener(enumChangeListener);

      return desc_enum;
   }

   /**
    * Gets a list of all the enumerations contained in this PropertyDesc.
    *
    * @return  the list of enumerations; an empty list if it contains no enums
    */
   public List getEnums()
   {
      List enums = new ArrayList();
      Iterator itr =
         mDomElement.getChildren(prop_enum_TOKEN).iterator();

      while (itr.hasNext())
      {
         DescEnum de = new DescEnum((Element)itr.next(), getValType());
         de.addPropertyChangeListener(enumChangeListener);
         enums.add(de);
      }

      return enums;
   }

   public boolean equals (PropertyDesc d)
   {
      if (d == null)
      {
         return false;
      }

      return mDomElement == d.mDomElement;
   }

   public String toString()
   {
      org.jdom.output.XMLOutputter outputter =
         new org.jdom.output.XMLOutputter("   ", true);
      outputter.setLineSeparator(System.getProperty("line.separator"));
      return outputter.outputString(mDomElement);
   }

   /** Maps a string to an enumeration value.
    *  @return The VarValue which is mapped by val, or null if no such
    *          mapping exists.
    */
   public VarValue getEnumValue (String val)
   {
      VarValue enum_value = null;
      Iterator i = mDomElement.getChildren(prop_enum_TOKEN).iterator();

      while ( i.hasNext() )
      {
         Element enum_elem = (Element) i.next();

         if ( enum_elem.getAttribute("name").getValue().equals(val) )
         {
            enum_value = new VarValue(this.getValType());

            if ( enum_elem.getAttribute("value") != null )
            {
               enum_value.set(enum_elem.getAttribute("value").getValue());
            }

            break;
         }
      }

      // If no enumeration value was found, make a new var value from the
      // original input string.
      if ( null == enum_value )
      {
         enum_value = new VarValue(this.getValType());
         enum_value.set(val);
      }

      return enum_value;
   }

   /**
    * Maps a value back to the name of the enum entry.  This does the reverse
    * of getEnumVal(), but it is much, much slower.
    */
   public String getEnumString (VarValue val)
   {
      VarValue val_storage = new VarValue(val.getValType());
      Iterator i =
         mDomElement.getChildren(prop_enum_TOKEN).iterator();
      Element enum_elem;
      String return_val = val.toString();

      while ( i.hasNext() )
      {
         enum_elem = (Element) i.next();

         // If the current element has a value attribute, we extract its and
         // set its value to the value of val_storage.  Slow, slow, slow...
         if ( enum_elem.getAttribute("value") != null )
         {
            val_storage.set(enum_elem.getAttribute("value").getValue());

            if ( val_storage.equals(val) )
            {
               return_val = enum_elem.getAttribute("name").getValue();
               break;
            }
         }
      }

      return return_val;
   }

   /**
    * Returns the internal DOM node for this chunk description.  It is
    * package visible to prevent abuse of this exposure of implementation
    * by outside packages.
    */
   Element getNode ()
   {
      return mDomElement;
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
    * This is used to contain the attributes of a <ProprtyDesc>'s <item>
    * child(ren).
    */
   public static class Item implements Cloneable
   {
      public Item (ValType type)
      {
         this.mNode    = new Element(item_TOKEN);
         this.mValType = type;

         this.setLabel("");
         this.setDefaultValue("");
      }

      public Item (Element root, ValType type)
      {
         this.mNode    = root;
         this.mValType = type;
      }

      public Object clone () throws CloneNotSupportedException
      {
         Item new_item  = (Item) super.clone();
         new_item.mNode = (Element) this.mNode.clone();

         return new_item;
      }

      public String getLabel ()
      {
         return mNode.getAttribute(item_label_TOKEN).getValue();
      }

      public void setLabel (String label)
      {
         mNode.setAttribute(item_label_TOKEN, label);
      }

      public ValType getValType ()
      {
         return mValType;
      }

      public VarValue getDefaultValue ()
      {
         VarValue default_value = null;

         // If the item actually has a default value, return it
         if ( mNode.getAttribute(default_value_TOKEN) != null )
         {
            default_value = new VarValue(mValType);
            default_value.set(mNode.getAttribute(default_value_TOKEN).getValue());
         }
         // Otherwise, create a new value for the default
         else
         {
            default_value = new VarValue(mValType);
            // If the type is an embedded chunk, create the chunk needed
            if (mValType == ValType.EMBEDDEDCHUNK)
            {
               // Get the enumeration containing the chunk desc token
               Element parent = mNode.getParent();
               Element desc_node = parent.getChild(prop_enum_TOKEN);
               String desc_token = desc_node.getAttributeValue(name_TOKEN);

               // Get the default config chunk for the found desc
               ChunkDesc desc = ChunkFactory.getChunkDescByToken(desc_token);
               default_value.set(new ConfigChunk(desc));
            }
         }

         return default_value;
      }

      public void setDefaultValue (VarValue value)
      {
         setDefaultValue(value.toString());
      }

      private void setDefaultValue (String value)
      {
         mNode.setAttribute(default_value_TOKEN, value);
      }

      /**
       * @note This is private so that only the PropertyDesc class call it.
       */
      private Element getNode ()
      {
         return mNode;
      }

      private Element mNode    = null;
      private ValType mValType = null;
   }

   /**
    * A specialized listener specifically for handling DescEnum property
    * changes.
    */
   class EnumChangeListener
      implements PropertyChangeListener
   {
      public void propertyChange(PropertyChangeEvent evt)
      {
         // TODO: do something about this ;)
      }
   }

   /**
    * The DOM element that maintains the content for this PropertyDesc.
    */
   private Element mDomElement = null;

   /**
    * This helps manage property change notification to listeners.
    */
   private PropertyChangeSupport changeSupport = null;

   /**
    * Listener for enumeration property changes.
    */
   private EnumChangeListener enumChangeListener = new EnumChangeListener();

}
