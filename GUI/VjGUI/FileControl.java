/*
 * FileControl.java
 *
 * Handles all file access for the config editor
 *
 * Author: Christopher Just
 *
 */


package VjGUI;

import VjGUI.ClientGlobals;
import java.io.*;
import java.awt.FileDialog;
import VjGUI.ElCheapoRequester;
import VjConfig.ConfigStreamTokenizer;
import VjConfig.ChunkDescDB;
import VjConfig.ConfigChunkDB;
import VjGUI.CfgFileRequester;


public class FileControl {

    final static String basedir_varname = "VJ_BASE_DIR";
    
    String fileseparator;
    String homedir;
    String basedir;
    String userchunkdescdbname;
    String userconfigchunkdbname;
    String baseconfigchunkdbname;
    String basechunkdescdbname;
    ClientGlobals core;
    
    public FileControl (ClientGlobals c) {
	core = c;
	java.util.Properties p = System.getProperties();
	fileseparator = p.getProperty("file.separator", "/");
	homedir = p.getProperty ("user.home", "");
	if (!homedir.endsWith(fileseparator))
	    homedir = homedir + fileseparator;
	System.out.println ("home directory is '" + homedir + "'");
	getBaseDir();
    }


    public boolean getBaseDir() {
	// this version of getBaseDir tries to get the base dir
	// from an environment variable (basedir_name)
	//basedir = System.getenv (basedir_varname);
	basedir = System.getProperty (basedir_varname);
	if (basedir == null) {
	    basedir = "";
	    System.err.println ("Couldn't find $" + basedir_varname
				+ " variable.");
	    return false;
	}
	else {
	    if (!basedir.endsWith (fileseparator))
		basedir = basedir + fileseparator;
	    return true;
	}
    }


//   public boolean getBaseDir() {
//     // the version of basedir tries to get the base dir from
//     // a file called .vjconfig/basedir in the user's home 
//     // directory.
//     basedir = null;
//     try {
//       String s = homedir + "/.vjconfig/basedir";
//       System.out.println ("trying to load " + s);
//       FileReader r = new FileReader (s);
//       StreamTokenizer st = new ConfigStreamTokenizer(r);
//       st.wordChars('/','/');
//       st.nextToken();
//       basedir = st.sval;
//       return (basedir != null);
//     }
//     catch (FileNotFoundException e) {
//       System.err.println ("Couldn't find file !/.vjconfig/basedir");
//       return false;
//     }
//     catch (Exception e) {
//       System.err.println ("Couldn't determine configfile base dir");
//       return false;
//     }
//   }


//----------------- GLOBAL ChunkDescDB -------------------------


  public boolean loadBaseChunkDescDB () {
      String fname;
      if (core.chunkdescusestandardfilereq) {
	  FileDialog fd = new FileDialog ( core.ui, 
					   "Load Global ChunkDesc File As...", 
					   FileDialog.SAVE);
    
	  //ElCheapoRequester fd = new ElCheapoRequester(core.ui);
	  fd.show();
	  fname = fd.getFile();
      }
      else {
	  fname = basedir + "Data/chunksDesc";
      }
      if (fname == null)
	  return false;

    try {
      FileReader r = new FileReader(fname);
      ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
      core.descs.read(st);
      basechunkdescdbname = fname;
      core.ui.update();
      return true;
    }
    catch (FileNotFoundException e) {
      System.err.println ("Couldn't open file " + fname);
      return false;
    }
    catch (IOException e) {
      System.err.println ("Error reading ChunkDescs from " + fname);
      return false;
    }
  }



  public boolean saveAsBaseChunkDescDB () {
    /* opens a file requester & saves to selected name */
        FileDialog fd = new FileDialog ( core.ui, 
				     "Save Global ChunkDesc File As...", 
				     FileDialog.SAVE);
    
    //ElCheapoRequester fd = new ElCheapoRequester(core.ui);
    fd.show();
    String name = fd.getFile();
    if ((name == null) || name.equals(""))
      return false;
    return saveBaseChunkDescDB(name);
  }



