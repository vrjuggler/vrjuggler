
package VjConfig;

import VjConfig.ConfigChunk;
import VjConfig.Property;
import java.util.Vector;

public class ChunkDependEntry {

    public ConfigChunk chunk;
    public Vector propdepends; // vector of PropDependEntry

    public ChunkDependEntry() {
	propdepends = new Vector();
    }

    public String toString() {
	int i;
	PropDependEntry pd;
	String s = "Chunk \"" + chunk.getName() + "\"\n";
	for (i = 0; i < propdepends.size(); i++)
	    s += ((PropDependEntry)propdepends.elementAt(i)).toString();
	return s;
    }

}
