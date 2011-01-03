/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;
import javax.swing.UIManager;
import javax.swing.tree.DefaultMutableTreeNode;
import com.birosoft.liquid.LiquidLookAndFeel;
import com.incors.plaf.kunststoff.KunststoffLookAndFeel;
import com.incors.plaf.kunststoff.mini.KunststoffMiniLookAndFeel;
import com.jgoodies.looks.windows.WindowsLookAndFeel;
import com.jgoodies.looks.plastic.PlasticLookAndFeel;
import com.jgoodies.looks.plastic.Plastic3DLookAndFeel;
import com.jgoodies.looks.plastic.PlasticXPLookAndFeel;
import org.vrjuggler.tweek.beans.*;
import org.vrjuggler.tweek.beans.loader.BeanInstantiationException;
import org.vrjuggler.tweek.event.FileActionGenerator;
import org.vrjuggler.tweek.event.FileActionListener;
import org.vrjuggler.tweek.event.UndoActionGenerator;
import org.vrjuggler.tweek.event.UndoActionListener;
import org.vrjuggler.tweek.gui.TweekFrame;
import org.vrjuggler.tweek.services.*;
import org.vrjuggler.tweek.text.MessageDocument;
import org.vrjuggler.tweek.net.corba.*;


/**
 * The singleton entry point into the Tweek Java GUI.
 */
