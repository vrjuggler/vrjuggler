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

import java.io.*;
import java.util.Iterator;
import java.util.Vector;
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
{
   public ChunkDesc(Element root)
   {
      this.mDomElement = root;
   }

   /**
    * Creates an empty chunk description with only a name property. The name and
    * token default to the empty string.
    */
   public ChunkDesc()
   {
      mDomElement = new Element(ConfigTokens.chunk_desc_TOKEN);
      this.setName("");
      this.setToken("");
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
         name = mDomElement.getAttribute("token").getValue();
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
      mDomElement.setAttribute("name", newName);
   }

   /**
    * Gets the unique token for this ChunkDesc.
    */
   public final String getToken()
   {
      return mDomElement.getAttribute("token").getValue();
   }

   /**
    * Sets the unique token for this ChunkDesc.
    */
   public final void setToken(String newToken)
   {
      mDomElement.setAttribute("token", newToken);
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

      help_child.setText(helpText);
   }

   public void addPropertyDesc(PropertyDesc newPropDesc)
   {
      mDomElement.addContent(newPropDesc.getNode());
   }

   public void removePropertyDesc(PropertyDesc propDesc)
   {
      mDomElement.removeContent(propDesc.getNode());
   }

   public Vector getPropertyDescs()
   {
      Vector prop_descs = new Vector();
      Iterator i =
         mDomElement.getChildren(ConfigTokens.property_desc_TOKEN).iterator();

      while ( i.hasNext() )
      {
         prop_descs.add(new PropertyDesc((Element)i.next()));
      }

      return prop_descs;
   }

   /**
    * Returns the number of property descriptions associated with this chunk
    * description.
    */
   public int propertyDescsSize()
   {
      return mDomElement.getChildren(ConfigTokens.property_desc_TOKEN).size();
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
            (Element)mDomElement.getChildren(ConfigTokens.property_desc_TOKEN).get(i);
         prop_desc = new PropertyDesc(prop_desc_child);
      }

      return prop_desc;
   }

   /**
    * Tests if this ChunkDesc is equal to the given ChunkDesc. They are
    * considered equal if they use the same DOM element object.
    */
   public boolean equals(ChunkDesc c)
   {
      return (mDomElement == c.mDomElement);
   }

   public PropertyDesc getPropertyDesc(String propDescToken)
   {
      Iterator i = mDomElement.getChildren(ConfigTokens.property_desc_TOKEN).iterator();
      PropertyDesc p = null;

      while ( i.hasNext() )
      {
         Element prop_desc_elem = (Element) i.next();
         String token = prop_desc_elem.getAttribute("token").getValue();

         if ( token.equals(propDescToken) )
         {
            p = new PropertyDesc(prop_desc_elem);
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
    * The XML DOM element that contains this ChunkDesc's properties.
    */
   private Element mDomElement = null;
}
