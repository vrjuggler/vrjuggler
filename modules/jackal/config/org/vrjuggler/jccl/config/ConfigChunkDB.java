
package VjConfig;

import java.util.Vector;
import VjConfig.ConfigChunk;
import java.io.*;
import VjConfig.ConfigStreamTokenizer;
import VjConfig.ChunkDependEntry;

public class ConfigChunkDB extends Vector {

    private ChunkDescDB descs;
    public String name;
    public File file;

    /* note: it's generally assumed that name will be the last component of file,
     * but that won't always be the case - e.g. 2 dbs named "config" in different
     * directories might get their names set to "config <1>" and "config <2>"
     * by the GUI.
     */

    public ConfigChunkDB (ChunkDescDB d) {
	super();
	descs = d;
	name = "Untitled";
	file = new File("Untitled");
    }



    public void setFile (File f) {
	file = f;
    }

    public void setName (String _name) {
	name = _name;
    }

    public String getName () {
	return name;
    }


    public ConfigChunkDB diff (ConfigChunkDB d) {
	/* builds a new ConfigChunkDB that's sort of the 
	 * "difference" of its arguments - the returned 
	 * db contains every chunk in d that isn't
	 * in self or differs from the same-named chunk in self 
	 */
	ConfigChunk c1, c2;
	ConfigChunkDB newdb = new ConfigChunkDB(d.descs);
	
	for (int i = 0; i < d.size(); i++) {
	    c1 = (ConfigChunk)d.elementAt(i);
	    c2 = get (c1.name);
	    if ((c2 == null) || (!c1.equals(c2)))
		newdb.insertOrdered(c1);
	}
	return newdb;
    }



    public boolean removeAll () {
	removeAllElements();
	return true;
    }



    public boolean remove(String name) {
	for (int i = 0; i < size(); i++) {
	    if (((ConfigChunk)elementAt(i)).getName().equalsIgnoreCase(name)) {
		removeElementAt(i);
		return true;
	    }
	}
	return false;
    }



    public ConfigChunk get (String name) {
	for (int i = 0; i < size(); i++) {
	    if (((ConfigChunk)elementAt(i)).getName().equalsIgnoreCase(name)) {
		return (ConfigChunk)elementAt(i);
	    }
	}
	return null;
    }



    public Vector getOfDescName (String typename) {
	Vector v = new Vector();
	for (int i = 0; i < size(); i++) {
	    if (((ConfigChunk)elementAt(i)).desc.name.equalsIgnoreCase(typename))
		v.addElement((ConfigChunk)elementAt(i));
	}
	return v;
    }
    


    public Vector getOfDescToken (String typename) {
	Vector v = new Vector();
	for (int i = 0; i < size(); i++) {
	    if (((ConfigChunk)elementAt(i)).desc.token.equalsIgnoreCase(typename))
		v.addElement((ConfigChunk)elementAt(i));
	}
	return v;
    }



    public void insertOrdered (ConfigChunk c) {
	/* adds a chunk into self, with the constraint 
	 * that it's stored with all the other chunks of its type.
	 * For the moment, we won't put any real order on that.
	 */
	ConfigChunk t;
	int i;

	remove(c.name);
	for (i = 0; i < size(); i++) {
	    t = (ConfigChunk)elementAt(i);
	    if (t.getDescToken().equalsIgnoreCase(c.getDescToken())) 
		break;
	}
	if (i == size())
	    addElement(c);
	else {
	    for ( ; i < size(); i++) {
		t = (ConfigChunk)elementAt(i);
		if (!t.getDescToken().equalsIgnoreCase(c.getDescToken()))
		    break;
	    }
	    insertElementAt(c, i);
	}
    }



    public String getNewName (String root) {
	/* generates a name for a chunk, prefixed with root, that
	 * won't conflict with any names of chunks currently in self
	 */
	String name;
	int num = 1;
	do {
	    name = root + Integer.toString(num++);
	} while (get(name) != null);
	return name;
    }



