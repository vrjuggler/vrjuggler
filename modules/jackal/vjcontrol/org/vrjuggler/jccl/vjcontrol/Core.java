/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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




package VjControl;

import java.lang.String;
import java.lang.StringBuffer;
import java.util.Vector;
import java.util.ArrayList;
import java.util.List;

import VjConfig.*;
import VjControl.LogMessageListener;
import VjControl.LogMessageEvent;
import VjControl.CoreModule;
import VjControl.VjComponent;
import VjControl.UserProfile;


/** Static owner-of-all-vjcontrol-objects.
 *  The primary job of the Core object is to own VjControl's core
 *  modules, such as the PerformanceModule or the UI.  Any component
 *  can call the Core's methods to get access to one of these core
 *  modules.  For example, the PerfAnalyzer GUI component requests
 *  access to the PerformanceModule, which stores the actual 
 *  performance data.
 *  <p>
 *  Core also contains a set of routines for outputting error and
 *  status messages (ConsoleErrorMessage etc.).  Core can print
 *  errors to stdout, and other components can register with Core
 *  to receive message notification.  This is used by ControlUI 
 *  to provide a status bar and by ConsolePane to record messages.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class Core 
    implements ChunkDBListener,
               VjComponent {


    /** ComponentFactory which creates all VjControl components. */
    static public ComponentFactory component_factory;


    /** Global Descdb (used by VjConfig.ChunkFactory).
     *  This is kind of messy... this descdb is for vjcontrols' own 
     *  configuration, and initially it's the one used by ChunkFactory.
     *  But, when the ConfigModule is loaded, it creates its own "global"
     *  ChunkDescDB which subsumes this descdb's contents and 
     *  responsibilities.  This is mainly because a) it's easier to keep
     *  things like ControlUI updated from that one and b) there is no
     *  b.
     *  The ideal solution would be separate ChunkFactories for the GUI's
     *  own config files and the data files vjcontrol's supposed to edit.
     *  Barring creating two copies of the Config code in separate 
     *  packages, I can't actually do that.  <sigh>
     */
    static public ChunkDescDB descdb;
    static public ConfigChunkDB vjcontrol_chunkdb;
    static public ConfigChunkDB default_chunkdb;

    // private data

    static protected String component_name;

    /** Contains all CoreModules added to self */
    static private List modules;

    static private List pending_chunks;
    static private List registered_components;

    static private String[] command_line;

    /** Dummy instance used as source for CoreDBEvents. */
    static private Core instance;

    /* logmessage stuff */
    static private List logmessage_targets;
    static protected boolean no_logmessage_targets;
    static final private boolean info_msg_to_stdout = true;
    static final private boolean error_msg_to_stdout = true;

    /* this stuff should move into ui - deal with this when i muck with
     * prefs later this summer.
     */
    static public int    screenWidth;
    static public int    screenHeight;
    static public boolean window_pos_kludge;

    static public UserProfile user_profile;

    static public FileControl file;

    private Core() {
    }



    static public void initialize () {

	instance = new Core();
	
        component_name = "VjControl Core";

        file = new FileControl();

        component_factory = new ComponentFactory();
        modules = new Vector();
        pending_chunks = new Vector();
        registered_components = new Vector();

	window_pos_kludge = false;
	screenWidth = 800;
	screenHeight = 600;
        user_profile = new UserProfile();

	logmessage_targets = new ArrayList();
        no_logmessage_targets = true;

        command_line = new String[0];

  	descdb = new ChunkDescDB();
        descdb.setName ("VjControl default Global ChunkDescDB");
	default_chunkdb = new ConfigChunkDB();
	ChunkFactory.setDefaultChunkDB (default_chunkdb);
	ChunkFactory.setChunkDescDB (descdb);
	vjcontrol_chunkdb = new ConfigChunkDB ();

	vjcontrol_chunkdb.addChunkDBListener (instance);

        registerComponent (instance);

    }



    /** Gives a reference to the named module */
    static public CoreModule getModule (String name) {
        CoreModule c;
        for (int i = 0; i < modules.size(); i++) {
            c = (CoreModule)modules.get (i);
            if (c.getComponentName().equalsIgnoreCase (name))
                return c;
        }
        return null;
    }



    /** Updates self based on VjControl global prefs */
    static protected void reconfigure(ConfigChunk ch) {
	// called whenever vjcontrol_preferences changes
	int i;
	String s;
	Property p;
	String defaultchunkfile = null;

	if (ch == null)
	    return;

	try {
            user_profile.setFromConfigChunk (ch);

	    //window_pos_kludge = ch.getPropertyFromToken("windowposkludge").getValue(0).getBool();

	    screenWidth = ch.getPropertyFromToken("windowsize").getValue(0).getInt();
	    screenHeight = ch.getPropertyFromToken("windowsize").getValue(1).getInt();	   

	}
	catch (Exception e) {
	    Core.consoleInfoMessage (component_name, "Old vjcontrol preferences file - please check preferences and re-save");
	}

	if (screenWidth < 1)
	    screenWidth = 800;
	if (screenHeight < 1)
	    screenHeight = 600;

    }



    //---------------------- Configuration Stuff ----------------------------

    // All of this code needs to be refactored somewhat to handle removing
    // as well as adding components.  in-place reconfiguration is also a 
    // good thing before the spec solidifies in juggler 1.2.0

    /** Adds a component creation ConfigChunk to the queue of pending adds.
     *  Once the ConfigChunk's dependencies are met, it will be forwarded
     *  to its parent component (via VjComponent.configAdd()).
     *  In most cases, the parent will immediately insantiate the new 
     *  component, but there are cases where the parent will instead
     *  create the new component "on demand".  A good example is the
     *  ConfigUIHelper Module's use of ChunkEditorPanels.  For more
     *  details, check the parent class' documentation.
     */
    static public void addPendingChunk (ConfigChunk ch) {
        pending_chunks.add (ch);
        //System.out.println ("addPendingChunk:\n" + ch.toString());
        while (configProcessPending())
            ;
    }


    /** Processes the pending ConfigChunk list.  Util for addPendingChunk(). 
     *  Searches for Chunks in the pending list whose dependencies are met
     *  and calls addComponentConfig.
     *  @return True if any Components were created (or attempted).
     */
    static protected boolean configProcessPending () {
        boolean retval = false;
        boolean add_success;
        ConfigChunk ch;
        for (int i = 0; i < pending_chunks.size(); ) {
            ch = (ConfigChunk)pending_chunks.get(i);
            if (checkDependencies (ch)) {
                add_success = addComponentConfig (ch);
                pending_chunks.remove(i);
                if (add_success) 
                    Core.consoleInfoMessage (component_name, "Add Component: " 
                                             + ch.getName());
                else
                    Core.consoleErrorMessage (component_name, 
                                              "Add Component failed: " 
                                              + ch.getName());
                retval = true;
            }
            else
                i++;
        }
        return retval;
    }


    /** Finds which component is ch's parent and calls addConfig for it.
     *  This shouldn't be confused with Core.addConfig, which just creates
     *  components that are children of Core directly.  addComponentConfig
     *  calls Core.addConfig if ch's parent is actually "VjControl Core".
     *
     *  @return true if the component was succesfully added.
     */
    static protected boolean addComponentConfig (ConfigChunk ch) {
        try {
            Property p = ch.getPropertyFromToken ("parentcomp");
            String parentname = p.getValue(0).getString();
            VjComponent vjc = getComponentFromRegistry (parentname);
            if (vjc.addConfig (ch))
                return true;
            else {
                consoleErrorMessage (component_name, 
                                     "Creating component failed: " 
                                     + ch.getName());
                return false;
            }
        }
        catch (Exception e) {
            consoleErrorMessage (component_name, "Creating component failed: " 
                                 + ch.getName());
            e.printStackTrace();
            return false;
        }
    }



    /** Checks if this component chunk's dependencies are met.
     *  This checks the chunk's "ParentComp" and "Dependencies"
     *  properties and makes sure all the components listed there 
     *  have already been added to VjControl.
     *
     *  @return true if all dependencies were met.
     */
    static protected boolean checkDependencies (ConfigChunk ch) {

        String cn = ch.getValueFromToken ("classname", 0).getString();
        if (!component_factory.isRegistered (cn))
            return false;

        List v = ch.getDependencyNames();
        for (int i = 0; i < v.size(); i++) {
            String s = (String)v.get(i);
            if (getComponentFromRegistry(s) == null) {
                return false;
            }
        }
        return true;
    }


    /** Adds vjc to a list of components that are part of VjControl.
     *  When addConfig is called on a component chunk's "parent"
     *  component, the parent should generally call Core.registerComponent()
     *  to make the new child visible.
     *  Components must be registered with this method if they are 
     *  to be parents or dependencies of other components.
     */
    static public void registerComponent (VjComponent vjc) {
        registered_components.add (vjc);
    }


    /** Finds a component that has been added with registerComponent().
     */
    static public VjComponent getComponentFromRegistry (String name) {
        VjComponent vjc;
        int i, n = registered_components.size();
        for (i = 0; i < n; i++) {
            vjc = (VjComponent)registered_components.get(i);
            if (vjc.getComponentName().equalsIgnoreCase (name))
                return vjc;
        }
        return null;
    }


    //---------------------- Command-line argument handling -----------------

    static public void setCommandLine (String[] args) {
        command_line = args;
    }

    /** Gets the command line that VjControl was started with.
     *  Components can search this command line for options they recognize.
     *  For example, the ConfigModule searches the command line for the
     *  -c and -d options, and for words ending with ".config" or ".desc".
     *  Components MUST NOT modify the returned String array in any way.
     */
    static public String[] getCommandLine () {
        return command_line;
    }


    //---------------------- Logging Routines -------------------------------

   
    /** Sends a "temporary" message to LogMessageListeners.
     *  Temporary messages should be put in status bars, etc., but do
     *  not generally have to be recorded in things like the main Messages
     *  console panel.  The individual listener is, of course, allowed to
     *  decide whether to display the message or not.
     */
    public static void consoleTempMessage (String s) {
	consoleTempMessage (null, s);
    }

    /** Sends a "temporary" message to LogMessageListeners.
     *  As consoleTempMessage (String s) but includes a source identifier.
     */
    public static void consoleTempMessage (String source, String s) {
	if (info_msg_to_stdout || no_logmessage_targets)
	    System.out.println (source + ": " + s);
	if (s == null || s.equals (""))
	    s = " ";

	notifyLogMessageTargets (new LogMessageEvent (instance, source, s, LogMessageEvent.TEMPORARY_MESSAGE));
    }


    public static void consoleInfoMessage (String s) {
	consoleInfoMessage ("", s);
    }
    public static void consoleInfoMessage (String source, String s) {
	if (info_msg_to_stdout || no_logmessage_targets)
	    System.out.println (source + ": " + s);
	notifyLogMessageTargets (new LogMessageEvent (instance, source, s, LogMessageEvent.PERMANENT_MESSAGE));
    }


    public static void consoleErrorMessage (String s) {
	consoleInfoMessage ("", s);
    }
    public static void consoleErrorMessage (String source, String s) {
	if (error_msg_to_stdout || no_logmessage_targets)
	    System.out.println ("Error (" + source + "): " + s);
	notifyLogMessageTargets (new LogMessageEvent (instance, source, s, LogMessageEvent.PERMANENT_ERROR));
    }



    //-------------------- LogMessage Target Stuff --------------------------

    static public synchronized void addLogMessageListener (LogMessageListener l) {
	synchronized (logmessage_targets) {
	    logmessage_targets.add (l);
            no_logmessage_targets = false;
	}
    }

    static public void removeLogMessageListener (LogMessageListener l) {
	synchronized (logmessage_targets) {
	    logmessage_targets.remove (logmessage_targets.indexOf(l));
            if (logmessage_targets.size() == 0)
                no_logmessage_targets = true;
	}
    }

    /** Used to ensure that logMessageListeners are notified by the AWT thread.
     */
    static protected class doNotifyLogMessageTargets implements Runnable {
        private LogMessageEvent event;
        private LogMessageListener[] targets;
        public doNotifyLogMessageTargets (LogMessageEvent e, LogMessageListener[] t) {
            event = e;
            targets = t;
        }

        public void run () {
            for (int i = 0; i < targets.length; i++) {
                targets[i].logMessage (event);
            }
        }
    };


    static protected void notifyLogMessageTargets (LogMessageEvent e) {
        if (!no_logmessage_targets) {
            // get array so we don't hold the lock for too long.
            LogMessageListener[] l;
            synchronized (logmessage_targets) {
                l = new LogMessageListener[logmessage_targets.size()];
                l = (LogMessageListener[])logmessage_targets.toArray(l);
            }
            if (javax.swing.SwingUtilities.isEventDispatchThread()) {
                for (int i = 0; i < l.length; i++) {
                    l[i].logMessage (e);
                }
            }
            else {
                try {
                    javax.swing.SwingUtilities.invokeAndWait (new
                        doNotifyLogMessageTargets (e, l));
                }
                catch (Exception ex) {
                }
            }
        }
    }



    //-------------------- ChunkDBListener Stuff ----------------------------

    /** Core listens to ChunkDBEvents on the gui db for vjcontrol config */
    public void addChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeChunk (ChunkDBEvent e) {;}
    public void replaceChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeAllChunks (ChunkDBEvent e) {;}



    //-------------------- VjComponent Stuff --------------------------------

    public String getComponentName () {
        return component_name;
    }


    public boolean configure (ConfigChunk ch) {
        System.out.println ("Core doesn't really configure itself you putz." +
                            "Why is this even getting called?");
        return true;
    }


    public ConfigChunk getConfiguration () {
        return null;
    }


    public boolean addConfig (ConfigChunk ch) {
        try {
            String classname = ch.getValueFromToken ("classname", 0).getString();
            VjComponent vjc = component_factory.createComponent(classname);
            if (vjc.configure(ch)) {
                modules.add (vjc);
                registerComponent (vjc);
                return true;
            }
            else {
                return false;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }


    public boolean removeConfig (String name) {
        return false;
    }


    /** This is a kludgey way of making sure that certain things get
     *  called at exit, such as ConfigSaveMonitorModule's checking.
     */
    public void destroy () {
        for (int i = 0; i < modules.size(); i++) {
            CoreModule cm = (CoreModule)modules.get(i);
            cm.destroy();
        }
        System.exit(0);
    }

}


