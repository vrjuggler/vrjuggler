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
package org.vrjuggler.tweek.wizard;

import java.io.*;
import java.util.*;
import java.util.jar.*;

/**
 * A WizardOutputStream writes a Wizard to an OutputStream. All the WizardSteps
 * along with the classes they require are written out so that the Wizard can be
 * successfully loaded in later with the all the resources it needs to work in a
 * separate VM.
 */
public class WizardOutputStream
   extends BufferedOutputStream
{
   /**
    * Creates an WizardOutputStream that writes to the specified OutputStream.
    */
   public WizardOutputStream(OutputStream out)
   {
      super(out);
   }

   /**
    * Write the specified wizard to this stream.
    */
   public void writeWizard(Wizard wizard)
      throws IOException
   {
      // Get a list of all the classes this wizard needs
      List classes = wizard.getRequiredClasses();
      classes.addAll(getInnerClasses(classes));
      stripDuplicates(classes);

      // Get a list of all the resources this wizard needs
      List resources = wizard.getRequiredResources();
      stripDuplicates(resources);

      // Start a JAR stream adding all the classes and resources
      JarOutputStream jar_out = new JarOutputStream(this);
      writeClasses(jar_out, classes);
      writeResources(jar_out, resources);

      // Add the serialized wizard to the JAR
      jar_out.putNextEntry(new JarEntry("wizard.ser"));
      ObjectOutputStream obj_out = new ObjectOutputStream(jar_out);
      obj_out.writeObject(wizard);
      obj_out.flush();
      jar_out.closeEntry();

      // Close the stream
      jar_out.close();
   }

   /**
    * Adds all the classes in the given list to the given jar output stream.
    */
   private void writeClasses(JarOutputStream out, List classes)
      throws IOException
   {
      try
      {
         for (Iterator itr = classes.iterator(); itr.hasNext(); )
         {
            // Add the class itself
            String classname = (String)itr.next();
            Class cls = getClass().getClassLoader().loadClass(classname);
            addFile(out, classToFile(cls.getName()));
         }
      }
      catch (ClassNotFoundException cnfe)
      {
         throw new IOException("Could not get class "+cnfe.getMessage());
      }
   }

   /**
    * Adds all the resources in the given list to the given jar output stream.
    */
   private void writeResources(JarOutputStream out, List resources)
      throws IOException
   {
      for (Iterator itr = resources.iterator(); itr.hasNext(); )
      {
         String res_name = (String)itr.next();
         addFile(out, res_name);
      }
   }

   /**
    * Finds the given file and adds it to the given JAR stream.
    */
   private void addFile(JarOutputStream out, String filename)
      throws IOException
   {
      // Create the next JAR entry
      JarEntry entry = new JarEntry(filename);
      out.putNextEntry(entry);

      try
      {
         // Copy the file into the JAR
         BufferedInputStream in = new BufferedInputStream(
               getClass().getClassLoader().getResourceAsStream(filename));

         // Loop over all the bytes in the resource
         while (true)
         {
            // Read in the bytes from the resource
            byte[] buf = new byte[2048];
            int nread = in.read(buf);
            if (nread == -1)
            {
               // We reached EOF
               break;
            }

            // Write out the bytes to the JAR
            out.write(buf, 0, nread);
         }

         in.close();
      }
      catch (EOFException eofe)
      {
         /* Ignore */
      }

      // Close out the JAR entry
      out.closeEntry();

   }

   /**
    * Converts a fully-qualified class name to the appropriate filename.
    */
   private String classToFile(String classname)
   {
      return classname.replace('.', '/') + ".class";
   }

   /**
    * Given the list of classes, all inner classes are found and added to the
    * list.
    *
    * XXX: This doesn't appear to work for anonymous classes.
    */
   private List getInnerClasses(List classes)
      throws IOException
   {
      List inner_classes = new ArrayList();

      try
      {
         for (Iterator itr = classes.iterator(); itr.hasNext(); )
         {
            String classname = (String)itr.next();
            Class cls = Class.forName(classname);

            // Get all declared classes
            Class[] declared_classes = cls.getDeclaredClasses();
            for (int i=0; i<declared_classes.length; ++i)
            {
               String decl_class_name = declared_classes[i].getName();
               inner_classes.add(decl_class_name);
            }
         }
      }
      catch (ClassNotFoundException cnfe)
      {
         throw new IOException("Could not find inner class: "+cnfe.getMessage());
      }

      return inner_classes;
   }

   /**
    * Removes all duplicate elements from the given list in place.
    */
   private void stripDuplicates(List list)
   {
      // Sort the list to put identical items together
      Collections.sort(list);

      // Remove duplicates
      Object prev = null;
      for (Iterator itr = list.iterator(); itr.hasNext(); )
      {
         Object cur = itr.next();
         // If the current item is the same as the previous, remove the current
         // item.
         if ((prev != null) && cur.equals(prev))
         {
            itr.remove();
         }
         else
         {
            prev = cur;
         }
      }
   }
}
