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

import java.util.Vector;
import java.util.Enumeration;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.tree.*;
import VjConfig.*;
import VjGUI.*;

public class ChunkDBTreeModel extends DefaultTreeModel implements ActionListener {

    public ConfigChunkDB chunkdb;
    ChunkOrgTree chunkorgtree;
    public JTree tree;
    public boolean inuse;



    public ChunkDBTreeModel (ConfigChunkDB _chunkdb, 
			     ChunkOrgTree _chunkorgtree) {
	super(new DefaultMutableTreeNode());
	chunkorgtree = _chunkorgtree;
	chunkdb = _chunkdb;
	asksAllowsChildren = true;
	buildTree();
	tree = new JTree ();
	
	DefaultTreeCellRenderer r = (DefaultTreeCellRenderer)tree.getCellRenderer();
	tree.setCellRenderer (new ChunkDBTreeCellRenderer(r.getOpenIcon(),
							  r.getClosedIcon(),
							  r.getLeafIcon()));
	tree.setShowsRootHandles (true);
	tree.setRootVisible (false);
	tree.setModel(this);
	inuse = false;
	chunkorgtree.addActionListener (this); // BUG - this never gets removed
    }



    public String getName() {
	if (chunkdb == null) 
	    return "No Selection";
	else
	    return chunkdb.name;
    }
    


    public void useChunkOrgTree (ChunkOrgTree orgtree) {
	chunkorgtree = orgtree;
	buildTree();  
    }



