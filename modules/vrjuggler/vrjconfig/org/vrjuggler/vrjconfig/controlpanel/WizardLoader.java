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
package org.vrjuggler.vrjconfig.controlpanel;

import java.util.jar.*;
import java.net.JarURLConnection;
import org.jdom.input.*;
import org.jdom.Document;
import org.jdom.Element;

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.EOFException;
import java.io.IOException;

import org.vrjuggler.tweek.wizard.*;
import org.vrjuggler.tweek.services.EnvironmentService;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;

public class WizardLoader
{
   static Wizard loadWizard(String jar_name, ClassLoader class_loader)
      throws IOException
   {
      Document doc = null;
      String wizard_xml_filename = null;
      
      // Expand in the case of enviroment variables
      EnvironmentService env_service = new EnvironmentServiceProxy();
      jar_name = env_service.expandEnvVars(jar_name);

      try
      {
         JarInputStream in = new JarInputStream(new BufferedInputStream(new FileInputStream(jar_name)));

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
         SAXBuilder sb = new SAXBuilder(false);
         java.net.URL url = new java.net.URL("jar:file:"
            + jar_name + "!/" + wizard_xml_filename);
      
         doc = sb.build(url);
         
         System.out.println(doc);
         System.out.println(doc.getRootElement());

         java.util.List c = doc.getRootElement().getChildren();
         for(java.util.Iterator itr = c.iterator() ; itr.hasNext() ; )
         {
            Element e = (Element)itr.next();
            System.out.println(e.getAttribute("class").getValue());
            Class test_class = Class.forName(e.getAttribute("class").getValue(),
                                             true, class_loader);
            wizard.add((WizardStep)test_class.newInstance());
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
}
