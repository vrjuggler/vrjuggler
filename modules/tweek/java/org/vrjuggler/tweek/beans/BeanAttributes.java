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
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
package org.vrjuggler.tweek.beans;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.jdom.Element;

/**
 * Parses and collects the comment contents of &lt;beanlist&gt; document
 * elements. These include the Bean name, dependencies and container file.
 */
public class BeanAttributes
{
   /**
    * Creates a new bean attributes object with the given attributes.
    *
    * @param name          the name of the bean
    * @param jarURL        the URL string of the jar that contains the bean
    * @param entry         the entry in the jar that contains the bean
    * @param dependencies  a list of BeanDependency objects that describe the
    *                      beans this bean depends on as
    */
   public BeanAttributes( String name, String jarURL, String entry,
                          List dependencies )
   {
      this.name         = name;
      this.jarURL       = jarURL;
      this.entry        = entry;
      this.dependencies = dependencies;
   }

   /**
    * Parses an XML element looking for the children &lt;dependency /&gt;,
    * &lt;path /&gt; and &lt;file /&gt;.
    *
    * @param root    the XML element to parse
    *
    * @return  a BeanAttributes containing the attributes common to all tweek
    *          beans
    */
   static BeanAttributes parseXMLElement( Element root )
   {
      String name = "";
      String jarURL = "";
      String entry = "";
      List dependencies = new ArrayList();

      name = root.getAttribute("name").getValue();

      List children = root.getChildren();
      Iterator i    = children.iterator();

      // Iterate through each child of the root element
      while ( i.hasNext() )
      {
         Element e = (Element) i.next();

         // Parse a dependency element
         if ( e.getName().equals("dependency") )
         {
            String dep_file = null;

            if ( e.getAttribute("file") != null )
            {
               dep_file = e.getAttribute("file").getValue();
            }

            String dep_path = null;

            if ( e.getAttribute("path") != null )
            {
               dep_path = expandEnvVars(e.getAttribute("path").getValue());
            }

            dependencies.add(new BeanDependency(dep_file, dep_path));
         }
         // Parse a file element that contains the JAR file and JAR entry
         else if ( e.getName().equals("file") )
         {
            jarURL = expandEnvVars(e.getAttribute("name").getValue());
            entry   = e.getAttribute("entry").getValue();
         }
      }

      // Convert the JAR filename into a URL
      if ( ! jarURL.startsWith("http://") && ! jarURL.startsWith("ftp://") )
      {
         // Prepend "file:" onto jarURL if it is not already present.
         if ( ! jarURL.startsWith("file:") )
         {
            jarURL = "file:" + jarURL;
         }
      }

      return new BeanAttributes( name, jarURL, entry, dependencies );
   }

   /**
    * Gets the name of the bean.
    *
    * @return  the name of the bean
    */
   public String getName()
   {
      return name;
   }

   /**
    * Gets the JAR URL the bean is contained within.
    *
    * @return  the URL string of the JAR
    */
   public String getJarURL()
   {
      return jarURL;
   }

   /**
    * Gets the entry in the JAR file that contains the bean.
    *
    * @return  a string entry of the bean in the JAR
    */
   public String getEntry()
   {
      return entry;
   }

   /**
    * Gets the list of the beans the bean depends on.
    *
    * @return  a list of BeanDependency objects that describe the beans that are
    *          required by for the bean described by this BeanAttributes object.
    */
   public List getDependencies()
   {
      return dependencies;
   }

   /**
    * Expand environment variables found within XML files as we know how.
    * This must exist here because the EnvironmentService is a JavaBean, and
    * it is this class' responsibility to *find* JavaBeans.
    *
    * @post Any environment variables known to the JVM as properties are
    *       expanded.  Unknown variables are left intact.
    */
   protected static String expandEnvVars (String name)
   {
      String new_name = name;
      int index, end_index;
      String env_var, value;

      while ( (index = new_name.indexOf("$")) != -1 )
      {
         if ( name.substring(index, index + 2).equals("$(") )
         {
            end_index = name.indexOf(")");
         }
         else if ( name.substring(index, index + 2).equals("${") )
         {
            end_index = name.indexOf("}");
         }
         else
         {
            continue;
         }

         env_var  = name.substring(index + 2, end_index);
         value    = System.getProperty(env_var);

         if ( value != null )
         {
            new_name = name.substring(0, index) + value +
                       name.substring(end_index + 1);
         }
         else
         {
            System.err.println("WARNING: Environment variable " + env_var +
                               " has no value");
            new_name = name.substring(0, index) + name.substring(end_index + 1);
         }
      }

      return new_name;
   }

   /**
    * The name of the bean.
    */
   private String name;

   /**
    * The JAR URL string in which the bean is contained.
    */
   private String jarURL;

   /**
    * The entry in the JAR that contains the bean.
    */
   private String entry;

   /**
    * A list of BeanDependency objects that describe the beans that are required
    * for the bean described by this BeanAttributes object.
    */
   private List dependencies;
}
