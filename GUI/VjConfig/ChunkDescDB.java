/* corresponds to ChunkDescDB.C/.h */

package VjConfig;

import java.util.Vector;
import java.io.*;

public class ChunkDescDB extends Vector {

    public String name;
    public File file;


    public ChunkDescDB () {
	name = "Unnamed";
	file = new File ("");
    }


    public void setName(String _name) {
	name = _name;
    }


    public void setFile (File _file) {
	file = _file;
    }


  public ChunkDescDB diff (ChunkDescDB d) {
    /* builds a new ChunkDescDB that's sort of the difference of its
     * arguments - the returned db contains every chunk in d that isn't
     * in self or differs from the same-named chunk in self 
     */
    ChunkDesc c1, c2;
    ChunkDescDB newdb = new ChunkDescDB();

    for (int i = 0; i < d.size(); i++) {
      c1 = (ChunkDesc)d.elementAt(i);
      c2 = get (c1.token);
      if ((c2 == null) || (!c1.equals(c2)))
        newdb.addElement(c1);
    }
    return newdb;
  }


  public void replace (ChunkDesc oldc, ChunkDesc newc) {
    int i = indexOf (oldc);
    if (i >= 0) {
      setElementAt (newc, i);
    }
    else
      addElement (newc);
  }

  public boolean removeAll () {
    removeAllElements();
    return true;
  }

  public boolean remove(String tok) {
    for (int i = 0; i < size(); i++) {
      if (((ChunkDesc)elementAt(i)).token.equals(tok)) {
	removeElementAt(i);
	return true;
      }
    }
    return false;
  }


  public boolean removeByName(String tok) {
    for (int i = 0; i < size(); i++) {
      if (((ChunkDesc)elementAt(i)).name.equalsIgnoreCase(tok)) {
	removeElementAt(i);
	return true;
      }
    }
    return false;
  }

    public Vector getTokenBegins (String tok) {
	ChunkDesc d;
	Vector v = new Vector();
	for (int i = 0; i < size(); i++) {
	    d = (ChunkDesc)elementAt(i);
	    if (d.token.startsWith (tok))
		v.addElement(d);
	}
	return v;
    }

  public ChunkDesc get (String tok) {
    for (int i = 0; i < size(); i++) {
      if (((ChunkDesc)elementAt(i)).token.equalsIgnoreCase(tok)) {
	return (ChunkDesc)elementAt(i);
      }
    }
    return null;
  }



    public ChunkDesc getByName (String tok) {
	for (int i = 0; i < size(); i++)
	    if (((ChunkDesc)elementAt(i)).name.equalsIgnoreCase(tok))
		return (ChunkDesc)elementAt(i);
	return null;
    }



    public String getTokenFromName (String n) {
	for (int i = 0; i < size(); i++)
	    if (((ChunkDesc)elementAt(i)).name.equalsIgnoreCase(n))
		return ((ChunkDesc)elementAt(i)).token;
	return "";
    }



    public String getNameFromToken (String n) {
	for (int i = 0; i < size(); i++)
	    if (((ChunkDesc)elementAt(i)).token.equalsIgnoreCase(n))
		return ((ChunkDesc)elementAt(i)).name;
	return "";
    }



    public void addElement (ChunkDesc d) {
	remove (d.token);
	super.addElement(d);
    }

  public void addElements (Vector v) {
    for (int i = 0; i < v.size(); i++)
      addElement ((ChunkDesc)v.elementAt(i));
  }


  public boolean read (ConfigStreamTokenizer st) {
    String name;
    ChunkDesc c;

    try {
      while (true) {
	st.nextToken();
	if (st.sval.equalsIgnoreCase ("end"))
	  return true;
	if (!st.sval.equalsIgnoreCase("chunk"))
	  throw new IOException();
	st.nextToken();
	name = st.sval;
	//System.err.println ("Reading chunkDesc: " + name);
	c = new ChunkDesc (name);
	c.read(st);
	addElement(c);
      }

    }
    catch (IOException io) {
      System.err.println ("IO Error in ChunkDescDB.read()");
      return false;
    }
  }

  public String fileRep () {
    String s = "";
    ChunkDesc d;
    for (int i = 0; i < size(); i++) {
      d = (ChunkDesc)elementAt(i);
      s += d.toString();
    }
    s += "End\n";
    return s;
  }

}




