
package VjConfig;

import VjConfig.ChunkDesc;
import java.util.Vector;
import java.io.DataInputStream;
import java.io.IOException;
import VjConfig.ConfigStreamTokenizer;

public class ConfigChunk {

  public Vector    props;
  public String    name;
  public ChunkDesc desc;

  public boolean equals (ConfigChunk c) {
    Property p1, p2;
    if (!name.equals(c.name))
      return false;
    if (!desc.token.equals(c.desc.token))
      return false;

    /* This next part is O(n^2) <sigh> */
    for (int i = 0; i < props.size(); i++) {
      p1 = (Property) props.elementAt(i);
      p2 = c.getProperty(p1.token);
      if (!p1.equals(p2))
	return false;
    }
    return true;
  }

  public Property getProperty(String n) {
    Property p;
    for (int i = 0; i < props.size(); i++) {
      p = (Property)props.elementAt(i);
      if (p.name.equalsIgnoreCase(n))
	return p;
    }
    return null;
  }

  public Property getPropertyFromToken(String n) {
    Property p;
    for (int i = 0; i < props.size(); i++) {
      p = (Property)props.elementAt(i);
      if (p.desc.token.equalsIgnoreCase(n))
	return p;
    }
    return null;
  }


  public ConfigChunk (ChunkDesc d) {
    desc = d;
    name = "";
    props = new Vector();
    /* we start at one here cuz we don't include name, which is always
     * the first element in a propertydesc
     */
    for (int i = 1; i < d.props.size(); i++)
      props.addElement (new Property((PropertyDesc)d.props.elementAt(i)));
  }

  public ConfigChunk (ChunkDesc d, String n) {
    desc = d;
    name = n;
    props = new Vector();
    for (int i = 1; i < d.props.size(); i++)
      props.addElement (new Property((PropertyDesc)d.props.elementAt(i)));
  }

  public String getName() {
    return name;
  }

  public String getDescName() {
    return desc.name;
  }

  public boolean read (ConfigStreamTokenizer st) {
    try {

      while (true) {
	st.nextToken();
	if (st.ttype == ConfigStreamTokenizer.TT_EOF)
	  break;
	if ((st.ttype != ConfigStreamTokenizer.TT_WORD) && (st.ttype != '"'))
	  continue;

	if (st.sval.equalsIgnoreCase("end"))
	  break;

	//System.out.println("read property line for " + st.sval);

	/* begin reading property info out of s */
	if (st.sval.equalsIgnoreCase("name")) {
	  st.nextToken();
	  if (st.ttype == '{') {
	    st.nextToken();
	    name = st.sval;
	    st.nextToken();
	  }
	  else
	    name = st.sval;
	  //System.out.println ("read chunk name '" + name + "'");
	}
	else {
	  Property p = getPropertyFromToken(st.sval);
	  if (p != null) {
	    p.read(st);
	  }
	  else {
	    System.err.println ("Error: ConfigChunk.Read() - no such property "
				+ st.sval);
	  }
	}
	/* end reading property info from s */
      }
      return true;
    }
    catch (IOException io) {
      System.err.println ("IO Error in ConfigChunk.read()");
      return false;
    }
  }  

  public String toString() {
    String s = desc.token + "\n";
    s += "  Name \"" + name + "\"\n";
    for (int i = 0; i < props.size(); i++)
      s += "  " + ((Property)props.elementAt(i)).toString() + "\n";
    s += "  end\n";
    //System.out.println ("string rep is:\n" + s);
    return s;
  }


}




