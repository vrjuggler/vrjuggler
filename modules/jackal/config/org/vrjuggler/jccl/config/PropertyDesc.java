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
{
   private int enumval = 0; // for assigning numeric defaults to enum entries

   // items for assisting in GUI displays of chunks.

   /** Creates an "empty" PropertyDesc */
   public PropertyDesc (Element root)
   {
      mDomElement = root;
   }

   public PropertyDesc ()
   {
      mDomElement = new Element(ConfigTokens.property_desc_TOKEN);
      this.setName("");
      this.setToken("");
      this.setNumAllowed(1);
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
      mDomElement.setAttribute("name", newName);
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
      mDomElement.setAttribute("token", newToken);
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

      help_child.setText(helpText);
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
      mDomElement.setAttribute("type", t.toString());
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
   }

   public Vector getItems ()
   {
      Vector items = new Vector();

      Iterator i = mDomElement.getChildren(ConfigTokens.item_TOKEN).iterator();

      while ( i.hasNext() )
      {
         Element item_elem = (Element) i.next();
         items.add(new Item(item_elem, this.getValType()));
      }

      return items;
   }

   public void setItem (int index, Item item)
   {
      if ( index >= mDomElement.getChildren(ConfigTokens.item_TOKEN).size() )
      {
         mDomElement.addContent(item.getNode());
      }
      else
      {
         Element item_elem =
            (Element) mDomElement.getChildren(ConfigTokens.item_TOKEN).get(index);

         // XXX: Not sure if this is right... There may be an issue with the
         // default value setting not being copied into item_elem.
         item_elem.setAttribute(ConfigTokens.item_label_TOKEN, item.getLabel());
         item_elem.setAttribute(ConfigTokens.default_value_TOKEN,
                                item.getDefaultValue().toString());
      }
   }

   /**
    * Returns a reference to the default value at the given index.  If there
    * is no such value, an empty VarValue object is returned.
    */
   public VarValue getDefaultValue (int index)
   {
      ValType val_type = this.getValType();
      VarValue val = new VarValue(val_type);

      List children = mDomElement.getChildren(ConfigTokens.item_TOKEN);

      if ( index < children.size() )
      {
         Element item_child = (Element) children.get(index);

         if ( item_child.getAttribute(ConfigTokens.default_value_TOKEN) != null )
         {
            val.set(item_child.getAttribute(ConfigTokens.default_value_TOKEN).getValue().trim());
         }
         // XXX: Default values for embedded chunks are not yet supported.
         else if ( ValType.EMBEDDEDCHUNK == val_type )
         {
            String chunk_type = this.getEnumAtIndex(0).str;
            ChunkDesc desc = ChunkFactory.getChunkDescByToken(chunk_type);
            ConfigChunk emb_chunk = new ConfigChunk(desc);
            emb_chunk.setName(item_child.getAttribute("label").getValue());
            val.set(emb_chunk);
         }
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
      if ( allowsVariable )
      {
         mDomElement.setAttribute("num", ConfigTokens.var_num_TOKEN);
      }
      else
      {
         mDomElement.setAttribute("num", "1");
      }
   }

   /**
    * Determines if this property description allows a variable number of
    * values.
    */
   public boolean hasVariableNumberOfValues()
   {
      boolean result;

      // If, for some weird reason, this property description does not have a
      // num attribute, this is a good place to add it.  The default is 1, so
      // that's the value we'll use.
      if ( mDomElement.getAttribute("num") == null )
      {
         result = false;
         mDomElement.setAttribute("num", "1");
      }
      // Otherwise, we test the attribute value against the variable number
      // token.
      else
      {
         String value = mDomElement.getAttribute("num").getValue();
         result = value.equals(ConfigTokens.var_num_TOKEN);
      }

      return result;
   }

   public void setNumAllowed (int n)
   {
      mDomElement.setAttribute("num", String.valueOf(n));
   }

   public int getNumAllowed()
   {
      int num = 0;
      String value = mDomElement.getAttribute("num").getValue();

      if ( value.equals(ConfigTokens.var_num_TOKEN) )
      {
         num = -1;
      }
      else
      {
         num = Integer.valueOf(value.trim()).intValue();
      }

      return num;
   }

   public void setUserLevel(int level)
   {
      mDomElement.setAttribute(ConfigTokens.user_level_TOKEN,
                               String.valueOf(level));
   }

   public int getUserLevel()
   {
      int level = 0;

      // XXX: The following code will probably not be needed in the long run.
      // (PH 5/10/2002)
      if ( mDomElement.getAttribute(ConfigTokens.user_level_TOKEN) != null )
      {
         String value =
            mDomElement.getAttribute(ConfigTokens.user_level_TOKEN).getValue();

         if ( value.equals(ConfigTokens.beginner_TOKEN) )
         {
            level = 0;
         }
         else if ( value.equals(ConfigTokens.expert_TOKEN) )
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
      return mDomElement.getChildren(ConfigTokens.item_TOKEN).size();
   }

   public String getValueLabel (int i)
   {
      if ( i < getValueLabelsSize() )
      {
         Element item_elem =
            (Element) mDomElement.getChildren(ConfigTokens.item_TOKEN).get(i);
         return item_elem.getAttribute(ConfigTokens.item_label_TOKEN).getValue();
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
      mDomElement.removeChildren(ConfigTokens.item_TOKEN);
      Iterator i = labels.iterator();

      while ( i.hasNext() )
      {
         Element new_elem = new Element(ConfigTokens.item_TOKEN);
         new_elem.setAttribute(ConfigTokens.item_label_TOKEN,
                               (String) i.next());
         mDomElement.addContent(new_elem);
      }
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
      Element new_item = new Element(ConfigTokens.item_TOKEN);
      new_item.setAttribute(ConfigTokens.item_label_TOKEN, label);
      mDomElement.addContent(new_item);
   }

   public void appendEnumeration(String label, String value)
   {
      VarValue v;
      ValType valtype = this.getValType();

      if ( value.equals("") )
      {
         /* no explicit value */
         if (valtype == ValType.STRING ||
             valtype == ValType.CHUNK ||
             valtype == ValType.EMBEDDEDCHUNK)
         {
            v = new VarValue(label);
         }
         // XXX: This seems incredibly unsafe.  Basically, no constant mapping
         // is being retained between the enumeration ID and the value.
         // (PH 5/9/2002)
         else
         {
            v = new VarValue(enumval++);
         }
      }
      else
      {
         v = new VarValue(valtype);
         v.set (value);
      }

      Element enum_elem = new Element(ConfigTokens.prop_enum_TOKEN);
      enum_elem.setAttribute("name", label);
      enum_elem.setAttribute("value", v.toString());
      mDomElement.addContent(enum_elem);
   }

   /* c had better be a collection of DescEnums, or things will get
    * real ugly real fast.
    */
   public void setEnumerations (Collection c)
   {
      mDomElement.removeChildren(ConfigTokens.prop_enum_TOKEN);

      Iterator i = c.iterator();
      while ( i.hasNext() )
      {
         DescEnum enum     = (DescEnum) i.next();
         Element enum_elem = new Element(ConfigTokens.prop_enum_TOKEN);

         // Fill in the enumeration element's attributes.
         enum_elem.setAttribute("name", enum.str);

         // Dang special cases...
         if ( this.getValType() != ValType.EMBEDDEDCHUNK )
         {
            enum_elem.setAttribute("value", enum.val.toString());
         }

         // Add the new element to the DOM tree.
         mDomElement.addContent(enum_elem);
      }
   }

   public DescEnum[] getEnumerations()
   {
      List enums = mDomElement.getChildren(ConfigTokens.prop_enum_TOKEN);
      DescEnum[] e = new DescEnum[enums.size()];
      String name;
      VarValue value;

      for ( int i = 0; i < enums.size(); ++i )
      {
         Element enum_elem = (Element) enums.get(i);
         name  = enum_elem.getAttribute("name").getValue();
         value = new VarValue(this.getValType());

         if ( enum_elem.getAttribute("value") != null )
         {
            value.set(enum_elem.getAttribute("value").getValue());
         }

         e[i] = new DescEnum(name, value);
      }

      return e;
   }

   public int getEnumerationsSize()
   {
      return mDomElement.getChildren(ConfigTokens.prop_enum_TOKEN).size();
   }

   public DescEnum getEnumAtIndex (int index)
   {
      DescEnum enum_val = null;
      List enums = mDomElement.getChildren(ConfigTokens.prop_enum_TOKEN);

      if ( index < enums.size() )
      {
         Element enum_element = (Element) enums.get(index);

         String name = enum_element.getAttribute("name").getValue();
         VarValue value = new VarValue(this.getValType());

         if ( enum_element.getAttribute("value") != null )
         {
            value.set(enum_element.getAttribute("value").getValue());
         }

         enum_val = new DescEnum(name, value);
      }

      return enum_val;
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
      Iterator i = mDomElement.getChildren(ConfigTokens.prop_enum_TOKEN).iterator();

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
         mDomElement.getChildren(ConfigTokens.prop_enum_TOKEN).iterator();
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
    * This is used to contain the attributes of a <ProprtyDesc>'s <item>
    * child(ren).
    */
   public static class Item implements Cloneable
   {
      public Item (ValType type)
      {
         this.mNode    = new Element(ConfigTokens.item_TOKEN);
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
         return mNode.getAttribute(ConfigTokens.item_label_TOKEN).getValue();
      }

      public void setLabel (String label)
      {
         mNode.setAttribute(ConfigTokens.item_label_TOKEN, label);
      }

      public ValType getValType ()
      {
         return mValType;
      }

      public VarValue getDefaultValue ()
      {
         VarValue default_value = null;

         if ( mNode.getAttribute(ConfigTokens.default_value_TOKEN) != null )
         {
            default_value = new VarValue(mValType);
            default_value.set(mNode.getAttribute(ConfigTokens.default_value_TOKEN).getValue());
         }

         return default_value;
      }

      public void setDefaultValue (VarValue value)
      {
         setDefaultValue(value.toString());
      }

      private void setDefaultValue (String value)
      {
         mNode.setAttribute(ConfigTokens.default_value_TOKEN, value);
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

   private Element mDomElement = null;
}
