
package VjConfig;

import java.util.Vector;
import VjConfig.ConfigChunk;
import java.io.DataInputStream;
import java.io.IOException;
import VjConfig.ConfigStreamTokenizer;
import java.io.InputStreamReader;

public class ConfigChunkDB extends Vector {

    private ChunkDescDB descs;



    public ConfigChunkDB (ChunkDescDB d) {
	super();
	descs = d;
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
	    if (t.getDescName().equalsIgnoreCase(c.getDescName())) 
		break;
	}
	if (i == size())
	    addElement(c);
	else {
	    for ( ; i < size(); i++) {
		t = (ConfigChunk)elementAt(i);
		if (!t.getDescName().equalsIgnoreCase(c.getDescName()))
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
	String s;
	ConfigChunk c;
	
	try {
	    while (true) {
		st.nextToken();
		if ((st.ttype == ConfigStreamTokenizer.TT_EOF) ||
		    st.sval.equalsIgnoreCase ("end"))
		    break;
		ChunkDesc d = descs.get(st.sval);
		if (d != null) {
		    c = new ConfigChunk(d);
		    c.read(st);
		    if ((c.name == null) || (c.name.equals(""))) {
			c.name = getNewName (c.desc.name);
			
		    }
		    insertOrdered(c);
		} else {
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
	    
	    return true;
	    
	}
	catch (IOException io) {
	    System.err.println ("IO Error in ConfigChunkDB.read()");
	    return false;
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


