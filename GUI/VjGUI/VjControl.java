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
    static Vector auto_defaultchunkdbnames;
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
	auto_defaultchunkdbnames = new Vector();
	Vector descdbnames = new Vector();
	Vector chunkdbnames = new Vector();
	Vector perfdatanames = new Vector();

	String orgtreename = null;

	Core.initialize();

	/* do config stuff... 
	 */
	FileControl.loadVjControlConfig();
	configure();

	Core.reconfigure();

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
	    else if (args[i].equalsIgnoreCase("-p")) {
		perfdatanames.addElement (args[++i]);
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
	    for (i = 0; i < auto_defaultchunkdbnames.size(); i++) {
		lastfname = (String)auto_defaultchunkdbnames.elementAt(i);
		FileControl.loadChunkDBFileInto(Core.default_chunkdb,lastfname);
	    }
	    //System.out.println("default are\n" + Core.default_chunkdb.fileRep());
	    for (i = 0; i < auto_chunkdbnames.size(); i++) {
		lastfname = (String)auto_chunkdbnames.elementAt(i);
		FileControl.loadNewChunkDBFile (lastfname, false);
	    }
	}
	for (i = 0; i < chunkdbnames.size(); i++) {
	    lastfname = (String)chunkdbnames.elementAt(i);
	    FileControl.loadNewChunkDBFile (lastfname, false);
	}
	for (i = 0; i < perfdatanames.size(); i++) {
	    lastfname = (String)perfdatanames.elementAt(i);
	    FileControl.loadNewPerfDataFile (lastfname, false);
	}
	if (orgtreename == null && autoload) {
	    FileControl.loadMainChunkOrgTree();
	}
	else if (orgtreename != null) {
	    FileControl.loadChunkOrgTree(orgtreename, false, Core.chunkorgtree);
	}
	
	Core.ui.selectLeftDB ("Active Configuration");
	Core.ui.selectRightDB (lastfname);
	Core.ui.selectDescDB (lastdname);
	Core.ui.refreshPerfData();

	
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
	    System.err.println ("didn't get chunk");
	    ch = ChunkFactory.createChunkWithDescToken ("vjcontrol");
	    ch.setPropertyFromToken ("fontname", new VarValue ("Courier"), 0);
	    ch.setPropertyFromToken ("fontsize", new VarValue (12), 0);
	    ch.setPropertyFromToken ("looknfeel", new VarValue ("Java"), 0);
	    ch.setPropertyFromToken ("name", new VarValue ("VjControl Config"), 0);
	    Core.gui_chunkdb.addElement (ch);
	}
	Core.vjcontrol_preferences = ch;

	p = ch.getPropertyFromToken ("descfiles");
	for (i = 0; i < p.getNum(); i++) {
	    s = p.getValue(i).getString();
	    auto_descdbnames.addElement (s);
	}
	p = ch.getPropertyFromToken ("chunkfiles");
	for (i = 0; i < p.getNum(); i++) {
	    s = p.getValue(i).getString();
	    auto_chunkdbnames.addElement (s);
	}
	p = ch.getPropertyFromToken ("defaultchunkfiles");
	if (p != null) 
	    for (i = 0; i < p.getNum(); i++) {
		s = p.getValue(i).getString();
		auto_defaultchunkdbnames.addElement (s);
	    }
    }
    


}







