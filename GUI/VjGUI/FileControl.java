/*
 * FileControl.java
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
import VjConfig.*;
import VjGUI.*;

public class FileControl {

    final static String basedir_varname = "VJ_BASE_DIR";


    public static void loadVjControlConfig() {
	ConfigStreamTokenizer st;
	File f1, f2 = null;
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
	    Core.consoleErrorMessage ("FileControl","Couldn't load " +descurl);
	}
		

	try {
 	    homedir = System.getProperty ("user.home", "");
	    f1 = new File (homedir, ".vjconfig");
	    f2 = new File (f1, "vjcontrol.cfg");
	    Core.gui_chunkdb.setName (f2.getName());
	    Core.gui_chunkdb.setDirectory (f2.getParent());
	    r = new FileReader (f2);
	    Core.gui_chunkdb.read (new ConfigStreamTokenizer (r));
	}
	catch (FileNotFoundException e2) {
	    Core.consoleErrorMessage ("FileControl","Couldn't load " + f2);
	    return;
	}
    }



    public static void loadMainChunkOrgTree () {
	FileReader r;
	ConfigStreamTokenizer st;
	File f1, f2;
	String homedir;

	try {
	    homedir = System.getProperty ("user.home", "");
	    f1 = new File (homedir, ".vjconfig");
	    f2 = new File (f1, "orgtree.org");
	    r = new FileReader (f2);
	}
	catch (FileNotFoundException e) {
	    try {
		homedir = System.getProperty (basedir_varname);
		f1 = new File (homedir, "Data");
		f2 = new File (f1, "orgtree.org");
		r = new FileReader(f2);
	    }
	    catch (FileNotFoundException e2) {
		Core.consoleErrorMessage ("FileControl", "Couldn't find default organization tree");
		return;
	    }
	}

	Core.consoleInfoMessage ("FileControl", "Loading organization tree: " + f2);
	st = new ConfigStreamTokenizer (r);
	Core.chunkorgtree.read(st);
	Core.rebuildAllTrees();
    }


    static public void loadMainChunkOrgTree (String name, boolean showrequester) {
	File f;
	FileReader r;
	ConfigStreamTokenizer st;

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
	    Core.chunkorgtree.read(st);
	    Core.rebuildAllTrees();
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
	chooser.addChoosableFileFilter (new SuffixFilter("Config Files (*.cfg)", ".cfg"));
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



  public static String loadNewChunkDBFile (String currentdir, boolean showrequester) {
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
      chunkdb.setDirectory(f.getParent());
      chunkdb.read(st);
      Core.addChunkDB (chunkdb);
      return f.getName();
    }
    catch (FileNotFoundException e) {
      Core.consoleErrorMessage ("FileControl", "File Not Found: " + e);
      return null;
    }
  }



  public static File requestSaveFile (String dir, String name) {
    if (dir == null || dir.equals(""))
      dir = System.getProperty("user.dir");
    /* opens up a file requester... */
    JFileChooser chooser = new JFileChooser(dir);
    chooser.setDialogType(JFileChooser.SAVE_DIALOG);
    chooser.setApproveButtonText("Save");
    chooser.setDialogTitle("Save Config File...");
    chooser.setSelectedFile(new File(dir, name));

    int returnVal = chooser.showSaveDialog(Core.ui); 
    if(returnVal == JFileChooser.APPROVE_OPTION) { 
     return chooser.getSelectedFile();
    } 
    else
      return null;
  }



  public static boolean saveChunkDBFile (ConfigChunkDB db) {
    File f = requestSaveFile (db.directory, db.name);
    if (f == null)
      return false;

    try {
      DataOutputStream out = new DataOutputStream(new FileOutputStream(f));
      out.writeBytes(db.fileRep());
      Core.consoleInfoMessage ("FileControl", "Saved ChunkDB file: " + f);
      return true;
    }
    catch (IOException e) {
      Core.consoleErrorMessage ("FileControl", "IOerror saving file " + f);
      return false;
    }
  }



  public static String loadNewDescDBFile (String currentdir, boolean showrequester) {
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
    Core.consoleInfoMessage ("FileControl", "Loading Descriptions file: " + f);
    try {
      r = new FileReader (f);
      st = new ConfigStreamTokenizer(r);
      ChunkDescDB descdb = new ChunkDescDB();
      descdb.setName(f.getName());
      descdb.setDirectory(f.getParent());
      descdb.read(st);
      Core.addDescDB (descdb);
      return f.getName();
    }
    catch (FileNotFoundException e) {
      Core.consoleErrorMessage ("FileControl", "File not found: " + e);
      return null;
    }
  }


  public static boolean saveDescDBFile (ChunkDescDB db) {
    File f = requestSaveFile (db.directory, db.name);
    if (f == null)
      return false;

    try {
      DataOutputStream out = new DataOutputStream(new FileOutputStream(f));
      out.writeBytes(db.fileRep());
      System.out.println ("Saved ChunkDesc File: " + f);
      return true;
    }
    catch (IOException e) {
      System.err.println ("IOerror saving file " + f);
      return false;
    }
  }



}
