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
}
