
/* This corresponds to the config ChunkDesc class */

package VjConfig;

import java.util.Vector;
import java.io.*;
import VjConfig.PropertyDesc;

public class ChunkDesc {

    public Vector props;
    public  String name;
    public String token;
    public String help;


    public ChunkDesc (String n) {
	props = new Vector();
	addNameProperty();
	name = n;
	token = n;
	help = "";
    }


    public ChunkDesc () {
	props = new Vector();
	addNameProperty();
	name = "";
	token = "";
	help = "";
    }



    public String getName () {
	return name;
    }


    public String getToken () {
	return token;
    }


    public boolean equals (ChunkDesc c) {
	PropertyDesc p1, p2;
	if (!token.equals(c.token))
	    return false;
	
	/* This next part is O(n^2) <sigh> */
	for (int i = 0; i < props.size(); i++) {
	    p1 = (PropertyDesc) props.elementAt(i);
	    p2 = c.getPropertyDesc(p1.token);
	    if ((p2 == null) || (!p1.equals(p2)))
		return false;
	}
	return true;
    }



    private void addNameProperty () {
	/* quick little function to add in a name property if it isn't already
	 * in the ChunkDesc.
	 */
	PropertyDesc named;
	named = getPropertyDesc("Name");
	if (named == null) {
	    named = new PropertyDesc();
	    named.name = named.token = "Name";
	    named.help = "Unique name of an instance of this chunk type";
	    named.valtype = new ValType("string");
	    props.insertElementAt (named,0);
	}
	else
	    named.help = "Unique name of an instance of this chunk type";
    }



    public String toString() {
	String s = "chunk " + token + " \""
	    + name + "\" \"" + help + "\"\n";
	for (int i = 0; i < props.size(); i++)
	    s += "  " + ((PropertyDesc)props.elementAt(i)).toString() + "\n";
	s += "  end\n";
	//System.out.println ("string rep is:\n" + s);
	return s;
    }



    public PropertyDesc getPropertyDesc (String tok) {
	PropertyDesc p;
	for (int i = 0; i < props.size(); i++) {
	    p = (PropertyDesc)props.elementAt(i);
	    if (p.token.equalsIgnoreCase(tok))
		return p;
	}
	return null;
    }



  public boolean read (ConfigStreamTokenizer st) {
    /* the first line of the description ("chunk name") has already been
     * read. So, we need to read a series of property lines terminated
     * with an "end"
     */
    String s;
    PropertyDesc p;
    try {
	st.nextToken();
	name = st.sval;
	st.nextToken();
	help = st.sval;
      while (true) {
	st.nextToken();
	if (st.sval.equalsIgnoreCase ("end"))
	  break;
	else
	  st.pushBack();
	// else it's a property description.
	p = new PropertyDesc (st);
	if (p != null && p.name != null)
	    if (!p.name.equalsIgnoreCase("Name"))
		props.addElement (p);
      }
      return true;
    }
    catch (IOException io) {
      System.err.println ("IOException in ChunkDesc read");
      return false;
    }

  }
  
}

