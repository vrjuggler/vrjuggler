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



/*
 * The primary job of the Core object is to own the primary databases
 * of chunks and chunkdescs.  It's also a primary source of events.
 * Objects can register with the Core to receive event notification
 * for the addition and removal of ChunkDBs or DescDBs (Objects can
 * register with the individual dbs for change notification).
 */

package VjGUI;

import java.lang.String;
import java.lang.StringBuffer;
import java.util.Vector;
import javax.swing.*;
import VjGUI.*;
import VjConfig.*;
import VjPerf.*;

public class Core implements NetControlListener, ChunkDBListener {

    static public ComponentFactory factory;

    // main components that the GUI elements are allowed to access
    static public ChunkOrgTree chunkorgtree;
    static public Vector chunkdbs;
    static public Vector descdbs;
    static public ChunkDescDB descdb;
    static public ConfigChunkDB active_chunkdb;
    static public NetControl net;
    static public ControlUI ui;
    static public ConfigChunkDB gui_chunkdb;
    static public ConfigChunkDB default_chunkdb;
    static public PerfDataCollection perf_collection;

    // private data
    static private Core instance; // dummy instance used as source for CoreDBEvents
    static private Vector coredb_targets;
    static private Vector logmessage_targets;
    static private Vector GUIrefresh_targets;

    static final private boolean info_msg_to_stdout = false;
    static final private boolean error_msg_to_stdout = true;
    static public int    screenWidth;
    static public int    screenHeight;
    static public boolean window_pos_kludge;

    static ImageIcon new_icn;
    static ImageIcon load_icn;
    static ImageIcon save_icn;
    static ImageIcon close_icn;

    static final private int ADD_CHUNKDB = 1;
    static final private int REMOVE_CHUNKDB = 2;
    static final private int ADD_DESCDB = 3;
    static final private int REMOVE_DESCDB = 4;


    private Core() {
    }


    static public ConfigChunk findPrefixMatchChunk (String name) {
	/* finds a chunk whose name is a prefix of name */
	ConfigChunk ch;
	ConfigChunkDB db;
	int i, j;
	for (i = 0; i < chunkdbs.size(); i++) {
	    db = (ConfigChunkDB)chunkdbs.elementAt(i);
	    for (j = 0; j < db.size(); j++) {
		ch = (ConfigChunk)db.elementAt(j);
		if (name.startsWith (ch.getName()))
		    return ch;
	    }
	}
	return null;
    }



    static public void initialize () {

	instance = new Core();
	
        factory = new ComponentFactory();
        factory.registerPanel ("VjGUI.ConfigurePane", "");
        factory.registerPanel ("VjGUI.ConnectionPane", "");
        factory.registerPanel ("VjGUI.DescDBPanel", "");
        factory.registerPanel ("VjGUI.ChunkOrgTreePane", "");
        factory.registerPanel ("VjGUI.ConsolePane", "");
        factory.registerPanel ("VjPerf.PerfAnalyzerPanel", "");


	window_pos_kludge = false;
	screenWidth = 800;
	screenHeight = 600;
	descdbs = new Vector();
	chunkdbs = new Vector();
	coredb_targets = new Vector();
	logmessage_targets = new Vector();
        GUIrefresh_targets = new Vector();
	perf_collection = new PerfDataCollection();
	chunkorgtree = new ChunkOrgTree();
	descdb = new ChunkDescDB();
	default_chunkdb = new ConfigChunkDB();
	ChunkFactory.setDefaultChunkDB (default_chunkdb);
	ChunkFactory.setChunkDescDB (descdb);
	gui_chunkdb = new ConfigChunkDB ();
	active_chunkdb = new ConfigChunkDB();
	active_chunkdb.setName ("Active Configuration");
	net = new NetControl();

	net.addNetControlListener (instance);
	gui_chunkdb.addChunkDBListener (instance);

	new_icn = new ImageIcon (ClassLoader.getSystemResource ("VjFiles/new.gif"))
;
	load_icn = new ImageIcon (ClassLoader.getSystemResource ("VjFiles/open.gif"));
	save_icn = new ImageIcon (ClassLoader.getSystemResource ("VjFiles/save.gif"));
	close_icn = new ImageIcon (ClassLoader.getSystemResource ("VjFiles/close.gif"));
	if (new_icn == null)
	    new_icn = new ImageIcon();
	if (load_icn == null)
	    load_icn = new ImageIcon();
	if (save_icn == null)
	    save_icn = new ImageIcon();
	if (close_icn == null)
	    close_icn = new ImageIcon();


	ui = new ControlUI();
    }



