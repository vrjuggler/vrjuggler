/* 
 * AppCore.java
 *
 * This class includes the main() routine & creates the "global"
 * variable area.
 *
 * Author: Christopher Just
 */

package VjGUI;

import java.util.Vector;
import VjConfig.ChunkDescDB;
import VjConfig.ConfigChunkDB;
import VjGUI.NetControl;
import VjGUI.ClientUI;
import VjGUI.ClientGlobals;
import VjGUI.FileControl;


public class AppCore extends java.applet.Applet {

    static ClientGlobals core;

    public void init () {
	/* Starting up as an applet */
	/* warning - not safe now - needs updating. see main */
	core = new ClientGlobals(true);
	core.descs = new ChunkDescDB();
	core.chunks = new ConfigChunkDB(core.descs);
	core.fileio = new FileControl(core);
	core.net = new NetControl (core);
	core.ui = new ClientUI(core);
	core.ui.showPanel("Descriptions");
	core.ui.update();
    }



    public static void main (String[] argv) {

	if (argv.length > 0) {
	    if (argv[0].equalsIgnoreCase("fileeditor")) {
		int i;
		Vector chunkfiles = new Vector();
		Vector descfiles = new Vector();

		/* start up in config file editor mode */
		System.out.println ("VR Juggler Configuration File Editor\n  Version 0.1");
		core = new ClientGlobals(true);
		core.mode = core.FILE_EDITOR;
		core.descs = new ChunkDescDB();
		core.chunks = new ConfigChunkDB(core.descs);
		core.fileio = new FileControl(core);
		core.ui = new ClientUI(core);
		core.noautoload = false;
		for (i = 1; i < argv.length; i++) {
		    if (argv[i].equalsIgnoreCase ("-noautoload"))
			core.noautoload = true;
		    else if (argv[i].equals ("-d")) {
			if (argv.length > i) {
			    i++;
			    descfiles.addElement (argv[i]);
			}
			else
			    System.err.println ("-d with no argument");
		    }
		    else {
			chunkfiles.addElement (argv[i]);
		    }
		}
		if (!core.noautoload) {
		    core.fileio.loadBaseChunkDescDB();
		    core.fileio.loadUserChunkDescDB();
		    core.fileio.loadBaseConfigChunkDB();
		}
	        for (i = 0; i < descfiles.size(); i++)
		    core.fileio.loadUserChunkDescDB ((String)descfiles.elementAt(i));
		for (i = 0; i < chunkfiles.size(); i++)
		    core.fileio.loadUserConfigChunkDB((String)chunkfiles.elementAt(i));
	    }
	    else if (argv[0].equalsIgnoreCase("appgui")) {
		System.out.println ("VjController v0.1");
		core = new ClientGlobals(true);
		core.mode = core.APP_GUI;
		core.descs = new ChunkDescDB();
		core.chunks = new ConfigChunkDB(core.descs);
		//core.fileio = new FileControl(core);
		core.net = new NetControl (core);
		core.ui = new ClientUI(core);
		core.ui.showPanel ("Connection");
		if (argv.length == 2)
		    core.net.connect(argv[1]);
		if (argv.length == 3)
		    core.net.connect(argv[1], Integer.parseInt(argv[2]));
	    }
	}
    }
    
    
}