  public boolean saveBaseChunkDescDB () {
    String name = basedir + "/chunksDesc";
    return saveBaseChunkDescDB(name);
  }



  public boolean saveBaseChunkDescDB (String name) {
    try {
      DataOutputStream out = new DataOutputStream(new FileOutputStream(name));
      out.writeBytes(core.descs.fileRep());
      basechunkdescdbname = name;
      return true;
    }
    catch (IOException e) {
      System.err.println ("IOerror saving file " + name);
      return false;
    }
  }



//---------------------- USER ChunkDescDB ----------------------




  public boolean loadUserChunkDescDB () {
      String fname = null;
      if (core.noautoload) {
	  FileDialog fd = new FileDialog ( core.ui, 
					   "Load User ChunkDesc File...", 
					   FileDialog.LOAD);
	  fd.show();
	  fname = fd.getFile();
	  if ((fname == null) || fname.equals(""))
	      return false;
      }
      else
	  fname = homedir + ".vjconfig/chunksDesc";    
      return loadUserChunkDescDB (fname);
  }



    public boolean loadUserChunkDescDB (String fname) {
	userchunkdescdbname = fname;
	
	try {
	    FileReader r = new FileReader(fname);
	    ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
	    core.descs.read(st);
	    userchunkdescdbname = fname;
	    core.ui.update();
	    return true;
	}
	catch (FileNotFoundException e) {
	    System.err.println ("Couldn't open file " + fname);
	    return false;
	}
	catch (IOException e) {
	    System.err.println ("Error reading chunk descriptions from " + fname);
	    return false;
	}
    }



    public boolean saveAsUserChunkDescDB () {
	/* opens a file requester & saves to selected name */
        FileDialog fd = new FileDialog ( core.ui, 
					 "Save User Chunk Descriptions As...", 
					 FileDialog.SAVE);
    
	/*CfgFileRequester fd = new CfgFileRequester(core.ui,
	  homedir + "/.vjconfig",
	  "",
	  true);
	*/
	fd.show();
	String name = fd.getFile();
	if ((name == null) || name.equals(""))
	    return false;
	return saveUserChunkDescDB(name);
    }



    public boolean saveUserChunkDescDB () {
	return saveUserChunkDescDB (userchunkdescdbname);
    }



    public boolean saveUserChunkDescDB (String name) {
	userchunkdescdbname = name;
	try {
	    if ((basechunkdescdbname != null) && !basechunkdescdbname.equals("")) {
		FileReader r = new FileReader(basechunkdescdbname);
		ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
		ChunkDescDB base = new ChunkDescDB();
		base.read(st);
		ChunkDescDB diff = base.diff(core.descs);
		DataOutputStream out = 
		    new DataOutputStream(new FileOutputStream(userchunkdescdbname));
		out.writeBytes(diff.fileRep());
		return true;
	    }
	    else {
		DataOutputStream out = 
		    new DataOutputStream(new FileOutputStream(userchunkdescdbname));
		out.writeBytes(core.descs.fileRep());
		return true;
	    }
	}
	catch (IOException e) {
	    System.err.println ("IOerror saving file " + userchunkdescdbname);
	    return false;
	}
    }
    




//------------------- Global ConfigChunk ------------------------


    public boolean loadBaseConfigChunkDB () {
	String fname = basedir + "Data/vjConfig";
	
	try {
	    FileReader r = new FileReader(fname);
	    ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
	    core.chunks.read(st);
	    baseconfigchunkdbname = fname;
	    core.ui.update();
	    return true;
	}
	catch (FileNotFoundException e) {
	    System.err.println ("Couldn't open file " + fname);
	    return false;
	}
	catch (IOException e) {
	    System.err.println ("Error reading chunks from " + fname);
	    return false;
	}
    }



//------------------------- USER ConfigChunk ----------------------------



