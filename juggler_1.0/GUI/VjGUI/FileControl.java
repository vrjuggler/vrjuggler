/*
 *  File:	    $Name$
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
 *
 * includes utility functions for loading and saving ChunkDesc and ConfigChunk
 * files into VjControl, and code to open a FileChooser
 *
 */

package VjGUI;

import java.io.*;
import java.net.*;
import javax.swing.*;
import javax.swing.filechooser.*;
import java.util.Vector;
import VjConfig.*;
import VjGUI.*;

public class FileControl {

    static String vjbasedir = null;
    static String homedir = null;


    private static String mangleFileName (String s) {
	// replace "HOME" and "VJ_BASE_DIR" in the string buffer w/
	// proper values i hope..........

	if (vjbasedir == null) {
	    homedir = System.getProperty ("user.home");
	    if (homedir == null)
		homedir = ".";
	    vjbasedir = System.getProperty ("VJ_BASE_DIR");
	    if (vjbasedir == null)
		vjbasedir = ".";
	}

	if (s == null)
	    return "";

	if (s.startsWith ("$VJ_BASE_DIR"))
	    s = vjbasedir + s.substring (12);
	else if (s.startsWith ("VJ_BASE_DIR"))
	    s = vjbasedir + s.substring (11);
	else if (s.startsWith ("$HOME"))
	    s = homedir + s.substring (5);
	else if (s.startsWith ("HOME"))
	    s = homedir + s.substring (4);

	//System.out.println ("stringReplacement: '" + s + "'");
	return s;
    }



    public static void loadVjControlConfig() {
	ConfigStreamTokenizer st;
	File f1 = null;
	FileReader r;
	InputStreamReader inr;
	String homedir;

	// 1st - load the chunkdesc into memory...

	URL descurl = ClassLoader.getSystemResource ("VjFiles/vjcontrol.dsc");
	try {
	    inr = new InputStreamReader (descurl.openStream());
	    Core.descdb.read (new ConfigStreamTokenizer (inr));
	}
	catch (IOException e2) {
	    Core.consoleErrorMessage ("FileControl","Couldn't load " +descurl
				      + "- vjcontrol.jar may be corrupt");
	}
		
	try {
	    f1 = new File (mangleFileName ("$HOME/.vjconfig/vjcontrol.cfg"));
	    Core.gui_chunkdb.setName (f1.getName());
	    Core.gui_chunkdb.setFile (f1);
	    r = new FileReader (f1);
	    Core.gui_chunkdb.read (new ConfigStreamTokenizer (r));
	}
	catch (FileNotFoundException e2) {
	    try {
		f1 = new File (mangleFileName ("$VJ_BASE_DIR/Data/vjcontrol.cfg"));
		Core.gui_chunkdb.setName (f1.getName());
		Core.gui_chunkdb.setFile (f1);
		r = new FileReader (f1);
		Core.gui_chunkdb.read (new ConfigStreamTokenizer (r));
	    }
	    catch (FileNotFoundException e3) {
		Core.consoleErrorMessage ("FileControl","Couldn't load " + f1);
		return;
	    }
	}
    }



    public static void loadMainChunkOrgTree () {
	FileReader r;
	ConfigStreamTokenizer st;
	File f1;
	String homedir;

	try {
	    f1 = new File (mangleFileName ("$HOME/.vjconfig/orgtree.org"));
	    r = new FileReader (f1);
	}
	catch (FileNotFoundException e) {
	    try {
		f1 = new File (mangleFileName ("$VJ_BASE_DIR/Data/orgtree.org"));
		r = new FileReader(f1);
	    }
	    catch (FileNotFoundException e2) {
		Core.consoleErrorMessage ("FileControl", "Couldn't find default organization tree");
		return;
	    }
	}

	Core.consoleInfoMessage ("FileControl", "Loading organization tree: " + f1);
	st = new ConfigStreamTokenizer (r);
	Core.chunkorgtree.setName (f1.getName());
	Core.chunkorgtree.setFile (f1);
	Core.chunkorgtree.read(st);
	Core.rebuildAllTrees();
    }



    static public void loadChunkOrgTree (String name, boolean showrequester, ChunkOrgTree orgtree) {
	File f;
	FileReader r;
	ConfigStreamTokenizer st;

	name = mangleFileName (name);

	if (showrequester) {
	    f = requestOpenFile(name);
	    if (f == null)
		return;
	}
	else {
	    f = new File (name);
	}

	try {
	    r = new FileReader (f);
	    Core.consoleInfoMessage ("FileControl", "Loading organization tree: " + f);
	    st = new ConfigStreamTokenizer (r);
	    orgtree.setName (f.getName());
	    orgtree.setFile (f);
	    orgtree.read(st);
	    //Core.rebuildAllTrees();
	}
	catch (FileNotFoundException e) {
	    Core.consoleErrorMessage ("FileControl", "File not found: " + f);
	}
    }



