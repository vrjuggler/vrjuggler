/*
 *  File:	    $Name$
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


package VjGUI;

import java.awt.event.*;
import java.util.Vector;
import java.io.IOException;
import VjConfig.ConfigStreamTokenizer;

public class OrgTreeElem {

    String label;     // label for this node
    Vector children;  // mixed vec. of Strings (chunkdescnames)
                      // and OrgTreeElems

//     public void copyValueFrom (OrgTreeElem ot) {
// 	// copies the values of the tree ot, but not its listeners
// 	label = ot.label;
// 	Object o;
// 	OrgTreeElem ot2;
// 	children.removeAllElements();
// 	for (int i = 0; i < ot.children.size(); i++) {
// 	    o = ot.children.elementAt(i);
// 	    if (o instanceof String)
// 		children.addElement((String)o);
// 	    else {
// 		ot2 = new OrgTreeElem ();
// 		ot2.copyValueFrom ((OrgTreeElem)o);
// 		children.addElement(ot2);
// 	    }
// 	}
	
// 	//processActionEvent (new ActionEvent (this, 5, "reload"));
//     }

//     public OrgTreeElem () {
//         label = "Root";
// 	children = new Vector();
// 	children.addElement (new OrgTreeElem ("*"));
// 	listeners = new Vector();
//     }


    public OrgTreeElem (String _label){
        label = _label;
	children = new Vector();
    }


    // reading... we assume that the initial 'begin label' has
    // not been read or has been pushed back.
    public boolean read (ConfigStreamTokenizer st) {
        OrgTreeElem ch;

	label = "";
	children.removeAllElements();

        try {
            st.nextToken();
            if ((st.ttype != ConfigStreamTokenizer.TT_WORD) ||
		(!st.sval.equalsIgnoreCase("begin"))) {
                throw new IOException ("expected 'begin'");
	    }
            st.nextToken();
            if ((st.ttype != st.TT_WORD) && (st.ttype != '"')) {
                throw new IOException ("unexpected token '" + st.ttype + "'");
            }
            label = st.sval;
            for (;;) {
                st.nextToken();
                if ((st.ttype != st.TT_WORD) && (st.ttype != '"')) {
                    throw new IOException ("unexpected token '" + st.ttype + "'");
                }
                if (st.sval.equalsIgnoreCase ("end"))
                    break;
                else if (st.sval.equalsIgnoreCase ("begin")) {
                    st.pushBack();
                    ch = new OrgTreeElem("");
                    ch.read(st);
                    children.addElement(ch);
                }
                else
                    children.addElement(st.sval);
            }
            return true;
        }
        catch (IOException e) {
            System.out.println ("OrgTreeElem::Read: " + e.getMessage());
            return false;
        }
    }



    public String toString (String pad) {
        int i;
        Object o;
        String newpad = pad + "    ";
        String s = pad + "Begin \"" + label + "\"\n";
        for (i = 0; i < children.size(); i++) {
            o = children.elementAt(i);
            if (o instanceof String)
                s += newpad + "\"" + (String)o + "\"\n";
            else if (o instanceof OrgTreeElem)
                s += ((OrgTreeElem)o).toString(newpad);
            else 
		System.err.println ("There's something here that shouldn't be\n");
        }
        s += pad + "End\n";
        return s;
    }


}





