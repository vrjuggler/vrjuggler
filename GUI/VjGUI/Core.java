
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

public class Core {

    static public ChunkOrgTree chunkorgtree;
    static public Vector chunkdbs;        // actually, a vector of ChunkDBTreeModel
    static public Vector descdbs;
    static public ChunkDescDB descdb;
    static public ChunkDBTreeModel active_treemodel;
    static public NetControl net;
    static public ControlUI ui;
    static public ConfigChunkDB gui_chunkdb;
    static public ConfigChunk vjcontrol_preferences = null;

    static final boolean info_msg_to_stdout = false;
    static final boolean error_msg_to_stdout = true;

    static public void initialize () {
	ConfigChunkDB db;
	
	descdbs = new Vector();
	chunkdbs = new Vector();
	chunkorgtree = new ChunkOrgTree();
	descdb = new ChunkDescDB();
	gui_chunkdb = new ConfigChunkDB (descdb);
	db = new ConfigChunkDB(descdb);
	db.setName ("Active Configuration");
	active_treemodel = new ChunkDBTreeModel (db, chunkorgtree);
	net = new NetControl();
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



    static public String addNewChunkDB () {
	// creates a new db, treemodel, etc.
	ConfigChunkDB db = new ConfigChunkDB (descdb);
	db.setDirectory ("");
	db.setName (createUniqueChunkDBName ("untitled"));
	addChunkDB (db);
	return db.name;
    }



    static public String createUniqueChunkDBName (String base) {
	// returns a string, starting with base, which doesn't
	// conflict with names of any dbs in chunkdbs.
	int i, j, n = 0;
	String s;
	for (i = 0; i < chunkdbs.size(); i++) {
	    s = ((ChunkDBTreeModel)chunkdbs.elementAt(i)).getName();
	    if (s.startsWith (base)) {
		try {
		    j = Integer.parseInt (s.substring (base.length()+1));
		    if (j >= n)
			n = j+1;
		}
		catch (Exception e) {
		   if (n < 1)
		       n = 1;
		}

	    }
	}
	if (n == 0)
	    return base;
	else
	    return base + " " + n;
    }



    static public String createUniqueDescDBName (String base) {
	// returns a string, starting with base, which doesn't
	// conflict with names of any dbs in chunkdbs.
	int i, j, n = -1;
	String s;
	for (i = 0; i < descdbs.size(); i++) {
	    s = ((ChunkDescDB)descdbs.elementAt(i)).name;
	    if (s.startsWith (base)) {
		try {
		    j = Integer.parseInt (s.substring (base.length()+1));
		    if (j >= n)
			n = j+1;
		}
		catch (Exception e) {
		   if (n < 1)
		       n = 1;
		}
	    }
	}
	if (n == -1)
	    return base;
	else
	    return base + " " + n;
    }



    static public void addChunkDB (ConfigChunkDB _chunkdb) {
	ChunkDBTreeModel ctm = new ChunkDBTreeModel (_chunkdb, chunkorgtree);
	chunkdbs.addElement (ctm);
	ui.addChunkDBTree (ctm);
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
	String looknfeel;
	int i;
	String s;
	Property p;
	int fontsize = -1;
	String fontname = "";

	ConfigChunk ch = vjcontrol_preferences;
	if (ch == null)
	    return;

	ui.connection_pane.setHost (ch.getPropertyFromToken("host").getVal(0).getString());
	ui.connection_pane.setPort (ch.getPropertyFromToken("port").getVal(0).getInt());
	looknfeel = ch.getPropertyFromToken ("looknfeel").getVal(0).getString();

	p = ch.getPropertyFromToken ("fontsize");
	if (p != null)
	    fontsize = p.getVal(0).getInt();
	p = ch.getPropertyFromToken ("fontname");
	if (p != null)
	    fontname = p.getVal(0).getString();

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




