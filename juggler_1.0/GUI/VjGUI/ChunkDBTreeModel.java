/*
 *  File:	    $RCSfile$
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

public class ChunkDBTreeModel extends DefaultTreeModel implements ActionListener, ChunkDBListener{

    public ConfigChunkDB chunkdb;
    ChunkOrgTree chunkorgtree;
    public JTree tree;
    public boolean inuse;



    public ChunkDBTreeModel (ConfigChunkDB _chunkdb, ChunkOrgTree orgtree) {
	super(new DefaultMutableTreeNode());
	chunkorgtree = orgtree;
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

//  	if (chunkdb == null) {
//  	    setRoot (
//  	}

	chunkorgtree.addActionListener (this); // BUG - this never gets removed
	if (chunkdb != null)
	    chunkdb.addChunkDBListener (this);
    }



    public String getName() {
	return (chunkdb == null) ? "No Selection" : chunkdb.getName();
    }
    


    public void useChunkOrgTree (ChunkOrgTree orgtree) {
	chunkorgtree = orgtree;
	buildTree();  
    }



    private void removeNode (ConfigChunk ch) {
	/* note: doing this in a single stage seems to screw up the enumeration
	 * when we remove elements from the tree, causing us to miss nodes that
	 * should be removed.
	 */
	DefaultMutableTreeNode n, p;
	ChunkTreeNodeInfo ni;
	int i;
	Vector v = new Vector();

	Enumeration nodes = ((DefaultMutableTreeNode)root).breadthFirstEnumeration();
	while (nodes.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)nodes.nextElement();
	    ni = (ChunkTreeNodeInfo)n.getUserObject();
	    if (ni.isChunkNode() && (ni.toString().equalsIgnoreCase (ch.getName())))
		v.addElement(n);
	}
	for (i = 0; i < v.size(); i++) {
	    n = (DefaultMutableTreeNode)v.elementAt(i);
	    p = (DefaultMutableTreeNode)n.getParent();
	    p.remove(n);
	    removedChildCount(p);
	    reload (p);
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
	    tree.repaint();
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
	    Core.consoleErrorMessage ("ChunkDBTree", "OrgTreeElem is NULL");
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
			s2 = Core.descdb.elementAt(j).name;
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


    /******************** ChunkDBListener Stuff ***********************/
    public void replaceChunk (ChunkDBEvent e) {
	ConfigChunk oldc = e.getOldChunk();
	ConfigChunk newc = e.getNewChunk();
	ChunkTreeNodeInfo ni;
	DefaultMutableTreeNode n;
	
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



    public void removeChunk (ChunkDBEvent e) { //String name) {
	removeNode (e.getOldChunk());
    }



    public void removeAllChunks (ChunkDBEvent e) {
	/* note: doing this in a single stage seems to screw up the enumeration
	 * when we remove elements from the tree, causing us to miss nodes that
	 * should be removed.
	 */
	DefaultMutableTreeNode n, p;
	int i;
	Vector v = new Vector();

	chunkdb.removeAll();
	Enumeration nodes = ((DefaultMutableTreeNode)root).breadthFirstEnumeration();
	while (nodes.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)nodes.nextElement();
	    if (((ChunkTreeNodeInfo)n.getUserObject()).isChunkNode()) {
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



    public void addChunk (ChunkDBEvent e) {
	ConfigChunk ch = e.getNewChunk();
	removeNode (ch);
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
	tree.repaint();
    }



}

