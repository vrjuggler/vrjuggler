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
package org.vrjuggler.tweek.wizard.builder;

import java.io.*;
import java.util.*;
import java.util.jar.*;

import org.vrjuggler.tweek.wizard.WizardStep;

/**
 * This class looks for classes in a JAR that implement the WizardStep
 * interface.
 */
public class WizardStepFinder
{
   /**
    * Creates a new wizard step finder.
    */
   public WizardStepFinder()
   {
      mClassLoader = new JarClassLoader(getClass().getClassLoader());
   }

   /**
    * Returns a list of all the classes in the given JAR file that are
    * WizardSteps.
    */
   public List find(JarFile jar)
      throws IOException
   {
      List steps = new ArrayList();

      Class wizardstep_class = WizardStep.class;

      mClassLoader.setJar(jar);

      // Enumerate through the classes in the JAR file looking for classes that
      // implement WizardStep.
      for (Enumeration e = jar.entries(); e.hasMoreElements(); )
      {
         JarEntry entry = (JarEntry)e.nextElement();
         // Ignore directories
         if (! entry.isDirectory())
         {
            // Check if this file is a WizardStep
            if (isAssignableTo(entry, wizardstep_class))
            {
               steps.add(fileToClass(entry.getName()));
            }
         }
      }

      return steps;
   }

   /**
    * Checks to see if the given jar entry is a class that implements the given
    * class type.
    */
   private boolean isAssignableTo(JarEntry entry, Class test_class)
   {
      // Make sure the entry corresponds to a class
      String filename = entry.getName();
      if (! filename.endsWith(".class"))
      {
         return false;
      }

      // Turn the filename into a fully qualified classname
      String class_name = fileToClass(filename);

      try
      {
         // Try to load in the Class object for the class for this entry
         Class entry_class = Class.forName(class_name, false, mClassLoader);

         // Check if the class is assignable from the test class
         if (test_class.isAssignableFrom(entry_class))
         {
            return true;
         }
      }
      catch (ClassNotFoundException cnfe)
      {
         System.out.println("Could not find class: "+cnfe.getMessage());
         // Just assume this Jar file sucks and fail silently
      }
      return false;
   }

   /**
    * Converts a filename to a classname.
    */
   private String fileToClass(String filename)
   {
      String classname = filename.substring(0, filename.length() - 6);
      return classname.replace('/', '.');
   }

   /** The customized ClassLoader for this finder. */
   private JarClassLoader mClassLoader;

   /**
    * A customized class loader that will search for classes in a Jar file.
    */
   class JarClassLoader
      extends ClassLoader
   {
      public JarClassLoader(ClassLoader parent)
      {
         super(parent);
      }

      public void setJar(JarFile jar)
      {
         mJar = jar;
      }

      public Class findClass(String classname)
         throws ClassNotFoundException
      {
         Class loaded_class = null;

         try
         {
            byte[] b = loadClassData(classname);
            loaded_class = defineClass(classname, b, 0, b.length);
         }
         catch (IOException e)
         {
            System.err.println("ERROR: Could not load class data for " + classname);
         }

         return loaded_class;
      }

      protected byte[] loadClassData(String classname)
         throws IOException, ClassNotFoundException
      {
         // Try to find the class in the JAR file
         String entry_name = classname.replace('.', '/') + ".class";
         JarEntry entry = mJar.getJarEntry(entry_name);
         if (entry == null)
         {
            throw new ClassNotFoundException("Could not find class " + classname);
         }

         // Get a stream to the class file
         InputStream in = mJar.getInputStream(entry);
         int size = (int)entry.getSize();

         // Read in the class data
         byte[] class_bytes = new byte[size];

         try
         {
            int bytes = 0, count;

            // Read in those bytes!
            while ( bytes < size )
            {
               count = in.read(class_bytes, bytes, size - bytes);

               if ( count == -1 )
               {
                  break;
               }
               else
               {
                  bytes += count;
               }
            }
         }
         catch (IOException ioe)
         {
            ioe.printStackTrace();
            class_bytes = null;
         }

         return class_bytes;
      }

      private JarFile mJar;
   }
}
