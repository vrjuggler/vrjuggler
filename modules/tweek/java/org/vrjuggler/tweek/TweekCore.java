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

package org.vrjuggler.tweek;

import java.io.File;
import java.util.Vector;
import org.vrjuggler.tweek.beans.*;
import org.vrjuggler.tweek.beans.loader.BeanInstantiationException;
import org.vrjuggler.tweek.services.*;
import org.vrjuggler.tweek.net.corba.*;


/**
 * @since 1.0
 */
public class TweekCore
{
   // ========================================================================
   // Public methods.
   // ========================================================================

   public static TweekCore instance ()
   {
      if ( m_instance == null )
      {
         m_instance = new TweekCore();
      }

      return m_instance;
   }

   public void init (String[] args) throws Exception
   {
      // This needs to be the first step to ensure that all the basic services
      // and viewers get loaded.
      findBeans(System.getProperty("TWEEK_BASE_DIR") + File.separator +
                "bin" + File.separator + "beans");

      // Load the service Beans into the services registry.
      buildServiceRegistry();

      // Register the command-line arguments with the Environment Service (if
      // it is available).
      try
      {
         EnvironmentService service =
            (EnvironmentService) ServiceRegistry.instance().getService("Environment");

         if ( service != null )
         {
            service.setCommandLineArgs(args);
         }
      }
      catch (ClassCastException e)
      {
         System.err.println("WARNING: Failed to register command-line arguments");
      }

      // Load the viewer Beans into the viewers registry.
      buildViewerRegistry();

      // Load the user's global preferences file.
      GlobalPreferencesService.instance().load();

      m_gui = new TweekFrame(org.vrjuggler.tweek.net.PlexusNodeInterfaceFactory.create(),
                             GlobalPreferencesService.instance());

      // Now we need to register the TweekFrame instance as a listener for
      // BeanFocusChangeEvents.
      Vector viewer_beans = ViewerRegistry.instance().getAllViewers();

      for ( int i = 0; i < viewer_beans.size(); i++ )
      {
         BeanModelViewer v = (BeanModelViewer) viewer_beans.elementAt(i);
         v.addBeanFocusChangeListener(m_gui);
      }

      m_gui.initGUI(BeanCollectionBuilder.instance().getPanelTree());
   }

   /**
    * Default constructor.
    */
   protected TweekCore ()
   {
   }

   /**
    * Searches for JavaBeans in the given path.
    *
    * @pre The GUI has not been initialized yet.
    */
   protected void findBeans (String path)
   {
      try
      {
         BeanCollectionBuilder.instance().build(path, true);
      }
      catch (org.vrjuggler.tweek.beans.BeanPathException e)
      {
         System.out.println("WARNING: Invalid path " + path);
      }
   }

   protected void buildServiceRegistry ()
   {
      Vector services = BeanCollectionBuilder.instance().getServices();

      for ( int i = 0; i < services.size(); i++ )
      {
         ServiceBean service = (ServiceBean) services.elementAt(i);

         try
         {
            service.instantiate();
            ServiceRegistry.instance().registerService(service.getServiceName(),
                                                       service.getService());
         }
         catch (BeanInstantiationException e)
         {
            System.err.println("WARNING: Failed to instantiate service '" +
                               service.getServiceName() + "': " +
                               e.getMessage());
         }
      }
   }

   protected void buildViewerRegistry ()
   {
      Vector viewers           = BeanCollectionBuilder.instance().getViewers();
      BeanTreeModel tree_model = BeanCollectionBuilder.instance().getPanelTree();

      GlobalPreferencesService prefs = GlobalPreferencesService.instance();

      for ( int i = 0; i < viewers.size(); i++ )
      {
         ViewerBean viewer = (ViewerBean) viewers.elementAt(i);

         try
         {
            // The process of loading a new BeanViewer object is a little
            // involved.  First, we need an instance (duh).  After that, we
            // need to pass it the tree model, initialize its GUI, and add it
            // to the available choices in the global preferences.  Once all
            // that is done, the viewer can be registered for use by other
            // code.
            viewer.instantiate();
            BeanModelViewer bv = (BeanModelViewer) viewer.getViewer();
            bv.initDataModel(tree_model);
            bv.initGUI();
            prefs.addBeanViewer(viewer.getViewerName());
            ViewerRegistry.instance().registerViewer(viewer.getViewerName(),
                                                     viewer.getViewer());
         }
         catch (BeanInstantiationException e)
         {
            System.err.println("WARNING: Failed to instantiate Bean viewer '" +
                               viewer.getViewerName() + "': " + e.getMessage());
         }
      }
   }

   // ========================================================================
   // Protected data members.
   // ========================================================================

   protected static TweekCore m_instance = null;

   // ========================================================================
   // Private data members.
   // ========================================================================

   private TweekFrame m_gui = null;
}
