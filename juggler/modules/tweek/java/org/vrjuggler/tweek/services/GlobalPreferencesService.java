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

package org.vrjuggler.tweek.services;

import java.io.File;
import java.io.FileWriter;
import java.util.List;
import java.util.Vector;
import org.jdom.*;
import org.jdom.output.XMLOutputter;


/**
 * This class provides access to the global preferences for the entire base
 * system.  Access is granted through a singleton interface.
 *
 * @since 1.0
 */
public class GlobalPreferencesService
{
   public static GlobalPreferencesService instance ()
   {
      if ( m_instance == null )
      {
         m_instance = new GlobalPreferencesService();
      }

      return m_instance;
   }

   public void setUserLevel (int level)
   {
      userLevel = level;

      Element e = m_prefs_doc_root.getChild("user");

      if ( e == null )
      {
         e = new Element("user");
         m_prefs_doc_root.addContent(e);
      }

      e.setAttribute("level", String.valueOf(level));
   }

   public int getUserLevel ()
   {
      return userLevel;
   }

   public void setLookAndFeel (String laf)
   {
      lookAndFeel = laf;

      Element e = m_prefs_doc_root.getChild("laf");

      if ( e == null )
      {
         e = new Element("laf");
         m_prefs_doc_root.addContent(e);
      }

      e.setAttribute("name", laf);
   }

   public String getLookAndFeel ()
   {
      return lookAndFeel;
   }

   public void addBeanViewer (String name)
   {
      beanViewers.add(name);
   }

   public Vector getBeanViewers ()
   {
      return beanViewers;
   }

   public void setBeanViewer (String v)
   {
      beanViewer = v;

      Element e = m_prefs_doc_root.getChild("viewer");

      if ( e == null )
      {
         e = new Element("viewer");
         m_prefs_doc_root.addContent(e);
      }

      e.setAttribute("name", v);
   }

   public String getBeanViewer ()
   {
      return beanViewer;
   }

   /**
    * Loads the user's prefernces file if one exists.  If the user has not
    * defined a preferences file, one is created.
    */
   public synchronized void load ()
   {
      m_prefs_file = new File(getUserHome() + File.separator + ".tweekrc");

      if ( m_prefs_file.exists() )
      {
         org.jdom.input.SAXBuilder builder = new org.jdom.input.SAXBuilder();

         try
         {
            m_prefs_doc      = builder.build(m_prefs_file);
            m_prefs_doc_root = m_prefs_doc.getRootElement();

            // Handle the elements.
            Element ui_element = m_prefs_doc_root.getChild("laf");

            if ( ui_element != null )
            {
               lookAndFeel = ui_element.getAttribute("name").getValue();
            }

            Element viewer_element = m_prefs_doc_root.getChild("viewer");
            beanViewer = viewer_element.getAttribute("name").getValue();

            Element user_element = m_prefs_doc_root.getChild("user");
            userLevel = Integer.parseInt(user_element.getAttribute("level").getValue());
         }
         catch (JDOMException e)
         {
            e.printStackTrace();
         }
      }
      // Build the preferences document by hand using defaults.
      else
      {
         m_prefs_doc_root = new Element("tweekrc");
         m_prefs_doc      = new Document(m_prefs_doc_root);

         Element ui_element = new Element("laf");
         ui_element.setAttribute("name", lookAndFeel);
         m_prefs_doc_root.addContent(ui_element);

         Element viewer_element = new Element("viewer");

         if ( beanViewers.size() > 0 )
         {
            viewer_element.setAttribute("name", (String) beanViewers.elementAt(0));
         }
         else
         {
            viewer_element.setAttribute("name", "IconViewer");
         }

         beanViewer = viewer_element.getAttribute("name").getValue();
         m_prefs_doc_root.addContent(viewer_element);

         Element user_element = new Element("user");
         user_element.setAttribute("level", String.valueOf(userLevel));
         m_prefs_doc_root.addContent(user_element);

         save(true);
      }
   }

   /**
    * Saves the current prefernces document.
    */
   public synchronized void save (boolean add_newlines)
   {
      XMLOutputter outputter = new XMLOutputter("  ", add_newlines);

      try
      {
         FileWriter writer = new FileWriter(m_prefs_file);
         outputter.output(m_prefs_doc, writer);
         writer.close();
      }
      catch (java.io.IOException e)
      {
         e.printStackTrace();
      }
   }

   // =========================================================================
   // Protected methods
   // =========================================================================

   /**
    * This exists as a protected member to ensure that instantiation cannot
    * be done except through the instance() method.
    */
   protected GlobalPreferencesService ()
   {
   }

   // =========================================================================
   // Protected data members
   // =========================================================================

   protected static GlobalPreferencesService m_instance = null;

   // =========================================================================
   // Private methods
   // =========================================================================

   private String getUserHome ()
   {
      Object o = ServiceRegistry.instance().getService("Environment");
      String path = null;

      if ( o != null )
      {
         try
         {
            EnvironmentService env_service = (EnvironmentService) o;
            path = EnvironmentService.getUserHome();
         }
         catch (ClassCastException e)
         {
            path = System.getProperty("user.home");
         }
      }
      else
      {
         path = System.getProperty("user.home");
      }

      return path;
   }

   // =========================================================================
   // Private data members
   // =========================================================================

   private File     m_prefs_file     = null;
   private Document m_prefs_doc      = null;
   private Element  m_prefs_doc_root = null;

   private Vector beanViewers = new Vector();

   private int    userLevel   = 1;
   private String lookAndFeel = javax.swing.UIManager.getSystemLookAndFeelClassName();
   private String beanViewer  = null;
}
