
package VjConfig;

import VjConfig.ConfigChunk;
import VjConfig.Property;
import java.util.Vector;

public class ChunkDependEntry {

    public ConfigChunk chunk;
    public Property property;
    public String dependency_name;

    public boolean satisfied_in_this_file;
    public Vector other_files;

    public ChunkDependEntry() {
	other_files = new Vector();
    }

    public String toString() {
	String s = "\"" + chunk.getName() + "\".\"" + property.getName()
	    + "\" needs \"" + dependency_name + "\"\n";
	if (other_files.size() > 0) {
	    s += "    satisfied in:\n";
	    for (int i = 0; i < other_files.size(); i++) {
		s += "        " + (String)other_files.elementAt(i) + "\n";
	    }
	}
	return s;
    }
}
