/* DescEnum -- an entry in the enum vector of a PropertyDesc */

package VjConfig;

import VjConfig.VarValue;

public class DescEnum {

  public String str;
  public VarValue val;

  public DescEnum (DescEnum other) {
    str = other.str;
    val = new VarValue (other.val);
  }

  public DescEnum (String s, int v) {
    str = s;
    val = new VarValue(v);
  }

  public DescEnum (String s, float v) {
    str = s;
    val = new VarValue(v);
  }

  public boolean equals (DescEnum d) {
    if (!str.equalsIgnoreCase(d.str))
      return false;
    if (!val.equals(d.val))
      return false;
    return true;
  }

  public DescEnum (String line) {
    // note that this requires an explicit value - 'string=6' or something
    int i = line.indexOf("=");
    if (i == -1) {
      val = new VarValue(0);
      str = line;
    }
    else {
      str = line.substring (0, i);
      try {
	val = new VarValue (Integer.parseInt(line.substring(i+1)));
      }
      catch ( NumberFormatException e ) {
	val = new VarValue (Float.valueOf(line.substring(i+1)).floatValue());
      }
    }
  }

}
