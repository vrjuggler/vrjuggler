
package VjConfig;

import java.util.Vector;
import VjConfig.ConfigChunkDB;
import VjConfig.ChunkDescDB;
import VjConfig.ChunkDesc;
import VjConfig.ConfigChunk;

public class ChunkFactory {

    static private ChunkDescDB descdb = null;
    static private ConfigChunkDB default_chunkdb = null;

    static public void setChunkDescDB (ChunkDescDB _db) {
	descdb = _db;
    }

    static public void setDefaultChunkDB (ConfigChunkDB _db) {
	default_chunkdb = _db;
    }

    static public ConfigChunk createChunkWithDescName (String s) {
	ConfigChunk newchunk = null;

	if (default_chunkdb != null) {
	    Vector v = default_chunkdb.getOfDescName (s);
	    if (!v.isEmpty()) {
		//System.out.println ("creating chunk from default");
		return new ConfigChunk ((ConfigChunk)v.elementAt(0));
	    }
	}
	if (descdb != null) {
	    ChunkDesc cd = descdb.getByName(s);
	    if (cd != null)
		return new ConfigChunk (cd, descdb);
	}
	return null;
    }

    static public ConfigChunk createChunk (ChunkDesc cd) {
	return new ConfigChunk (cd, descdb);
    }
}
