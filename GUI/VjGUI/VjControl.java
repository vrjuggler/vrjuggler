/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
	String new_host = "";
	int new_port = 4450;
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
		new_host = args[i];
		hostset = true;
	    }
	    else {
		new_port = Integer.parseInt(args[i]);
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
	
	//Core.ui.refreshPerfData();

	
	Core.consoleInfoMessage ("VjControl 1.0", "VR Juggler Control and Configuration Program");
	
	Core.ui.show();

	if (hostset) {
	    Core.net.setRemoteHost (new_host, new_port);
	    Core.net.connect();
	}
    }



    //: does minimal sanity checking on the vjcontrol config.  This just
    //  means that if loading the config failed, we manually add a chunk
    //  with some reasonable(?) defaults.
    static void configure() {
	int i;
	String s;
	Property p;
	ConfigChunk ch;
	Vector v = Core.gui_chunkdb.getOfDescToken ("vjcontrol");
	if (v.size() == 0 || ((ch = (ConfigChunk)v.elementAt(0)) == null)) {
	    System.err.println ("didn't get chunk");
	    ch = ChunkFactory.createChunkWithDescToken ("vjcontrol");
	    ch.setPropertyFromToken ("fontname", new VarValue ("Courier"), 0);
	    ch.setPropertyFromToken ("fontsize", new VarValue (12), 0);
	    ch.setPropertyFromToken ("looknfeel", new VarValue ("Java"), 0);
	    ch.setPropertyFromToken ("name", new VarValue ("VjControl Config"), 0);
	    ch.setPropertyFromToken ("host", new VarValue ("localhost"), 0);
	    ch.setPropertyFromToken ("port", new VarValue (4450), 0);
	    Core.gui_chunkdb.addElement (ch);
	}

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







