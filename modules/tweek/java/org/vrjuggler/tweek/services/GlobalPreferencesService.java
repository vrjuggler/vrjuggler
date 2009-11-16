/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

import org.vrjuggler.tweek.beans.BeanRegistrationEvent;
import org.vrjuggler.tweek.beans.BeanRegistrationListener;


/**
 * This class provides access to the global preferences for the entire base
 * system.  Access is granted through a singleton interface.
 */
public interface GlobalPreferencesService
   extends BeanRegistrationListener
{
   public static final double PREFS_VERSION_VALUE = 1.2;

   /**
    * Returns the name of the directory where Tweek-specific data files and
    * preferences should be stored.  This will be rooted under the
    * platform-specific application data directory, as returned by
    * EnvironmentService.getAppDataDir().
    *
    * @see EnvironmentService
    */
   public String getPrefsDir();

   /**
    * Called by the BeanRegistry singleton whenever a new bean is registered
    * with it.
    *
    * @param evt     the event describing the bean that got registered
    */
   public void beanRegistered(BeanRegistrationEvent evt);

   public static final int MIN_USER_LEVEL = 1;
   public static final int MAX_USER_LEVEL = 10;

   public static final String CWD_START     = "<cwd>";
   public static final String HOME_START    = "<home>";
   public static final String DEFAULT_START = CWD_START;

   /**
    * Returns a list of the basic starting directories from which the user may
    * choose.  This does not necessarily include the user's current preference.
    * A separate check should be done to determine how to handle the user's
    * preferred start directory versus the default possible choices.
    *
    * @see #getChooserStartDir()
    * @see #setChooserStartDir(String)
    */
   public java.util.List getStartDirList();

   public void setUserLevel(int level);

   public int getUserLevel();

   public void setLookAndFeel(String laf);

   public String getLookAndFeel();

   public java.util.Vector getBeanViewers();

   public void setBeanViewer(String v);

   public String getBeanViewer();

   public void setWindowWidth(int width);

   public int getWindowWidth();

   public void setWindowHeight(int height);

   public int getWindowHeight();

   /**
    * Sets the preferred start directory for file choosers.  It is up to the
    * code that opens file choosers to act on this preference.  The given
    * directory may be one of the default possible choices, or it may be a
    * user-entered string, possibly containing one or more environment
    * variables.
    */
   public void setChooserStartDir(String d);

   /**
    * Returns the user's current preferred starting directory for file
    * choosers.  The string returned is the actual preferred path rather than
    * the internal preference setting.
    *
    * @return A string that represents a path on the local system.  This can
    *         be passed directly to the java.io.File constructor, for example.
    */
   public String getChooserStartDir();

   /**
    * Returns the "raw" version of the user's current preferred starting
    * directory for file choosers.  This may be one of the default list, or it
    * may be a user-defined string (an actual path).
    */
   public String getRawChooserStartDir();

   /**
    * Sets the user's preference for lazy Panel Bean instantiation.  This
    * defines whether Panel Beans are instantiated upon discovery or upon
    * first interaction.
    */
   public void setLazyPanelBeanInstantiation(boolean enabled);

   public boolean getLazyPanelBeanInstantiation();

   /**
    * This method was renamed from setDefaultCorbaHost() in version 1.3.4.
    *
    * @since 1.3.4
    */
   public void setDefaultNamingServiceHost(String host);

   /**
    * This method was renamed from getDefaultCorbaHost() in version 1.3.4.
    *
    * @since 1.3.4
    */
   public String getDefaultNamingServiceHost();

   /**
    * This method was renamed from setDefaultCorbaPort() in version 1.3.4.
    *
    * @since 1.3.4
    */
   public void setDefaultNamingServicePort(int port);

   /**
    * This method was renamed from getDefaultCorbaPort() in version 1.3.4.
    *
    * @since 1.3.4
    */
   public int getDefaultNamingServicePort();

   public void setDefaultIiopVersion(String version);

   public String getDefaultIiopVersion();

   /**
    * Sets the default GIOP end point host address.
    *
    * @since 1.3.4
    */
   public void setDefaultOrbAddress(String addr);

   /**
    * Returns the default GIOP end point host address.
    *
    * @since 1.3.4
    */
   public String getDefaultOrbAddress();

   /**
    * Sets the default GIOP end point port number.
    *
    * @since 1.3.4
    */
   public void setDefaultOrbPort(int port);

   /**
    * Returns the deffault GIOP end point port number.
    *
    * @since 1.3.4
    */
   public int getDefaultOrbPort();

   /**
    * Changes the default preferences file name to be the given name.
    */
   public void setFileName(String name);

   /**
    * Changes the default preferences file object to use the given object.
    */
   public void setFile(java.io.File prefsFile);

   /**
    * Loads the user's prefernces file if one exists.  If the user has not
    * defined a preferences file (i.e., the named preferences file does not
    * exist), one is created.
    */
   public void load();

   /**
    * Saves the current prefernces document.
    */
   public void save();
}
