/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



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
//import java.awt.Color;
import javax.swing.*;
import VjGUI.*;
import VjConfig.*;
import VjPerf.*;

public class Core {

    static Core instance;

    static public ChunkOrgTree chunkorgtree;
    static public Vector chunkdbs;
    static public Vector descdbs;
    static public ChunkDescDB descdb;
    static public ConfigChunkDB active_chunkdb;
    static public NetControl net;
    static public ControlUI ui;
    static public ConfigChunkDB gui_chunkdb;
    static public ConfigChunkDB default_chunkdb;
    static public ConfigChunk vjcontrol_preferences = null;

    static public PerfDataCollection perf_collection;

    static private Vector coredb_targets;
    static private Vector logmessage_targets;

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
	ConfigChunkDB db;

	instance = new Core();
	
	window_pos_kludge = false;
	screenWidth = 800;
	screenHeight = 600;
	descdbs = new Vector();
	chunkdbs = new Vector();
	coredb_targets = new Vector();
	logmessage_targets = new Vector();
	perf_collection = new PerfDataCollection();
	chunkorgtree = new ChunkOrgTree();
	descdb = new ChunkDescDB();
	default_chunkdb = new ConfigChunkDB();
	ChunkFactory.setDefaultChunkDB (default_chunkdb);
	ChunkFactory.setChunkDescDB (descdb);
	gui_chunkdb = new ConfigChunkDB ();
	db = new ConfigChunkDB();
	db.setName ("Active Configuration");
	active_chunkdb = new ConfigChunkDB ();
	net = new NetControl();


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



    static public void enableActiveDB () {
	chunkdbs.addElement (active_chunkdb);
	notifyCoreDBTargets (ADD_CHUNKDB, active_chunkdb, null);
    }



    static public void disableActiveDB () {
	notifyCoreDBTargets (REMOVE_CHUNKDB, active_chunkdb, null);
	chunkdbs.removeElement (active_chunkdb);
    }


    static public void closeChunkDB (ConfigChunkDB db) {
	if (db == null || db == active_chunkdb)
	    return;
	notifyCoreDBTargets (REMOVE_CHUNKDB, db, null);
	chunkdbs.removeElement (db);
    }



    static public void closeDescDB (ChunkDescDB db) {
	if (db == null || db.name.equalsIgnoreCase ("No Selection") || db == descdb)
	    return;
	notifyCoreDBTargets (REMOVE_DESCDB, null, db);
	descdbs.removeElement (db);
    }




    static public String createUniqueChunkDBName (String base) {
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



    static public String createUniqueDescDBName (String base) {
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



//      static public void rebuildAllTrees () {
//  	// rebuilds all the trees in chunkdbs...
//  	ChunkDBTreeModel dbt;
//  	for (int i = 0; i < chunkdbs.size(); i++) {
//  	    dbt = (ChunkDBTreeModel)chunkdbs.elementAt(i);
//  	    dbt.buildTree();
//  	}
//      }



    static ConfigChunkDB getChunkDB (String name) {
	ConfigChunkDB db;
	for (int i = 0; i < chunkdbs.size(); i++) {
	    db = (ConfigChunkDB)chunkdbs.elementAt(i);
	    if (db.getName().equalsIgnoreCase(name))
		return db;
	}
	return null;
    }
    


    static ChunkDescDB getChunkDescDB (String name) {
	ChunkDescDB db;
	for (int i = 0; i < descdbs.size(); i++) {
	    db = (ChunkDescDB)descdbs.elementAt(i);
	    if (db.name.equalsIgnoreCase(name))
		return db;
	}
	return null;
    }



    static public void reconfigure() {
	// called whenever vjcontrol_preferences changes
	String looknfeel = null;
	int i;
	String s;
	Property p;
	int fontsize = -1;
	String fontname = "";
	String defaultchunkfile = null;

	ConfigChunk ch = vjcontrol_preferences;
	if (ch == null)
	    return;

	try {
	    ui.connection_pane.setHost (ch.getPropertyFromToken("host").getValue(0).getString());
	    ui.connection_pane.setPort (ch.getPropertyFromToken("port").getValue(0).getInt());
	    looknfeel = ch.getPropertyFromToken ("looknfeel").getValue(0).getString();

	    fontsize = ch.getPropertyFromToken("fontsize").getValue(0).getInt();
	    fontname = ch.getPropertyFromToken("fontname").getValue(0).getString();
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

	ui.totalSetFont (fontname, fontsize);
	ui.setLookNFeel (looknfeel);
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

    static protected void notifyLogMessageTargets (LogMessageEvent e) {
	Vector l;
	synchronized (logmessage_targets) {
	    l = (Vector) logmessage_targets.clone();
	}
	for (int i = 0; i < l.size(); i++) {
	    LogMessageListener lis = (LogMessageListener)l.elementAt (i);
	    lis.logMessage (e);
	}
    }



}


