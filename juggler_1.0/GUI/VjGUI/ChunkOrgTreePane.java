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

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.border.*;
import javax.swing.event.*;
import java.util.Enumeration;
import VjGUI.*;
import VjConfig.*;

/* there are some big issues with this piece of code.
 * handling of orgtrees is pathologically stupid in some ways 
 * (do we need to have the orgtree member at all?)
 */

public class ChunkOrgTreePane extends JPanel 
    implements ActionListener, CellEditorListener, MouseListener, CoreDBListener {

    private JButton   new_button;
    private JButton   load_button;
    private JButton   save_button;
    private JButton   apply_button;

    private JButton   remove_button;
    private JButton   insertfolder_button;
    private JButton   insertdescname_button;

    private JPopupMenu treeitem_menu, childlesstreeitem_menu;
    private TreePath treeitem_menu_path;
    private JMenuItem addfolder_mi, remove_mi, cremove_mi, unnameddesc_mi;
    private JMenu addchunkdesc_menu;

    ChunkOrgTree orgtree;

    JScrollPane scroll_pane;
    JTree     tree;
    DefaultTreeModel treemodel;
    DefaultMutableTreeNode root;
    //ChunkOrgTree orgtree;
    boolean ignore_next_reload; // used so we don't reload tree as a result
                                // of message sent because of an apply
                                // ie. we already knew that.


    public ChunkOrgTreePane () {
        super();

        JPanel south_panel;
        JPanel side_panel;
        Box center_panel;

	ignore_next_reload = false;

	// popup menu for tree
	treeitem_menu_path = null;
	treeitem_menu = new JPopupMenu();
	treeitem_menu.add (addfolder_mi = new JMenuItem ("Add Folder"));
	treeitem_menu.add (addchunkdesc_menu = new JMenu ("Add ChunkDesc"));
	treeitem_menu.add (remove_mi = new JMenuItem ("Remove"));
	treeitem_menu.addSeparator();

	childlesstreeitem_menu = new JPopupMenu();
	childlesstreeitem_menu.add (cremove_mi = new JMenuItem ("Remove"));
	childlesstreeitem_menu.addSeparator();

	addchunkdesc_menu.add (unnameddesc_mi = new JMenuItem ("Unnamed"));
	addchunkdesc_menu.addSeparator();

        setBorder (new EmptyBorder (5,5,5,5));
        setLayout (new BorderLayout (5, 5));

	//add (new JLabel ("Warning: Be careful where this saves to"), "North"); 

	root = new DefaultMutableTreeNode ("nothing", false);
	treemodel = new DefaultTreeModel(root, true);
	tree = new JTree (treemodel);

        center_panel = new Box (BoxLayout.Y_AXIS);
	scroll_pane = new JScrollPane (tree, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                                       JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	center_panel.add (scroll_pane);
        add (center_panel, "Center");

	Core.chunkorgtree.addActionListener (this);

	orgtree = new ChunkOrgTree();
	orgtree.copyValueFrom (Core.chunkorgtree);
	//orgtree.addActionListener (this);
	//orgtree = chunkorgtree;
	buildTree (Core.chunkorgtree);

	JTextField editfield = new JTextField();
	DefaultCellEditor editor = new DefaultCellEditor (editfield);
	tree.setEditable (true);
	tree.setCellEditor (editor);
	editor.addCellEditorListener (this);

	// Side Panel
	side_panel = new JPanel();
	side_panel.setLayout (new GridLayout (9, 1));

        ImageIcon new_icn, load_icn, save_icn, close_icn;
	
	load_button = new JButton ("Load", Core.load_icn);
	save_button = new JButton ("Save", Core.save_icn);
	new_button = new JButton ("New", Core.new_icn);
	//close_button = new JButton ("Close", Core.close_icn);

        Insets ins = new Insets (0,0,0,0);
        new_button.setMargin (ins);
        load_button.setMargin (ins);
        save_button.setMargin (ins);
        //close_button.setMargin (ins);
 
	apply_button = new JButton ("Apply");
	remove_button = new JButton ("Remove");

	side_panel.add (new JLabel ("File"));
	side_panel.add (new_button);
	side_panel.add (load_button);
	side_panel.add (save_button);
	side_panel.add (apply_button);
	side_panel.add (new JLabel ("Element"));
	side_panel.add (remove_button);
	add (side_panel, "East");

	new_button.addActionListener (this);
	load_button.addActionListener (this);
	//load_button.addActionListener (this);
	save_button.addActionListener (this);
	apply_button.addActionListener (this);
	remove_button.addActionListener (this);
	tree.addMouseListener (this);

	addfolder_mi.addActionListener (this);
	remove_mi.addActionListener (this);
	unnameddesc_mi.addActionListener (this);
	cremove_mi.addActionListener (this);

	Core.addCoreDBListener (this);
    }



    private void updateInsertTypes () {
	int i;
	ChunkDesc d;
	JMenuItem mi;
	addchunkdesc_menu.removeAll();
	addchunkdesc_menu.add (unnameddesc_mi);
	addchunkdesc_menu.addSeparator();
	for (i = 0; i < Core.descdb.size(); i++) {
	    d = Core.descdb.elementAt(i);
	    mi = new JMenuItem (d.getName());
	    mi.addActionListener (this);
	    addchunkdesc_menu.add (mi);
	}
    }

    public void buildTree (ChunkOrgTree orgtree) {
	root = buildTreeHelper (orgtree.root);
	treemodel.setRoot (root);
	treemodel.reload();
    }

    private DefaultMutableTreeNode buildTreeHelper (Object orgtree) {
	OrgTreeElem ot;
	int i;
	DefaultMutableTreeNode n1, n2;
	if (orgtree instanceof String) {
	    n1 = new DefaultMutableTreeNode ((String)orgtree, 
					     false);
	    return n1;
	}
	else if (orgtree instanceof OrgTreeElem) {
	    ot = (OrgTreeElem)orgtree;
	    n1 = new DefaultMutableTreeNode (ot.label, 
					     true);
	    for (i = 0; i < ot.children.size(); i++)
		n1.add (buildTreeHelper (ot.children.elementAt(i)));
	    return n1;
	}
	else
	    return null;
    }
	


    public void setCOTValue (ChunkOrgTree ot) {
	// replaces the old value of ot w/ the value defined by the panel's tree
	ignore_next_reload = true;
	ot.setRoot ((OrgTreeElem)setCOTValueHelper (root));
    }
   
    protected Object setCOTValueHelper (DefaultMutableTreeNode n) {
	DefaultMutableTreeNode n1, n2;
	OrgTreeElem ot;
	Enumeration e;
	if (n.getAllowsChildren() == false) {
	    return n.toString();
	}
	else {
	    ot = new OrgTreeElem(n.toString());
	    e = n.children();
	    while (e.hasMoreElements()) {
		ot.children.addElement (setCOTValueHelper ((DefaultMutableTreeNode)e.nextElement()));
	    }
	    return ot;
	}
    }



    public void actionPerformed (ActionEvent e) {
	ChunkOrgTree ot;
	Object source = e.getSource();
	if (e.getActionCommand().equalsIgnoreCase ("reload")) {
	    if (ignore_next_reload)
		ignore_next_reload = false;
	    else {
		orgtree.copyValueFrom (Core.chunkorgtree);
		buildTree (orgtree);
	    }
	}
	else if (source == new_button) {
	    //System.out.println ("pushed new");
	    orgtree.copyValueFrom (new ChunkOrgTree());
	    buildTree (orgtree);
	}
	else if (source == save_button) {
	    setCOTValue (orgtree);
	    //System.out.println ("saving chunkorgtree:\n" + ot);
	    FileControl.saveChunkOrgTree (orgtree);
	}
	else if (source == load_button) {
	    //System.out.println ("load pressed");
	    //ChunkOrgTree orgtree = new ChunkOrgTree();
	    FileControl.loadChunkOrgTree (orgtree.getFile().getParent(), true, orgtree);
	    buildTree (orgtree);
	}
	else if (source == apply_button) {
	    setCOTValue (Core.chunkorgtree);
	}
	else if (source == remove_button) {
	    //System.out.println ("Remove button pressed");
	    TreePath[] tp = tree.getSelectionPaths();
	    if (tp == null)
		return;
	    tree.removeSelectionPaths (tp);
	    DefaultMutableTreeNode n, n1;
	    for (int i = 0; i < tp.length; i++) {
		n = (DefaultMutableTreeNode)tp[i].getLastPathComponent();
		n1 = (DefaultMutableTreeNode)n.getParent();
		n.removeFromParent();
		treemodel.reload(n1);
	    }
	}
	else if ((source == remove_mi) || (source == cremove_mi)) {
	    DefaultMutableTreeNode n, n1;
	    n = (DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent();
	    n1 = (DefaultMutableTreeNode)n.getParent();
	    n.removeFromParent();
	    treemodel.reload(n1);
	}
	else if (source == addfolder_mi) {
	    DefaultMutableTreeNode n = (DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent();
	    if (n != null && n.getAllowsChildren()) {
		n.add (new DefaultMutableTreeNode ("New Folder", true));
		treemodel.reload(n);
	    }
	}
	else if (e.getSource() instanceof JMenuItem) {
	    /* well, it's got to be something in the chunkdesc menu */
	    DefaultMutableTreeNode n = (DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent();
	    if (n != null && n.getAllowsChildren()) {
		n.add (new DefaultMutableTreeNode (e.getActionCommand(), false));
		treemodel.reload(n);
	    }

	}
    }


    public void mouseClicked(MouseEvent e) {}
    public void mouseEntered(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}

    public void mousePressed(MouseEvent e) {
	int mod = e.getModifiers();
	if ((mod == MouseEvent.BUTTON2_MASK) || (mod == MouseEvent.BUTTON3_MASK)) {
	    //System.out.println ("RightButtonClick");
	    treeitem_menu_path = tree.getPathForLocation (e.getX(), e.getY());
	    //System.out.println (tp);
	    if (treeitem_menu_path != null) {
		DefaultMutableTreeNode n = (DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent();
		if (n.getAllowsChildren())
		    treeitem_menu.show (tree, e.getX(), e.getY());
		else
		    childlesstreeitem_menu.show (tree, e.getX(), e.getY());
	    }
	}
    }
    public void mouseReleased(MouseEvent e) {
    }
 

    public void editingCanceled (ChangeEvent e) {
    }

    public void editingStopped (ChangeEvent e) {
	//System.out.println ("Edit event :" + e + ", from " + e.getSource());
    }



    // CoreDBListener stuff

    public void addChunkDB (CoreDBEvent e) {
    }

    public void removeChunkDB (CoreDBEvent e) {
    }

    public void addDescDB (CoreDBEvent e) {
	updateInsertTypes();
    }

    public void removeDescDB (CoreDBEvent e) {
	updateInsertTypes();
    }
}








