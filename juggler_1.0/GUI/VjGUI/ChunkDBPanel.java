/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


package VjGUI;

import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;

import VjConfig.*;
import VjGUI.*;
import VjGUI.util.*;
import VjGUI.configchunk.ConfigChunkFrame;

public class ChunkDBPanel extends JPanel
    implements PlugPanel, ActionListener, MouseListener, CoreDBListener { 


    protected int controls_on_side; // 0 for left, 1 for right;
    protected ChunkDBTreeModel dbt;
    protected ConfigChunkDB chunkdb;
    protected ChunkDBPanel sendacross_target; // target for the >>, >all> buttons

    protected JButton load_button;
    protected JButton save_button;
    protected JButton insert_button;
    protected JButton remove_button;
    protected JButton send_button;
    protected JButton send_all_button;
    protected JButton new_button;
    protected JButton close_button;
    protected JButton duplicate_button;
    protected JButton chunkhelp_button;
    protected JButton checkdepend_button;
    protected JComboBox db_combobox;
    protected JComboBox insert_type;
    protected JScrollPane scroll_pane;
    protected Font current_font;  // used to make sure when we add a new tree
                                // that it's using the correct font.

    protected TreePath treeitem_menu_path;
    protected JPopupMenu desctreeitem_menu;
    protected JPopupMenu chunktreeitem_menu;
    protected JMenuItem help1_mi, help2_mi, remove_mi, insert_mi;

    protected static ChunkDBTreeModelFactory dbt_factory;



    public ChunkDBPanel (int _controls_on_side) {
	super();

	if (dbt_factory == null)
	    dbt_factory = new ChunkDBTreeModelFactory();

	JPanel south_panel;
	JPanel side_panel; 
	Box center_panel;

	current_font = null;
	chunkdb = null;
	dbt = null;
	sendacross_target = null;

	controls_on_side = _controls_on_side;
	setBorder (new EmptyBorder (5,5,5,5));
	setLayout (new BorderLayout (5, 5));

	// south panel - add/load etc. buttons
 	south_panel = new JPanel();
 	add (south_panel, "South");
	south_panel.setLayout (new BoxLayout (south_panel, BoxLayout.X_AXIS));

	ImageIcon new_icn, load_icn, save_icn, close_icn;

	load_button = new JButton ("Load", Core.load_icn);
	save_button = new JButton ("Save", Core.save_icn);
	new_button = new JButton ("New", Core.new_icn);
	close_button = new JButton ("Close", Core.close_icn);

	Insets ins = new Insets (0,0,0,0);
	new_button.setMargin (ins);
	load_button.setMargin (ins);
	save_button.setMargin (ins);
	close_button.setMargin (ins);

	side_panel = new JPanel();
	side_panel.setLayout (new GridLayout (12, 1));

	side_panel.add (new JLabel ("File"));

	side_panel.add (new_button);
	side_panel.add (load_button);
	side_panel.add (save_button);
	side_panel.add (close_button);

	side_panel.add (checkdepend_button = new JButton ("Verify"));

	side_panel.add (new JLabel ("Chunk"));
	if (controls_on_side == 0) {
	    side_panel.add (send_button = new JButton (">>"));
	    side_panel.add (send_all_button = new JButton (">All>"));
	    add (side_panel, "East");
	}
	else {
	    side_panel.add (send_button = new JButton ("<<"));
	    side_panel.add (send_all_button = new JButton ("<All<"));
	    add (side_panel, "West");
	}

	side_panel.add (remove_button = new JButton ("Remove"));
	side_panel.add (duplicate_button = new JButton ("Duplicate"));
	side_panel.add (insert_button = new JButton ("Insert"));

	GridBagLayout gbl = new GridBagLayout();
	south_panel.setLayout (gbl);
	GridBagConstraints gbc = new GridBagConstraints();
	gbc.fill = gbc.BOTH;
	gbc.gridwidth = 1;
	JLabel tl = new JLabel ("Insert type:");
	gbl.setConstraints (tl, gbc);
	south_panel.add (tl);
	gbc.gridwidth = gbc.RELATIVE;
	insert_type = new JComboBox();
	gbl.setConstraints (insert_type, gbc);
	south_panel.add (insert_type);
	chunkhelp_button = new JButton ("Help");
	chunkhelp_button.setMargin (new Insets (0,0,0,0));
	gbc.gridwidth = 1;
	gbl.setConstraints (chunkhelp_button, gbc);
	south_panel.add (chunkhelp_button);

	// center: scrolled pane w/ tree
	center_panel = new Box (BoxLayout.Y_AXIS);
	center_panel.add (db_combobox = new JComboBox());
	buildDBList();

	scroll_pane = new JScrollPane (JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				       JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

	center_panel.add (scroll_pane);
	add (center_panel, "Center");

	// set up initial tree viewing stuff. can't use selectDB() for this
	// because it sees the chunkdb is already null and decides not to do
	// anything.
	dbt = dbt_factory.getTreeModel (chunkdb);
	scroll_pane.setViewportView (dbt.tree);
	dbt.tree.addMouseListener(this);
	db_combobox.setSelectedItem (dbt.getName());
	setButtonsEnabled (false);

	// setup event handling
	db_combobox.addActionListener (this);
	load_button.addActionListener (this);
	save_button.addActionListener (this);
	checkdepend_button.addActionListener (this);
	insert_button.addActionListener (this);
	duplicate_button.addActionListener (this);
	remove_button.addActionListener (this);
	send_button.addActionListener (this);
	send_all_button.addActionListener (this);
	new_button.addActionListener (this);
	close_button.addActionListener (this);
	chunkhelp_button.addActionListener (this);

	// tooltips
	new_button.setToolTipText ("Create a new Config file");
	load_button.setToolTipText ("Load another Config file");
	save_button.setToolTipText ("Save this Config file");
	close_button.setToolTipText ("Close this file (doesn't save)");
	checkdepend_button.setToolTipText ("Check for missing dependencies");
	send_button.setToolTipText ("Copy selected chunks to the other panel");
	send_all_button.setToolTipText ("Copy all chunks over to the other panel");
	insert_button.setToolTipText ("Inserts new chunk (select type below)");
	duplicate_button.setToolTipText ("Insert copies of all selected chunks");
	remove_button.setToolTipText ("Remove all selected chunks");
	chunkhelp_button.setToolTipText ("Information about the insert selection");

	// initialize popup menus
	treeitem_menu_path = null;
	desctreeitem_menu = new JPopupMenu ();
	desctreeitem_menu.add (help1_mi = new JMenuItem ("Chunk Help"));
	desctreeitem_menu.add (insert_mi = new JMenuItem ("Insert"));
	chunktreeitem_menu = new JPopupMenu ();
	chunktreeitem_menu.add (help2_mi = new JMenuItem ("Chunk Help"));
	chunktreeitem_menu.add (remove_mi = new JMenuItem ("Remove"));

	help1_mi.addActionListener (this);
	help2_mi.addActionListener (this);
	remove_mi.addActionListener (this);
	insert_mi.addActionListener (this);

	// listen for new/delete chunkdb events from Core
	Core.addCoreDBListener (this);

    }



    // sets the target for >> and >all> buttons
    public void setSendAcrossTarget (ChunkDBPanel _target) {
	sendacross_target = _target;
    }



    public void setFont (Font f) {
	super.setFont(f);
	current_font = f;
    }



    protected void buildDBList () {
	int i;
	db_combobox.addItem ("No Selection");
	for (i = 0; i < Core.chunkdbs.size(); i++) {
	    db_combobox.addItem (((ConfigChunkDB)Core.chunkdbs.elementAt(i)).getName());
	}
    }



    public void selectDB (String name) {
	/* changes the currently displayed db in the panel to name... */
	ConfigChunkDB newchunkdb = Core.getChunkDB (name);
	if (newchunkdb == chunkdb)
	    return;
	chunkdb = newchunkdb;

	if (dbt != null) {
	    dbt.tree.removeMouseListener (this);
	    dbt_factory.releaseTreeModel (dbt);
	}
	    
	dbt = dbt_factory.getTreeModel (chunkdb);
	if (current_font != null)
	    dbt.tree.setFont(current_font);
	scroll_pane.setViewportView (dbt.tree);
	dbt.tree.addMouseListener(this);
	db_combobox.setSelectedItem (dbt.getName());
	validate();
	scroll_pane.repaint();

	if (dbt == null)
	    System.out.println ("dbt is null & shouldn't be");
	if (dbt.tree == null)
	    System.out.println ("dbt tree is null & shouldn't be");

	setButtonsEnabled (chunkdb != null);

    }



    // typically, one chunkdbpanel will call this on another to perform a 
    // sendacross action
    protected void addChunks (ConfigChunkDB newdb) {
	if (chunkdb == null)
	    return;
	else if (chunkdb == Core.active_chunkdb) {
	    for (int i = 0; i < newdb.size(); i++)
		Core.net.sendChunk ((ConfigChunk)newdb.elementAt(i));
	}
	else {
	    for (int i = 0; i < newdb.size(); i++)
		chunkdb.addElement ((ConfigChunk)newdb.elementAt(i));
	}
    }	



    // updates the list of chunk types which can be inserted.
    protected void updateInsertTypes () {
	int i, j;
	if (insert_type == null)
	    return;
	// as of swing 1.2, there seems to be a bug in removeAllItems...
	//insert_type.removeAllItems();
	j = insert_type.getItemCount();
	for (i = 0; i < j; i++)
	    insert_type.removeItemAt (0);
	for (i = 0; i < Core.descdb.size(); i++) {
	    insert_type.addItem (Core.descdb.elementAt(i).name);
	}
    }



    // enables/disables some of the side panel buttons. Used when there
    // is no loaded db.
    protected void setButtonsEnabled (boolean b) {
	save_button.setEnabled(b);
	insert_button.setEnabled(b);
	remove_button.setEnabled(b);
	send_button.setEnabled(b);
	send_all_button.setEnabled(b);
	close_button.setEnabled(b);
	duplicate_button.setEnabled(b);
	checkdepend_button.setEnabled(b);
    }



    /******************** ActionListener Stuff ***********************/

    // public, as the saying goes, as an implementation detail
    public void actionPerformed (ActionEvent e) {
	int i,j;
	String s;
	ChunkTreeNodeInfo ni;
	TreePath[] tp;
	ConfigChunkDB v;
	Object source = e.getSource();
	ConfigChunk ch;

	if (source == send_button) {
	    tp = dbt.tree.getSelectionPaths();
	    if (tp == null)
		return;
	    v = new ConfigChunkDB();
	    if (tp != null) {
		for (i = 0; i < tp.length; i++) {
		    ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
		    if (ni.ch != null)
			v.addElement (ni.ch);
		}
	    }
	    if (sendacross_target != null)
		sendacross_target.addChunks (v);
	}
	else if (source == send_all_button) {
	    if (sendacross_target != null)
		sendacross_target.addChunks (chunkdb);
	}
	else if (source == new_button) {
	    selectDB (Core.addChunkDB (new ConfigChunkDB()));
	}
	else if (source == close_button) {
	    Core.closeChunkDB (chunkdb);
	}
	else if (source == checkdepend_button) {
	    Vector vec = chunkdb.getDependencies();
	    for (i = 0; i < Core.chunkdbs.size(); i++) {
		ConfigChunkDB db = (ConfigChunkDB)Core.chunkdbs.elementAt(i);
		if (db != chunkdb)
		    db.searchDependencies (vec);
	    }
            DependencyFrame dependency_frame = (DependencyFrame)Core.ui.getChildFrameMatching ("VjGUI.DependencyFrame", chunkdb, null);
            if (dependency_frame != null)
                dependency_frame.refreshData (vec);
            else {
		dependency_frame = new DependencyFrame(this, chunkdb, vec);
                Core.ui.addChildFrame (dependency_frame);
	    }
            dependency_frame.show();
	}
	else if (source == remove_button) {
	    tp = dbt.tree.getSelectionPaths();
	    if (tp == null)
		return;
	    if (chunkdb == Core.active_chunkdb) {
		ConfigChunkDB db = new ConfigChunkDB ();
		for (i = 0; i < tp.length; i++) {
		    ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
		    if (ni.ch != null) {
			db.addElement (ni.ch);
		    }
		}
		Core.net.removeChunks (db);
	    }
	    else {
		for (i = 0; i < tp.length; i++) {
		    ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
		    if (ni.ch != null) {
			chunkdb.remove(ni.ch.getName());
		    }
		}
	    }
	}
	else if (source == insert_button) {
	    ch = ChunkFactory.createChunkWithDescName ((String)insert_type.getSelectedItem());
	    if (ch != null) {
		ch.name = (chunkdb.getNewName(ch.getDescName()));
		chunkdb.addElement (ch);
	    }
	}
	else if (source == chunkhelp_button) {
	    ChunkDesc cd = Core.descdb.getByName ((String)insert_type.getSelectedItem());
	    if (cd != null)
		Core.ui.loadDescHelp (cd.getToken());
	}
	else if (source == duplicate_button) {
	    tp = dbt.tree.getSelectionPaths();
	    if (tp == null)
		return;
	    for (i = 0; i < tp.length; i++) {
		ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
		if (ni.ch != null) {
		    // create a copy of this node...
		    ch = new ConfigChunk (ni.ch);
		    ch.name = "copy of " + ch.name;
		    chunkdb.addElement (ch);
		}
	    }
	}
	else if (source == load_button) {
	    if (chunkdb == null)
		s = FileControl.loadNewChunkDBFile ("", true);
	    else
		s = FileControl.loadNewChunkDBFile (chunkdb.file.getParent(), true);
	    if (s != null)
		selectDB(s);
	}
	else if (source == save_button) {
	    if (chunkdb != null)
		selectDB (FileControl.saveChunkDBFile(chunkdb));
	}
	else if (source == db_combobox) {
	    selectDB ((String)db_combobox.getSelectedItem());
	}
	else if (source == help1_mi || source == help2_mi) {
	    ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());
	    if (ni.isDescNode()) {
		ChunkDesc d = Core.descdb.getByName (ni.toString());
		if (d != null)
		    Core.ui.loadDescHelp (d.getToken());
	    }
	    else if (ni.isChunkNode()){
		Core.ui.loadDescHelp (ni.ch.desc.getToken());
	    }
	}
	else if (source == remove_mi) {
	    ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());
	    if (ni.isChunkNode()) {
		if (chunkdb == Core.active_chunkdb) {
		    ConfigChunkDB db = new ConfigChunkDB();
		    db.addElement (ni.ch);
		    Core.net.removeChunks (db);
		} 
		else
		    chunkdb.remove (ni.ch.getName());
	    }
	}
	else if (source == insert_mi) {
	    ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());
	    if (ni.isDescNode()) {
		ch = ChunkFactory.createChunkWithDescName (ni.toString());
		ch.name = (chunkdb.getNewName(ni.toString()));
		chunkdb.addElement (ch);
	    }
	    dbt.tree.expandPath(treeitem_menu_path);
	}
    }



    public void mouseClicked(MouseEvent e) {
	ConfigChunkFrame f;

	if (dbt == null)
	    return;

	int selRow = dbt.tree.getRowForLocation(e.getX(), e.getY());
	if (selRow == -1)
	    return;
	treeitem_menu_path = dbt.tree.getPathForLocation(e.getX(), e.getY());
	ChunkTreeNodeInfo ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());


	int mod = e.getModifiers();
	if (e.getClickCount() == 1) {
	    if (ni.ch != null) {
		String h = (ni.ch.desc.help.equals(""))?"No help available"
		    :ni.ch.desc.help;
		Core.consoleTempMessage (ni.ch.desc.getName(), h);
	    }
	    else if (ni.isDescNode()) {
		ChunkDesc d = Core.descdb.getByName (ni.toString());
		String h = (d.help.equals(""))?"No help available":d.help;
		Core.consoleTempMessage (d.getName(), h);
	    }
	}
	else if(e.getClickCount() == 2) {
	    openChunkFrame (ni.ch);
	}
    }



    // public because our child Chunkframes might wish to call it to
    // display their embedded chunks
    public void openChunkFrame (String name) {
	ConfigChunk ch = chunkdb.get (name);
	openChunkFrame (ch);
    }



    public void openChunkFrame (ConfigChunk ch) {
	if (ch == null)
	    return;
	ConfigChunkFrame f = (ConfigChunkFrame)Core.ui.getChildFrameMatching ("VjGUI.configchunk.ConfigChunkFrame", chunkdb, ch);
	if (f == null) {
	    f = new ConfigChunkFrame (Core.ui, ch, chunkdb);
	    Core.ui.addChildFrame (f);
	}
	else
	    f.show();
    }



    /******************* MouseListener Stuff ************************/

    public void mouseEntered(MouseEvent e) {}
    public void mouseReleased(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}

    public void mousePressed(MouseEvent e) {
	if (dbt == null)
	    return;

	int selRow = dbt.tree.getRowForLocation(e.getX(), e.getY());
	if (selRow == -1)
	    return;
	treeitem_menu_path = dbt.tree.getPathForLocation(e.getX(), e.getY());
	ChunkTreeNodeInfo ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());
	
	int mod = e.getModifiers();
	if ((mod == MouseEvent.BUTTON2_MASK) || (mod == MouseEvent.BUTTON3_MASK)) {
	    if (treeitem_menu_path != null) {
		DefaultMutableTreeNode n = (DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent();
		ChunkTreeNodeInfo info = (ChunkTreeNodeInfo)n.getUserObject();
		if (info.isDescNode())
		    desctreeitem_menu.show (dbt.tree, e.getX(), e.getY());
		if (info.isChunkNode())
		    chunktreeitem_menu.show (dbt.tree, e.getX(), e.getY());
	    }
	}
    }



    /****************** CoreDBListener stuff *********************/
    public void addChunkDB (CoreDBEvent e) {
	db_combobox.addItem (e.getChunkDB().getName());
    }

    public void removeChunkDB (CoreDBEvent e) {
	if (chunkdb == e.getChunkDB())
	    selectDB ("");
	db_combobox.removeItem (e.getChunkDB().getName());
    }

    public void addDescDB (CoreDBEvent e) {
	updateInsertTypes();
    }

    public void removeDescDB (CoreDBEvent e) {
	updateInsertTypes();
    }


    /*********************** PlugPanel Stuff ***************************/

    public void destroy () {
	Core.removeCoreDBListener (this);
    }


}




