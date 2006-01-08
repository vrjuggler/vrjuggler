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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
package org.vrjuggler.tweek.wizard;

import java.io.*;
import java.net.*;
import java.util.*;
import java.util.jar.*;

/**
 * A WizardInputStream reads a Wizard from an InputStream. All the WizardSteps
 * along with the classes they require are read so that the Wizard can be
 * successfully loaded in with the all the resources it needs.
 */
public class WizardInputStream
   extends JarInputStream
{
   /**
    * Creates an WizardInputStream that reads from the specified InputStream.
    */
   public WizardInputStream(String jarfile)
      throws IOException
   {
      super(new BufferedInputStream(new FileInputStream(jarfile)));
      mJar = new JarFile(jarfile);
   }

   /**
    * Reads the specified wizard from this stream.
    */
   public Wizard readWizard()
      throws IOException
   {
      Wizard wizard = null;
      WizardClassLoader class_loader =
               new WizardClassLoader(getClass().getClassLoader(), mJar);

      try
      {
         JarEntry entry = getNextJarEntry();
         while (entry != null)
         {
            System.out.println("Looking at: " + entry.getName());
            // We've run into the wizard entry
            if (entry.getName().equals("wizard.ser"))
            {
               wizard = readSerializedWizard(class_loader);
            }

            closeEntry();
            entry = getNextJarEntry();
         }
      }
      catch (EOFException e)
      {
         System.err.println("Reached EOF!");
      }

      return wizard;
   }

   /**
    * Deserializes the wizard stored in the stream using the given class loader.
    */
   private Wizard readSerializedWizard(ClassLoader classLoader)
      throws IOException
   {
      try
      {
         ObjectInputStream obj_in = new WizardObjectInputStream(this, classLoader);
         return (Wizard)obj_in.readObject();
      }
      catch (ClassNotFoundException cnfe)
      {
         cnfe.printStackTrace();
         return null;
      }
   }

   /**
    * Converts a fully-qualified class name to the appropriate filename.
    */
   private String classToFile(String classname)
   {
      return classname.replace('.', '/') + ".class";
   }

   private JarFile mJar;
}

/**
 * This customized ClassLoader is responsible for providing a way for custom
 * wizard step classes used by the wizard along with the resources needed for
 * those steps to be retrieved correctly from the wizard JAR.
 */
class WizardClassLoader
   extends ClassLoader
{
   public WizardClassLoader(ClassLoader parent, JarFile jar)
   {
      super(parent);
      mJar = jar;
   }

   public Class findClass(String name)
      throws ClassNotFoundException
   {
      System.out.println("[WizardClassLoader] Finding class: " + name);
      byte[] b = loadClassData(name);
      return defineClass(name, b, 0, b.length);
   }

   private byte[] loadClassData(String name)
      throws ClassNotFoundException
   {
      JarEntry entry = mJar.getJarEntry(classToFile(name));
      if (entry == null)
      {
         throw new ClassNotFoundException(name);
      }

      // Get the bytes for the entry
      int size = (int)entry.getSize();
      byte[] bytes = new byte[size];

      // Read in the class data
      try
      {
         InputStream in = mJar.getInputStream(entry);
         int bytes_read = 0;

         while (bytes_read < size)
         {
            int count = in.read(bytes, bytes_read, size - bytes_read);
            if (count == -1)
            {
               break;
            }
            else
            {
               bytes_read += count;
            }
         }
      }
      catch (IOException ioe)
      {
         ioe.printStackTrace();
         bytes = null;
      }

      return bytes;
   }

   protected URL findResource(String name)
   {
      URL path = null;

      JarEntry entry = mJar.getJarEntry(name);
      if (entry != null)
      {
         try
         {
            path = new URL("jar:file:" + mJar.getName() + "!/" + name);
         }
         catch (MalformedURLException mue)
         {
            System.err.println("WARNING: Invalid URL contructed when " +
                               "trying to find resource " + name + " in " +
                               mJar.getName());
         }
      }

      return path;
   }

   private String classToFile(String classname)
   {
      return classname.replace('.', '/') + ".class";
   }

   private JarFile mJar;
}

/**
 * Customization of ObjectInputStream that uses a particular class loader to
 * resolve classes when deserializing objects.
 */
class WizardObjectInputStream
   extends ObjectInputStream
{
   public WizardObjectInputStream(InputStream in, ClassLoader classLoader)
      throws IOException
   {
      super(in);
      mClassLoader = classLoader;
   }

   protected Class resolveClass(ObjectStreamClass desc)
      throws IOException, ClassNotFoundException
   {
      String name = desc.getName();
      try
      {
         return mClassLoader.loadClass(name);
      }
      catch (ClassNotFoundException cnfe)
      {
         return super.resolveClass(desc);
      }
   }

   private ClassLoader mClassLoader;
}
