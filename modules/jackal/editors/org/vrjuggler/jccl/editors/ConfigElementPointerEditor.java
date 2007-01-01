/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
import org.vrjuggler.jccl.config.event.ConfigEvent;
import org.vrjuggler.jccl.config.event.ConfigListener;
import org.vrjuggler.tweek.beans.BeanRegistry;

/**
 * An editor that knows how to edit ConfigElementPointer properties in a
 * configuration element.
 */
public class ConfigElementPointerEditor
   extends PropertyEditorSupport
{
   /**
    * Gets the property value as text.
    */
   public String getAsText()
   {
      if (null == mValue || mValue.equals(""))
      {
         return "None";
      }
      return mValue;
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
         if (text.equals("None"))
         {
            mValue = "";
         }
         else
         {
            mValue = text;
         }
      }
   }

   /**
    * Gets a list of known values that this property may be.
    */
   public String[] getTags()
   {
      return (String[])mTags.toArray(new String[0]);
   }

   /**
    * Sets up the tags list based on the given definition of the property.
    */
   public void setPropertyDefinition(PropertyDefinition propDef, ConfigContext ctx)
   {
      mPropertyDefinition = propDef;
      mConfigContext = ctx;
      generateTags();
      
      ConfigBroker broker = new ConfigBrokerProxy();
      broker.addConfigListener(new ConfigListener()
         {
            public void configElementAdded(ConfigEvent evt)
            {
               if (mConfigContext.contains(evt.getResource()))
               {
                  generateTags();
               }
            }

            public void configElementRemoved(ConfigEvent evt)
            {
               if (mConfigContext.contains(evt.getResource()))
               {
                  generateTags();
               }
            }
         });
   }
   
   public void generateTags()
   {
      // For each definition token this definition can point to, look for
      // matching config elements we can use.
      mTags.clear();

      mTags.add("None");

      for (Iterator at_itr = mPropertyDefinition.getAllowedAndDerivedTypes().iterator(); at_itr.hasNext(); )
      {
         String type = (String)at_itr.next();

         List elements = getElementsWithToken(type);

         // Grab the name of each matching ConfigElement
         for (Iterator elt_itr = elements.iterator(); elt_itr.hasNext(); )
         {
            ConfigElement elt = (ConfigElement)elt_itr.next();
            mTags.add(elt.getFullName());
         }
      }
   }


   /**
    * Gets a list of the config elements in the current configuration that have
    * the definition with the given token.
    */
   private List getElementsWithToken(String token)
   {
      List matches = new ArrayList();
      List all_elements = new ArrayList();
      
      ConfigBroker broker = new ConfigBrokerProxy();
      // XXX: ACK! We really should look only in the current context, not all resources
      for (Iterator itr = mConfigContext.getResources().iterator(); itr.hasNext(); )
      {
         List elements = broker.getElementsIn((String)itr.next());

         // Get the elements embedded within the current element
         for (Iterator elt_itr = elements.iterator(); elt_itr.hasNext(); )
         {
            ConfigElement elt = (ConfigElement)elt_itr.next();
            all_elements.add(elt);
            all_elements.addAll(getEmbeddedElementsRecursive(elt));
         }

         // Cull out the elements that use the given desc token
         matches.addAll(ConfigUtilities.getElementsWithDefinition(all_elements, token));
      }

      // Sort alphabetically
      Collections.sort(matches, new Comparator()
      {
         public int compare(Object a, Object b)
         {
            if (a instanceof ConfigElement && b instanceof ConfigElement)
            {
               return ((ConfigElement)a).getFullName().compareTo(((ConfigElement)b).getFullName());
            }
            return 0;
         }
      });
      return matches;
   }

   /**
    * Gets all embedded elements within the given element recursively.
    *
    * @param src           the elemetn in which to retrieve embedded elements
    */
   private List getEmbeddedElementsRecursive(ConfigElement src)
   {
      List results = new ArrayList();

      // Get a list of all embedded elements
      List emb_elements = new ArrayList();
      for (Iterator itr = src.getDefinition().getPropertyDefinitions().iterator(); itr.hasNext(); )
      {
         PropertyDefinition prop_def = (PropertyDefinition)itr.next();
         if (prop_def.getType() == ConfigElement.class)
         {
            emb_elements.addAll(src.getPropertyValues(prop_def.getToken()));
         }
      }

      // Recurse on each newly found embedded element
      for (Iterator itr = emb_elements.iterator(); itr.hasNext(); )
      {
         ConfigElement elt = (ConfigElement)itr.next();
         results.add(elt);

         // Look for embedded elements within the current element
         results.addAll(getEmbeddedElementsRecursive(elt));
      }

      return results;
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
      this.mValue = (String)value;
      firePropertyChange();
   }

   /**
    * The String name of the Element pointer being edited.
    */
   private String mValue = null;

   /**
    * The set of tags supported by this value.  A java.util.Set is used here
    * instead of a java.util.List to protect against entries showing up
    * multiple times.  A valid configuration context should haev a unique
    * name for each config element, but the implementation of generateTags()
    * does not take steps to guarantee that it does not add the same item more
    * than once.  Using a set provides an inherent guarantee that each entry
    * will be unique and that duplicates will be filtered out.
    */
   private Set mTags = new LinkedHashSet();
   private ConfigContext mConfigContext = null;
   private PropertyDefinition mPropertyDefinition = null;
}
