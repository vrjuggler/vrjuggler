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
import java.io.DataInputStream;
import java.io.IOException;
import java.util.*;
import javax.swing.event.EventListenerList;

import org.jdom.Element;
import org.jdom.Attribute;


/**
 * A ConfigChunk is a logical collection of configuration information. It stores
 * a number of Properties that describe the configuration of a particular
 * component of the system. It has an associated ChunkDesc which describes its
 * type and the Properties that belong to it.
 *
 * @see ChunkDesc
 */
public class ConfigChunk
   implements ConfigTokens
{
   public final static String EMBEDDED_SEPARATOR = "/";

   public ConfigChunk(Element root)
   {
      mDomElement = root;
      desc = ChunkFactory.getChunkDescByToken(root.getName());

      if (null == desc)
      {
         System.err.println("WARNING: Unknown chunk type '" + root.getName() +
                            "' encountered");
      }
   }

   /**
    * Creates a new ConfigChunk that is an exact copy of the source ConfigChunk.
    *
    * @param srcChunk      the ConfigChunk to copy
    */
   public ConfigChunk(ConfigChunk srcChunk)
   {
      desc = srcChunk.desc;
      mDomElement = (Element)srcChunk.mDomElement.clone();
   }

   /**
    * Create a new ConfigChunk using the given chunk description.
    *
    * @param desc    the ChunkDesc that describes the ConfigChunk to create
    */
   public ConfigChunk(ChunkDesc d)
   {
      desc = d;
      mDomElement = new Element(d.getToken());

      Iterator i = d.getPropertyDescs().iterator();
      PropertyDesc prop_desc;
      String prop_desc_type;

      while (i.hasNext())
      {
         prop_desc = (PropertyDesc) i.next();
         prop_desc_type = prop_desc.getToken();
         System.out.println("Processing property: " + prop_desc.getName());

         if (prop_desc.hasVariableNumberOfValues())
         {
            this.setProperty(prop_desc_type, 0, prop_desc.getDefaultValue(0));
         }
         else
         {
            int num_items = prop_desc.getItemsSize();

            for (int j = 0; j < num_items; ++j)
            {
               this.setProperty(prop_desc_type, j,
                                prop_desc.getDefaultValue(j));
            }
         }
      }
   }

   /**
    * Returns the symbolic, human-friendly name of this chunk.  If this chunk
    * does not have a name when this method is invoked, it is assigned here.
    */
   public final String getName()
   {
      Attribute name_attr = mDomElement.getAttribute("name");

      // If this chunk does not yet have a name, we have to set one now.
      if (null == name_attr)
      {
         name_attr = new Attribute("name", this.getDescToken());
      }

      return name_attr.getValue();
   }

   /**
    * Sets the symbolic, human-friendly name of this config chunk to the given
    * value.
    */
   public final void setName(String name)
   {
      String old = getName();
      mDomElement.setAttribute("name", name);
      fireNameChanged(old);
   }

   /**
    * Returns the version of the chunk desc this config chunk refers to.
    */
   public String getVersion()
   {
      Attribute vers_attr = mDomElement.getAttribute(version_TOKEN);

      // If this chunk does not have a name, we will default to 1.1
      if (null == vers_attr)
      {
         vers_attr = new Attribute(version_TOKEN, "1.1");
      }
      return vers_attr.getValue();
   }

   /**
    * Sets the version of the chunk desc this config chunk refers to to the
    * given version string.
    */
   public void setVersion(String version)
   {
      String old = getVersion();
      mDomElement.setAttribute(version_TOKEN, version);
      //fireVersionChanged(old);
   }

   /**
    * Returns the fully qualified, unique name of this chunk.  This will be
    * different from the value returned by getName() if this chunk is a child
    * of another chunk.  In that case, the full name will be the path to this
    * chunk based on the chunk hierarchy and the property tokens.
    */
   public final String getFullName()
   {
      Element chunk_parent, prop_parent;
      String full_name = this.getName();

      chunk_parent = mDomElement;

      while ((prop_parent = chunk_parent.getParent()) != null &&
              ! prop_parent.getName().equals(chunk_db_TOKEN))
      {
         full_name = prop_parent.getName() + EMBEDDED_SEPARATOR + full_name;
         chunk_parent = prop_parent.getParent();
         full_name = chunk_parent.getAttribute("name").getValue() +
                     EMBEDDED_SEPARATOR + full_name;
      }

      return full_name;
   }

   //-------- Stuff for dealing with embedded chunk names ---------------

   /**
    * Retrieves the named embedded chunk from this chunk.
    *
    * @note Expects self's own name/prop pair at start of pathname.
    * @note
    */
   public ConfigChunk getEmbeddedChunk(String pathname)
   {
      ConfigChunk ch = null;

      // note: a nonrecursive version of this is probably worth it
      //       for the performance increase.  this is yicky.
      try
      {
         Element root = mDomElement;

         while (ConfigChunk.hasSeparator(pathname) && root != null)
         {
            String prop_name = ConfigChunk.getFirstNameComponent(pathname);

            // Get the child element matching the current property name.
            Element property = root.getChild(prop_name);
            PropertyDesc property_desc = desc.getPropertyDesc(prop_name);

            // At this point, we (hopefully) have a valid child of prop_name
            // and the corresponding property description.  The next phase of
            // work will set root to the next child chunk element in pathname.
            // If it doesn't, then we're done or something went wrong.
            root = null;

            // If the path element is valid, we can continue.
            if (null != property && null != property_desc &&
                 property_desc.getValType() == ValType.EMBEDDEDCHUNK)
            {
               // Chop off the part of the path that is prop_name.
               pathname = ConfigChunk.getNameRemainder(pathname);

               // Get the name of the chunk from the newly truncated
               // pathname string.
               String next_chunk_name =
                  ConfigChunk.getFirstNameComponent(pathname);

               // Chop off the part of the path that is next_chunk_name.
               pathname = ConfigChunk.getNameRemainder(pathname);

               // Now we have to search the children of property for the
               // next chunk name in the path.
               Iterator i = property.getChildren().iterator();

               while (i.hasNext())
               {
                  Element child = (Element)i.next();

                  // Hooray!  We found the next child chunk in the path.
                  // We're done with this phase.
                  if (child.getAttribute("name").equals(next_chunk_name))
                  {
                     root = child;
                     break;
                  }
               }
            }
         }

         // If we have a valid root element and its name attribute matches
         // what is left of pathname, then we found our chunk!
         if (root != null && root.getAttribute("name").equals(pathname))
         {
            ch = new ConfigChunk(root);
         }
      }
      catch (Exception e)
      {
         /* Do nothing.  We'll end up returning null. */ ;
      }

      return ch;
   }

   public static final String getLastNameComponent(String chunkName)
   {
      int i = chunkName.lastIndexOf(EMBEDDED_SEPARATOR);
      return ((i < 0) ? chunkName
                      : chunkName.substring(i + EMBEDDED_SEPARATOR.length()));
   }

   /**
    * This is a helper function for classes wanting to deal with fully
    * qualified chunk names.
    */
   public static final String getFirstNameComponent(String _name)
   {
      int i = _name.indexOf(EMBEDDED_SEPARATOR);
      return(i < 0) ? "" : _name.substring(0, i);
   }

   /* Returns everything after the first divider in a name.
    * If there is no separator, returns name.
    */
   public final static String getNameRemainder(String chunkName)
   {
      int i = chunkName.indexOf(EMBEDDED_SEPARATOR);
      return (i < 0) ? chunkName
                     : chunkName.substring(i + EMBEDDED_SEPARATOR.length());
   }

   public final static boolean hasSeparator(String chunkName)
   {
      return (chunkName.indexOf(EMBEDDED_SEPARATOR) >= 0);
   }

   public final ChunkDesc getDesc()
   {
      return desc;
   }

   public final PropertyDesc getPropertyDesc(String propertyToken)
   {
      return desc.getPropertyDesc(propertyToken);
   }

   public final String getDescName()
   {
      return desc.getName();
   }

   /**
    * Returns the type of this chunk using its chunk description.  The type of
    * this chunk is defined by its description's token/type identifier.  If
    * this chunk has no description, null is returned.
    *
    * @return A string containing this chunk's type or null if this chunk has
    *         no description.
    */
   public final String getDescToken()
   {
      String desc_token = null;

      if ( null != desc )
      {
         desc_token = desc.getToken();
      }

      return desc_token;
   }

   public final String getDescHelp()
   {
      return desc.getHelp();
   }

   /**
    * Gets a list of all the embedded chunks embedded directly within this
    * ConfigChunk.
    *
    * @return  a list of the embedded ConfigChunk objects.
    */
   public List getEmbeddedChunks()
   {
      List chunks = new ArrayList();

      // Iterate over the property descs looking for those that contain embedded
      // chunks that we want.
      for (Iterator itr = getDesc().getPropertyDescs().iterator(); itr.hasNext(); )
      {
         PropertyDesc prop_desc = (PropertyDesc)itr.next();

         // If this property desc contains embedded chunks, get the chunks
         // contained therein.
         if (prop_desc.getValType() == ValType.EMBEDDEDCHUNK)
         {
            // Retrieve all the embedded chunks for the current property
            String propdesc_token = prop_desc.getToken();
            for (int i=0; i<getNumPropertyValues(propdesc_token); ++i)
            {
               ConfigChunk embedded_chunk = (ConfigChunk)getProperty(propdesc_token, i);
               if (embedded_chunk != null)
               {
                  chunks.add(embedded_chunk);
               }
            }
         }
      }

      return chunks;
   }

   /**
    * Tests if this ConfigChunk is equal to the given ConfigChunk. This is true
    * if both ConfigChunks share the same DOM element object.
    *
    * @param chunk   the chunk to compare equality against
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof ConfigChunk)
      {
         return mDomElement == ((ConfigChunk)obj).mDomElement;
      }
      return false;
   }

   /**
    * Returns the named property value in propStorage if the property exists
    * in this chunk.
    */
   public Object getProperty (String propType)
   {
      return getProperty(propType, 0);
   }

   /**
    * Gets the value of the given property type at the specified index. If the
    * chunk has not defined a value for that property, a default value is
    * returned.
    *
    * @param propType A property type that matches those defined in this
    *                 chunk's description.
    * @param index    The indexed property matching the named type.
    *
    * @return  An object containing the property value.
    *
    * @note The rest of the code expects this method to return null if the
    *       index is valid but not within the range of currently defined
    *       values.  This seems incorrect, but it may be too hard to change
    *       every place.  Either that, or determining the difference between
    *       an "empty" VarValue object and a default value may be too hard.
    */
   public Object getProperty(String propType, int index)
   {
      Object value = null;

      // Get the property description for the given property type.
      PropertyDesc prop_desc = desc.getPropertyDesc(propType);

      // Invalid property type!  We have to return null.
      if (null == prop_desc)
      {
         System.err.println("WARNING: Invalid property type '" + propType +
                            "' requested in chunk of type '" +
                            desc.getToken() + "'");
      }
      // The property type is valid, so we can look up the indexed value.
      else
      {
         // JDOM will return an empty list if there are no children matching
         // propType.
         List props = mDomElement.getChildren(propType);

         // Extract the information relevant for the given property type.
         ValType val_type = prop_desc.getValType();

         // Verify that the index is valid.
         if ((index < prop_desc.getItemsSize() ||
             prop_desc.hasVariableNumberOfValues()))
         {
            // Ensure that the index actually falls within the loaded list of
            // property values.
            if (index < props.size())
            {
               // Look up the property in the list.
               Element prop = (Element) props.get(index);
               String prop_val = prop.getTextTrim();

               // A child chunk property.
               if (ValType.EMBEDDEDCHUNK == val_type)
               {
                  // This gets the child of prop that is the embedded chunk.
                  String chunk_type = prop_desc.getAllowedType(0);

                  Element child = prop.getChild(chunk_type);

                  // If we have a child chunk, we can return it.  Otherwise,
                  // we return an empty VarValue object.
                  if (null != child)
                  {
                     ConfigChunk child_chunk = new ConfigChunk(child);
                     value = child_chunk;
                  }
               }
               // All other property types.
               else
               {
                  value = prop_desc.getEnumValue(prop_val);
               }
            }
            // The index is outside the range of currently defined property
            // values.  In this case, we fall back on the default value.
            // Thank goodness for those!
            else
            {
               System.out.println(">>>ConfigChunk.getProperty(): No value set, using a default value.");
               value = prop_desc.getDefaultValue(index);
               System.out.println("\tvalue = "+value);
            }
         }
      }

      return value;
   }

   /**
    * Gets the number of values the given named property has in this
    * ConfigChunk.
    *
    * @param propType   the name of the property to query
    */
   public int getNumPropertyValues(String propType)
   {
      List children = mDomElement.getChildren(propType);
      int count = 0;

      if (null != children)
      {
         count = children.size();
      }

      return count;
   }

   /**
    * Sets the value for the property of type propType at the given index.
    *
    * @pre value must not be null.
    *
    * @param propType The property description identifier/token for the
    *                 property being set.
    * @param index    The index within the named property type that will be
    *                 set.  If this index is not within the valid range, no
    *                 value is set.
    * @param value    The value to be set.  It must not be a null reference.
    *
    * @return A boolean value stating whether or not the set operation
    *         succeeded is returned to the caller.
    */
   public boolean setProperty(String propType, int index, Object value)
   {
      List props = mDomElement.getChildren(propType);
      boolean status = false;

      PropertyDesc prop_desc = desc.getPropertyDesc(propType);

      // Extract the information relevant for the given property type.
      ValType val_type = prop_desc.getValType();

      // Verify that we can set the property value at the given index.
      if (index < prop_desc.getItemsSize() ||
          prop_desc.hasVariableNumberOfValues())
      {
         if (null != props && index < props.size())
         {
            Element prop = (Element) props.get(index);

            // The special case for embedded chunks is needed because we do
            // not want to insert them as text strings.
            if (val_type == ValType.EMBEDDEDCHUNK)
            {
               prop.removeChildren();
               prop.addContent(((ConfigChunk)value).getNode());
            }
            else
            {
               prop.setText(value.toString());
            }

            // Notify listeners of the change
            firePropertyValueChanged(prop_desc.getToken(), index, value);
         }
         // The property does not currently exist, so we'll add it.
         else
         {
            Element prop = new Element(propType);

            // The special case for embedded chunks is needed because we do
            // not want to insert them as text strings.
            if (val_type == ValType.EMBEDDEDCHUNK)
            {
               ConfigChunk emb_chunk = (ConfigChunk)value;
               if (emb_chunk != null)
               {
                  prop.addContent(emb_chunk.getNode());
               }
            }
            else
            {
               prop.addContent(value.toString());
            }

            mDomElement.addContent(prop);

            // Notify listeners of the addition
            firePropertyValueAdded(prop_desc.getToken(), index, value);
         }

         status = true;
      }

      return status;
   }

   /**
    * Removes the property of type propType at the given index.
    */
   public boolean removeProperty(String propType, int index)
   {
      List props = mDomElement.getChildren(propType);
      boolean status = false;

      PropertyDesc prop_desc = desc.getPropertyDesc(propType);

      if (index < props.size())
      {
         Object old_value = getProperty(propType, index);

         Element del_prop = (Element)props.get(index);
         mDomElement.removeContent(del_prop);

         // Notify listeners of the removal
         firePropertyValueRemoved(prop_desc.getToken(), index, old_value);
         status = true;
      }

      return status;
   }

   /**
    * Returns a vector of name of ConfigChunks that this ConfigChunk depends on.
    */
   public List getDependencyNames()
   {
      ArrayList results = new ArrayList();

      Iterator i = desc.getPropertyDescs().iterator();

      while (i.hasNext())
      {
         PropertyDesc prop_desc = (PropertyDesc)i.next();

         if (prop_desc.getValType() == ValType.CHUNK)
         {
            // Get all the elements matching the current property description
            // token.
            Iterator j = mDomElement.getChildren(prop_desc.getToken()).iterator();

            while (j.hasNext())
            {
               String dep_text = ((Element)j.next()).getTextTrim();

               // JDOM will return an empty string rather than null if the
               // element has no text.
               if (! dep_text.equals(""))
               {
                  results.add(dep_text);
               }
            }
         }
         else if (prop_desc.getValType() == ValType.EMBEDDEDCHUNK)
         {
            // Get all the elements matching the current property description
            // token.
            Iterator j = mDomElement.getChildren(prop_desc.getToken()).iterator();

            while (j.hasNext())
            {
               Element parent_prop = (Element)j.next();

               // This is needed to go to to next level in the hierarchy--that
               // is, to the position of the actual embedded chunk.
               Iterator k = parent_prop.getChildren().iterator();

               while (k.hasNext())
               {
                  ConfigChunk child_chunk = new ConfigChunk((Element) k.next());
                  results.addAll(child_chunk.getDependencyNames());
               }
            }
         }
      }

      return results;
   }

   /**
    * Gets the XML representation of this ConfigChunk as a String. This is
    * suitable for a replacement to serialization.
    */
   public final String toString()
   {
      org.jdom.output.XMLOutputter outputter =
         new org.jdom.output.XMLOutputter("   ", true);
      outputter.setLineSeparator(System.getProperty("line.separator"));
      return outputter.outputString(mDomElement);
   }

   /**
    * Returns the DOM tree node for this chunk.  The constructors guarantee
    * that it is non-null.  It is package visible so that code in other
    * packages cannot abuse this implementation exposure.
    */
   Element getNode()
   {
      return mDomElement;
   }

   /**
    * Adds the given listener to be notified when this config chunk changes.
    */
   public void addConfigChunkListener(ConfigChunkListener listener)
   {
      listenerList.add(ConfigChunkListener.class, listener);
   }

   /**
    * Removes the given listener that was registered to be notified when this
    * config chunk changed.
    */
   public void removeConfigChunkListener(ConfigChunkListener listener)
   {
      listenerList.remove(ConfigChunkListener.class, listener);
   }

   /**
    * Notifies listeners that this ConfigChunk's name has changed.
    */
   protected void fireNameChanged(String old_name)
   {
      ConfigChunkEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigChunkListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigChunkEvent(this, null, 0, old_name);
            }
            ((ConfigChunkListener)listeners[i+1]).nameChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that this ConfigChunk's property has changed.
    */
   protected void firePropertyValueChanged(String prop_token, int index,
                                           Object old_value)
   {
      ConfigChunkEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigChunkListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigChunkEvent(this, prop_token, index, old_value);
            }
            ((ConfigChunkListener)listeners[i+1]).propertyValueChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners that a new property value has been added to this
    * ConfigChunk.
    */
   protected void firePropertyValueAdded(String prop_token, int index,
                                         Object value)
   {
      ConfigChunkEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigChunkListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigChunkEvent(this, prop_token, index, value);
            }
            ((ConfigChunkListener)listeners[i+1]).propertyValueAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that a new property value has been removed from this
    * ConfigChunk.
    */
   protected void firePropertyValueRemoved(String prop_token, int index,
                                         Object old_value)
   {
      ConfigChunkEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigChunkListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigChunkEvent(this, prop_token, index, old_value);
            }
            ((ConfigChunkListener)listeners[i+1]).propertyValueRemoved(evt);
         }
      }
   }

   /**
    * The DOM tree node for this chunk.
    */
   private Element mDomElement = null;

   /**
    * The ChunkDesc that was used to create this chunk.
    */
   private ChunkDesc desc = null;

   /**
    * Listeners interested in this ConfigChunk.
    */
   private EventListenerList listenerList = new EventListenerList();
}
