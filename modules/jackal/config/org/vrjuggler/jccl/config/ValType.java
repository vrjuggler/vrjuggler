/* ValType - just replaces our valtype enumeration, more or less */

package VjConfig;

public class ValType {

  public final static int t_bool = 4;
  public final static int t_float = 2;
  public final static int t_int = 3;
  public final static int t_string = 1;
  public final static int t_chunk = 5;

  int val;

  public ValType (String s) {
    String s2 = s.toLowerCase().trim();
    if (s2.equalsIgnoreCase ("string"))
      val = t_string;
    else if (s2.equalsIgnoreCase ("float"))
      val = t_float;
    else if (s2.equalsIgnoreCase ("int"))
      val = t_int;
    else if (s2.equalsIgnoreCase ("bool"))
      val = t_bool;
    else if (s2.equalsIgnoreCase ("chunk"))
      val = t_chunk;
    else
      val = 0;
  }


  public String strVal() {
    switch (val) {
    case t_string:
      return "String";
    case t_float:
      return "Float";
    case t_int:
      return "Int";
    case t_bool:
      return "Bool";
    case t_chunk:
      return "Chunk";
    default:
      return "Undefined Type";
    }
  }

  public boolean equals (int t) {
    return (val == t);
  }

  public boolean equals (ValType v) {
    return (val == v.val);
  }
}

