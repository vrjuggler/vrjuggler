
package VjConfig;

import VjConfig.ChunkDescDB;
import VjConfig.ChunkDesc;
import VjConfig.ConfigChunk;

public class ChunkFactory {

    static private ChunkDescDB descdb = null;

    static public void setChunkDescDB (ChunkDescDB _db) {
	descdb = _db;
    }

    static public ConfigChunk createChunkWithDescName (String s) {
	ChunkDesc cd = descdb.getByName(s);
	if (cd != null)
	    return new ConfigChunk (cd, descdb);
	else
	    return null;
    }

    static public ConfigChunk createChunk (ChunkDesc cd) {
	return new ConfigChunk (cd, descdb);
    }
}
