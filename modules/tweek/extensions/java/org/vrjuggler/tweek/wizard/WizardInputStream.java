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
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.tweek.wizard;

import java.io.*;
import java.util.*;
import java.util.jar.*;

/**
 * A WizardInputStream reads a Wizard from an InputStream. All the WizardSteps
 * along with the classes they require are read so that the Wizard can be
 * successfully loaded in with the all the resources it needs.
 */
public class WizardInputStream
   extends BufferedInputStream
{
   /**
    * Creates an WizardInputStream that reads from the specified InputStream.
    */
   public WizardInputStream(InputStream out)
   {
      super(out);
   }

   /**
    * Reads the specified wizard from this stream.
    */
   public Wizard readWizard()
      throws IOException
   {
      Wizard wizard = null;
      JarInputStream jar_in = new JarInputStream(this);

      try
      {
         JarEntry entry = jar_in.getNextJarEntry();
         while (entry != null)
         {
            if (entry.getName().equals("wizard.ser"))
            {
               wizard = readSerializedWizard(jar_in);
            }

            jar_in.closeEntry();
         }
      }
      catch (EOFException e)
      {
         System.err.println("Reached EOF!");
      }

      return wizard;
   }

   private Wizard readSerializedWizard(InputStream in)
      throws IOException
   {
      try
      {
         ObjectInputStream obj_in = new ObjectInputStream(in);
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
}
