package VjGUI;

import java.awt.event.*;
import java.util.Vector;
import java.io.IOException;
import VjConfig.ConfigStreamTokenizer;

public class ChunkOrgTree {

    String label;     // label for this node
    Vector children;  // mixed vec. of Strings (chunkdescnames)
                      // and ChunkOrgTrees
    Vector listeners; // ActionListeners - we'll send a "reload" action when we change

    public void copyValueFrom (ChunkOrgTree ot) {
	// copies the values of the tree ot, but not its listeners
	label = ot.label;
	Object o;
	ChunkOrgTree ot2;
	children.removeAllElements();
	for (int i = 0; i < ot.children.size(); i++) {
	    o = ot.children.elementAt(i);
	    if (o instanceof String)
		children.addElement((String)o);
	    else {
		ot2 = new ChunkOrgTree ();
		ot2.copyValueFrom ((ChunkOrgTree)o);
		children.addElement(ot2);
	    }
	}
	
	processActionEvent (new ActionEvent (this, 5, "reload"));
    }

    public void addActionListener (ActionListener l) {
	if (!listeners.contains (l))
	    listeners.addElement (l);
    }
    public void removeActionListener (ActionListener l) {
	listeners.removeElement (l);
    }
    public void processActionEvent (ActionEvent e) {
	int i;
	ActionListener l;
	for (i = 0; i < listeners.size(); i++) {
	    l = (ActionListener)listeners.elementAt(i);
	    l.actionPerformed (e);
	}
    }

    public ChunkOrgTree () {
        label = "Root";
	children = new Vector();
	children.addElement (new ChunkOrgTree ("*"));
	listeners = new Vector();
    }

    public ChunkOrgTree (String _label){
        label = _label;
	children = new Vector();
	listeners = new Vector();
    }

    // reading... we assume that the initial 'begin label' has
    // not been read or has been pushed back.
    public boolean read (ConfigStreamTokenizer st) {
	boolean retval = readHelper (st);
	processActionEvent (new ActionEvent (this, 5, "reload"));
	return retval;
    }

    private boolean readHelper (ConfigStreamTokenizer st) {
        ChunkOrgTree ch;

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
                    ch = new ChunkOrgTree();
                    ch.readHelper(st);
                    children.addElement(ch);
                }
                else
                    children.addElement(st.sval);
            }
            return true;
        }
        catch (IOException e) {
            System.out.println ("ChunkOrgTree::Read: " + e.getMessage());
            return false;
        }
    }



    // and writing
    public String toString() {
        return toString("");
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
            else if (o instanceof ChunkOrgTree)
                s += ((ChunkOrgTree)o).toString(newpad);
            else 
		System.err.println ("There's something here that shouldn't be\n");
        }
        s += pad + "End\n";
        return s;
    }


}





