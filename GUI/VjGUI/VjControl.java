package VjGUI;

import java.util.Vector;
import java.awt.*;
import java.io.*;
import java.lang.StringBuffer;
import VjGUI.*;
import VjConfig.*;

public class VjControl {

    static Vector descdbnames;
    static Vector chunkdbnames;
    static Vector auto_descdbnames;
    static Vector auto_chunkdbnames;
    static String default_looknfeel = "java";

    static int fontsize = -1;
    static String fontname = "";

    public static void main (String args[]) {
	FileReader r;
	int i;
	String s;
	ConfigStreamTokenizer st;
	String lastfname = "";
	String lastdname = "";
	boolean hostset = false;
	boolean autoload = true;
	auto_descdbnames = new Vector();
	auto_chunkdbnames = new Vector();
	Vector descdbnames = new Vector();
	Vector chunkdbnames = new Vector();

	String orgtreename = null;

	Core.initialize();

	/* do config stuff... 
	 */
	FileControl.loadVjControlConfig();
	configure();

	/* read & parse command line arguments
	 *
	 * command line is this: VjControl host port -d descriptfile -c cfgfile
	 *                             -o orgtree
	 * ( allow multiple -d & -c arguments )
	 */

	for (i = 0; i < args.length; i++) {
	    if (args[i].equalsIgnoreCase("-d")) {
		descdbnames.addElement (args[++i]);
	    }
	    else if (args[i].equalsIgnoreCase("-c")) {
		chunkdbnames.addElement (args[++i]);
	    }
	    else if (args[i].equalsIgnoreCase("-o")) {
		orgtreename = args[++i];
	    }
	    else if (args[i].equalsIgnoreCase("-noautoload")) {
		autoload = false;
	    }
	    else if (hostset == false) {
		Core.ui.connection_pane.setHost(args[i]);
		hostset = true;
	    }
	    else {
		Core.ui.connection_pane.setPort(Integer.parseInt(args[i]));
	    }
	    
	}
     

	if (autoload) {
	    for (i = 0; i < auto_descdbnames.size(); i++) {
		lastdname = (String)auto_descdbnames.elementAt(i);
		FileControl.loadNewDescDBFile (lastdname, false);
	    }
	}
	for (i = 0; i < descdbnames.size(); i++) {
	    lastdname = (String)descdbnames.elementAt(i);
	    FileControl.loadNewDescDBFile (lastdname, false);
	}
	if (autoload) {
	    for (i = 0; i < auto_chunkdbnames.size(); i++) {
		lastfname = (String)auto_chunkdbnames.elementAt(i);
		FileControl.loadNewChunkDBFile (lastfname, false);
	    }
	}
	for (i = 0; i < chunkdbnames.size(); i++) {
	    lastfname = (String)chunkdbnames.elementAt(i);
	    FileControl.loadNewChunkDBFile (lastfname, false);
	}
	
	if (orgtreename == null && autoload) {
	    FileControl.loadMainChunkOrgTree();
	}
	else if (orgtreename != null) {
	    FileControl.loadChunkOrgTree(orgtreename, false, Core.chunkorgtree);
	}

	//Core.rebuildAllTrees();
	
	Core.ui.selectLeftDB ("Active Configuration");
	Core.ui.selectRightDB (lastfname);
	Core.ui.selectDescDB (lastdname);

	Core.reconfigure();
	
	Core.consoleInfoMessage ("VjControl 1.0", "VR Juggler Control and Configuration Program");
	
	Core.ui.show();

	if (hostset)
	    Core.net.connect();
    }



    static void configure() {
	int i;
	String s;
	Property p;
	Vector v = Core.gui_chunkdb.getOfDescToken ("vjcontrol");
	if (v.size() > 1) 
	    Core.consoleErrorMessage ("Config", "VjControl's own config file seems messed up. Making a guess...");
	ConfigChunk ch;
	if (v.size() == 0 || ((ch = (ConfigChunk)v.elementAt(0)) == null)) {
	    //ConfigChunk ch = (ConfigChunk)v.elementAt(0);
	    //if (ch == null) {
	    System.err.println ("didn't get chunk");
	    ch = new ConfigChunk (Core.descdb.get ("vjcontrol"), Core.descdb);
	    ch.setPropertyFromToken ("fontname", "Courier", 0);
	    ch.setPropertyFromToken ("fontsize", 12, 0);
	    ch.setPropertyFromToken ("looknfeel", "Java", 0);
	    ch.setPropertyFromToken ("name", "VjControl Config", 0);
	    Core.gui_chunkdb.addElement (ch);
	}
	Core.vjcontrol_preferences = ch;

	p = ch.getPropertyFromToken ("descfiles");
	for (i = 0; i < p.getNum(); i++) {
	    s = p.getVal(i).getString();
	    s = stringReplacements (s);
	    auto_descdbnames.addElement (s);
	}
	p = ch.getPropertyFromToken ("chunkfiles");
	for (i = 0; i < p.getNum(); i++) {
	    s = p.getVal(i).getString();
	    s = stringReplacements (s);
	    auto_chunkdbnames.addElement (s);
	}
    }

    

    private static String stringReplacements (String s) {
	// replace "HOME" and "VJ_BASE_DIR" in the string buffer w/
	// proper values i hope..........
	int i, j;
	String homedir, basedir;

	homedir = System.getProperty ("user.home");
	if (homedir == null)
	    homedir = ".";
	basedir = System.getProperty ("VJ_BASE_DIR");
	if (basedir == null)
	    basedir = ".";

	i = s.indexOf ("HOME");
	if (i != -1) {
	    s = s.substring (0, i) +
		homedir +
		s.substring (i + 4);
	}
	i = s.indexOf ("VJ_BASE_DIR");
	if (i != -1) {
	    s = s.substring (0, i) +
		basedir +
		s.substring (i + 11);
	}
	//System.out.println ("stringReplacement: '" + s + "'");
	return s;
    }

}