    //: Removes a ChunkDB from the system
    static public void closeChunkDB (ConfigChunkDB db) {
	if (db == null || db == active_chunkdb)
	    return;
	notifyCoreDBTargets (REMOVE_CHUNKDB, db, null);
	chunkdbs.removeElement (db);
    }


    
    //: Removes a DescDB from the system
    static public void closeDescDB (ChunkDescDB db) {
	if (db == null || db.name.equalsIgnoreCase ("No Selection") || db == descdb)
	    return;
	notifyCoreDBTargets (REMOVE_DESCDB, null, db);
	descdbs.removeElement (db);
    }



    static protected String createUniqueChunkDBName (String base) {
	// returns a string, starting with base, which doesn't
	// conflict with names of any dbs in chunkdbs.
	int i;
	String name;

	if (getChunkDB (base) == null)
	    return base;
	for (i = 2; true; i++) {
	    name = base + " <" + i + ">";
	    if (getChunkDB (name) == null)
		return name;
	}
    }



    static protected String createUniqueDescDBName (String base) {
	int i;
	String name;
	if (getChunkDescDB (base) == null)
	    return base;
	for (i = 2; true; i++) {
	    name = base + " <" + i + ">";
	    if (getChunkDescDB (name) == null)
		return name;
	}
    }



    static public String renameChunkDB (ConfigChunkDB _db, String newbase) {
	if (_db.name.equals (newbase))
	    return newbase;
	newbase = createUniqueChunkDBName (newbase);
	notifyCoreDBTargets (REMOVE_CHUNKDB, _db, null);
	_db.setName (newbase);
	notifyCoreDBTargets (ADD_CHUNKDB, _db, null);
	return newbase;
    }



    static public String renameDescDB (ChunkDescDB _db, String newbase) {
	if (_db.name.equals (newbase))
	    return newbase;
	newbase = createUniqueDescDBName (newbase);
	notifyCoreDBTargets (REMOVE_DESCDB, null, _db);
	_db.setName (newbase);
	notifyCoreDBTargets (ADD_DESCDB, null, _db);
	return newbase;
    }



    static public String addChunkDB (ConfigChunkDB _chunkdb) {
	_chunkdb.setName (createUniqueChunkDBName (_chunkdb.name));
	chunkdbs.addElement (_chunkdb);
	notifyCoreDBTargets (ADD_CHUNKDB, _chunkdb, null);
	return _chunkdb.name;
    }



    static public String addDescDB (ChunkDescDB _descdb) {
	_descdb.setName(createUniqueDescDBName (_descdb.name));
	descdbs.addElement( _descdb);
	descdb.addElements (_descdb);
	notifyCoreDBTargets (ADD_DESCDB, null, _descdb);
	return _descdb.name;
    }



    static protected ConfigChunkDB getChunkDB (String name) {
	ConfigChunkDB db;
	for (int i = 0; i < chunkdbs.size(); i++) {
	    db = (ConfigChunkDB)chunkdbs.elementAt(i);
	    if (db.getName().equalsIgnoreCase(name))
		return db;
	}
	return null;
    }
    


    static protected ChunkDescDB getChunkDescDB (String name) {
	ChunkDescDB db;
	for (int i = 0; i < descdbs.size(); i++) {
	    db = (ChunkDescDB)descdbs.elementAt(i);
	    if (db.name.equalsIgnoreCase(name))
		return db;
	}
	return null;
    }



    static protected void reconfigure(ConfigChunk ch) {
	// called whenever vjcontrol_preferences changes
	int i;
	String s;
	Property p;
	String defaultchunkfile = null;

	if (ch == null)
	    return;

	try {
	    //window_pos_kludge = ch.getPropertyFromToken("windowposkludge").getValue(0).getBool();

	    screenWidth = ch.getPropertyFromToken("windowsize").getValue(0).getInt();
	    screenHeight = ch.getPropertyFromToken("windowsize").getValue(1).getInt();
	    // default chunk files not dealt with here - only at startup.
	    //defaultchunkfile = ch.getPropertyFromToken("defaultchunkfiles").getValue(0).getString();
	   

	}
	catch (Exception e) {
	    Core.consoleInfoMessage ("vjControl", "Old vjcontrol preferences file - please check preferences and re-save");
	}

	if (screenWidth < 1)
	    screenWidth = 800;
	if (screenHeight < 1)
	    screenHeight = 600;

    }


    /************************ Logging Routines ******************************/

   
    public static void consoleTempMessage (String s) {
	/* like a consoleInfoMessage, but not logged */
	consoleTempMessage (null, s);
    }
    public static void consoleTempMessage (String source, String s) {
	if (info_msg_to_stdout)
	    System.out.println (source + ": " + s);
	if (s == null || s.equals (""))
	    s = " ";

	notifyLogMessageTargets (new LogMessageEvent (instance, source, s, LogMessageEvent.TEMPORARY_MESSAGE));
    }


