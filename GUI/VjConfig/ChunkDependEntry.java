
package VjConfig;

import VjConfig.ConfigChunk;
import VjConfig.Property;
import java.util.Vector;

public class ChunkDependEntry {

    public class PropDependEntry {
	public Property property;
	public String dependency_name;
	public Vector other_files;

	public PropDependEntry () {
	    other_files = new Vector();
	}

	public String toString () {
	    String s = property.getName()
		+ " needs \"" + dependency_name + "\"\n";
	    if (other_files.size() > 0) {
		s += "    satisfied in:\n";
		for (int i = 0; i < other_files.size(); i++) {
		    s += "        " + (String)other_files.elementAt(i) + "\n";
		}
	    }
	    else {
		s += "    no match found in any loaded file.\n";
	    }
	    return s;
	}
    }


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
