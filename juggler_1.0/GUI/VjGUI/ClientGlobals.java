
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

    public ClientGlobals (boolean l) {
	multilevelconfigenabled = false;
	multilevelchunkdescenabled = false;
	chunkdescusestandardfilereq = true;
    }

    public boolean isConnected() {
	return net.isConnected();
    }

}
