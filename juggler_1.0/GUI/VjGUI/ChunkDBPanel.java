package VjGUI;

import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;

import VjConfig.*;
import VjGUI.*;
import VjGUI.configchunk.ConfigChunkFrame;

public class ChunkDBPanel extends JPanel 
  implements ActionListener, MouseListener, 
	     ConfigChunkFrame.ConfigChunkFrameParent {


    int controls_on_side; // 0 for left, 1 for right;
    ChunkDBTreeModel current_treemodel, empty_treemodel;
    Vector chunk_frames;


    JButton load_button;
    JButton save_button;
    JButton insert_button;
    JButton remove_button;
    JButton send_button;
    JButton send_all_button;
    JButton new_button;
    JButton close_button;
    JButton duplicate_button;
    JComboBox db_combobox;
    JComboBox insert_type;
    JScrollPane scroll_pane;



    public ChunkDBPanel (int _controls_on_side) {
	super();

	JPanel south_panel;
	JPanel side_panel; 
	Box center_panel;



	chunk_frames = new Vector();
	current_treemodel = null;
	empty_treemodel = new ChunkDBTreeModel (null, Core.chunkorgtree);

	controls_on_side = _controls_on_side;
	setBorder (new EmptyBorder (5,5,5,5));
	setLayout (new BorderLayout (5, 5));

	// south panel - add/load etc. buttons
 	south_panel = new JPanel();
 	add (south_panel, "South");
	south_panel.setLayout (new BoxLayout (south_panel, BoxLayout.X_AXIS));

	ImageIcon new_icn, load_icn, save_icn, close_icn;

	try {
	    new_icn = new ImageIcon (ClassLoader.getSystemResource ("VjFiles/new.gif"))
;
	    load_icn = new ImageIcon (ClassLoader.getSystemResource ("VjFiles/open.gif"));
	    save_icn = new ImageIcon (ClassLoader.getSystemResource ("VjFiles/save.gif"));
	    close_icn = new ImageIcon (ClassLoader.getSystemResource ("VjFiles/close.gif"));
	    load_button = new JButton ("Load", load_icn);
	    save_button = new JButton ("Save", save_icn);
	    new_button = new JButton ("New", new_icn);
	    close_button = new JButton ("Close", close_icn);
	}
	catch (NullPointerException e) {
	    load_button = new JButton ("Load");
	    save_button = new JButton ("Save");
	    new_button = new JButton ("New");
	    close_button = new JButton ("Close");
	}
	Insets ins = new Insets (0,0,0,0);
	new_button.setMargin (ins);
	load_button.setMargin (ins);
	save_button.setMargin (ins);
	close_button.setMargin (ins);

	side_panel = new JPanel();
	side_panel.setLayout (new GridLayout (11, 1));

	side_panel.add (new JLabel ("File"));

	side_panel.add (new_button);
	side_panel.add (load_button);
	side_panel.add (save_button);
	side_panel.add (close_button);

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

	south_panel.add (new JLabel ("Insert type:"));
	south_panel.add (insert_type = new JComboBox ());

	// center: scrolled pane w/ tree
	center_panel = new Box (BoxLayout.Y_AXIS);
	center_panel.add (db_combobox = new JComboBox());
	buildDBList();
	scroll_pane = new JScrollPane (JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				       JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	center_panel.add (scroll_pane);
	add (center_panel, "Center");

	// setup event handling
	db_combobox.addActionListener (this);
	load_button.addActionListener (this);
	save_button.addActionListener (this);
	insert_button.addActionListener (this);
	duplicate_button.addActionListener (this);
	remove_button.addActionListener (this);
	send_button.addActionListener (this);
	send_all_button.addActionListener (this);
	new_button.addActionListener (this);
	close_button.addActionListener (this);

	new_button.setToolTipText ("Create a new Config file");
	load_button.setToolTipText ("Load another Config file");
	save_button.setToolTipText ("Save this Config file");
	close_button.setToolTipText ("Close this file (doesn't save)");
	send_button.setToolTipText ("Copy selected chunks to the other panel");
	send_all_button.setToolTipText ("Copy all chunks over to the other panel");
	insert_button.setToolTipText ("Inserts new chunk (select type below)");
	
    }



    public void buildDBList () {
	int i;
	db_combobox.addItem ("No Selection");
	for (i = 0; i < Core.chunkdbs.size(); i++) {
	    db_combobox.addItem (((ChunkDBTreeModel)Core.chunkdbs.elementAt(i)).getName());
	}
    }



    public void removeChunkDBTree (ChunkDBTreeModel dbt) {
	if (dbt == current_treemodel)
	    selectDB ("");
	db_combobox.removeItem (dbt.getName());
    }



    public void addChunkDBTree (ChunkDBTreeModel dbt) {
	db_combobox.addItem (dbt.getName());
    }



    public void selectDB (String name) {
	/* changes the currently displayed db in the panel to name... */
	//System.out.println ("SelectDB: " + name);
	ChunkDBTreeModel dbt;
	dbt = Core.getChunkDBTree (name);
	if (dbt == null || name.equals("") || name.equalsIgnoreCase ("No Selection")) {
	    dbt = empty_treemodel;
	    name = "No Selection";
	}
	if (dbt == current_treemodel)
	    return;
	if (!dbt.inuse) {
	    destroyChunkFrames();
	    dbt.inuse = true;
	    if (current_treemodel != null) {
		current_treemodel.tree.removeMouseListener(this);
		current_treemodel.inuse = false;   // possible sync bug?
	    }
	    current_treemodel = dbt;
	    db_combobox.setSelectedItem (dbt.getName());
	    scroll_pane.setViewportView(dbt.tree);
	    dbt.tree.addMouseListener(this);
	    validate();
	}
	else {
	    db_combobox.setSelectedItem (current_treemodel.getName());
	}
    }



    public void addChunks (ConfigChunkDB newdb) {
	for (int i = 0; i < newdb.size(); i++)
	    current_treemodel.insertNode ((ConfigChunk)newdb.elementAt(i));
    }

	

    public void updateInsertTypes () {
	int i, j;
	if (insert_type == null)
	    return;
	// as of swing 1.2, there seems to be a bug in removeAllItems...
	//insert_type.removeAllItems();
	j = insert_type.getItemCount();
	for (i = 0; i < j; i++)
	    insert_type.removeItemAt (0);
	for (i = 0; i < Core.descdb.size(); i++) {
	    insert_type.addItem (((ChunkDesc)Core.descdb.elementAt(i)).name);
	}
    }



    public void actionPerformed (ActionEvent e) {
	int i,j;
	String s;
	ChunkTreeNodeInfo nodeinfo;
	TreePath[] tp;
	ConfigChunkDB v;
	Object source = e.getSource();
	ConfigChunk ch;

	if (source == send_button) {
	    tp = current_treemodel.tree.getSelectionPaths();
	    if (tp == null)
		return;
	    v = new ConfigChunkDB(Core.descdb);
	    if (tp != null) {
		for (i = 0; i < tp.length; i++) {
		    nodeinfo = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
		    if (nodeinfo.ch != null)
			v.addElement (nodeinfo.ch);
		}
	    }
	    Core.ui.configure_pane.sendAcross (v, this);
	    //System.out.println ("Chunks to send:\n" + v.toString());
	}
	else if (source == send_all_button) {
	    Core.ui.configure_pane.sendAcross (current_treemodel.chunkdb, this);
	}
	else if (source == new_button) {
	    selectDB (Core.addChunkDB (new ConfigChunkDB (Core.descdb)));
	    // 	String name = Core.addNewChunkDB ();
	    // 	selectDB (name);
	}
	else if (source == close_button) {
	    Core.closeChunkDB (current_treemodel);
	}
	else if (source == remove_button) {
	    System.out.println ("Remove button pressed");
	    tp = current_treemodel.tree.getSelectionPaths();
	    if (tp == null)
		return;
	    for (i = 0; i < tp.length; i++) {
		nodeinfo = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
		if (nodeinfo.ch != null) {
		    System.out.println ("removing node " + nodeinfo.ch.getName());
		    current_treemodel.removeNode(nodeinfo.ch.getName());
		}
	    }
	}
	else if (source == insert_button) {
	    ChunkDesc cd;
	    System.out.println ("Insert button pressed");
	    cd = Core.descdb.getByName ((String)insert_type.getSelectedItem());
	    if (cd != null) {
		ch = new ConfigChunk (cd);
		ch.name = (current_treemodel.chunkdb.getNewName(cd.name));
		current_treemodel.insertNode (ch);
	    }
	}
	else if (source == duplicate_button) {
	    tp = current_treemodel.tree.getSelectionPaths();
	    if (tp == null)
		return;
	    for (i = 0; i < tp.length; i++) {
		nodeinfo = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
		if (nodeinfo.ch != null) {
		    // create a copy of this node...
		    System.out.println ("not implemented");
		    ch = nodeinfo.ch.copy();
		    ch.name = "copy of " + ch.name;
		    System.out.println ("doing insert of\n" + ch);
		    current_treemodel.insertNode (ch);
		}
		else
		    System.out.println ("nothin to do for node " + nodeinfo);
	    }
	}
	else if (source == load_button) {
	    if (current_treemodel == empty_treemodel)
		s = FileControl.loadNewChunkDBFile ("", true);
	    else
		s = FileControl.loadNewChunkDBFile (current_treemodel.chunkdb.file.getParent(), true);
	    if (s != null)
		selectDB(s);
	}
	else if (source == save_button) {
	    if (current_treemodel != empty_treemodel)
		selectDB (FileControl.saveChunkDBFile(current_treemodel.chunkdb));
	}
	else if (source == db_combobox) {
	    selectDB ((String)db_combobox.getSelectedItem());
	}
    }



  public void mouseClicked(MouseEvent e) {
    ConfigChunkFrame f;

    if(e.getClickCount() == 2) {
      int selRow = current_treemodel.tree.getRowForLocation(e.getX(), e.getY());
      TreePath selPath = current_treemodel.tree.getPathForLocation(e.getX(), e.getY());
      if(selRow != -1) {
	  //System.out.println ("Double click on path: " + selPath);
	ChunkTreeNodeInfo ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)selPath.getLastPathComponent()).getUserObject());
	
	if (ni.ch != null) {
	  f = getChunkFrame (ni.ch.getName());
	  if (f == null) {
	    f = new ConfigChunkFrame (this, ni.ch);
	    chunk_frames.addElement(f);
	  }
	  else
	    f.show();
	}
      }
    }
  }

  public ConfigChunkFrame getChunkFrame (String name) {
    int i;
    ConfigChunkFrame f;
    for (i = 0; i < chunk_frames.size(); i++) {
      f = (ConfigChunkFrame)chunk_frames.elementAt(i);
      if (f.getOldValue().getName().equalsIgnoreCase (name))
	return f;
    }
    return null;
  }


  public void destroyChunkFrames () {
    int i;
    ConfigChunkFrame f;
    for (i = 0; i < chunk_frames.size(); i++) {
      f = (ConfigChunkFrame)chunk_frames.elementAt(i);
      f.setVisible(false);
      f.dispose();
    }
    chunk_frames.removeAllElements();
  }


  public void mouseEntered(MouseEvent e) {}
  public void mouseExited(MouseEvent e) {}
  public void mousePressed(MouseEvent e) {}
  public void mouseReleased(MouseEvent e) {}



  public void closedChunkFrame (ConfigChunkFrame f, boolean ok) {
    if (ok) {
      ConfigChunk newc, oldc;
      oldc = f.getOldValue();
      newc = f.getValue();
      current_treemodel.replaceNode (oldc, newc);
      if (current_treemodel == Core.active_treemodel) {
	System.out.println ("ought to send chunk across network...");
	//VjControl.net.sendChunk(newc);
      }
    }
    chunk_frames.removeElement(f);
  }

}




