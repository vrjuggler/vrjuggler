/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.beans.loader;

import java.io.InputStream;
import java.io.IOException;
import java.net.JarURLConnection;
import java.util.Iterator;
import java.util.List;
import java.util.jar.*;
import java.util.jar.JarEntry;


/**
 * This class provides a simple interface to loading JavaBeans from JAR files.
 */
public class BeanLoader
{
   /**
    * Loads the JavaBean contained in the JAR file pointed to by the given
    * java.io.File argument.
    */
   public void loadBeanFromJar (java.io.File file, List deps, String class_path)
      throws IOException
   {
      if ( file.isFile() )
      {
         loadBeanFromJar("file:" + file.getAbsoluteFile(), deps, class_path);
      }
   }

   /**
    * Loads the JavaBean contained in the JAR file found at the given URL.
    * The URL should be a "normal" URL rather than a JAR URL.
    */
   public void loadBeanFromJar (String url, List deps, String class_path)
      throws java.net.MalformedURLException, IOException
   {
      java.net.URL jar_url = new java.net.URL("jar:" + url + "!/");
      JarURLConnection jar_conn = (JarURLConnection) jar_url.openConnection();

      mBeanJar = new BeanJarFile(jar_conn.getJarFile(), deps, class_path);
   }

   /**
    * Creates an instance of the loaded Bean.
    *
    * Pre-condition: loadBeanFromJar() loaded the JAR file successfully.
    *
    * @see #loadBeanFromJar(String,List,String)
    */
   public Object instantiate (BeanJarClassLoader loader, String bean_class)
      throws BeanInstantiationException
   {
      int index;
      Object obj = null;

      if ( ! verifyBean(bean_class) )
      {
         throw new BeanInstantiationException("Bean not found in " +
                                              mBeanJar.getJarFile().getName());
      }

      loader.addJarFile(mBeanJar.getJarFile());
      loader.extendClassPath(mBeanJar.getClassPath());
      List remainder = loader.resolveDependencies(mBeanJar.getDependencies());

      if ( remainder.size() != 0 )
      {
         System.err.println("The following dependencies are unresolved:");

         for ( Iterator i = remainder.iterator(); i.hasNext(); )
         {
            System.err.println("\t" + (String) i.next());
         }
      }

      try
      {
         obj = java.beans.Beans.instantiate(loader, bean_class);
      }
      catch (IOException e)
      {
         e.printStackTrace();
      }
      catch (ClassNotFoundException e)
      {
         System.err.println("ERROR: Could not instantiate Bean of type " +
                            bean_class + "\n" +
                            "       " + e.getMessage());

         if ( e.getCause() != null )
         {
            System.err.println("       Cause: " + e.getCause().getMessage());
//            e.getCause().printStackTrace();
         }
         else if ( e.getException() != null )
         {
            System.err.println("       Cause: " +
                               e.getException().getMessage());
//            e.getException().printStackTrace();
         }

         throw new BeanInstantiationException("Instantiation of " + bean_class +
                                              " failed!\n(" +
                                              "Class " + e.getMessage() +
                                              " not found", e);
      }
      catch (NoClassDefFoundError e)
      {
         System.err.println("ERROR: Could not instantiate Bean of type " +
                            bean_class);
         System.err.println("       No definition found for class " +
                            e.getMessage());
//         e.printStackTrace();
         throw new BeanInstantiationException("Instantiation of " + bean_class +
                                              " failed!\n" +
                                              "No definition found for class " +
                                              e.getMessage(), e);
      }

      return obj;
   }

   // =========================================================================
   // Protected methods.
   // =========================================================================

   protected boolean verifyBean (String bean_class)
   {
      boolean found   = false;
      String entry_name = bean_class.replace('.', '/');
      JarEntry entry  = mBeanJar.getJarFile().getJarEntry(entry_name + ".class");

      if ( entry == null )
      {
         entry = mBeanJar.getJarFile().getJarEntry(entry_name + ".ser");

         if ( entry != null )
         {
            found = true;
         }
      }
      else
      {
         found = true;
      }

/* XXX: This doesn't work for some reason ...
      // If the class is in the JAR file, ensure that it identifies itself as
      // a JavaBean.
      if ( found )
      {
         try
         {
            Attributes attr = entry.getAttributes();

            if ( attr == null ||
                 ! attr.containsKey(new Attributes.Name("Java-Bean")) ||
                 ! attr.getValue("Java-Bean").equalsIgnoreCase("true") )
            {
               found = false;
            }
         }
         catch (IOException e)
         {
            found = false;
         }
      }
*/

      return found;
   }

   // =========================================================================
   // Private data members.
   // =========================================================================

   private BeanJarFile mBeanJar = null; /**< The JAR file holding the Bean */
}
