
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

    public ClientGlobals (boolean l) {
    }

    public boolean isConnected() {
	return net.isConnected();
    }

}
