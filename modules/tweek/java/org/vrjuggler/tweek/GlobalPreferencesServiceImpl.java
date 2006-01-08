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

package org.vrjuggler.tweek;

import java.io.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;
import org.jdom.*;
import org.jdom.filter.ContentFilter;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;
import org.vrjuggler.tweek.services.EnvironmentServiceProxy;
import org.vrjuggler.tweek.services.GlobalPreferencesService;
import org.vrjuggler.tweek.beans.*;


/**
 * This class provides access to the global preferences for the entire base
 * system.  Access is granted through a singleton interface.
 *
 * @since 0.15.0
 */
class GlobalPreferencesServiceImpl
   extends ServiceBean
   implements GlobalPreferencesService
{
   private static final String PREFS_INSTRUCTION  = "org-vrjuggler-tweek-settings";
   private static final String PREFS_VERSION_ATTR = "global.cfg.version";

   /**
    * Creates a new global preferences service.
    */
   public GlobalPreferencesServiceImpl(BeanAttributes attr)
      throws IOException
   {
      super(attr);

      String data_dir_name = getAppDataDir();
      String os_name       = System.getProperty("os.name");

      // Mac OS X and Windows preferences location.
      if ( os_name.toLowerCase().startsWith("mac os x") ||
           os_name.indexOf("Windows") != -1 )
      {
         data_dir_name = data_dir_name + File.separator + "Tweek";
      }
      // UNIX.
      else
      {
         data_dir_name = data_dir_name + File.separator + ".tweek";
      }

      verifyPrefsDirExistence(data_dir_name);
      prefsDir = data_dir_name;
      mPrefsFile = new File(data_dir_name + File.separator + "tweekrc");
      BeanRegistry.instance().addBeanRegistrationListener(this);
   }

   /**
    * Returns the name of the directory where Tweek-specific data files and
    * preferences should be stored.  This will be rooted under the
    * platform-specific application data directory, as returned by
    * EnvironmentService.getAppDataDir().
    *
    * @see EnvironmentService
    */
   public String getPrefsDir()
   {
      return prefsDir;
   }

   /**
    * Called by the BeanRegistry singleton whenever a new bean is registered
    * with it.
    *
    * @param evt     the event describing the bean that got registered
    */
   public void beanRegistered(BeanRegistrationEvent evt)
   {
      TweekBean bean = evt.getBean();
      if ( bean instanceof ViewerBean )
      {
         addBeanViewer(bean.getName());
      }
   }

/*
   public static final int MIN_USER_LEVEL = 1;
   public static final int MAX_USER_LEVEL = 10;

   public static final String CWD_START     = "<cwd>";
   public static final String HOME_START    = "<home>";
   public static final String DEFAULT_START = CWD_START;
*/

   /**
    * Returns a list of the basic starting directories from which the user may
    * choose.  This does not necessarily include the user's current preference.
    * A separate check should be done to determine how to handle the user's
    * preferred start directory versus the default possible choices.
    *
    * @see getChooserStartDir()
    * @see setChooserStartDir()
    */
   public List getStartDirList()
   {
      List start_dirs = new ArrayList();
      start_dirs.add(CWD_START);
      start_dirs.add(HOME_START);

      return start_dirs;
   }

   public void setUserLevel(int level)
   {
      userLevel = level;

      Element e = mPrefsDocRoot.getChild("user");

      if ( e == null )
      {
         e = new Element("user");
         mPrefsDocRoot.addContent(e);
      }

      e.setAttribute("level", String.valueOf(level));
   }

   public int getUserLevel()
   {
      return userLevel;
   }

   public void setLookAndFeel(String laf)
   {
      lookAndFeel = laf;

      Element e = mPrefsDocRoot.getChild("laf");

      if ( e == null )
      {
         e = new Element("laf");
         mPrefsDocRoot.addContent(e);
      }

      e.setAttribute("name", laf);
   }

   public String getLookAndFeel()
   {
      return lookAndFeel;
   }

   private void addBeanViewer(String name)
   {
      beanViewers.add(name);
   }

   public Vector getBeanViewers()
   {
      return beanViewers;
   }

   public void setBeanViewer(String v)
   {
      beanViewer = v;

      Element e = mPrefsDocRoot.getChild("viewer");

      if ( e == null )
      {
         e = new Element("viewer");
         mPrefsDocRoot.addContent(e);
      }

      e.setAttribute("name", v);
   }

   public String getBeanViewer()
   {
      return beanViewer;
   }

   public void setWindowWidth(int width)
   {
      windowWidth = width;
      Element e = mPrefsDocRoot.getChild("windowsize");

      if ( null == e )
      {
         e = new Element("windowsize");
         mPrefsDocRoot.addContent(e);
         e.setAttribute("height", String.valueOf(windowHeight));
      }

      e.setAttribute("width", String.valueOf(width));
   }

   public int getWindowWidth()
   {
      return windowWidth;
   }

   public void setWindowHeight(int height)
   {
      windowHeight = height;
      Element e = mPrefsDocRoot.getChild("windowsize");

      if ( null == e )
      {
         e = new Element("windowsize");
         mPrefsDocRoot.addContent(e);
         e.setAttribute("width", String.valueOf(windowWidth));
      }

      e.setAttribute("height", String.valueOf(height));
   }

   public int getWindowHeight()
   {
      return windowHeight;
   }

   /**
    * Sets the preferred start directory for file choosers.  It is up to the
    * code that opens file choosers to act on this preference.  The given
    * directory may be one of the default possible choices, or it may be a
    * user-entered string, possibly containing one or more environment
    * variables.
    */
   public void setChooserStartDir(String d)
   {
      chooserStartDir = d;

      Element e = mPrefsDocRoot.getChild("chooser");

      if ( e == null )
      {
         e = new Element("chooser");
         mPrefsDocRoot.addContent(e);
      }

      // XXX: It might be good to validate d before saving, but how?
      e.setAttribute("start", d);
   }

   /**
    * Returns the user's current preferred starting directory for file
    * choosers.  The string returned is the actual preferred path rather than
    * the internal preference setting.
    *
    * @return A string that represents a path on the local system.  This can
    *         be passed directly to the java.io.File constructor, for example.
    */
   public String getChooserStartDir()
   {
      String start_dir = chooserStartDir;

      // If either the user's home directory or the application's current
      // working directory is the preferred start directory, convert that
      // into a real path string.  Otherwise, return whatever string the user
      // specified in the Tweek configuration file.
      if ( start_dir.equals(GlobalPreferencesService.HOME_START) )
      {
         start_dir = this.getUserHome();
      }
      else if ( start_dir.equals(GlobalPreferencesService.CWD_START) )
      {
         start_dir = System.getProperty("user.dir");
      }

      return start_dir;
   }

   /**
    * Returns the "raw" version of the user's current preferred starting
    * directory for file choosers.  This may be one of the default list, or it
    * may be a user-defined string (an actual path).
    */
   public String getRawChooserStartDir()
   {
      return chooserStartDir;
   }

   /**
    * Sets the user's preference for lazy Panel Bean instantiation.  This
    * defines whether Panel Beans are instantiated upon discovery or upon
    * first interaction.
    */
   public void setLazyPanelBeanInstantiation(boolean enabled)
   {
      lazyPanelBeanInstantiation = enabled;

      Element e = mPrefsDocRoot.getChild("lazyinst");

      if ( e == null )
      {
         e = new Element("lazyinst");
         mPrefsDocRoot.addContent(e);
      }

      e.setAttribute("enabled", enabled ? "true" : "false");
   }

   public boolean getLazyPanelBeanInstantiation()
   {
      return lazyPanelBeanInstantiation;
   }

   public void setDefaultCorbaHost(String host)
   {
      defaultCorbaHost = host;

      Element e = mPrefsDocRoot.getChild("corba");

      if ( e == null )
      {
         e = new Element("corba");
         mPrefsDocRoot.addContent(e);
      }

      e.setAttribute("host", host);
   }

   public String getDefaultCorbaHost()
   {
      return defaultCorbaHost;
   }

   public void setDefaultCorbaPort(int port)
   {
      defaultCorbaPort = port;

      Element e = mPrefsDocRoot.getChild("corba");

      if ( e == null )
      {
         e = new Element("corba");
         mPrefsDocRoot.addContent(e);
      }

      e.setAttribute("port", String.valueOf(port));
   }

   public int getDefaultCorbaPort()
   {
      return defaultCorbaPort;
   }

   public void setDefaultIiopVersion(String version)
   {
      defaultIiopVersion = version;

      Element e = mPrefsDocRoot.getChild("corba");

      if ( e == null )
      {
         e = new Element("corba");
         mPrefsDocRoot.addContent(e);
      }

      e.setAttribute("iiop", version);
   }

   public String getDefaultIiopVersion()
   {
      return defaultIiopVersion;
   }

   /**
    * Changes the default preferences file name to be the given name.
    */
   public void setFileName(String name)
   {
      setFile(new File(name));
   }

   /**
    * Changes the default preferences file object to use the given object.
    */
   public void setFile(File prefsFile)
   {
      mPrefsFile = prefsFile;
   }

   /**
    * Loads the user's prefernces file if one exists.  If the user has not
    * defined a preferences file (i.e., the named preferences file does not
    * exist), one is created.
    */
   public synchronized void load()
   {
      if ( mPrefsFile.exists() )
      {
         org.jdom.input.SAXBuilder builder = new org.jdom.input.SAXBuilder();

         try
         {
            mPrefsDoc = builder.build(mPrefsFile);
            checkVersion();
            mPrefsDocRoot = mPrefsDoc.getRootElement();

            // Handle the elements.
            Element ui_element = mPrefsDocRoot.getChild("laf");

            if ( ui_element != null )
            {
               lookAndFeel = ui_element.getAttribute("name").getValue();
            }

            Element viewer_element = mPrefsDocRoot.getChild("viewer");
            beanViewer = viewer_element.getAttribute("name").getValue();

            Element user_element = mPrefsDocRoot.getChild("user");

            if ( user_element != null )
            {
               userLevel = Integer.parseInt(user_element.getAttribute("level").getValue());
            }

            Element win_size_element = mPrefsDocRoot.getChild("windowsize");

            if ( win_size_element != null )
            {
               windowWidth =
                  Integer.parseInt(win_size_element.getAttributeValue("width"));
               windowHeight =
                  Integer.parseInt(win_size_element.getAttributeValue("height"));
            }

            Element chooser_element = mPrefsDocRoot.getChild("chooser");

            if ( chooser_element != null )
            {
               chooserStartDir = chooser_element.getAttribute("start").getValue();
            }

            Element lazyinst_element = mPrefsDocRoot.getChild("lazyinst");

            if ( lazyinst_element != null )
            {
               lazyPanelBeanInstantiation = lazyinst_element.getAttribute("enabled").getBooleanValue();
            }

            Element corba_element = mPrefsDocRoot.getChild("corba");

            if ( corba_element != null )
            {
               // Read the preferred Naming Service host identifier.
               Attribute corba_attr = corba_element.getAttribute("host");

               if ( null != corba_attr )
               {
                  defaultCorbaHost = corba_attr.getValue();
               }

               // Read the preferred Naming Service port number.
               corba_attr = corba_element.getAttribute("port");

               if ( null != corba_attr )
               {
                  defaultCorbaPort = corba_attr.getIntValue();
               }

               // Read the preferred IIOP version.
               corba_attr = corba_element.getAttribute("iiop");

               if ( null != corba_attr )
               {
                  defaultIiopVersion = corba_attr.getValue();
               }
            }
         }
         catch (Exception e)
         {
            e.printStackTrace();
         }
      }
      // Build the preferences document by hand using defaults.
      else
      {
         mPrefsDocRoot = new Element("tweekrc");
         mPrefsDoc     = new Document(mPrefsDocRoot);

         // Add version information to the new file.
         addVersion();

         Element ui_element = new Element("laf");
         ui_element.setAttribute("name", lookAndFeel);
         mPrefsDocRoot.addContent(ui_element);

         Element viewer_element = new Element("viewer");

         if ( beanViewers.size() > 0 )
         {
            viewer_element.setAttribute("name", (String) beanViewers.elementAt(0));
         }
         else
         {
            // XXX: Hard coding a Bean viewer this way is rather bad, but if
            // beanViewers is empty, then no Viewer Beans have been discovered
            // yet.
            viewer_element.setAttribute("name", "Tree Viewer");
         }

         beanViewer = viewer_element.getAttribute("name").getValue();
         mPrefsDocRoot.addContent(viewer_element);

         Element user_element = new Element("user");
         user_element.setAttribute("level", String.valueOf(userLevel));
         mPrefsDocRoot.addContent(user_element);

         Element win_size_element = new Element("windowsize");
         win_size_element.setAttribute("width", String.valueOf(windowWidth));
         win_size_element.setAttribute("height", String.valueOf(windowHeight));

         Element chooser_element = new Element("chooser");
         chooser_element.setAttribute("start", chooserStartDir);

         mPrefsDocRoot.addContent(chooser_element);

         Element lazyinst_element = new Element("lazyinst");
         lazyinst_element.setAttribute("enabled",
                                       lazyPanelBeanInstantiation ? "true"
                                                                  : "false");
         mPrefsDocRoot.addContent(lazyinst_element);

         Element corba_element = new Element("corba");
         corba_element.setAttribute("host", defaultCorbaHost);
         corba_element.setAttribute("port", String.valueOf(defaultCorbaPort));
         corba_element.setAttribute("iiop", defaultIiopVersion);
         mPrefsDocRoot.addContent(corba_element);

         save();
      }
   }

   /**
    * Saves the current prefernces document.
    */
   public synchronized void save()
   {
      Format f = Format.getPrettyFormat();
      f.setLineSeparator(System.getProperty("line.separator"));
      XMLOutputter outputter = new XMLOutputter(f);

      try
      {
         FileWriter writer = new FileWriter(mPrefsFile);
         outputter.output(mPrefsDoc, writer);
         writer.close();
      }
      catch(java.io.IOException e)
      {
         e.printStackTrace();
      }
   }

   // =========================================================================
   // Private methods
   // =========================================================================

   private void verifyPrefsDirExistence(String prefsDirName)
      throws IOException
   {
      File prefs_dir = new File(prefsDirName);

      if ( prefs_dir.exists() && ! prefs_dir.isDirectory() )
      {
         throw new IOException(prefsDirName +
                               " exists as a file, not a directory");
      }
      else if ( ! prefs_dir.exists() )
      {
         prefs_dir.mkdir();
      }
   }

   private String getUserHome()
   {
      String path = null;

      try
      {
         EnvironmentServiceProxy env_service = new EnvironmentServiceProxy();
         path = env_service.getUserHome();
      }
      catch(RuntimeException ex)
      {
         ex.printStackTrace();
         path = System.getProperty("user.home");
      }

      return path;
   }

   private String getAppDataDir()
   {
      String path = null;

      try
      {
         EnvironmentServiceProxy env_service = new EnvironmentServiceProxy();
         path = env_service.getAppDataDir();
      }
      catch(RuntimeException ex)
      {
         path = getUserHome();
      }

      return path;
   }

   /**
    * Checks the version information in the loaded document.  If there is no
    * version information, it is added.
    */
   private void checkVersion()
   {
      // Deal with any processing instructions in the preferences file.
      ContentFilter proc_inst_filter = new ContentFilter(ContentFilter.PI);
      List proc_inst = mPrefsDoc.getContent(proc_inst_filter);

      if ( proc_inst.size() > 0 )
      {
         Iterator i = proc_inst.iterator();
         ProcessingInstruction pi;

         while ( i.hasNext() )
         {
            pi = (ProcessingInstruction) i.next();

            if ( pi.getTarget().equals(PREFS_INSTRUCTION) )
            {
               String prefs_version =
                  pi.getPseudoAttributeValue(PREFS_VERSION_ATTR);

               if ( prefs_version != null )
               {
                  Float version = Float.valueOf(prefs_version);

                  // Warn the uers if their configuration file is out of date.
                  if ( PREFS_VERSION_VALUE > version.floatValue() )
                  {
                     System.err.println("WARNING: Preferences file is out of date (" +
                                        version + " < " + PREFS_VERSION_VALUE +
                                        ")");
                  }
               }
            }
         }
      }
      // If the preferences document does not have a version processing
      // directive, we add it now.  We also write out the modified document to
      // ensure that the modifications are not lost.
      else
      {
         addVersion();
         save();
      }
   }

   /**
    * Adds the version processing directive to mPrefsDoc.
    */
   private void addVersion()
   {
      // Create a map to hold the attributes for the processing instruction
      // to be created.
      java.util.Map pi_attrs = new java.util.HashMap();

      // Set the version information.
      pi_attrs.put(PREFS_VERSION_ATTR, String.valueOf(PREFS_VERSION_VALUE));

      ProcessingInstruction ver =
         new ProcessingInstruction(PREFS_INSTRUCTION, pi_attrs);
      mPrefsDoc.addContent(ver);
   }

   // =========================================================================
   // Private data members
   // =========================================================================

   private File     mPrefsFile    = null;
   private Document mPrefsDoc     = null;
   private Element  mPrefsDocRoot = null;

   private String prefsDir = null;

   private Vector beanViewers = new Vector();

   private int     userLevel        = 1;
   private String  lookAndFeel      = javax.swing.UIManager.getSystemLookAndFeelClassName();
   private String  beanViewer       = null;
   private int     windowWidth      = 1024;
   private int     windowHeight     = 768;
   private String  chooserStartDir  = CWD_START;
   private boolean lazyPanelBeanInstantiation = true;
   private String  defaultCorbaHost   = "";
   private int     defaultCorbaPort   = 2809;
   private String  defaultIiopVersion = "1.0";
}
