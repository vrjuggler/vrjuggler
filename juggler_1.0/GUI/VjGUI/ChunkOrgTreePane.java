
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.border.*;
import javax.swing.event.*;
import VjGUI.*;


public class ChunkOrgTreePane extends JPanel 
    implements ActionListener, CellEditorListener {

    JButton   load_button;
    JButton   save_button;
    JButton   apply_button;

    JButton   remove_button;
    JButton   insertfolder_button;
    JButton   insertdescname_button;

    JScrollPane scroll_pane;
    JTree     tree;
    DefaultTreeModel treemodel;
    DefaultMutableTreeNode root;

    public class NodeInfo {
	String name;
	int type;  // 0 - label, 1 - chunkdesc name

	public NodeInfo( String _name, int _type) {
	    name = _name;
	    type = _type;
	}

	public String toString () {
	    return name;
	}
    }

    public ChunkOrgTreePane () {
        super();

        JPanel south_panel;
        JPanel side_panel;
        Box center_panel;

        setBorder (new EmptyBorder (5,5,5,5));
        setLayout (new BorderLayout (5, 5));

	add (new JLabel ("Warning: Unfinished panel. DO NOT USE!"), "North"); 

	root = new DefaultMutableTreeNode ("nothing", false);
	treemodel = new DefaultTreeModel(root, true);
	tree = new JTree (treemodel);

        center_panel = new Box (BoxLayout.Y_AXIS);
	scroll_pane = new JScrollPane (tree, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                                       JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	center_panel.add (scroll_pane);
        add (center_panel, "Center");

	Core.chunkorgtree.addActionListener (this);

	buildTree (Core.chunkorgtree);

	JTextField editfield = new JTextField();
	DefaultCellEditor editor = new DefaultCellEditor (editfield);
	tree.setEditable (true);
	tree.setCellEditor (editor);
	editor.addCellEditorListener (this);
    }

    public void buildTree (ChunkOrgTree orgtree) {
	root = buildTreeHelper (orgtree);
	treemodel.setRoot (root);
	treemodel.reload();
    }

    private DefaultMutableTreeNode buildTreeHelper (Object orgtree) {
	ChunkOrgTree ot;
	int i;
	DefaultMutableTreeNode n1, n2;
	if (orgtree instanceof String) {
	    n1 = new DefaultMutableTreeNode (new NodeInfo ((String)orgtree, 1), 
					     false);
	    return n1;
	}
	else if (orgtree instanceof ChunkOrgTree) {
	    ot = (ChunkOrgTree)orgtree;
	    n1 = new DefaultMutableTreeNode (new NodeInfo (ot.label, 0), 
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
    }


    public void actionPerformed (ActionEvent e) {
	if (e.getActionCommand().equalsIgnoreCase ("reload")) {
	    buildTree (Core.chunkorgtree);
	}
    }

    public void editingCanceled (ChangeEvent e) {
    }

    public void editingStopped (ChangeEvent e) {
	System.out.println ("Edit event :" + e + ", from " + e.getSource());
    }
}
