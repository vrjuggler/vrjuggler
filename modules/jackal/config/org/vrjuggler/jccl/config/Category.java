/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

/**
 * Defines a particular category for a configuration definition. A category
 * object is considered immutable.
 */
public class Category
{
   /**
    * Creates a new category using the given path. A category path is split by
    * forward slashes.
    *
    * @param path         the / delimited category path
    *
    * @throws IllegalArgumentException
    *            if the path does not start with a "/"
    */
   public Category(String path)
      throws IllegalArgumentException
   {
      // Make sure the path starts with a /
      if (!path.startsWith("/"))
      {
         throw new IllegalArgumentException("Path must start with a '/'");
      }

      // Simple case for the root
      if (path.equals("/"))
      {
         setPath(new String[] {});
      }
      // Build up the path as a list by splitting on the /
      else
      {
         List path_list = new ArrayList();
         int idx = 1;
         while (idx != 0)
         {
            String name;
            int next_idx = path.indexOf('/', idx);
            if (next_idx != -1)
            {
               name = path.substring(idx, next_idx);
            }
            else
            {
               name = path.substring(idx);
            }
            // The name will be empty if we reached path ends with a '/' or if
            // it has two '/'s in a row. In this case we ignore the slash.
            if (!name.equals(""))
            {
               path_list.add(name);
            }
            idx = next_idx + 1;
         }

         // Init this object to the path list
         setPath((String[])path_list.toArray(new String[] {}));
      }
   }

   /**
    * Creates a new category using the given path.
    *
    * @param path         the category path from root to leaf
    */
   public Category(String[] path)
   {
      this(path, path.length);
   }

   /**
    * Creates a new category using the given subset of the given path.
    *
    * @param path         the category path from root to leaf
    * @param length       the number of items in the path to use.
    */
   public Category(String[] path, int length)
   {
      String[] new_path = new String[length];
      for (int i=0; i<length; ++i)
      {
         new_path[i] = path[i];
      }
      setPath(new_path);
   }

   /**
    * Gets the name for this category. This corresponds to the last element in
    * the path. The root category has the special name "/".
    */
   public String getName()
   {
      // Case: this is the root category
      if (mPath.length == 0)
      {
         return "/";
      }

      // Case: any other category
      return mPath[mPath.length - 1];
   }

   /**
    * Gets this category's parent.
    *
    * @return  this category's parent or null if this category is the root
    */
   public Category getParent()
   {
      // Return null if this category has no parent
      if (mPath.length == 0)
      {
         return null;
      }

      // Return the parent category
      return new Category(mPath, mPath.length-1);
   }

   /**
    * Gets the string representation of this category.
    *
    * @return  this category with the path delimited by '/'
    */
   public String toString()
   {
      StringBuffer sb = new StringBuffer("/");
      for (int i=0; i<mPath.length; ++i)
      {
         sb.append(mPath[i]).append('/');
      }

      return sb.toString();
   }

   /**
    * Tests if this category equals the given one.
    *
    * @param obj          the object to test against
    *
    * @return  true if they equal, false otherwise
    */
   public boolean equals(Object obj)
   {
      if (obj instanceof Category)
      {
         Category cat = (Category)obj;
         if (mPath.length == cat.mPath.length)
         {
            // Test each element in the path list
            for (int i=0; i<mPath.length; ++i)
            {
               if (!mPath[i].equals(cat.mPath[i]))
               {
                  return false;
               }
            }
            // All elements in the path list are equal
            return true;
         }
      }
      return false;
   }

   /**
    * Sets the path for this category.
    *
    * @param path         the path to use for this category
    */
   private void setPath(String[] path)
   {
      mPath = path;
   }

   /** This category's path. */
   private String[] mPath;
}