    public static File requestOpenFile(String dir) {
	if (dir == null || dir.equals(""))
	    dir = System.getProperty("user.dir");
	/* opens up a file requester... */
	JFileChooser chooser = new JFileChooser(dir);
	chooser.addChoosableFileFilter (new SuffixFilter("Config Files (*.config)", ".config"));
	chooser.addChoosableFileFilter (new SuffixFilter("Description Files (*.dsc)", ".dsc"));
	chooser.setFileHidingEnabled (false);
	chooser.setDialogType(JFileChooser.OPEN_DIALOG);
	int returnVal = chooser.showOpenDialog(Core.ui); 
	if(returnVal == JFileChooser.APPROVE_OPTION) { 
	    File f = chooser.getSelectedFile();
	    return chooser.getSelectedFile();
	} 
	return null;
    }



    // this is used specifically to load into an existing chunkdb without
    // adding anything into the gui's list of chunkdb's... useful
    // for loading the default chunkdb and keeping it hidden
    public static boolean loadChunkDBFileInto (ConfigChunkDB chunkdb, String currentdir) {
	currentdir = mangleFileName (currentdir);

	File f = null;
	FileReader r;
	ConfigStreamTokenizer st;
	boolean retval = true;
	try {
	    f = new File (currentdir);
	    r = new FileReader (f);
	    st = new ConfigStreamTokenizer(r);
	    chunkdb.setName(f.getName());
	    chunkdb.setFile(f);
	    chunkdb.read(st);
	    Vector v = chunkdb.getOfDescToken("vjIncludeFile");
	    for (int i = 0; i < v.size(); i++)
		retval = retval && loadChunkDBFileInto(chunkdb, ((ConfigChunk)v.elementAt(i)).getName());
	    return retval;
	}
	catch (FileNotFoundException e) {
	    Core.consoleErrorMessage ("FileControl", "File Not Found: " + f);
	    return false;
	}

    }


    public static String loadNewChunkDBFile (String currentdir, boolean showrequester) {
	currentdir = mangleFileName (currentdir);

	File f;
	FileReader r;
	ConfigStreamTokenizer st;

	if (showrequester) {
	    f = requestOpenFile(currentdir);
	    if (f == null)
		return null;
	}
	else {
	    f = new File (currentdir);
	}
	Core.consoleInfoMessage ("FileControl", "Loading ChunkDB: " + f);
	try {
	    r = new FileReader (f);
	    st = new ConfigStreamTokenizer(r);
	    ConfigChunkDB chunkdb = new ConfigChunkDB(Core.descdb);
	    chunkdb.setName(f.getName());
	    chunkdb.setFile(f);
	    chunkdb.read(st);
	    Core.addChunkDB (chunkdb);
	    
	    // load included files...
	    Vector v = chunkdb.getOfDescToken("vjIncludeFile");
	    for (int i = 0; i < v.size(); i++)
		loadNewChunkDBFile(((ConfigChunk)v.elementAt(i)).getName(), false);

	    return chunkdb.name;
	}
	catch (FileNotFoundException e) {
	    Core.consoleErrorMessage ("FileControl", "File Not Found: " + f);
	    return null;
	}
    }



    public static File requestSaveFile (File file) {
	JFileChooser chooser;

	/* opens up a file requester... */
	if (file == null) 
	    chooser = new JFileChooser();
	else if (file.isDirectory())
	    chooser = new JFileChooser(file);
	else {
	    //System.out.println ("File is " + file + "\n and parent is " + file.getParent());
	    String dir = file.getParent();
	    if (dir == null)
		dir = "";
	    chooser = new JFileChooser (new File(dir));
	    chooser.setSelectedFile (file);
	}
	chooser.setDialogType(JFileChooser.SAVE_DIALOG);
	chooser.setFileHidingEnabled (false);
	chooser.setApproveButtonText("Save");
	chooser.setDialogTitle("Save File...");

	int returnVal = chooser.showSaveDialog(Core.ui); 
	if(returnVal == JFileChooser.APPROVE_OPTION) { 
	    return chooser.getSelectedFile();
	} 
	else
	    return null;
    }



    public static String saveChunkDBFile (ConfigChunkDB db) {
	File f = requestSaveFile (db.file);
	if (f == null)
	    return db.name;

	try {
	    DataOutputStream out = new DataOutputStream(new FileOutputStream(f));
	    out.writeBytes(db.fileRep());
	    Core.consoleInfoMessage ("FileControl", "Saved ChunkDB file: " + f);

	    /* do some fixing up if the name changed */
	    db.setFile (f);
	    return Core.renameChunkDB (db, f.getName());
	}
	catch (IOException e) {
	    Core.consoleErrorMessage ("FileControl", "IOerror saving file " + f);
	    return db.name;
	}
    }


