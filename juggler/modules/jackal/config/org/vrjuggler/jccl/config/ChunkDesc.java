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
import java.io.*;
import java.util.*;
import javax.swing.event.EventListenerList;
import org.jdom.Attribute;
import org.jdom.Element;

/**
 * A ChunkDesc describes what properties a ConfigChunk should have in addition
 * to other useful metadata. Essentially ChunkDescs are used to validate
 * existing ConfigChunks and provide a blueprint for creating new ConfigChunks.
 * <p>
 * Currently the ChunkDesc is implemented as a DOM element wrapper.
 */
public class ChunkDesc
   implements Cloneable
            , ConfigTokens
            , PropertyChangeListener
{
   public ChunkDesc(Element root)
   {
      this.mDomElement = root;
      changeSupport = new PropertyChangeSupport(this);
   }

   /**
    * Creates an empty chunk description with only a name property. The name and
    * token default to the empty string.
    */
   public ChunkDesc()
   {
      mDomElement = new Element(chunk_desc_TOKEN);
      changeSupport = new PropertyChangeSupport(this);
      this.setToken("");
      this.setName("");
      addCategory("Unknown");
   }

   /**
    * Creates a deep copy of this ChunkDesc. The copy has an entirely different
    * DOM element object.
    */
   public Object clone()
   {
      try
      {
         ChunkDesc d = (ChunkDesc)super.clone();
         d.mDomElement = (Element)mDomElement.clone();
         return d;
      }
      catch (CloneNotSupportedException e)
      {
         return null;
      }
   }

   /**
    * Gets the name of this ChunkDesc. If it has not yet been assigned a name,
    * it will be lazily initialized to the value of the token property.
    */
   public final String getName()
   {
      String name;
      org.jdom.Attribute attr = mDomElement.getAttribute("name");

      // If this description does not have a name, assign it the value of its
      // token.
      if ( attr == null || attr.getValue().equals("") )
      {
         name = getToken();
         mDomElement.setAttribute("name", name);
      }
      // Otherwise, just return the current name token.
      else
      {
         name = mDomElement.getAttribute("name").getValue();
      }

      return name;
   }

   /**
    * Sets the name of this ChunkDesc to the given value.
    */
   public final void setName(String newName)
   {
      String old = getName();
      mDomElement.setAttribute("name", newName);
      changeSupport.firePropertyChange("name", old, newName);
      fireNameChanged();
   }

   /**
    * Gets the unique token for this ChunkDesc.
    */
   public final String getToken()
   {
      if (mDomElement.getAttribute("token") != null)
      {
         return mDomElement.getAttribute("token").getValue();
      }
      return null;
   }

   /**
    * Sets the unique token for this ChunkDesc.
    */
   public final void setToken(String newToken)
   {
      String old = getToken();
      mDomElement.setAttribute("token", newToken);
      changeSupport.firePropertyChange("token", old, newToken);
      fireTokenChanged();
   }

   /**
    * Gets the version of this ChunkDesc.
    */
   public String getVersion()
   {
      Attribute vers_attr = mDomElement.getAttribute(version_TOKEN);
      if (null == vers_attr)
      {
         vers_attr = new Attribute(version_TOKEN, "1.1");
         mDomElement.setAttribute(vers_attr);
      }
      return vers_attr.getValue();
   }

   /**
    * Sets the version of this ChunkDesc to the given value.
    */
   public void setVersion(String version)
   {
      String old = getToken();
      mDomElement.setAttribute(version_TOKEN, version);
      changeSupport.firePropertyChange("version", old, version);
      //fireVersionChanged();
   }

   /**
    * Gets an iterator for the categories in which this ChunkDesc belongs.
    *
    * @see #addCategory
    */
   public final Iterator getCategories()
   {
      Set result = new HashSet();

      Iterator itr = mDomElement.getChildren(category_TOKEN).iterator();
      while (itr.hasNext())
      {
         result.add(((Element)itr.next()).getText());
      }

      return result.iterator();
   }

   /**
    * Gets the ith category in which the ChunkDesc belongs.
    *
    * @see #addCategory
    */
   public final String getCategory(int idx)
   {
      Element element = (Element)mDomElement.getChildren(
                                       category_TOKEN).get(idx);
      return element.getText();
   }

   /**
    * Gets the number of categories in which this ChunkDesc belongs.
    *
    * @see #addCategory
    */
   public final int getNumCategories()
   {
      return mDomElement.getChildren(category_TOKEN).size();
   }

   /**
    * Adds this ChunkDesc to the given category. Categories are defined as
    * '/'-delimited strings. For example, "Devices" and "Devices/Simulator" are
    * valid categories where Simulator is a sub-category of Devices.
    */
   public final void addCategory(String category)
   {
      // Check if this desc is already in the given category
      List cats = mDomElement.getChildren(category_TOKEN);
      for (Iterator itr = cats.iterator(); itr.hasNext(); )
      {
         // Return if this desc is already in the category
         Element element = (Element)itr.next();
         if (element.getText().equals(category))
         {
            return;
         }
      }

      // This desc isn't already in the category, add it
      Element newCat = new Element(category_TOKEN);
      newCat.setText(category);
      mDomElement.addContent(newCat);
      changeSupport.firePropertyChange("category", null, category);
      fireCategoryAdded(getNumCategories()-1);
   }

   /**
    * Removes this ChunkDesc from the given category.
    *
    * @see #addCategory
    */
   public final void removeCategory(String category)
   {
      List cats = mDomElement.getChildren(category_TOKEN);
      int index = 0;

      for (Iterator itr = cats.iterator(); itr.hasNext(); )
      {
         // If we found the category, remove it and we're done
         Element element = (Element)itr.next();
         if (element.getText().equals(category))
         {
            // Remove the category
            mDomElement.removeContent(element);

            // Notify listeners
            changeSupport.firePropertyChange("category", null, null);
            fireCategoryRemoved(index, category);
            break;
         }
         ++index;
      }
   }

   /**
    * Gets the help text for this ChunkDesc if such help is defined.
    *
    * @return  the help text or empty string if not defined
    */
   public final String getHelp()
   {
      String help = "";
      Element help_child = mDomElement.getChild("help");

      if ( null != help_child )
      {
         help = help_child.getText();
      }

      return help;
   }

   /**
    * Sets the help text for this ChunkDesc.
    */
   public final void setHelp(String helpText)
   {
      Element help_child = mDomElement.getChild("help");

      if ( null == help_child )
      {
         help_child = new Element("help");
         mDomElement.addContent(help_child);
      }

      String old = help_child.getText();
      help_child.setText(helpText);
      changeSupport.firePropertyChange("help", null, null);
      fireHelpChanged();
   }

   public void addPropertyDesc(PropertyDesc newPropDesc)
   {
      mDomElement.addContent(newPropDesc.getNode());
      newPropDesc.addPropertyChangeListener(this);
      firePropertyDescAdded(newPropDesc);
   }

   public void removePropertyDesc(PropertyDesc propDesc)
   {
      mDomElement.removeContent(propDesc.getNode());
      propDesc.removePropertyChangeListener(this);
      firePropertyDescRemoved(propDesc);
   }

   public Vector getPropertyDescs()
   {
      Vector prop_descs = new Vector();
      Iterator i =
         mDomElement.getChildren(property_desc_TOKEN).iterator();

      while ( i.hasNext() )
      {
         PropertyDesc desc = new PropertyDesc((Element)i.next());
         desc.addPropertyChangeListener(this);
         prop_descs.add(desc);
      }

      return prop_descs;
   }

   /**
    * Returns the number of property descriptions associated with this chunk
    * description.
    */
   public int propertyDescsSize()
   {
      return mDomElement.getChildren(property_desc_TOKEN).size();
   }

   /**
    * Retrives the i'th property description.  If no such property description
    * exists, null is returned.
    */
   public PropertyDesc getPropertyDesc(int i)
   {
      PropertyDesc prop_desc = null;

      // Verify that i is a valid index.
      if ( i < this.propertyDescsSize() )
      {
         Element prop_desc_child =
            (Element)mDomElement.getChildren(property_desc_TOKEN).get(i);
         prop_desc = new PropertyDesc(prop_desc_child);
         prop_desc.addPropertyChangeListener(this);
      }

      return prop_desc;
   }

   /**
    * Tests if this ChunkDesc is equal to the given ChunkDesc. They are
    * considered equal if they use the same DOM element object.
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof ChunkDesc)
      {
         return mDomElement == ((ChunkDesc)obj).mDomElement;
      }
      return false;
   }

   public PropertyDesc getPropertyDesc(String propDescToken)
   {
      Iterator i = mDomElement.getChildren(property_desc_TOKEN).iterator();
      PropertyDesc p = null;

      while ( i.hasNext() )
      {
         Element prop_desc_elem = (Element) i.next();
         String token = prop_desc_elem.getAttribute("token").getValue();

         if ( token.equals(propDescToken) )
         {
            p = new PropertyDesc(prop_desc_elem);
            p.addPropertyChangeListener(this);
            break;
         }
      }

      return p;
   }

   /**
    * Returns the internal DOM node for this chunk description.  It is
    * package visible to prevent abuse of this exposure of implementation
    * by outside packages.
    */
   Element getNode()
   {
      return mDomElement;
   }

   /**
    * Gets the XML representation of this ConfigChunk as a String. This is
    * suitable for a replacement to serialization.
    */
   public String toString()
   {
      org.jdom.output.XMLOutputter outputter =
         new org.jdom.output.XMLOutputter("   ", true);
      outputter.setLineSeparator(System.getProperty("line.separator"));
      return outputter.outputString(mDomElement);
   }

   /**
    * Called when a property desc contained within this chunk desc has changed.
    */
   public void propertyChange(PropertyChangeEvent evt)
   {
      PropertyDesc desc = (PropertyDesc)evt.getSource();
      System.out.println("ChunkDesc.propertyChange src="+desc.getToken()+", prop="+evt.getPropertyName());
      firePropertyDescChanged(desc);
//      changeSupport.firePropertyChange(evt.getPropertyName(), null, desc);
   }

   /**
    * Adds the given listener to be notified when this chunk desc's properties
    * change.
    */
   public void addPropertyChangeListener(PropertyChangeListener listener)
   {
      changeSupport.addPropertyChangeListener(listener);
   }

   /**
    * Removes the given listener that was registered to be notified when this
    * chunk descs's properties change.
    */
   public void removePropertyChangeListener(PropertyChangeListener listener)
   {
      changeSupport.removePropertyChangeListener(listener);
   }

   /**
    * Adds the given listener to be notified when this ChunkDesc has been
    * modified.
    */
   public void addChunkDescListener(ChunkDescListener listener)
   {
      listenerList.add(ChunkDescListener.class, listener);
   }

   /**
    * Removes the given listener from this ChunkDesc.
    */
   public void removeChunkDescListener(ChunkDescListener listener)
   {
      listenerList.remove(ChunkDescListener.class, listener);
   }

   /**
    * Notifies listeners of this chunk desc that the name has been changed.
    */
   protected void fireNameChanged()
   {
      String name = getName();

      ChunkDescEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            if (evt == null)
            {
               evt = new ChunkDescEvent(this, name, 0);
            }
            ((ChunkDescListener)listeners[i+1]).nameChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners of this chunk desc that the token has been changed.
    */
   protected void fireTokenChanged()
   {
      String token = getToken();

      ChunkDescEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            if (evt == null)
            {
               evt = new ChunkDescEvent(this, token, 0);
            }
            ((ChunkDescListener)listeners[i+1]).tokenChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners of this chunk desc that the help has been changed.
    */
   protected void fireHelpChanged()
   {
      String help = getHelp();

      ChunkDescEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            if (evt == null)
            {
               evt = new ChunkDescEvent(this, help, 0);
            }
            ((ChunkDescListener)listeners[i+1]).helpChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners of this chunk desc that a category has been added.
    */
   protected void fireCategoryAdded(int index)
   {
      String category = getCategory(index);

      ChunkDescEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            if (evt == null)
            {
               evt = new ChunkDescEvent(this, category, index);
            }
            ((ChunkDescListener)listeners[i+1]).categoryAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners of this chunk desc that a category has been removed.
    */
   protected void fireCategoryRemoved(int index, String oldCategory)
   {
      ChunkDescEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            if (evt == null)
            {
               evt = new ChunkDescEvent(this, oldCategory, index);
            }
            ((ChunkDescListener)listeners[i+1]).categoryRemoved(evt);
         }
      }
   }

   /**
    * Notifies listeners of this chunk desc that the given contained property
    * desc has been modified in some way.
    */
   protected void firePropertyDescChanged(PropertyDesc desc)
   {
      ChunkDescEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            if (evt == null)
            {
               evt = new ChunkDescEvent(this, desc);
            }
            ((ChunkDescListener)listeners[i+1]).propertyDescChanged(evt);
         }
      }
   }

   /**
    * Notifies listeners of this chunk desc that the given contained property
    * desc has been added to this chunk desc.
    */
   protected void firePropertyDescAdded(PropertyDesc desc)
   {
      ChunkDescEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            if (evt == null)
            {
               evt = new ChunkDescEvent(this, desc);
            }
            ((ChunkDescListener)listeners[i+1]).propertyDescAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners of this chunk desc that the given contained property
    * desc has been removed from this chunk desc.
    */
   protected void firePropertyDescRemoved(PropertyDesc desc)
   {
      ChunkDescEvent evt = null;
      Object[] listeners = listenerList.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ChunkDescListener.class)
         {
            if (evt == null)
            {
               evt = new ChunkDescEvent(this, desc);
            }
            ((ChunkDescListener)listeners[i+1]).propertyDescRemoved(evt);
         }
      }
   }

   /**
    * The XML DOM element that contains this ChunkDesc's properties.
    */
   private Element mDomElement = null;

   /**
    * Support for handling property changes and submitting them to listeners.
    */
   private PropertyChangeSupport changeSupport = null;

   private EventListenerList listenerList = new EventListenerList();
}
