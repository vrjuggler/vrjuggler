/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


package VjComponents.ConfigEditor;

import java.util.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.tree.*;

import VjControl.Core;
import VjConfig.*;
import VjComponents.ConfigEditor.*;

/** Trees used by ChunkDBPanel.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
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


    public void updateUI () {
        tree.updateUI();
        ((ChunkDBTreeCellRenderer)tree.getCellRenderer()).updateUI();
    }


    public String getName() {
	return (chunkdb == null) ? "No Selection" : chunkdb.getName();
    }
    


    public void useChunkOrgTree (ChunkOrgTree orgtree) {
	chunkorgtree = orgtree;
	buildTree();  
    }




    protected void addedChildCount (DefaultMutableTreeNode n) {
	if (n == null)
	    return;
	try {
	    ChunkTreeNodeInfo ni = (ChunkTreeNodeInfo)n.getUserObject();
	    ni.childchunks++;
	    addedChildCount ((DefaultMutableTreeNode)n.getParent());
            if (ni.childchunks == 1)
                nodeChanged (n);
	}
	catch (ClassCastException exc) {  // root node
	}
    }



    protected void removedChildCount (DefaultMutableTreeNode n) {
	if (n == null)
	    return;
	try {
	    ChunkTreeNodeInfo ni = (ChunkTreeNodeInfo)n.getUserObject();
	    ni.childchunks--;
	    removedChildCount ((DefaultMutableTreeNode)n.getParent());
            if (ni.childchunks == 0)
                nodeChanged (n);
	}
	catch (ClassCastException exc) {  // root node
	}
    }



    public void buildTree() {
        //System.out.println ("buildtree (chunkdbtreemodel)");
        DefaultMutableTreeNode newroot;

	if (chunkdb == null) {
	    newroot = new DefaultMutableTreeNode ("No DB selected");
	}
	else {
	    if (chunkorgtree == null) 
		Core.consoleErrorMessage ("ChunkDBTreeModel", "chunkorgtree is null..");

	    newroot = new DefaultMutableTreeNode (new ChunkTreeNodeInfo (ChunkTreeNodeInfo.FOLDER, chunkdb, chunkdb.getName()));
	    buildTreeEntry (chunkorgtree.root, newroot, chunkdb);
	}

        // fix up the child counts...
	DefaultMutableTreeNode n, n2;
	ChunkTreeNodeInfo inf, inf2;
	Enumeration ch, e;
	e = ((DefaultMutableTreeNode)newroot).depthFirstEnumeration();
	while (e.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)e.nextElement();
	    try {
		inf = (ChunkTreeNodeInfo)n.getUserObject();
                if (inf.isChunkNode())
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
        setRoot (newroot);
    }


    /** Helper for buildTree. */
    protected void buildTreeEntry (OrgTreeElem on, DefaultMutableTreeNode tn,
				ConfigChunkDB db) {
	DefaultMutableTreeNode newtn, newtn1, newtn2;
	ConfigChunk ch;
	OrgTreeElem newon;
	int i, j, k, n;
	String s, s2;
	List v;

	if (on == null) {
	    Core.consoleErrorMessage ("ChunkDBTree", "OrgTreeElem is NULL");
	    return;
	}
        n = on.children.size();
	for (i = 0; i < n; i++) {
	    if (on.children.get(i) instanceof String) {
		s = (String)(on.children.get(i));
		newtn = new DefaultMutableTreeNode (new ChunkTreeNodeInfo(ChunkTreeNodeInfo.DESC_FOLDER, db, s), true);
		// get all chunks in db of type s & add as children of newtn
		v = db.getOfDescName(s);
		for (j = 0; j < v.size(); j++) {
		    ch = (ConfigChunk)v.get(j);
		    newtn2 = new DefaultMutableTreeNode (new ChunkTreeNodeInfo(ChunkTreeNodeInfo.CHUNK, db, ch.getName()), false);
		    newtn.add(newtn2);
		}
		tn.add(newtn);
	    }
	    else if (on.children.get(i) instanceof OrgTreeElem) {
		newon = (OrgTreeElem)(on.children.get(i));
		newtn = new DefaultMutableTreeNode (new ChunkTreeNodeInfo (ChunkTreeNodeInfo.FOLDER, db, newon.label));
		
		if (newon.label.equals ("*")) {
		    newtn = new DefaultMutableTreeNode (new ChunkTreeNodeInfo (ChunkTreeNodeInfo.FOLDER, db, "All Chunks"));
                    int m = Core.descdb.size();
		    for (j = 0; j < m; j++) {
			s2 = Core.descdb.get(j).name;
			newtn1 = new DefaultMutableTreeNode (new ChunkTreeNodeInfo (ChunkTreeNodeInfo.DESC_FOLDER, db, s2));
			v = db.getOfDescName(s2);
			for (k = 0; k < v.size(); k++) {
			    ch = (ConfigChunk)v.get(k);
			    newtn2 = new DefaultMutableTreeNode (new ChunkTreeNodeInfo(ChunkTreeNodeInfo.CHUNK, db, ch.getName()), false);
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



    protected void doReplaceChunk (ConfigChunk oldc, ConfigChunk newc) {
	ChunkTreeNodeInfo ni;
	DefaultMutableTreeNode n;
	
	Enumeration nodes = ((DefaultMutableTreeNode)root).depthFirstEnumeration();
	while (nodes.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)nodes.nextElement();
	    ni = (ChunkTreeNodeInfo)n.getUserObject();
	    if (ni.isChunkNode() && ni.name.equals(oldc.getName())) {
		ni.name = newc.getName();
                nodeChanged (n);
	    }
	}
    }


    protected void doRemoveChunk (ConfigChunk ch) {
	/* note: doing this in a single stage seems to screw up the enumeration
	 * when we remove elements from the tree, causing us to miss nodes that
	 * should be removed.
	 */
        //System.out.println ("removing node: " + ch.getName());
	DefaultMutableTreeNode node, p;
	ChunkTreeNodeInfo ni;
	int i, n;
	List v = new ArrayList();

	Enumeration nodes = ((DefaultMutableTreeNode)root).depthFirstEnumeration();
	while (nodes.hasMoreElements()) {
	    node = (DefaultMutableTreeNode)nodes.nextElement();
            if (node.isLeaf()) {
                ni = (ChunkTreeNodeInfo)node.getUserObject();
                if (ni.isChunkNode() && (ni.toString().equalsIgnoreCase (ch.getName())))
                    v.add(node);
            }
	}
        n = v.size();
	for (i = 0; i < n; i++) {
	    node = (DefaultMutableTreeNode)v.get(i);
	    removedChildCount((DefaultMutableTreeNode)node.getParent());
            removeNodeFromParent (node);
	}
    }


    protected void doRemoveAllChunks () {
	/* note: doing this in a single stage seems to screw up the enumeration
	 * when we remove elements from the tree, causing us to miss nodes that
	 * should be removed.
	 */
        //System.out.println ("remove all chunks called on chundbtreemodel");
	DefaultMutableTreeNode node, p;
	int i, n;
	List v = new ArrayList();

	Enumeration nodes = ((DefaultMutableTreeNode)root).depthFirstEnumeration();
	while (nodes.hasMoreElements()) {
	    node = (DefaultMutableTreeNode)nodes.nextElement();
	    if (((ChunkTreeNodeInfo)node.getUserObject()).isChunkNode()) {
		v.add(node);
	    }
	}
        n = v.size();
	for (i = 0; i < n; i++) {
	    node = (DefaultMutableTreeNode)v.get(i);
            removedChildCount ((DefaultMutableTreeNode)node.getParent());
            removeNodeFromParent (node);
	}
    }


    protected void doAddChunk (ConfigChunk ch) {
        //System.out.println ("adding node: " + ch.getName());
	doRemoveChunk (ch);
	ChunkTreeNodeInfo ni;
	DefaultMutableTreeNode parent, child;
	Enumeration nodes = ((DefaultMutableTreeNode)root).depthFirstEnumeration();
	while (nodes.hasMoreElements()) {
	    parent = (DefaultMutableTreeNode)nodes.nextElement();
	    ni = (ChunkTreeNodeInfo)parent.getUserObject();
	    if (ni.isDescNode() && 
		ni.toString().equalsIgnoreCase(ch.getDescName())) {
                child = new DefaultMutableTreeNode (new ChunkTreeNodeInfo(ChunkTreeNodeInfo.CHUNK, chunkdb, ch.getName()), false);
		addedChildCount(parent);
                insertNodeInto (child, parent, parent.getChildCount());
	    } 
	}
    }


    //------------------ Listener helper classes ---------------------

    // These classes are used to encapsulate various change requests 
    // so that they can be placed on the EventDispatchThread's queue.
    // Remember that Swing is single-threaded <sigh>.

    final public class ReplaceChunkHandler implements Runnable {
        ConfigChunk oldc, newc;
        public ReplaceChunkHandler (ConfigChunk _oldc, ConfigChunk _newc) {
            oldc = _oldc;
            newc = _newc;
        }
        public void run () {
            doReplaceChunk (oldc, newc);
        }
    }

    final public class AddChunkHandler implements Runnable {
        ConfigChunk newc;
        public AddChunkHandler (ConfigChunk _newc) {
            newc = _newc;
        }
        public void run () {
            doAddChunk (newc);
        }
    }

    final public class RemoveChunkHandler implements Runnable {
        ConfigChunk oldc;
        public RemoveChunkHandler (ConfigChunk _oldc) {
            oldc = _oldc;
        }
        public void run () {
            doRemoveChunk (oldc);
        }
    }

    final public class RemoveAllChunksHandler implements Runnable {
        public RemoveAllChunksHandler () {
        }
        public void run () {
            doRemoveAllChunks ();
        }
    }
            

    //------------------------ ChunkDBListener Stuff -------------------------

    public void replaceChunk (ChunkDBEvent e) {
        //System.out.println ("replace chunk... chunkdbtreemodel");
	ConfigChunk oldc = e.getOldChunk();
	ConfigChunk newc = e.getNewChunk();

        if (SwingUtilities.isEventDispatchThread()) {
            doReplaceChunk (oldc, newc);
        }
        else {
            try {
                SwingUtilities.invokeLater (new ReplaceChunkHandler (oldc, newc));
            }
            catch (Exception ex) {
            }
        }
    }



    public void removeChunk (ChunkDBEvent e) {
        if (SwingUtilities.isEventDispatchThread()) {
            doRemoveChunk (e.getOldChunk());
        }
        else {
            try {
                SwingUtilities.invokeLater (new RemoveChunkHandler (e.getOldChunk()));
            }
            catch (Exception ex) {
            }
        }
    }



    public void removeAllChunks (ChunkDBEvent e) {
        if (SwingUtilities.isEventDispatchThread()) {
            doRemoveAllChunks ();
        }
        else {
            try {
                SwingUtilities.invokeLater (new RemoveAllChunksHandler ());
            }
            catch (Exception ex) {
            }
        }
    }



    public void addChunk (ChunkDBEvent e) {
        if (SwingUtilities.isEventDispatchThread()) {
            doAddChunk (e.getNewChunk());
        }
        else {
            try {
                SwingUtilities.invokeLater (new AddChunkHandler (e.getNewChunk()));
            }
            catch (Exception ex) {
            }
        }
    }


    //--------------------- ActionListener stuff -----------------------------

    public void actionPerformed (ActionEvent e) {
	if (e.getActionCommand().equalsIgnoreCase ("reload")) {
	    buildTree();
	}
    }


}
