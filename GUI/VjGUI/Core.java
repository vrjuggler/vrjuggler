
/* holder for global variables & utility functions 
 */
package VjGUI;

import java.lang.String;
import java.lang.StringBuffer;
import java.util.Vector;
import java.awt.Color;
import javax.swing.*;
import VjGUI.*;
import VjConfig.*;
import VjPerf.*;

public class Core {

    static public ChunkOrgTree chunkorgtree;
    static public Vector chunkdbs;        // actually, a vector of ChunkDBTreeModel
    static public Vector descdbs;
    static public ChunkDescDB descdb;
    static public ChunkDBTreeModel active_treemodel;
    static public NetControl net;
    static public ControlUI ui;
    static public ConfigChunkDB gui_chunkdb;
    static public ConfigChunkDB default_chunkdb;
    static public ConfigChunk vjcontrol_preferences = null;

    static public PerfDataCollection perf_collection;

    static final boolean info_msg_to_stdout = false;
    static final boolean error_msg_to_stdout = true;
    static public int    screenWidth;
    static public int    screenHeight;
    static public boolean window_pos_kludge;

    static ImageIcon new_icn;
    static ImageIcon load_icn;
    static ImageIcon save_icn;
    static ImageIcon close_icn;

    static public ConfigChunk findPrefixMatchChunk (String name) {
	/* finds a chunk whose name is a prefix of name */
	ConfigChunk ch;
	ConfigChunkDB db;
	int i, j;
	for (i = 0; i < chunkdbs.size(); i++) {
	    db = ((ChunkDBTreeModel)chunkdbs.elementAt(i)).chunkdb;
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
	
	window_pos_kludge = false;
	screenWidth = 800;
	screenHeight = 600;
	descdbs = new Vector();
	chunkdbs = new Vector();
	perf_collection = new PerfDataCollection();
	chunkorgtree = new ChunkOrgTree();
	descdb = new ChunkDescDB();
	default_chunkdb = new ConfigChunkDB (descdb);
	ChunkFactory.setDefaultChunkDB (default_chunkdb);
	ChunkFactory.setChunkDescDB (descdb);
	gui_chunkdb = new ConfigChunkDB (descdb);
	db = new ConfigChunkDB(descdb);
	db.setName ("Active Configuration");
	active_treemodel = new ChunkDBTreeModel (db, chunkorgtree);
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
	chunkdbs.addElement (active_treemodel);
	ui.addChunkDBTree (active_treemodel);
    }



    static public void disableActiveDB () {
	chunkdbs.removeElement (active_treemodel);
	ui.removeChunkDBTree (active_treemodel);
    }

    static public void closeChunkDB (ChunkDBTreeModel dbt) {
	if (dbt == null || dbt == active_treemodel || dbt.getName().equalsIgnoreCase ("No Selection"))
	    return;
	ui.removeChunkDBTree (dbt);
	chunkdbs.removeElement (dbt);
    }



    static public void closeDescDB (ChunkDescDB db) {
	if (db == null || db.name.equalsIgnoreCase ("No Selection") || db == descdb)
	    return;
	ui.removeDescDB (db.name);
	descdbs.removeElement (db);
    }


    /*
    static public String addNewChunkDB () {
	// creates a new db, treemodel, etc.
	ConfigChunkDB db = new ConfigChunkDB (descdb);
	addChunkDB (db);
	return db.name;
    }
    */


    static public String createUniqueChunkDBName (String base) {
	// returns a string, starting with base, which doesn't
	// conflict with names of any dbs in chunkdbs.
	int i;
	String name;

	if (getChunkDBTree (base) == null)
	    return base;
	for (i = 2; true; i++) {
	    name = base + " <" + i + ">";
	    if (getChunkDBTree (name) == null)
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
	ChunkDBTreeModel dbt = getChunkDBTree (_db.name);
	ui.removeChunkDBTree (dbt);
	_db.setName (newbase);
	ui.addChunkDBTree (dbt);
	return newbase;
    }


    static public String renameDescDB (ChunkDescDB _db, String newbase) {
	if (_db.name.equals (newbase))
	    return newbase;
	newbase = createUniqueDescDBName (newbase);
	ui.removeDescDB (_db.name);
	_db.setName (newbase);
	ui.addDescDB (_db);
	return newbase;
    }

    static public String addChunkDB (ConfigChunkDB _chunkdb) {
	_chunkdb.setName (createUniqueChunkDBName (_chunkdb.name));
	ChunkDBTreeModel ctm = new ChunkDBTreeModel (_chunkdb, chunkorgtree);
	chunkdbs.addElement (ctm);
	ui.addChunkDBTree (ctm);
	return _chunkdb.name;
    }



    static public String addDescDB (ChunkDescDB _descdb) {
	_descdb.setName(createUniqueDescDBName (_descdb.name));
	descdbs.addElement( _descdb);
	descdb.addElements (_descdb);
	ui.addDescDB (_descdb);
	return _descdb.name;
    }



  static public void rebuildAllTrees () {
    // rebuilds all the trees in chunkdbs...
    ChunkDBTreeModel dbt;
    for (int i = 0; i < chunkdbs.size(); i++) {
      dbt = (ChunkDBTreeModel)chunkdbs.elementAt(i);
      dbt.buildTree();
    }
  }



  static ChunkDBTreeModel getChunkDBTree (String name) {
    ChunkDBTreeModel dbt;
    for (int i = 0; i < chunkdbs.size(); i++) {
      dbt = (ChunkDBTreeModel)chunkdbs.elementAt(i);
      if (dbt.getName().equalsIgnoreCase(name))
	return dbt;
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
	    ui.connection_pane.setHost (ch.getPropertyFromToken("host").getVal(0).getString());
	    ui.connection_pane.setPort (ch.getPropertyFromToken("port").getVal(0).getInt());
	    looknfeel = ch.getPropertyFromToken ("looknfeel").getVal(0).getString();

	    fontsize = ch.getPropertyFromToken("fontsize").getVal(0).getInt();
	    fontname = ch.getPropertyFromToken("fontname").getVal(0).getString();
	    //window_pos_kludge = ch.getPropertyFromToken("windowposkludge").getVal(0).getBool();

	    screenWidth = ch.getPropertyFromToken("windowsize").getVal(0).getInt();
	    screenHeight = ch.getPropertyFromToken("windowsize").getVal(1).getInt();
	    defaultchunkfile = ch.getPropertyFromToken("defaultchunkfile").getVal(0).getString();
	   

	}
	catch (Exception e) {
	    Core.consoleInfoMessage ("vjControl", "Old vjcontrol preferences file - please check preferences and re-save");
	}

//  	if (defaultchunkfile != null) {
//  	    default_chunkdb.removeAllElements();
	    

	if (screenWidth < 1)
	    screenWidth = 800;
	if (screenHeight < 1)
	    screenHeight = 600;

	ui.totalSetFont (fontname, fontsize);
	ui.setLookNFeel (looknfeel);
    }




    public static void consoleTempMessage (String s) {
	/* like a consoleInfoMessage, but not logged */
	consoleTempMessage (null, s);
    }
    public static void consoleTempMessage (String source, String s) {
	if (info_msg_to_stdout)
	    System.out.println (source + ": " + s);
	if (s == null || s.equals (""))
	    s = "  ";
	//if (Core.ui != null && Core.ui.console_pane != null) {
	try {
	    if (source != null)
		Core.ui.status_label.setText ("(" + source + "): " + s);
	    else
		Core.ui.status_label.setText (s);
	    Core.ui.status_label.setForeground (UIManager.getColor ("Label.foreground"));
	    //Core.ui.status_label.repaint((long)1);
	    Core.ui.paint (Core.ui.getGraphics());
	}
	catch (NullPointerException e) {
	    System.out.println (s);
	}
    }

    public static void consoleInfoMessage (String s) {
	consoleInfoMessage ("", s);
    }
    public static void consoleInfoMessage (String source, String s) {
	if (info_msg_to_stdout)
	    System.out.println (source + ": " + s);
	if (Core.ui != null && Core.ui.console_pane != null) {
	    Core.ui.console_pane.addInfoMessage (source, s);
	    Core.ui.status_label.setText ("(" + source + "): " + s);
	    Core.ui.status_label.setForeground (UIManager.getColor ("Label.foreground"));
	}
    }

    public static void consoleErrorMessage (String s) {
	consoleInfoMessage ("", s);
    }
    public static void consoleErrorMessage (String source, String s) {
	if (error_msg_to_stdout)
	    System.out.println ("Error (" + source + "): " + s);
	if (Core.ui != null && Core.ui.console_pane != null) {
	    Core.ui.console_pane.addErrorMessage (source, s);
	    Core.ui.status_label.setText ("Error (" + source + "): " + s);
	    Core.ui.status_label.setForeground (Color.red);
	}
    }

}




