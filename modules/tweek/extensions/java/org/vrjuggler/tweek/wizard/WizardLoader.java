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

import java.util.jar.*;
import java.net.JarURLConnection;
import org.jdom.input.*;
import org.jdom.Document;
import org.jdom.Element;

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.EOFException;
import java.io.IOException;

import java.util.ArrayList;
import java.util.List;

import org.vrjuggler.tweek.beans.loader.BeanJarClassLoader;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;

public class WizardLoader
{
   private static final String STEP                   = "step";
   private static final String SEQUENCE               = "sequence";

   static public Wizard loadWizard(String jarName)
      throws IOException
   {
      Document doc = null;
      String wizard_xml_filename = null;

      // Expand in the case of enviroment variables
      EnvironmentService env_service = new EnvironmentServiceProxy();
      jarName = env_service.expandEnvVars(jarName);

      try
      {
         JarInputStream in =
            new JarInputStream(
               new BufferedInputStream(new FileInputStream(jarName))
            );

         JarEntry entry = in.getNextJarEntry();
         while (entry != null && wizard_xml_filename == null)
         {
            System.out.println("Looking at: " + entry.getName());

            if(entry.getName().endsWith("Wizard.xml"))
            {
               System.out.println("Wizard.xml found at: " + entry.getName());
               wizard_xml_filename = entry.getName();
            }

            in.closeEntry();
            entry = in.getNextJarEntry();
         }
      }
      catch (EOFException e)
      {
         System.err.println("Reached EOF!");
      }

      Wizard wizard = new Wizard();

      try
      {
         // Add the .jar file for the wizard to the search path.
         // XXX: Is this the best way to do this?
         BeanJarClassLoader class_loader = BeanJarClassLoader.instance();
         class_loader.addJarFile(new JarFile(jarName));

         SAXBuilder sb = new SAXBuilder(false);
         java.net.URL url = new java.net.URL("jar:file:" + jarName + "!/" +
                                             wizard_xml_filename);

         doc = sb.build(url);

         System.out.println(doc);
         System.out.println(doc.getRootElement());

         java.util.List c = doc.getRootElement().getChildren();
         for(java.util.Iterator itr = c.iterator() ; itr.hasNext() ; )
         {
            Element e = (Element)itr.next();
            WizardStep step = parse(class_loader, e);
            wizard.add(step);
         }
      }
      catch(IOException ex)
      {
         throw ex;
      }
      catch(Exception ex)
      {
         System.out.println(ex);
         ex.printStackTrace();
      }

      return wizard;
   }

   private static WizardStep parse(ClassLoader classLoader, Element root)
   {
      // Get the name of this configuration element
      String name = root.getName();
      System.out.println("Name: " + name);
      /*
      if (name == null)
      {
         throw new MissingAttributeException(NAME);
      }
      */
      WizardStep step = null;
      if (name.equals(STEP))
      {
         step = parseStep(classLoader, root);
      }
      else if (name.equals(SEQUENCE))
      {
         step = parseSequence(classLoader, root);
      }
      else
      {
         /*
         throw new MissingAttributeException(NAME);
         */
      }
      return step;
   }

   private static WizardStep parseStep(ClassLoader classLoader, Element root)
   {
      WizardStep step = null;
      try
      {
         System.out.println(root.getAttribute("class").getValue());
         Class test_class =
            Class.forName(root.getAttribute("class").getValue(), true,
                          classLoader);
         step = (WizardStep)test_class.newInstance();
      }
      catch (Exception e)
      {
         System.out.println(e);
         e.printStackTrace();
      }
      return step;
   }
   
   private static WizardStep parseSequence(ClassLoader classLoader, Element root)
   {
      WizardSequence parent_step = null;
      try
      {
         System.out.println(root.getAttribute("class").getValue());
         Class test_class =
            Class.forName(root.getAttribute("class").getValue(), true,
                          classLoader);
         parent_step = (WizardSequence)test_class.newInstance();
         
         java.util.List c = root.getChildren();
         for(java.util.Iterator itr = c.iterator() ; itr.hasNext() ; )
         {
            Element e = (Element)itr.next();
            
            WizardStep step = parse(classLoader, e);
            parent_step.add(step);
         }
      }
      catch (Exception e)
      {
         System.out.println(e);
         e.printStackTrace();
      }
      return parent_step;
   }
}
