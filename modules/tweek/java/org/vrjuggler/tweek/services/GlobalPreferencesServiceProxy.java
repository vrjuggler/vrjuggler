/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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


/**
 * A proxy to the built-in Tweek Global Preferences Service Bean.  Instantiate
 * this class to get access to the Global Preferences Service.
 */
public class GlobalPreferencesServiceProxy
   implements GlobalPreferencesService
{
   /**
    * Creates a new GlobalPreferencesService proxy to the Tweek Global
    * Preferences Service.
    *
    * @throws RuntimeException if the GlobalPreferencesService Bean could not
    *                          be found.
    */
   public GlobalPreferencesServiceProxy()
      throws RuntimeException
           , java.io.IOException
   {
      TweekBean bean = BeanRegistry.instance().getBean("GlobalPreferences");

      if ( null == bean )
      {
         throw new RuntimeException("Could not find Global Preferences Service");
      }

      if ( bean instanceof GlobalPreferencesService )
      {
         mPrefsService = (GlobalPreferencesService) bean;
      }
      else
      {
         throw new RuntimeException("The Bean registered as 'GlobalPreferences' is not a GlobalPreferencesService implementation.");
      }
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
      return mPrefsService.getPrefsDir();
   }

   /**
    * Called by the BeanRegistry singleton whenever a new bean is registered
    * with it.
    *
    * @param evt     the event describing the bean that got registered
    */
   public void beanRegistered(BeanRegistrationEvent evt)
   {
      mPrefsService.beanRegistered(evt);
   }

   /**
    * Returns a list of the basic starting directories from which the user may
    * choose.  This does not necessarily include the user's current preference.
    * A separate check should be done to determine how to handle the user's
    * preferred start directory versus the default possible choices.
    *
    * @see #getChooserStartDir()
    * @see #setChooserStartDir(String)
    */
   public java.util.List getStartDirList()
   {
      return mPrefsService.getStartDirList();
   }

   public void setUserLevel(int level)
   {
      mPrefsService.setUserLevel(level);
   }

   public int getUserLevel()
   {
      return mPrefsService.getUserLevel();
   }

   public void setLookAndFeel(String laf)
   {
      mPrefsService.setLookAndFeel(laf);
   }

   public String getLookAndFeel()
   {
      return mPrefsService.getLookAndFeel();
   }

   public java.util.Vector getBeanViewers()
   {
      return mPrefsService.getBeanViewers();
   }

   public void setBeanViewer(String v)
   {
      mPrefsService.setBeanViewer(v);
   }

   public String getBeanViewer()
   {
      return mPrefsService.getBeanViewer();
   }

   public void setWindowWidth(int width)
   {
      mPrefsService.setWindowWidth(width);
   }

   public int getWindowWidth()
   {
      return mPrefsService.getWindowWidth();
   }

   public void setWindowHeight(int height)
   {
      mPrefsService.setWindowHeight(height);
   }

   public int getWindowHeight()
   {
      return mPrefsService.getWindowHeight();
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
      mPrefsService.setChooserStartDir(d);
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
      return mPrefsService.getChooserStartDir();
   }

   /**
    * Returns the "raw" version of the user's current preferred starting
    * directory for file choosers.  This may be one of the default list, or it
    * may be a user-defined string (an actual path).
    */
   public String getRawChooserStartDir()
   {
      return mPrefsService.getRawChooserStartDir();
   }

   /**
    * Sets the user's preference for lazy Panel Bean instantiation.  This
    * defines whether Panel Beans are instantiated upon discovery or upon
    * first interaction.
    */
   public void setLazyPanelBeanInstantiation(boolean enabled)
   {
      mPrefsService.setLazyPanelBeanInstantiation(enabled);
   }

   public boolean getLazyPanelBeanInstantiation()
   {
      return mPrefsService.getLazyPanelBeanInstantiation();
   }

   public void setDefaultCorbaHost(String host)
   {
      mPrefsService.setDefaultCorbaHost(host);
   }

   public String getDefaultCorbaHost()
   {
      return mPrefsService.getDefaultCorbaHost();
   }

   public void setDefaultCorbaPort(int port)
   {
      mPrefsService.setDefaultCorbaPort(port);
   }

   public int getDefaultCorbaPort()
   {
      return mPrefsService.getDefaultCorbaPort();
   }

   public void setDefaultIiopVersion(String version)
   {
      mPrefsService.setDefaultIiopVersion(version);
   }

   public String getDefaultIiopVersion()
   {
      return mPrefsService.getDefaultIiopVersion();
   }

   /**
    * Changes the default preferences file name to be the given name.
    */
   public void setFileName(String name)
   {
      mPrefsService.setFileName(name);
   }

   /**
    * Changes the default preferences file object to use the given object.
    */
   public void setFile(java.io.File prefsFile)
   {
      mPrefsService.setFile(prefsFile);
   }

   /**
    * Loads the user's prefernces file if one exists.  If the user has not
    * defined a preferences file (i.e., the named preferences file does not
    * exist), one is created.
    */
   public synchronized void load()
   {
      mPrefsService.load();
   }

   /**
    * Saves the current prefernces document.
    */
   public synchronized void save()
   {
      mPrefsService.save();
   }

   private GlobalPreferencesService mPrefsService = null;
}