    public boolean read (ConfigStreamTokenizer st) {
	ConfigChunk c;

	for (;;) {
	    c = readAChunk(st);
	    if (c != null)
		insertOrdered(c);
	    else
		break;
	}
	return true;
    }



    public ConfigChunk readAChunk (ConfigStreamTokenizer st) {
	String s;
	ConfigChunk c = null;
	ChunkDesc d;

	try {
	    while (c == null) {
		st.nextToken();
		if ((st.ttype == ConfigStreamTokenizer.TT_EOF) ||
		    ((st.ttype == ConfigStreamTokenizer.TT_WORD) &&
		     st.sval.equalsIgnoreCase ("end")))
		    return null;
		d = descs.get(st.sval);
		if (d != null) {
		    c = new ConfigChunk(d, descs);
		    c.read(st);
		    if ((c.name == null) || (c.name.equals(""))) {
			c.name = getNewName (c.desc.name);    
		    }
		} 
		else {
		    System.err.println ("Error reading ConfigChunks - no such chunk type " + st.sval);
		    for(;;) {
			st.nextToken();
			if ((st.ttype == ConfigStreamTokenizer.TT_WORD &&
			     st.sval.equalsIgnoreCase("end")) ||
			    (st.ttype == ConfigStreamTokenizer.TT_EOF))
			    break;
		    }    
		}
	    }
	    return c;   
	}
	catch (IOException io) {
	    System.err.println ("IO Error in ConfigChunkDB.read()");
	    return null;
	}
    }



    public void replace(ConfigChunk o, ConfigChunk n) {
	/* removes old from the db and inserts new in old's place */
	ConfigChunk t;
	for (int i = 0; i < size(); i++) {
	    t = (ConfigChunk)elementAt(i);
	    if (t.getName().equals(o.getName())) {
		removeElementAt(i);
		insertElementAt(n,i);
		return;
	    }
	}
    }



    public Vector getDependencies () {
	// gets the dependencies of _this_chunk_only_
	int i, j, k;
	ConfigChunk ch, ch2;
	Property p;
	String s;
	VarValue val;
	ChunkDependEntry cde;
	ChunkDependEntry.PropDependEntry pde;

	Vector dep = new Vector();
	
	for (i = 0; i < size(); i++) {
	    cde = null;
	    ch = (ConfigChunk)elementAt(i);
	    for (j = 0; j < ch.props.size(); j++) {
		p = (Property)ch.props.elementAt(j);
		if (p.valtype.equals (ValType.t_chunk)) {
		    for (k = 0; k < p.vals.size(); k++) {
			s = ((VarValue)p.vals.elementAt(k)).getString();
			if (s.equals (""))
			    continue;
			ch2 = get(s);
			if (ch2 == null) {
			    if (cde == null) {
				cde = new ChunkDependEntry();
				cde.chunk = ch;
			    }
			    pde = cde.new PropDependEntry();
			    pde.property = p;
			    pde.dependency_name = s;
			    cde.propdepends.addElement (pde);
			}
		    }
		}
	    }
	    if (cde != null)
		dep.addElement (cde);
	}
	return dep;
    }


    public void searchDependencies (Vector v) {
	// searches self for any chunks that satisfy dependencies in v,
	// which is a vector of DependEntry.  DependEntries in v are
	// updated with that information
	ChunkDependEntry cde;
	ChunkDependEntry.PropDependEntry pde;
	ConfigChunk ch;
	int i, j;

	for (i = 0; i < v.size(); i++) {
	    cde = (ChunkDependEntry)v.elementAt(i);
	    for (j = 0; j < cde.propdepends.size(); j++) {
		pde = (ChunkDependEntry.PropDependEntry)cde.propdepends.elementAt(j);
		ch = get (pde.dependency_name);
		if (ch != null)
		    pde.other_files.addElement (getName());
	    }
	}
    }



    public String fileRep() {
	/* returns a string representation (actually a config file representation)
	 * of the db.
	 * Can't just overload toString cuz that's final for vector.
	 */
	String s = "";
	for (int i = 0; i < size(); i++)
	    s = s + ((ConfigChunk)elementAt(i)).toString();
	s = s + "End\n";
	return s;
    }
}


