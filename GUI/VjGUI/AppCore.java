
/* core of the configuration application */

package VjGUI;

import VjConfig.ChunkDescDB;
import VjConfig.ConfigChunkDB;
import VjGUI.NetControl;
import VjGUI.ClientUI;
import VjGUI.ClientGlobals;
import VjGUI.FileControl;


public class AppCore extends java.applet.Applet {

  static ClientGlobals core;

  public void init () {
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
    core = new ClientGlobals(true);
    core.descs = new ChunkDescDB();
    core.chunks = new ConfigChunkDB(core.descs);
    core.fileio = new FileControl(core);
    core.net = new NetControl (core);
    core.ui = new ClientUI(core);

    if (argv.length > 0) {
	if (argv[0].equalsIgnoreCase("fileeditor")) {
	    //core.fileio.loadBaseChunkDescDB();
	    //core.fileio.loadUserChunkDescDB();
	    //core.fileio.loadBaseConfigChunkDB();
	    if (argv.length > 1)
		core.fileio.loadUserChunkDescDB (argv[1]);
	    if (argv.length > 2)
		core.fileio.loadUserConfigChunkDB (argv[2]);
	    core.ui.showPanel ("Chunks");
	}
	else if (argv[0].equalsIgnoreCase("appgui")) {
	    core.ui.showPanel ("Connection");
	    if (argv.length == 2)
	      core.net.connect(argv[1]);
	    if (argv.length == 3)
	      core.net.connect(argv[1], Integer.parseInt(argv[2]));
	}
    }
  }


}









