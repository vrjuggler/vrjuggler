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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


package VjGUI;

import javax.swing.*;
import javax.swing.tree.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import java.util.Enumeration;

import VjConfig.*;
import VjGUI.*;
import VjGUI.chunkdesc.ChunkDescFrame;
import VjGUI.util.PlugPanel;


public class DescDBPanel extends JPanel 
    implements PlugPanel, ActionListener, MouseListener, CoreDBListener, DescDBListener {

    JTree tree;
    ChunkDescDB currdb;
    DefaultTreeModel model;
    JButton new_button;
    JButton load_button;
    JButton save_button;
    JButton close_button;
    JButton insert_button;
    JButton remove_button;
    JComboBox db_combobox;
    JScrollPane scroll_pane;



    public DescDBPanel () {
	super();

	ImageIcon new_icn, load_icn, save_icn, close_icn;
	JPanel south_panel, side_panel; Box center_panel;

	currdb = null;

	setBorder (BorderFactory.createEmptyBorder (5,5,5,5));
	setLayout (new BorderLayout (5, 5));

	// create buttons
	load_button = new JButton ("Load", Core.load_icn);
	save_button = new JButton ("Save", Core.save_icn);
	new_button = new JButton ("New", Core.new_icn);
	close_button = new JButton ("Close", Core.close_icn);
	insert_button = new JButton ("Insert");
	remove_button = new JButton ("Remove");
	
	// south panel - add/load etc. buttons
	side_panel = new JPanel();
	side_panel.setLayout (new GridLayout (8, 1));
	side_panel.add (new_button);
	side_panel.add (load_button);
	side_panel.add (save_button);
	side_panel.add (close_button);
	side_panel.add (insert_button);
	side_panel.add (remove_button);
	add (side_panel, "East");

	// side panel - send & send all buttons
//  	side_panel = new JPanel();
//  	side_panel.setLayout (new BoxLayout (side_panel, BoxLayout.Y_AXIS));
//  	add (side_panel, "West");
	
	// center: scrolled pane w/ tree
	center_panel = new Box (BoxLayout.Y_AXIS);
	center_panel.add (db_combobox = new JComboBox());
	buildDBList();
	tree = new JTree();
	scroll_pane = new JScrollPane (tree, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				       JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	center_panel.add (scroll_pane);
	add (center_panel, "Center");
	
	// setup event handling
	tree.addMouseListener (this);
	db_combobox.addActionListener (this);
	new_button.addActionListener (this);
	load_button.addActionListener (this);
	save_button.addActionListener (this);
	close_button.addActionListener (this);
	insert_button.addActionListener (this);
	remove_button.addActionListener (this);
	
	selectDB ("");

	Core.addCoreDBListener (this);
    }



    public void buildDBList () {
	int i;
	db_combobox.addItem ("No Selection");
	for (i = 0; i < Core.descdbs.size(); i++) {
	    db_combobox.addItem (((ChunkDescDB)Core.descdbs.elementAt(i)).name);
	}
    }



    public void selectDB (String name) {
	/* changes the currently displayed db in the panel to name... */
	int i;
	ChunkDesc d;
	DefaultMutableTreeNode root;

	//System.out.println ("SelectDB: " + name);

	if (name == null || name.equals (""))
	    name = "No Selection";
	ChunkDescDB db = Core.getChunkDescDB (name);
	if (db == null)
	    name = "No Selection";
	if (name.equalsIgnoreCase ("No Selection")) {
	    tree.setModel (new DefaultTreeModel (new DefaultMutableTreeNode ("no selection")));
	    if (currdb != null)
		currdb.removeDescDBListener(this);
	    currdb = null;
	}
	else {
	    if (currdb != null)
		currdb.removeDescDBListener(this);
	    currdb = db;
	    currdb.addDescDBListener(this);
	    root = new DefaultMutableTreeNode ("root");
	    for (i = 0; i < currdb.size(); i++) {
		d = currdb.elementAt(i);
		root.add (new DefaultMutableTreeNode (d.name));
	    }
	    model = new DefaultTreeModel (root);
	    tree.setModel (model);
	    tree.setRootVisible (false);
	    db_combobox.setSelectedItem (currdb.name);
	}
	if (name.equalsIgnoreCase ("No Selection"))
	    setButtonsEnabled(false);
	else
	    setButtonsEnabled(true);
    }



    private void setButtonsEnabled (boolean b) {
	save_button.setEnabled(b);
	close_button.setEnabled(b);
	insert_button.setEnabled(b);
	remove_button.setEnabled(b);
    }



    public void actionPerformed (ActionEvent e) {
	int i,j;
	String s;
	DefaultMutableTreeNode n;
	DefaultTreeModel model;
	TreePath[] tp;
	ChunkDesc d;

	Object source = e.getSource();
    
	if (source == remove_button) {
	    model = (DefaultTreeModel)tree.getModel();
	    tp = tree.getSelectionPaths();
	    if (tp == null)
		return;
	    for (i = 0; i < tp.length; i++) {
		n = (DefaultMutableTreeNode)tp[i].getLastPathComponent();
		s = (String)n.getUserObject();
		currdb.removeByName (s);
	    }
	}
	else if (source == insert_button) {
	    model = (DefaultTreeModel)tree.getModel();
	    d = new ChunkDesc();
	    for (j = 0; j <50; j++) {
		if ((currdb.getByName("unnamed"+j) == null) &&
		    (currdb.getByToken("unnamed"+j) == null))
		    break;
	    }
	    d.name = "unnamed" + j;
	    d.token = "unnamed" + j;
	    currdb.addElement (d);
	}
	else if (source == load_button) {
	    if (currdb == null)
		s = FileControl.loadNewDescDBFile("", true);
	    else
		s = FileControl.loadNewDescDBFile (currdb.file.getParent(), true);
	    if (s != null)
		selectDB(s);
	}
	else if (source == new_button) {
	    ChunkDescDB descdb = new ChunkDescDB();
	    selectDB (Core.addDescDB (descdb));
	}
	else if (source == close_button) {
	    Core.closeDescDB (currdb);
	}
	else if (source == save_button) {
	    selectDB (FileControl.saveDescDBFile(currdb));
	}
	else if (source == db_combobox) {
	    selectDB ((String)db_combobox.getSelectedItem());
	}
    }



    public void mouseClicked(MouseEvent e) {
	ChunkDescFrame f;
	DefaultMutableTreeNode n;
	String selected;

	if(e.getClickCount() == 2) {
	    TreePath selPath = tree.getPathForLocation(e.getX(), e.getY());
	    if(selPath != null) {
		n = (DefaultMutableTreeNode)selPath.getLastPathComponent();
		if (tree.getModel().getRoot() == n)
		    return; // it was the root node...
		selected = (String)n.getUserObject();
		if (selected != null) {
		    f = (ChunkDescFrame)Core.ui.getChildFrameMatching ("VjGUI.chunkdesc.ChunkDescFrame", currdb, currdb.getByName(selected));
		    //f = getDescFrame (selected);
		    if (f == null) {
			f = new ChunkDescFrame (Core.ui, currdb.getByName(selected), currdb, true);
			//desc_frames.addElement(f);
			Core.ui.addChildFrame(f);
		    }
		    else
			f.show();
		}
	    }
	}
    }



    public void mouseEntered(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}
    public void mousePressed(MouseEvent e) {}
    public void mouseReleased(MouseEvent e) {}



    /****************** CoreDBListener stuff **********************/

    public void addDescDB (CoreDBEvent e) {
	db_combobox.addItem (e.getDescDB().getName());
    }

    public void removeDescDB (CoreDBEvent e) {
	if (currdb == e.getDescDB())
	    selectDB ("No Selection");
	db_combobox.removeItem (e.getDescDB().getName());
    }

    public void addChunkDB (CoreDBEvent e) {
    }

    public void removeChunkDB (CoreDBEvent e) {
    }


    /***************** PlugPanel Stuff ***************************/

    public void destroy () {
	Core.removeCoreDBListener (this);
	if (currdb != null)
	    currdb.removeDescDBListener (this);
    }



    /***************** DescDBListener Stuff **********************/


    public void addDesc (DescDBEvent e) {
	// need to update the tree model too <sigh>
	DefaultMutableTreeNode root = (DefaultMutableTreeNode)tree.getModel().getRoot();
	DefaultMutableTreeNode n = new DefaultMutableTreeNode(e.getNewDesc().name);
	root.add(n);
	((DefaultTreeModel)tree.getModel()).reload(root);
    }	



    public void removeDesc (DescDBEvent e) {
	DefaultMutableTreeNode n;
	DefaultMutableTreeNode root = (DefaultMutableTreeNode)tree.getModel().getRoot();
	Enumeration en = root.children();
	while (en.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)en.nextElement();
	    if (e.getOldDesc().name.equals((String)n.getUserObject())) {
		root.remove(n);
		((DefaultTreeModel)tree.getModel()).reload(root);
		break;
	    }
	}
    }



    public void replaceDesc (DescDBEvent e) {
	DefaultMutableTreeNode n;
	DefaultMutableTreeNode root = (DefaultMutableTreeNode)tree.getModel().getRoot();
	Enumeration en = root.children();
	while (en.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)en.nextElement();
	    if (e.getOldDesc().name.equals((String)n.getUserObject())) {
	        n.setUserObject(e.getNewDesc().name);
		((DefaultTreeModel)tree.getModel()).reload(n);
		break;
	    }
	}
    }



    public void removeAllDescs (DescDBEvent e) {
	DefaultMutableTreeNode root = (DefaultMutableTreeNode)tree.getModel().getRoot();
	root.removeAllChildren();
	((DefaultTreeModel)tree.getModel()).reload(root);
    }


}




