/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



package VjConfig;

import java.util.Vector;
import java.io.*;
import VjConfig.*;

public class ConfigChunkDB {

    private Vector chunks;
    public String name;
    public File file;

    /* note: it's generally assumed that name will be the last component of file,
     * but that won't always be the case - e.g. 2 dbs named "config" in different
     * directories might get their names set to "config <1>" and "config <2>"
     * by the GUI.
     */

    public ConfigChunkDB () {
	super();
	chunks = new Vector();
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
	ConfigChunkDB newdb = new ConfigChunkDB();
	
	for (int i = 0; i < d.size(); i++) {
	    c1 = d.elementAt(i);
	    c2 = get (c1.name);
	    if ((c2 == null) || (!c1.equals(c2)))
		newdb.insertOrdered(c1);
	}
	return newdb;
    }



    public int size() {
	return chunks.size();
    }


    public ConfigChunk elementAt (int i) throws ArrayIndexOutOfBoundsException {
	return (ConfigChunk)chunks.elementAt(i);
    }

    public void removeAll () {
	chunks.removeAllElements();
    }

    public void removeElementAt (int i) {
	chunks.removeElementAt(i);
    }



    public boolean remove(String name) {
	for (int i = 0; i < size(); i++) {
	    if (elementAt(i).getName().equalsIgnoreCase(name)) {
		removeElementAt(i);
		return true;
	    }
	}
	return false;
    }



    public ConfigChunk get (String name) {
	ConfigChunk ch;
	for (int i = 0; i < size(); i++) {
	    ch = elementAt(i);
	    if (ch.getName().equalsIgnoreCase(name)) {
		return ch;
	    }
	}
	return null;
    }



    public Vector getOfDescName (String typename) {
	Vector v = new Vector();
	for (int i = 0; i < size(); i++) {
	    if (elementAt(i).desc.name.equalsIgnoreCase(typename))
		v.addElement(elementAt(i));
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


    public void addElement (Object o) {
	if (o instanceof ConfigChunk)
	    insertOrdered ((ConfigChunk)o);
	else
	    System.err.println ("Error: tried to add non-configchunk to chunkdb");
    }


    public void insertOrdered (ConfigChunk c) {
	/* adds a chunk into self, with the constraint 
	 * that it's stored with all the other chunks of its type.
	 * For the moment, we won't put any real order on that.
	 *
	 */
	ConfigChunk t;
	int i = 0;

	remove(c.name);

	// (slightly hacked to make sure includes are first in file)
	if (!c.getDescToken().equalsIgnoreCase("vjIncludeFile")) {
	    for (; i < chunks.size(); i++) {
		t = (ConfigChunk)chunks.elementAt(i);
		if (t.getDescToken().equalsIgnoreCase(c.getDescToken())) 
		    break;
	    }
	}
	if (i == size())
	    chunks.addElement(c);
	else {
	    for ( ; i < size(); i++) {
		t = (ConfigChunk)chunks.elementAt(i);
		if (!t.getDescToken().equalsIgnoreCase(c.getDescToken()))
		    break;
	    }
	    chunks.insertElementAt(c, i);
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
		c = ChunkFactory.createChunkWithDescToken(st.sval);
		if (c != null) {
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
		chunks.removeElementAt(i);
		chunks.insertElementAt(n,i);
		return;
	    }
	}
    }


    private Vector addDependsForChunk (ConfigChunk ch, Vector v, String nameprefix) {
	// returns vector of pde's
	// finds dependencies in ch and adds 'em to v.
	int i, j, k;
	ConfigChunk ch2;
	Property p;
	String s;
	VarValue val;
	ChunkDependEntry cde;
	PropDependEntry pde;

	for (j = 0; j < ch.props.size(); j++) {
	    p = (Property)ch.props.elementAt(j);
	    if (p.valtype.equals (ValType.t_chunk)) {
		for (k = 0; k < p.vals.size(); k++) {
		    s = ((VarValue)p.vals.elementAt(k)).getString();
		    if (s.equals (""))
			continue;
		    ch2 = get(s);
		    if (ch2 == null) {
			pde = new PropDependEntry();
			pde.propertyname = nameprefix + p.getName();
			if (p.vals.size() > 1)
			    pde.propertyname += "[" + k + "]";
			pde.dependency_name = s;
			v.addElement (pde);
		    }
		}
	    }
	    else if (p.valtype.equals (ValType.t_embeddedchunk)) {
		for (k = 0; k < p.vals.size(); k++) {
		    ch2 = ((VarValue)p.vals.elementAt(k)).getEmbeddedChunk();
		    String prefix = nameprefix + p.getName();
		    if (p.vals.size() > 1)
			prefix += "[" + k + "].";
		    else
			prefix += ".";
		    addDependsForChunk (ch2, v, nameprefix + prefix);
		}
	    }
	}
	return v;
    }


    public Vector getDependencies () {
	// gets the dependencies of _this_chunkdb_only_
	int i;
	ConfigChunk ch;
	ChunkDependEntry cde;

	Vector dep = new Vector();
	
	for (i = 0; i < size(); i++) {
	    cde = null;
	    ch = (ConfigChunk)elementAt(i);
	    cde = new ChunkDependEntry();
	    cde.chunk = ch;
	    addDependsForChunk (ch, cde.propdepends, "");
	    if (cde.propdepends.size() > 0)
		dep.addElement (cde);
	}
	return dep;
    }


    public void searchDependencies (Vector v) {
	// searches self for any chunks that satisfy dependencies in v,
	// which is a vector of DependEntry.  DependEntries in v are
	// updated with that information
	ChunkDependEntry cde;
	PropDependEntry pde;
	ConfigChunk ch;
	int i, j;

	for (i = 0; i < v.size(); i++) {
	    cde = (ChunkDependEntry)v.elementAt(i);
	    for (j = 0; j < cde.propdepends.size(); j++) {
		pde = (PropDependEntry)cde.propdepends.elementAt(j);
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


