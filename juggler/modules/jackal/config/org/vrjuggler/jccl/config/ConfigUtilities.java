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

import java.util.*;
import org.jdom.Element;

/**
 * A collection of commonly-used utility methods for use in manipulating
 * configuration elements.
 */
public class ConfigUtilities
{
   /**
    * Looks through the given list of ChunkDescs and finds the ones that have
    * the given token.
    *
    * @param descs      the list of ChunkDescs to search through
    * @param token      the token for which to match ChunkDescs
    *
    * @return  a list of the ChunkDescs that match
    */
   public static List getDescsWithToken(List descs, String token)
   {
      List results = new ArrayList();
      for (Iterator itr = descs.iterator(); itr.hasNext(); )
      {
         ChunkDesc desc = (ChunkDesc)itr.next();
         if (desc.getToken().equals(token))
         {
            results.add(desc);
         }
      }
      return results;
   }

   /**
    * Looks through the given list of ChunkDescs and finds the ones that have
    * the given name.
    *
    * @param descs      the list of ChunkDescs to search through
    * @param name       the name for which to match ChunkDescs
    *
    * @return  a list of the ChunkDescs that match
    */
   public static List getDescsWithName(List descs, String name)
   {
      List results = new ArrayList();
      for (Iterator itr = descs.iterator(); itr.hasNext(); )
      {
         ChunkDesc desc = (ChunkDesc)itr.next();
         if (desc.getName().equals(name))
         {
            results.add(desc);
         }
      }
      return results;
   }

   /**
    * Looks through the given list of ConfigChunks and finds the ones that have
    * the given name.
    *
    * @param chunks     the list of ConfigChunks to search through
    * @param name       the name for which to match ConfigChunks
    *
    * @return  a list of the ConfigChunks that match
    */
   public static List getChunksWithName(List chunks, String name)
   {
      List results = new ArrayList();
      for (Iterator itr = chunks.iterator(); itr.hasNext(); )
      {
         ConfigChunk chunk = (ConfigChunk)itr.next();
         if (chunk.getName().equals(name))
         {
            results.add(chunk);
         }
      }
      return results;
   }

   /**
    * Looks through the given list of ConfigChunks and finds the ones that are
    * defined by a chunk description with the given token.
    *
    * @param chunks     the list of ConfigChunks to search through
    * @param token      the token of the ChunkDesc to match
    *
    * @return  a list of the ConfigChunks that match
    */
   public static List getChunksWithDescToken(List chunks, String token)
   {
      List results = new ArrayList();
      for (Iterator itr = chunks.iterator(); itr.hasNext(); )
      {
         ConfigChunk chunk = (ConfigChunk)itr.next();
         if (chunk.getDesc().getToken().equals(token))
         {
            results.add(chunk);
         }
      }
      return results;
   }

   /**
    * Looks through the given list of ConfigChunks and finds the ones that are
    * defined by the given chunk description. This is equivalent to calling
    * <code>getChunksWithDescToken(chunks, desc.getToken())</code>.
    *
    * @param chunks     the list of ConfigChunks to search through
    * @param desc       the ChunkDesc to match against
    *
    * @return  a list of the ConfigChunks that match
    */
   public static List getChunksWithDescToken(List chunks, ChunkDesc desc)
   {
      return getChunksWithDescToken(chunks, desc.getToken());
   }

   /**
    * Given a DOM element, the name of an attribute in that element, and the
    * type of the value that is supposed to be stored in the attribute, this
    * method will return the value in that attribute as the correct type.
    *
    * @param element    the DOM element to examine
    * @param attrName   the name of the attribute in the element
    * @param type       the type of the value stored in the attribute
    *
    * @return  the value of the attribute as the correct class
    */
   public static Object getAttributeValue(Element element, String attrName,
                                          ValType type)
   {
      String value_string = element.getAttribute(attrName).getValue();
      return makeProperty(value_string, type);
   }

   /**
    * Given the string representation of a property value and the type it is
    * supposed to be, return the value as the correct class.
    *
    * @param valueString   the string representation of the value
    * @param type          the type of the value
    *
    * @return  the value as the correct class
    */
   public static Object makeProperty(String valueString, ValType type)
   {
      // Covert the attribute to the appropriate type
      if (type == ValType.STRING)
      {
         return new String(valueString);
      }
      else if (type == ValType.FLOAT)
      {
         try
         {
            return new Float(valueString);
         }
         catch (NumberFormatException nfe)
         {
            return new Float(0.0f);
         }
      }
      else if (type == ValType.INT)
      {
         try
         {
            return new Integer(valueString);
         }
         catch (NumberFormatException nfe)
         {
            return new Integer(0);
         }
      }
      else if (type == ValType.BOOL)
      {
         return new Boolean(valueString);
      }
      else if (type == ValType.CHUNK)
      {
         return new String(valueString);
      }
      else
      {
         throw new IllegalArgumentException("Invalid type");
      }
   }
}
