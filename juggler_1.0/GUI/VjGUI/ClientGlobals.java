
package VjGUI;

import VjConfig.ChunkDescDB;
import VjConfig.ConfigChunkDB;
import VjGUI.NetControl;
import VjGUI.ClientUI;
import VjGUI.FileControl;

public class ClientGlobals {
    public ChunkDescDB descs;
    public ConfigChunkDB chunks;
    public NetControl net;
    public FileControl fileio;
    public ClientUI ui;

    /* some annoying flag-type variables */
    public boolean        multilevelconfigenabled;
    /* If true, we can load/save global _and_ user config/chunkdesc
     * files.  Otherwise, we work with one file at a time.
     * basically changes what menu items get displayed.
     * (basically, if true, all files are treated as user files)
     */
    public boolean        multilevelchunkdescenabled;
    public boolean        chunkdescusestandardfilereq;

    public int            mode;          // FILE_EDITOR or APP_GUI
    public static int     FILE_EDITOR = 1;
    public static int     APP_GUI = 2;

    public boolean        noautoload;
    /* if true, the gui won't try to automatically load base & user
     * chunkdesc & config files.
     */

    public ClientGlobals (boolean l) {
	noautoload = false;
	multilevelconfigenabled = false;
	multilevelchunkdescenabled = false;
	chunkdescusestandardfilereq = false;
	mode = APP_GUI;
    }

    public boolean isConnected() {
	if (mode == APP_GUI)
	    return net.isConnected();
	else
	    return false;
    }

}