    public void replaceNode (ConfigChunk oldc, ConfigChunk newc) {
	ChunkTreeNodeInfo ni;
	DefaultMutableTreeNode n;
	
	chunkdb.replace(oldc, newc);
	Enumeration nodes = ((DefaultMutableTreeNode)root).breadthFirstEnumeration();
	while (nodes.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)nodes.nextElement();
	    ni = (ChunkTreeNodeInfo)n.getUserObject();
	    if (ni.ch == oldc) {
		ni.ch = newc;
		reload (n);
	    }
	}
    }



    public void removeNode (String name) {
	/* note: doing this in a single stage seems to screw up the enumeration
	 * when we remove elements from the tree, causing us to miss nodes that
	 * should be removed.
	 */
	DefaultMutableTreeNode n, p;
	int i;
	Vector v = new Vector();

	chunkdb.remove(name);
	Enumeration nodes = ((DefaultMutableTreeNode)root).breadthFirstEnumeration();
	while (nodes.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)nodes.nextElement();
	    if (n.toString().equalsIgnoreCase (name)) {
		v.addElement(n);
	    }
	}
	for (i = 0; i < v.size(); i++) {
	    n = (DefaultMutableTreeNode)v.elementAt(i);
	    p = (DefaultMutableTreeNode)n.getParent();
	    p.remove(n);
	    removedChildCount(p);
	    reload (p);
	}
    }



    public void insertNode (ConfigChunk ch) {
	removeNode (ch.getName());
	chunkdb.insertOrdered(ch);
	ChunkTreeNodeInfo ni;
	DefaultMutableTreeNode n;
	Enumeration nodes = ((DefaultMutableTreeNode)root).breadthFirstEnumeration();
	while (nodes.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)nodes.nextElement();
	    ni = (ChunkTreeNodeInfo)n.getUserObject();
	    if (ni.isDescNode() && 
		ni.toString().equalsIgnoreCase(ch.getDescName())) {
		n.add (new DefaultMutableTreeNode (new ChunkTreeNodeInfo(chunkdb, ch), false));
		addedChildCount(n);	
		reload(n);
	    } 
	}
    }



    public void addedChildCount (DefaultMutableTreeNode n) {
	if (n == null)
	    return;
	try {
	    ChunkTreeNodeInfo ni = (ChunkTreeNodeInfo)n.getUserObject();
	    ni.childchunks++;
	    addedChildCount ((DefaultMutableTreeNode)n.getParent());
	}
	catch (ClassCastException exc) {  // root node
	}
    }



    public void removedChildCount (DefaultMutableTreeNode n) {
	if (n == null)
	    return;
	try {
	    ChunkTreeNodeInfo ni = (ChunkTreeNodeInfo)n.getUserObject();
	    ni.childchunks--;
	    removedChildCount ((DefaultMutableTreeNode)n.getParent());
	}
	catch (ClassCastException exc) {  // root node
	}
    }



    public void buildTree() {

	if (chunkdb == null) {
	    root = new DefaultMutableTreeNode ("No DB selected");
	}
	else {
	    if (chunkorgtree == null) 
		Core.consoleErrorMessage ("ChunkDBTreeModel", "chunkorgtree is null..");

	    root = new DefaultMutableTreeNode (new ChunkTreeNodeInfo (chunkdb));
	    buildTreeEntry (chunkorgtree.root, (DefaultMutableTreeNode)root, chunkdb);
	}

	DefaultMutableTreeNode n, n2;
	ChunkTreeNodeInfo inf, inf2;
	Enumeration ch, e;
	e = ((DefaultMutableTreeNode)root).depthFirstEnumeration();
	while (e.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)e.nextElement();
	    try {
		inf = (ChunkTreeNodeInfo)n.getUserObject();
		if (inf.ch != null)
		    continue;
		ch = n.children();
		while (ch.hasMoreElements()) {
		    n2 = (DefaultMutableTreeNode)ch.nextElement();
		    inf2 = (ChunkTreeNodeInfo)n2.getUserObject();
		    inf.childchunks += inf2.childchunks;
		}
	    }
	    catch (ClassCastException ex) {
	    }
	}
	reload();
    }



    public void buildTreeEntry (OrgTreeElem on, DefaultMutableTreeNode tn,
				ConfigChunkDB db) {
	DefaultMutableTreeNode newtn, newtn1, newtn2;
	ConfigChunk ch;
	OrgTreeElem newon;
	int i, j, k;
	String s, s2;
	Vector v;

	if (on == null) {
	    System.out.println ("treeelem is null");
	    return;
	}
	for (i = 0; i < on.children.size(); i++) {
	    if (on.children.elementAt(i) instanceof String) {
		s = (String)(on.children.elementAt(i));
		newtn = new DefaultMutableTreeNode (new ChunkTreeNodeInfo(db, s, true));
		// get all chunks in db of type s & add as childrne of newtn
		v = db.getOfDescName(s);
		for (j = 0; j < v.size(); j++) {
		    ch = (ConfigChunk)v.elementAt(j);
		    newtn2 = new DefaultMutableTreeNode (new ChunkTreeNodeInfo(db, ch), false);
		    newtn.add(newtn2);
		}
		tn.add(newtn);
	    }
	    else if (on.children.elementAt(i) instanceof OrgTreeElem) {
		newon = (OrgTreeElem)(on.children.elementAt(i));
		newtn = new DefaultMutableTreeNode (new ChunkTreeNodeInfo (db, newon.label));
		
		if (newon.label.equals ("*")) {
		    newtn = new DefaultMutableTreeNode (new ChunkTreeNodeInfo (db, "All Chunks"));
		    for (j = 0; j < Core.descdb.size(); j++) {
			s2 = ((ChunkDesc)Core.descdb.elementAt(j)).name;
			newtn1 = new DefaultMutableTreeNode (new ChunkTreeNodeInfo (db, s2, true));
			v = db.getOfDescName(s2);
			for (k = 0; k < v.size(); k++) {
			    ch = (ConfigChunk)v.elementAt(k);
			    newtn2 = new DefaultMutableTreeNode (new ChunkTreeNodeInfo(db, ch), false);
			    newtn1.add(newtn2);
			}
			newtn.add(newtn1);
		    }
		    tn.add(newtn);
		}
		else {
		    buildTreeEntry (newon, newtn, db);
		    tn.add(newtn);
		}
	    }
	}
    }



    public void actionPerformed (ActionEvent e) {
	if (e.getActionCommand().equalsIgnoreCase ("reload")) {
	    buildTree();
	}
    }

}
