
package VjConfig;

import VjConfig.PropertyDesc;
import java.util.Vector;
import java.io.StreamTokenizer;
import java.util.NoSuchElementException;
import java.io.IOException;

public class Property {

  public String name;
  public String token;
  int num;
  public PropertyDesc desc;
  ValType valtype;
  public Vector vals;

  public boolean equals (Property p) {
    /* note - this does a total value comparison, not just checking names
     * The exception to this is that right now I only check the name of the
     * chunkdesc
     */
    VarValue v1,v2;

    if (!name.equals(p.name))
      return false;
    if (num != p.num)
      return false;
    if (!desc.name.equals(p.desc.name))
      return false;
    if (!valtype.equals(p.valtype))
      return false;
    if (vals.size() != p.vals.size())
      return false;
    for (int i = 0; i < vals.size(); i++) {
      v1 = (VarValue)vals.elementAt(i);
      v2 = (VarValue)p.vals.elementAt(i);
      if (!v1.equals(v2))
	return false;
    }
    return true;
  }

  public String toString() {
    VarValue v;
    String s = desc.token + " { ";
    for (int i = 0; i < vals.size(); i++) {
      v = (VarValue)vals.elementAt(i);
      if (valtype.equals(ValType.t_string) ||
	  valtype.equals(ValType.t_chunk)) 
	s += '"' + v.toString() + '"';
      else if (valtype.equals(ValType.t_bool) ||
	       (desc.enums.size() == 0))
	s += v.toString();
      else // int or float with enumeration
	if (valtype.equals(ValType.t_int)) {
	  try {
	    s += desc.getEnumString (v.getInt());
	  }
	  catch (java.util.NoSuchElementException e) {
	    ;
	  }
	}
      else
	if (valtype.equals(ValType.t_float)) {
	  try {
	    s += desc.getEnumString (v.getFloat());
	  }
	  catch (java.util.NoSuchElementException e) {
	    ;
	  }
	}
      s+= " ";
    }
    s += "}";
    return s;
  }

  public boolean read (StreamTokenizer st) throws IOException {
    /* the idea here is that st holds the string, less the property name
     * which was already read by ConfigChunk.read()
     */
    VarValue v;
    int n = 0;
    try {
      int t = st.nextToken();
      String token = st.sval;

      if (st.ttype == '{') {
	/* multiple entries */
	t = st.nextToken();
	token = st.sval;
	vals.removeAllElements();
	while (st.ttype != '}') {
	  v = buildVarValue(token);
	  vals.addElement(v);
	  st.nextToken();
	  token = st.sval;
	}
      }
      else {
	/* one entry */
	v = buildVarValue(token);
	vals.removeAllElements();
	vals.addElement(v);
      }
      if (num != -1) {
	/* are we a proper # of values? */
	while (vals.size() < num)
	  vals.addElement (new VarValue(valtype));
      }
      return true;
    }
    catch (IOException e) {
      /* Either a streamtokenizer error or unexpected eof */
      System.err.println ("Error in Property.read()");
      return false;
    }
  }

  public VarValue buildVarValue (String token) {
    /* here we build a token into an appopriate VarValue; includes
     * type resolution and enumeration lookup 
     */
    try {
      return (desc.getEnumValue(token));
    }
    catch (NoSuchElementException e) {
      VarValue v = new VarValue(valtype);
      v.set(token);
      return v;
    }
  }

  public Property (PropertyDesc d) {

    desc = d;
    name = d.name;
    token = d.token;
    valtype = d.valtype;
    num = d.num;
    vals = new Vector();
    if (num > 0) {
      for (int i = 0; i < num; i++)
	vals.addElement (new VarValue (d.valtype));
    }

  }

}


