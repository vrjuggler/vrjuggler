/*
 * Event type for add/remove of chunkdbs and descdbs notification
 * in VjGUI.Core
 */

package VjGUI;

import java.util.EventObject;
import VjConfig.ChunkDescDB;
import VjConfig.ConfigChunkDB;

public class CoreDBEvent extends EventObject {
    private ConfigChunkDB chunkdb;
    private ChunkDescDB descdb;


    public CoreDBEvent (Object _source, ConfigChunkDB _chunkdb, ChunkDescDB _descdb) {
	super(_source);
	descdb = _descdb;
	chunkdb = _chunkdb;
    }

    public ConfigChunkDB getChunkDB () {
	return chunkdb;
    }

    public ChunkDescDB getDescDB () {
	return descdb;
    }

}