    public static void consoleInfoMessage (String s) {
	consoleInfoMessage ("", s);
    }
    public static void consoleInfoMessage (String source, String s) {
	if (info_msg_to_stdout)
	    System.out.println (source + ": " + s);
	notifyLogMessageTargets (new LogMessageEvent (instance, source, s, LogMessageEvent.PERMANENT_MESSAGE));
    }


    public static void consoleErrorMessage (String s) {
	consoleInfoMessage ("", s);
    }
    public static void consoleErrorMessage (String source, String s) {
	if (error_msg_to_stdout)
	    System.out.println ("Error (" + source + "): " + s);
	notifyLogMessageTargets (new LogMessageEvent (instance, source, s, LogMessageEvent.PERMANENT_ERROR));
    }





    /******************** CoreDB Target Stuff *********************/

    static public synchronized void addCoreDBListener (CoreDBListener l) {
	synchronized (coredb_targets) {
	    coredb_targets.addElement (l);
	}
    }

    static public void removeCoreDBListener (CoreDBListener l) {
	synchronized (coredb_targets) {
	    coredb_targets.removeElement (l);
	}
    }

    static protected void notifyCoreDBTargets (int msgtype, ConfigChunkDB _chunkdb, ChunkDescDB _descdb) {
	Vector l;
	CoreDBEvent e = new CoreDBEvent (instance, _chunkdb, _descdb);
	synchronized (coredb_targets) {
	    l = (Vector) coredb_targets.clone();
	}
	for (int i = 0; i < l.size(); i++) {
	    CoreDBListener lis = (CoreDBListener)l.elementAt (i);
	    switch (msgtype) {
	    case ADD_CHUNKDB:
		lis.addChunkDB (e);
		break;
	    case REMOVE_CHUNKDB:
		lis.removeChunkDB (e);
		break;
	    case ADD_DESCDB:
		lis.addDescDB (e);
		break;
	    case REMOVE_DESCDB:
		lis.removeDescDB (e);
		break;
	    }
	}
    }



    /******************** LogMessage Target Stuff *********************/

    static public synchronized void addLogMessageListener (LogMessageListener l) {
	synchronized (logmessage_targets) {
	    logmessage_targets.addElement (l);
	}
    }

    static public void removeLogMessageListener (LogMessageListener l) {
	synchronized (logmessage_targets) {
	    logmessage_targets.removeElement (l);
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
//  	Vector l;
//  	synchronized (logmessage_targets) {
//  	    l = (Vector) logmessage_targets.clone();
//  	}
//  	for (int i = 0; i < l.size(); i++) {
//  	    LogMessageListener lis = (LogMessageListener)l.elementAt (i);
//  	    lis.logMessage (e);
//  	}
    }



    /******************** GUIrefresh Target Stuff *********************/

//      static public synchronized void addGUIRefreshListener (GUIRefreshListener l) {
//  	synchronized (GUIrefresh_targets) {
//  	    GUIrefresh_targets.addElement (l);
//  	}
//      }

//      static public void removeGUIRefreshListener (GUIRefreshListener l) {
//  	synchronized (GUIrefresh_targets) {
//  	    GUIrefresh_targets.removeElement (l);
//  	}
//      }

//      static protected void notifyGUIRefreshTargets (int msgtype) {
//  	Vector l;
//  	GUIRefreshEvent e = new GUIRefreshEvent (msgtype);
//  	synchronized (GUIrefresh_targets) {
//  	    l = (Vector) GUIrefresh_targets.clone();
//  	}
//  	for (int i = 0; i < l.size(); i++) {
//  	    GUIRefreshListener lis = (CoreDBListener)l.elementAt (i);
//              lis.refreshChunkDBDisplay(e);
//  //  	    switch (msgtype) {
//  //  	    case fpp:
//  //  		lis.addChunkDB (e);
//  //  		break;
//  //  	    case REMOVE_CHUNKDB:
//  //  		lis.removeChunkDB (e);
//  //  		break;
//  //  	    case ADD_DESCDB:
//  //  		lis.addDescDB (e);
//  //  		break;
//  //  	    case REMOVE_DESCDB:
//  //  		lis.removeDescDB (e);
//  //  		break;
//  //  	    }
//  	}
//      }



    /**************** NetControlListener Stuff *********************/
    public void openedConnection (NetControlEvent e) {
	chunkdbs.addElement (active_chunkdb);
	notifyCoreDBTargets (ADD_CHUNKDB, active_chunkdb, null);
    }

    public void closedConnection (NetControlEvent e) {
	notifyCoreDBTargets (REMOVE_CHUNKDB, active_chunkdb, null);
	chunkdbs.removeElement (active_chunkdb);
    }

    public void addressChanged (NetControlEvent e) {
    }


    /****************** ChunkDBListener Stuff **********************/
    //: Core only listens to ChunkDB events from the GUI chunkdb
    public void addChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeChunk (ChunkDBEvent e) {;}
    public void replaceChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeAllChunks (ChunkDBEvent e) {;}

}