    public static String saveChunkOrgTree (ChunkOrgTree ot) {
	System.out.println ("save msg for file " + ot.getFile());
	File f = requestSaveFile (ot.getFile());
	if (f == null)
	    return "";

	try {
	    DataOutputStream out = new DataOutputStream(new FileOutputStream(f));
	    out.writeBytes(ot.toString());
	    Core.consoleInfoMessage ("FileControl", "Saved ChunkOrgTree file: " + f);

	    /* do some fixing up if the name changed */
	    ot.setFile (f);
	    ot.setName (f.getName());
	    //return Core.renameChunkDB (db, f.getName());
	    return f.getName();
	}
	catch (IOException e) {
	    Core.consoleErrorMessage ("FileControl", "IOerror saving file " + f);
	    return "";
	}
    }



    public static String loadNewDescDBFile (String currentdir, 
					    boolean showrequester) {
	currentdir = mangleFileName (currentdir);

	File f;
	FileReader r;
	ConfigStreamTokenizer st;

	if (showrequester) {
	    f = requestOpenFile(currentdir);
	    if (f == null)
		return null;
	}
	else {
	    f = new File (currentdir);
	}
	Core.consoleInfoMessage ("FileControl", 
				 "Loading Descriptions file: " + f);
	try {
	    r = new FileReader (f);
	    st = new ConfigStreamTokenizer(r);
	    ChunkDescDB descdb = new ChunkDescDB();
	    descdb.setName(f.getName());
	    descdb.setFile (f);
	    descdb.read(st);
	    Core.addDescDB (descdb);
	    return descdb.name;
	}
	catch (FileNotFoundException e) {
	    Core.consoleErrorMessage ("FileControl", "File not found: " + e);
	    return null;
	}
    }


    public static String saveDescDBFile (ChunkDescDB db) {
	File f = requestSaveFile (db.file);
	if (f == null)
	    return db.name;
	
	try {
	    DataOutputStream out = 
		new DataOutputStream(new FileOutputStream(f));
	    out.writeBytes(db.fileRep());
	    Core.consoleInfoMessage ("FileControl", 
				     "Saved ChunkDesc File: " + f);
	    /* do some fixing up if the name changed */
	    db.setFile (f);
	    return Core.renameDescDB (db, f.getName());
	}
	catch (IOException e) {
	    Core.consoleErrorMessage ("FileControl", "IOerror saving file " + f);
	    return db.name;
	}
    }


    //---------------- PERF FILE STUFF ------------------------

    public static String loadNewPerfDataFile (String currentdir, 
					      boolean showrequester) {
	currentdir = mangleFileName (currentdir);

	File f;
	FileReader r;
	ConfigStreamTokenizer st;

	if (showrequester) {
	    f = requestOpenFile(currentdir);
	    if (f == null)
		return null;
	}
	else {
	    f = new File (currentdir);
	}
	Core.consoleInfoMessage ("FileControl", 
				 "Loading Timing Data file: " + f);

	try {
	    r = new FileReader(f);
	    st = new ConfigStreamTokenizer(r);
	    st.eolIsSignificant(true);

	    do {
		st.nextToken();
		//System.out.println ("foo" + st.ttype);
	    } while (st.ttype != '\n');

	    st.eolIsSignificant(false);
	    st.quoteChar('"');

	    Core.perf_collection.removeAllData();
	    Core.perf_collection.read (st, true);
	    Core.perf_collection.file = f;
	    return new String ("unnamed perf data file");
	}
	catch (FileNotFoundException e) {
	    Core.consoleErrorMessage ("FileControl", "File not found: " + e);
	    return null;
	}
	catch (IOException e) {
	    Core.consoleErrorMessage ("FileControl", "Loading error: " + e);
	    return null;
	}
    }



    public static String savePerfDataFile (VjPerf.PerfDataCollection col) {
	File f = requestSaveFile (col.file);
	if (f == null)
	    return "";

	try {
	    DataOutputStream out = new DataOutputStream(new FileOutputStream(f));
	    out.writeBytes ("VR Juggler FileConnect output gui_save\n");
	    col.write(out);
	    Core.consoleInfoMessage ("FileControl", "Saved PerfData file: " + f);
	    col.file = f;
	    /* do some fixing up if the name changed */
	    //db.setFile (f);
	    //return Core.renameChunkDB (db, f.getName());
	    return f.getName();
	}
	catch (IOException e) {
	    Core.consoleErrorMessage ("FileControl", "IOerror saving file " + f);
	    return "";
	}
    }
    


}

