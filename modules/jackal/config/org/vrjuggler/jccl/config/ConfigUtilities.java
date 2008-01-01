/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
package org.vrjuggler.jccl.config;

import java.util.*;
import org.jdom.Element;

/**
 * A collection of commonly-used utility methods for use in manipulating
 * configuration elements and definitions
 */
public class ConfigUtilities
{
   /**
    * Looks through the given list of configuration definitions and finds the
    * ones that have the given token.
    *
    * @param defs       the list of definitions to search through
    * @param token      the token for which to match definitions
    *
    * @return  a list of the definitions that match
    */
   public static List getDefinitionsWithToken(List defs, String token)
   {
      List results = new ArrayList();
      for (Iterator itr = defs.iterator(); itr.hasNext(); )
      {
         ConfigDefinition def = (ConfigDefinition)itr.next();
         if (def.getToken().equals(token))
         {
            results.add(def);
         }
      }
      return results;
   }

   /**
    * Looks through the given list of configuration definitions and finds the
    * ones that have the given name.
    *
    * @param defs       the list of definitions to search through
    * @param name       the name for which to match definitions
    *
    * @return  a list of the definitions that match
    */
   public static List getDefinitionsWithName(List defs, String name)
   {
      List results = new ArrayList();
      for (Iterator itr = defs.iterator(); itr.hasNext(); )
      {
         ConfigDefinition def = (ConfigDefinition)itr.next();
         if (def.getName().equals(name))
         {
            results.add(def);
         }
      }
      return results;
   }

   /**
    * Returns a list of all the <code>ConfigDefinition</code> objects that
    * are of the given type.  The base types of the config definitions are
    * searched, which means that this performs a "polymorphic" search through
    * the given list of config definitions.  For an exact type match, use
    * <code>getDefinitionsWithToken(List,String)</code>.
    *
    * @param defs               the list of config definitions to search
    * @param baseTypeToken      the type name used for the polymorphic
    *                           comparison
    *
    * @see #getDefinitionsWithToken(List,String)
    *
    * @since 0.92.9
    */
   public static List getDefinitionsOfType(List defs, String baseTypeToken)
   {
      List results = new ArrayList();

      for ( Iterator d = defs.iterator(); d.hasNext(); )
      {
         ConfigDefinition def = (ConfigDefinition) d.next();
         if ( def.isOfType(baseTypeToken) )
         {
            results.add(def);
         }
      }

      return results;
   }

   /**
    * Returns a list of all the <code>ConfigDefinition</code> objects that
    * are of the given type.  The base types of the config definitions are
    * searched, which means that this performs a "polymorphic" search through
    * the given list of config definitions.  For an exact type match, use
    * <code>getDefinitionsWithToken(List,String)</code>.
    *
    * @param defs       the list of config definitions to search
    * @param baseType   the type used for the polymorphic comparison
    *
    * @see #getDefinitionsOfType(List,String)
    *
    * @since 0.92.9
    */
   public static List getDefinitionsOfType(List defs,
                                           ConfigDefinition baseType)
   {
      return getDefinitionsOfType(defs, baseType.getToken());
   }

   /**
    * Looks through the given list of configuration elements and finds the ones
    * that have the given name.
    *
    * @param elts       the list of elements to search through
    * @param name       the name for which to match elements
    *
    * @return  a list of the elements that match
    */
   public static List getElementsWithName(List elts, String name)
   {
      List results = new ArrayList();
      for (Iterator itr = elts.iterator(); itr.hasNext(); )
      {
         ConfigElement elt = (ConfigElement)itr.next();
         if (elt.getName().equals(name))
         {
            results.add(elt);
         }
      }
      return results;
   }

   /**
    * Looks through the given list of configuration elements and finds the ones
    * that are defined by a configuration definition with the given token.
    *
    * @param elts       the list of elements to search through
    * @param token      the token of the definition to match
    *
    * @return  a list of the elements that match
    */
   public static List getElementsWithDefinition(List elts, String token)
   {
      List results = new ArrayList();
      for (Iterator itr = elts.iterator(); itr.hasNext(); )
      {
         ConfigElement elt = (ConfigElement)itr.next();
         if (elt.getDefinition().getToken().equals(token))
         {
            results.add(elt);
         }
      }
      return results;
   }

   /**
    * Looks through the given list of configuration elements and finds the ones
    * that are defined by the given configuraition definition. This is
    * equivalent to calling <code>getElementsWithDefinition(elts,
    * def.getToken())</code>.
    *
    * @param elts       the list of elements to search through
    * @param def        the definition to match against
    *
    * @return  a list of the elements that match
    */
   public static List getElementsWithDefinition(List elts, ConfigDefinition def)
   {
      return getElementsWithDefinition(elts, def.getToken());
   }

   /**
    * Gets all embedded elements within the given element recursively.
    *
    * @param src        the element in which to retrieve embedded elements
    */
   public static List getEmbeddedElementsRecursive(ConfigElement src)
   {
      List result = new ArrayList();

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
         result.add(elt);

         // Look for embedded elements within the current element
         result.addAll(getEmbeddedElementsRecursive(elt));
      }

      return result;
   }
}