public class TweekCore
   implements BeanInstantiationListener
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

   public void init(String[] args) throws Exception
   {
      // Register the internal static beans
      registerStaticBeans();

      // This needs to be the first step to ensure that all the basic services
      // and viewers get loaded.
      EnvironmentServiceProxy env_service = new EnvironmentServiceProxy();
      String default_path = env_service.getenv("TWEEK_BASE_DIR") +
                            File.separator +
                            env_service.getenv("TWEEK_DATA_DIR") +
                            File.separator + "beans";
      mBeanDirs.add(default_path);

      // Add in a user-specific Bean search path.
      try
      {
         GlobalPreferencesService service = new GlobalPreferencesServiceProxy();
         String dir_name = service.getPrefsDir() + File.separator + "beans";
         File bean_dir = new File(dir_name);

         if ( bean_dir.exists() && ! bean_dir.isDirectory() )
         {
            System.err.println("WARNING: " + dir_name +
                               " exists, but not as a directory!");
         }
         // If the directory does not exist, create it to prevent a warning
         // from being printed later.
         else if ( ! bean_dir.exists() )
         {
            bean_dir.mkdir();
         }

         mBeanDirs.add(dir_name);
      }
      catch(RuntimeException ex)
      {
         System.err.println("WARNING: Failed to add user-specific directory " +
                            "to the default Bean search path");
         System.err.println(ex.getMessage());
      }

      // As a side effect, the following may add more paths to mBeanDirs.
      String[] new_args = parseTweekArgs(args);

      // Explicitly load the global preferences now.  The
      // GlobalPreferencesService class does not load them automatically.  This
      // must happen after the command line arguments have been parsed so that
      // the user can specify an alternate preferences file.
      try
      {
         GlobalPreferencesService global_prefs =
            new GlobalPreferencesServiceProxy();
         global_prefs.load();

         // Set the look and feel now so that any GUI components that are
         // instantiated hereafter will have the correct look and feel.
         // XXX: If there are GUI components loaded statically (see above),
         // they will need to be updated.
         setLookAndFeel(global_prefs);
      }
      catch(java.io.IOException ex)
      {
         // This exception should never be thrown because failure to
         // register to GlobalPreferences Service Bean is a fatal error.
      }

      // Register the command-line arguments with the Environment Service (if
      // it is available).
      try
      {
         EnvironmentService service = new EnvironmentServiceProxy();
         service.setCommandLineArgs(new_args);
      }
      catch(RuntimeException e)
      {
         // Use System.err here because the GUI has not been displayed yet.
         System.err.println("WARNING: Failed to register command-line arguments");
      }

      // We need our TweekFrame instance before any dynamically discovered
      // Beans are loaded (or instantiated) because TweekFrame needs to know
      // about Bean instantiations.  Furthermore, any given Bean may register
      // information (directly or indirectly) with TweekFrame in its
      // constructor, so TweekFrame needs to exist first.
      m_gui = new TweekFrame(messageDocument);

      // Loop over all the known Bean directories to search for and load any
      // Beans that are found.  This must occur after the global preferences
      // have been loaded so that the user can enable or disable lazy Panel
      // Bean instantiation.
      Iterator i = mBeanDirs.iterator();
      while ( i.hasNext() )
      {
         String path = (String) i.next();

         try
         {
            findAndLoadBeans(path);
         }
         catch (BeanPathException e)
         {
            System.out.println("WARNING: Invalid path " + path);
         }
      }

      // Now we need to register the TweekFrame instance as a listener for
      // BeanFocusChangeEvents.
      List viewer_beans = BeanRegistry.instance().getBeansOfType( ViewerBean.class.getName() );
      for ( Iterator itr = viewer_beans.iterator(); itr.hasNext(); )
      {
         BeanModelViewer v = ((ViewerBean)itr.next()).getViewer();
         v.addBeanFocusChangeListener(m_gui);
      }

      m_gui.initGUI();

      // Now select the default bean if necessary
      if (defaultBean != null)
      {
         messageDocument.printStatusnl("Trying to focus default bean: " +
                                       defaultBean);
         TweekBean bean = BeanRegistry.instance().getBean(defaultBean);

         // Valid the bean registered under the default bean's name
         if (bean == null)
         {
            messageDocument.printWarningnl("WARNING: Default Bean doesn't exist");
         }
         else if (! (bean instanceof PanelBean))
         {
            messageDocument.printWarningnl("WARNING: Default Bean is not a Panel Bean");
         }
         else
         {
            ViewerBean viewer = m_gui.getBeanViewer();
            if (viewer != null)
            {
               viewer.getViewer().focusBean((PanelBean)bean);
            }
         }
      }
   }

   /**
    * Registers the beans that are internal to Tweek that are required to exist
    * before the bean loading can begin.
    */
   public static void registerStaticBeans()
   {
      BeanRegistry registry = BeanRegistry.instance();

      // environment service
      registry.registerBean( new EnvironmentServiceImpl(
            new BeanAttributes( "Environment" ) ) );

      // global preferences service
      try
      {
         registry.registerBean( new GlobalPreferencesServiceImpl(
               new BeanAttributes( "GlobalPreferences" ) ) );
      }
      catch(java.io.IOException ex)
      {
         System.err.println("FATAL ERROR: Could not register " +
                            "GlobalPreferences Service Bean!");
         System.err.println(ex.getMessage());
         System.exit(1);
      }
   }

   /**
    * Called by the BeanInstantiationCommunicator singleton whenever a new bean
    * is instantiated.
    */
   public void beanInstantiated (BeanInstantiationEvent evt)
   {
      // If the bean created is a viewer bean, initialize it with tweek
      TweekBean bean = evt.getTweekBean();
      if ( bean instanceof ViewerBean ) {
         BeanModelViewer viewer = ((ViewerBean)bean).getViewer();
         viewer.setModel(panelTreeModel);
         viewer.initGUI();
      }
   }


   /**
    * Look for TweekBeans in standard locations and register them in the
    * registry.
    *
    * @param path    the path in which to search for TweekBeans
    */
   public void findAndLoadBeans( String path )
      throws BeanPathException
   {
      // Get the beans in the given path and add them to the dependency manager
      XMLBeanFinder finder = new XMLBeanFinder(mValidateXML);
      loadBeans(finder.find(path));
   }

   public void loadBeans(List beans)
   {
      // Just to be safe...
      if ( null == beans )
      {
         return;
      }

      mDepManager = BeanDependencyManager.instance();
      
      for ( Iterator itr = beans.iterator(); itr.hasNext(); )
      {
         TweekBean bean = (TweekBean)itr.next();
         mDepManager.add(bean);
      }

      // Instantiate the beans pending in the dependency manager
      while (mDepManager.hasBeansPending())
      {
         TweekBean bean = (TweekBean)mDepManager.pop();
         if (bean == null)
         {
            // There are more beans pending, but they all have unsatisfied
            // dependencies so we can instantiate them ...
            break;
         }

         // Try to instantiate the Bean.
         try
         {
            boolean lazy_inst = true;

            // This service is loaded statically, so we do not have to worry
            // about finding the Bean first.
            try
            {
               GlobalPreferencesService prefs =
                  new GlobalPreferencesServiceProxy();
               lazy_inst = prefs.getLazyPanelBeanInstantiation();
            }
            catch(java.io.IOException ioEx)
            {
            }

            // If the current Bean is not a Panel Bean or the user has disabled
            // lazy Panel Bean instantiation, we can instantiate the Bean.
            if ( ! (bean instanceof org.vrjuggler.tweek.beans.PanelBean) ||
                 ! lazy_inst )
            {
               bean.instantiate();
            }

            BeanRegistry.instance().registerBean(bean);
         }
         catch(BeanInstantiationException e)
         {
            messageDocument.printWarningnl("WARNING: Failed to instantiate Bean '" +
                                           bean.getName() + "': " +
                                           e.getMessage());
         }
         catch(RuntimeException ex)
         {
            ex.printStackTrace();
         }
      }
   }

   public BeanTreeModel getPanelTreeModel()
   {
      return panelTreeModel;
   }

   /**
    * Gets the name of the panel bean to select/instantiate by default when
    * Tweek is started.
    *
    * @return  the name of the default bean; null if there is no default
    */
   public String getDefaultBean()
   {
      return defaultBean;
   }

   /**
    * Gets the message document used for displaying run-time informational
    * messages in the Tweek Java GUI.
    *
    * @since 0.92.2
    */
   public MessageDocument getMessageDocument()
   {
      return messageDocument;
   }

   /**
    * Registers all the objects interested in file action events with the
    * given file action generator.
    *
    * @param gen        the file action generator associated with a Tweek
    *                   Bean that implements the FileLoader interface.
    *
    * @see org.vrjuggler.tweek.beans.FileLoader
    *
    * @since 0.92.3
    */
   public void registerFileActionGenerator(FileActionGenerator gen)
   {
     gen.addFileActionListener(m_gui);
   }

   /**
    * Un-registers all the objects interested in file action events with the
    * given file action generator.
    *
    * @param gen        the file action generator associated with a Tweek
    *                   Bean that implements the FileLoader interface.
    *
    * @see org.vrjuggler.tweek.beans.FileLoader
    *
    * @since 0.92.3
    */
   public void unregisterFileActionGenerator(FileActionGenerator gen)
   {
     gen.removeFileActionListener(m_gui);
   }

   /**
    * Returns all the FileActionListener objects known to the Tweek Java GUI.
    *
    * @since 0.92.3
    */
   public FileActionListener[] getFileActionListeners()
   {
      return new FileActionListener[]{m_gui};
   }

   /**
    * Registers all the objects interested in undo action events with the
    * given undo action generator.
    *
    * @param gen        the undo action generator associated with a Tweek
    *                   Bean that implements the UndoHandler interface.
    *
    * @see org.vrjuggler.tweek.beans.UndoHandler
    *
    * @since 0.92.4
    */
   public void registerUndoActionGenerator(UndoActionGenerator gen)
   {
     gen.addUndoActionListener(m_gui);
   }

   /**
    * Un-registers all the objects interested in undo action events with the
    * given undo action generator.
    *
    * @param gen        the undo action generator associated with a Tweek
    *                   Bean that implements the UndoHandler interface.
    *
    * @see org.vrjuggler.tweek.beans.UndoHandler
    *
    * @since 0.92.4
    */
   public void unregisterUndoActionGenerator(UndoActionGenerator gen)
   {
     gen.removeUndoActionListener(m_gui);
   }

   /**
    * Returns all the UndoActionListener objects known to the Tweek Java GUI.
    *
    * @since 0.92.4
    */
   public UndoActionListener[] getUndoActionListeners()
   {
      return new UndoActionListener[]{m_gui};
   }

   /**
    * Default constructor.
    */
   protected TweekCore()
   {
      BeanInstantiationCommunicator.instance().addBeanInstantiationListener( this );
   }

   /**
    * Looks through the given array of arguments for any that are specific to
    * the Tweek Java GUI.  Those that are recognized are removed and handled
    * here.  Unrecognized options are left in the array.  The remaining
    * arguments are returned to the caller in a new array.
    *
    * Post condition: Any Tweek-specific arguments are removed and a new
    * array without those arguments is returned.
    */
   protected String[] parseTweekArgs (String[] args)
   {
      Vector save_args = new Vector();

      for ( int i = 0; i < args.length; i++ )
      {
         if ( args[i].startsWith("--beanpath=") )
         {
            int start = args[i].indexOf('=') + 1;
            String path = args[i].substring(start);
            mBeanDirs.add(path);
         }
         else if ( args[i].startsWith("--prefs=") )
         {
            int start   = args[i].indexOf('=') + 1;
            String path = args[i].substring(start);
            GlobalPreferencesService prefs =
               (GlobalPreferencesService) BeanRegistry.instance().getBean("GlobalPreferences");
            prefs.setFileName(path);
         }
         else if ( args[i].startsWith("--defaultbean=") )
         {
            int start = args[i].indexOf('=') + 1;
            // Replace instances of non-breaking space characters with ASCII
            // space characters.
            defaultBean = args[i].substring(start).replace('\u00A0', ' ');
         }
         else if ( args[i].startsWith("--validate") )
         {
            mValidateXML = true;
         }
         else if ( args[i].startsWith("--help") || args[i].startsWith("-h") )
         {
            printUsage();
            System.exit(0);
         }
         else
         {
            save_args.add(args[i]);
         }
      }

      String[] new_args = null;

      if ( save_args.size() > 0 )
      {
         new_args = new String[save_args.size()];

         for ( int i = 0; i < save_args.size(); i++ )
         {
            new_args[i] = (String) save_args.elementAt(i);
         }
      }

      return new_args;
   }

   /**
    * Prints the usage information for running the Tweek Java GUI.
    */
   protected void printUsage()
   {
      System.out.println("--help, -h");
      System.out.println("    Prints this usage information");
      System.out.println("--beanpath=<path>");
      System.out.println("    Specifiy one or more additional directories where Beans may be found");
      System.out.println("    When using a list of directories, use the platform-specific path separator");
      System.out.println("--defaultbean=<Panel Bean name>");
      System.out.println("    Name a Panel Bean to load automatically at startup");
      System.out.println("--prefs=<file>");
      System.out.println("    Name a specific preferences file to load instead of <home-dir>/.tweekrc");
      System.out.println("--validate");
      System.out.println("    Enable validation of XML Bean list documents");
   }

   // ========================================================================
   // Protected data members.
   // ========================================================================

   protected static TweekCore m_instance = null;

   // ========================================================================
   // Private methods.
   // ========================================================================

   /**
    * Sets the look and feel for the GUI.  This assumes that the GUI will be
    * based on Swing and that Swing is available.
    */
   private void setLookAndFeel(GlobalPreferencesService prefs)
   {
      // Install extra look and feels.
      UIManager.installLookAndFeel("Kunststoff",
                                   KunststoffLookAndFeel.class.getName());
      KunststoffLookAndFeel.setIsInstalled(true);

      UIManager.installLookAndFeel("Kunststoff Mini",
                                   KunststoffMiniLookAndFeel.class.getName());
      KunststoffMiniLookAndFeel.setIsInstalled(true);

      // These install themselves with the UI Manager automatically.
      new LiquidLookAndFeel();
      new net.sourceforge.mlf.metouia.MetouiaLookAndFeel();

      UIManager.installLookAndFeel("JGoodies Windows",
                                   WindowsLookAndFeel.class.getName());
      UIManager.installLookAndFeel("JGoodies Plastic",
                                   PlasticLookAndFeel.class.getName());
      UIManager.installLookAndFeel("JGoodies Plastic 3D",
                                   Plastic3DLookAndFeel.class.getName());
      UIManager.installLookAndFeel("JGoodies Plastic XP",
                                   PlasticXPLookAndFeel.class.getName());

      try
      {
         UIManager.setLookAndFeel(prefs.getLookAndFeel());
      }
      catch (Exception e)
      {
         messageDocument.printWarningnl("Failed to set look and feel to " +
                                        prefs.getLookAndFeel());
      }
   }

   // ========================================================================
   // Private data members.
   // ========================================================================

   /**
    * The name of the panel bean to select by default when Tweek is started.
    */
   private String defaultBean = null;

   /**
    * The bean dependency manager used to load beans in a stable order.
    */
   private BeanDependencyManager mDepManager;

   private boolean mValidateXML = false;
   private List    mBeanDirs    = new ArrayList();

   private MessageDocument messageDocument = new MessageDocument();
   private TweekFrame m_gui = null;

   private BeanTreeModel panelTreeModel =
      new BeanTreeModel(new DefaultMutableTreeNode());
}
