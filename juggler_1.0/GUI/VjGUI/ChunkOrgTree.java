package VjGUI;

import java.awt.event.*;
import java.util.Vector;
import java.io.*;
import VjConfig.ConfigStreamTokenizer;
import VjGUI.*;

public class ChunkOrgTree {

    public OrgTreeElem root;
    Vector listeners; // ActionListeners - we'll send a "reload" action when we change
    String name;
    File file;

    public void copyValueFrom (ChunkOrgTree ot) {
	root = ot.root;
	name = ot.name;
	file = ot.file;
    }

    public void setRoot (OrgTreeElem _root) {
	root = _root;
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
	root = new OrgTreeElem ("Root");
	root.children.addElement (new OrgTreeElem ("*"));
	listeners = new Vector();
	name = "orgtree.org";
	file = new File("orgtree.org");
    }


    public void setName (String s) {
	name = s;
    }
    public void setFile (File f) {
	file = f;
    }
    public String getName () {
	return name;
    }
    public File getFile () {
	return file;
    }



    // reading... we assume that the initial 'begin label' has
    // not been read or has been pushed back.
    public boolean read (ConfigStreamTokenizer st) {
	boolean retval = root.read (st);
	processActionEvent (new ActionEvent (this, 5, "reload"));
	return retval;
    }


    // and writing
    public String toString() {
        return root.toString("");
    }


}





