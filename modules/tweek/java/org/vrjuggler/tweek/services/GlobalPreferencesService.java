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

import org.vrjuggler.tweek.beans.*;

import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;
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
   extends ServiceBean
   implements BeanRegistrationListener
{
   /**
    * Creates a new global preferences service.
    */
   public GlobalPreferencesService (BeanAttributes attr)
   {
      super(attr);
      m_prefs_file = new File(getUserHome() + File.separator + ".tweekrc");
      BeanRegistry.instance().addBeanRegistrationListener( this );
   }

   /**
    * Called by the BeanRegistry singleton whenever a new bean is registered
    * with it.
    *
    * @param evt     the event describing the bean that got registered
    */
   public void beanRegistered( BeanRegistrationEvent evt )
   {
      TweekBean bean = evt.getBean();
      if ( bean instanceof ViewerBean )
      {
         addBeanViewer( bean.getName() );
      }
   }

   public static final int WINDOWS_CHOOSER = 0;
   public static final int EMACS_CHOOSER   = 1;
   public static final int DEFAULT_CHOOSER = WINDOWS_CHOOSER;

   public static final String CWD_START     = "<cwd>";
   public static final String HOME_START    = "<home>";
   public static final String DEFAULT_START = CWD_START;

   /**
    * Returns a list of the basic starting directories from which the user may
    * choose.  This does not necessarily include the user's current preference.
    * A separate check should be done to determine how to handle the user's
    * preferred start directory versus the default possible choices.
    *
    * @see getChooserStartDir()
    * @see setChooserStartDir()
    */
   public static List getStartDirList ()
   {
      List start_dirs = new ArrayList();
      start_dirs.add(CWD_START);
      start_dirs.add(HOME_START);

      return start_dirs;
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
    * Sets the preferred start directory for file choosers.  It is up to the
    * code that opens file choosers to act on this preference.  The given
    * directory may be one of the default possible choices, or it may be a
    * user-entered string, possibly containing one or more environment
    * variables.
    */
   public void setChooserStartDir (String d)
   {
      chooserStartDir = d;

      Element e = m_prefs_doc_root.getChild("chooser");

      if ( e == null )
      {
         e = new Element("chooser");
         m_prefs_doc_root.addContent(e);
      }

      // XXX: It might be good to validate d before saving, but how?
      e.setAttribute("start", d);
   }

   /**
    * Returns the user's current preferred starting directory.  This may be
    * one of the default list, or it may be a user-defined setting.
    */
   public String getChooserStartDir ()
   {
      return chooserStartDir;
   }

   /**
    * Sets the user's current preferred file chooser "open style".  This
    * defines how file choosers should behave with respect to the directory
    * they use when first opening.  It is up to the code that opens the file
    * choosers to act on this preference.
    */
   public void setChooserOpenStyle (int style)
   {
      chooserOpenStyle = style;

      Element e = m_prefs_doc_root.getChild("chooser");

      if ( e == null )
      {
         e = new Element("chooser");
         m_prefs_doc_root.addContent(e);
      }

      switch (style)
      {
         case EMACS_CHOOSER:
            e.setAttribute("style", "Emacs");
            break;
         case WINDOWS_CHOOSER:
         default:
            e.setAttribute("style", "Windows");
            break;
      }
   }

   public int getChooserOpenStyle ()
   {
      return chooserOpenStyle;
   }

   /**
    * Sets the user's preference for lazy Panel Bean instantiation.  This
    * defines whether Panel Beans are instantiated upon discovery or upon
    * first interaction.
    */
   public void setLazyPanelBeanInstantiation (boolean enabled)
   {
      lazyPanelBeanInstantiation = enabled;

      Element e = m_prefs_doc_root.getChild("lazyinst");

      if ( e == null )
      {
         e = new Element("lazyinst");
         m_prefs_doc_root.addContent(e);
      }

      e.setAttribute("enabled", enabled ? "true" : "false");
   }

   public boolean getLazyPanelBeanInstantiation ()
   {
      return lazyPanelBeanInstantiation;
   }

   /**
    * Changes the default preferences file name to be the given name.
    */
   public void setFileName (String name)
   {
      File temp_file = new File(name);

      if ( ! temp_file.exists() )
      {
         System.err.println("WARNING: Tweek preferences file " + name +
                            " does not exist, defaulting to " +
                            m_prefs_file.getAbsolutePath());
      }
      else
      {
         m_prefs_file = temp_file;
      }
   }

   /**
    * Loads the user's prefernces file if one exists.  If the user has not
    * defined a preferences file, one is created.
    */
   public synchronized void load ()
   {
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

            if ( user_element != null )
            {
               userLevel = Integer.parseInt(user_element.getAttribute("level").getValue());
            }

            Element chooser_element = m_prefs_doc_root.getChild("chooser");

            if ( chooser_element != null )
            {
               chooserStartDir = chooser_element.getAttribute("start").getValue();

               String style = chooser_element.getAttribute("style").getValue();

               if ( style.equals("Windows") )
               {
                  chooserOpenStyle = WINDOWS_CHOOSER;
               }
               else
               {
                  chooserOpenStyle = EMACS_CHOOSER;
               }
            }

            Element lazyinst_element = m_prefs_doc_root.getChild("lazyinst");

            if ( lazyinst_element != null )
            {
               lazyPanelBeanInstantiation = chooser_element.getAttribute("enabled").getBooleanValue();
            }
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

         Element chooser_element = new Element("chooser");
         chooser_element.setAttribute("start", chooserStartDir);

         switch ( chooserOpenStyle )
         {
            case EMACS_CHOOSER:
               chooser_element.setAttribute("style", "Emacs");
               break;
            case WINDOWS_CHOOSER:
            default:
               chooser_element.setAttribute("style", "Windows");
               break;
         }

         Element lazyinst_element = new Element("lazyinst");
         chooser_element.setAttribute("enabled",
                                      lazyPanelBeanInstantiation ? "true"
                                                                 : "false");

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
   // Private methods
   // =========================================================================

   private String getUserHome ()
   {
      TweekBean bean = BeanRegistry.instance().getBean( "Environment" );
      String path = null;

      if ( bean != null )
      {
         try
         {
            EnvironmentService env_service = (EnvironmentService) bean;
            path = env_service.getUserHome();
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

   private int    userLevel        = 1;
   private String lookAndFeel      = javax.swing.UIManager.getSystemLookAndFeelClassName();
   private String beanViewer       = null;
   private String chooserStartDir  = CWD_START;
   private int    chooserOpenStyle = WINDOWS_CHOOSER;
   private boolean lazyPanelBeanInstantiation = true;
}