  public boolean loadUserConfigChunkDB () {
    /* open up a file requester to get name */
          FileDialog fd = new FileDialog ( core.ui, 
				       "Load User Configuration File", 
				       FileDialog.LOAD);
    
    //ElCheapoRequester fd = new ElCheapoRequester(core.ui);
      /*CfgFileRequester fd = new CfgFileRequester(core.ui,
					       homedir + "/.vjconfig",
					       ".cfg",
					       false);
      */
    fd.show();
    String name = fd.getFile();
    //System.out.println ("attempting to load '" + name + "'");
    if ((name == null) || name.equals(""))
      return false;
    //return loadUserConfigChunkDB (homedir + "/.vjconfig/" + name);
    return loadUserConfigChunkDB (name);
  }



  public boolean loadUserConfigChunkDB (String fname) {
      userconfigchunkdbname = fname;
    try {
      FileReader r = new FileReader(fname);
      ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
      core.chunks.read(st);
      userconfigchunkdbname = fname;
      core.ui.update();
      return true;
    }
    catch (FileNotFoundException e) {
      System.err.println ("Couldn't open file " + fname);
      return false;
    }
    catch (IOException e) {
      System.err.println ("Error reading chunks from " + fname);
      return false;
    }
 
  }



  /************************ global chunk saves ***************************/

  public boolean saveAsBaseConfigChunkDB () {
    /* opens a file requester & saves to selected name */
    /*    FileDialog fd = new FileDialog ( core.ui, 
				     "Save Configuration File As...", 
				     FileDialog.SAVE);
    */
    ElCheapoRequester fd = new ElCheapoRequester(core.ui);
    fd.show();
    String name = fd.getFile();
    if ((name == null) || name.equals(""))
      return false;
    return saveBaseConfigChunkDB(name);
  }



  public boolean saveBaseConfigChunkDB () {
     String name = basedir + "Data/vjConfig";
     return saveBaseConfigChunkDB(name);
  }

  public boolean saveBaseConfigChunkDB (String name) {
    try {
      DataOutputStream out = new DataOutputStream(new FileOutputStream(name));
      out.writeBytes(core.chunks.fileRep());
      baseconfigchunkdbname = name;
      return true;
    }
    catch (IOException e) {
      System.err.println ("IOerror saving file " + name);
      return false;
    }
  }



  /****************** User Chunk saves *******************/

  public boolean saveAsUserConfigChunkDB () {
    /* opens a file requester & saves to selected name */
        FileDialog fd = new FileDialog ( core.ui, 
				     "Save User Configuration File As...", 
				     FileDialog.SAVE);
    
      /*CfgFileRequester fd = new CfgFileRequester(core.ui,
					       homedir + "/.vjconfig",
					       ".cfg",
					       true);
      */
    fd.show();
    String name = fd.getFile();
      if ((name == null) || name.equals(""))
	return false;
      //return saveUserConfigChunkDB(homedir + "/.vjconfig/" + name);
      return saveUserConfigChunkDB (name);
  }

  public boolean saveUserConfigChunkDB () {
    if (userconfigchunkdbname == null)
      return saveAsUserConfigChunkDB();
    else
      return saveUserConfigChunkDB(userconfigchunkdbname);
  }

  public boolean saveUserConfigChunkDB (String name) {
      userconfigchunkdbname = name;
    try {
      /* load in the base file, if it exists, to check against */
      if (baseconfigchunkdbname != null) {
	FileReader r = new FileReader(baseconfigchunkdbname);
	ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
	ConfigChunkDB base = new ConfigChunkDB(core.descs);
	base.read(st);
	ConfigChunkDB diff = base.diff(core.chunks);
	DataOutputStream out = new DataOutputStream(new FileOutputStream(name));
	out.writeBytes(diff.fileRep());
	//userconfigchunkdbname = name;
	return true;
      }
      else {
	DataOutputStream out = new DataOutputStream(new FileOutputStream(name));
	out.writeBytes(core.chunks.fileRep());
	//userconfigchunkdbname = name;
	return true;
      }
    }
    catch (IOException e) {
      System.err.println ("IOerror saving file " + name);
      return false;
    }
  }




}












