
package VjConfig;

import VjConfig.ConfigChunk;
import VjConfig.Property;
import java.util.Vector;

public class PropDependEntry {
    //public Property property;
    public String propertyname;
    public String dependency_name;
    public Vector other_files;
    
    public PropDependEntry () {
	other_files = new Vector();
    }

    public String toString () {
	String s = propertyname//property.getName()
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

