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

import javax.swing.*;
import javax.swing.tree.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import java.util.Enumeration;
import java.io.File;

import VjConfig.*;
import VjControl.*;
import VjComponents.ConfigEditor.*;
import VjComponents.UI.*;
import VjComponents.UI.Widgets.*;


/** PlugPanel for editing ChunkDescDB files.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class DescDBPanel 
    extends JPanel 
    implements PlugPanel, 
               ActionListener, 
               MouseListener, 
               ConfigModuleListener, 
               DescDBListener {

    protected ConfigModule config_module;
    protected ControlUIModule ui_module;
    protected ConfigUIHelper confighelper_module;
    protected String component_name;
    protected ConfigChunk component_chunk;

    protected SuffixFilter descdb_filter;

    protected JTree tree;
    protected ChunkDescDB currdb;
    protected DefaultTreeModel model;
    protected JButton new_button;
    protected JButton load_button;
    protected JButton save_button;
    protected JButton close_button;
    protected JButton insert_button;
    protected JButton remove_button;
    protected JComboBox db_combobox;
    protected JScrollPane scroll_pane;

    protected boolean ui_initialized;


    public DescDBPanel () {
	super();

        component_name = "Unconfigured DescDBPanel";
        component_chunk = null;

        ui_initialized = false;

	currdb = null;
        model = null;

        config_module = null;
        ui_module = null;
        confighelper_module = null;
    }



    protected void buildDBList () {
	int i;
	db_combobox.addItem ("No Selection");
        String[] descdb_names = config_module.getDescDBNames();
        for (i = 0; i < descdb_names.length; i++)
            db_combobox.addItem (descdb_names[i]);
    }



    public void selectDB (String name) {
	/* changes the currently displayed db in the panel to name... */
	ChunkDesc d;
	DefaultMutableTreeNode root;

	//System.out.println ("SelectDB: " + name);

	if (name == null || name.equals (""))
	    name = "No Selection";
	ChunkDescDB db = config_module.getChunkDescDB (name);
	if (db == null)
	    name = "No Selection";
	if (name.equalsIgnoreCase ("No Selection")) {
            model = new DefaultTreeModel (new DefaultMutableTreeNode ("no selection"));
	    tree.setModel (model);
	    if (currdb != null)
		currdb.removeDescDBListener(this);
	    currdb = null;
            setButtonsEnabled (false);
	}
	else {
	    if (currdb != null)
		currdb.removeDescDBListener(this);
	    currdb = db;
	    currdb.addDescDBListener(this);
	    root = new DefaultMutableTreeNode ("root");
            int i, n = currdb.size();
	    for (i = 0; i < n; i++) {
		d = currdb.get(i);
		root.add (new DefaultMutableTreeNode (d.name));
	    }
	    model = new DefaultTreeModel (root);
	    tree.setModel (model);
	    tree.setRootVisible (false);
	    db_combobox.setSelectedItem (currdb.name);
            setButtonsEnabled (true);
	}
    }



    protected void setButtonsEnabled (boolean b) {
	save_button.setEnabled(b);
	close_button.setEnabled(b);
	insert_button.setEnabled(b);
	remove_button.setEnabled(b);
    }


    //----------------------- Action Listener stuff --------------------------

    public void actionPerformed (ActionEvent e) {
	int i,j;
	String s;
	DefaultMutableTreeNode n;
	DefaultTreeModel model;
	TreePath[] tp;
	ChunkDesc d;

	Object source = e.getSource();
    
	if (source == remove_button) {
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
	    d = new ChunkDesc();
	    for (j = 0; j <50; j++) {
		if ((currdb.getByName("unnamed"+j) == null) &&
		    (currdb.getByToken("unnamed"+j) == null))
		    break;
	    }
	    d.name = "unnamed" + j;
	    d.token = "unnamed" + j;
	    currdb.add (d);
	}
	else if (source == load_button) {
            File current_dir = (currdb == null)?null:currdb.getFile().getParentFile();
            File f = ui_module.getEasyFileDialog().requestOpenFile (current_dir, ui_module, descdb_filter);
            if (f != null) {
                s = config_module.loadNewDescDBFile(f);
                if (s != null)
                    selectDB(s);
            }
	}
	else if (source == new_button) {
	    ChunkDescDB descdb = new ChunkDescDB();
	    selectDB (config_module.addDescDB (descdb));
	}
	else if (source == close_button) {
	    config_module.closeDescDB (currdb);
	}
	else if (source == save_button) {
	    if (currdb != null) {
                File f = currdb.file;
                f = ui_module.getEasyFileDialog().requestSaveFile (f, ui_module, descdb_filter);
                if (f != null) {
                    s = config_module.saveDescDBFile (currdb, f);
                    selectDB(s);
                }
            }

            //	    selectDB (config_module.saveDescDBFile(currdb));
	}
	else if (source == db_combobox) {
	    selectDB ((String)db_combobox.getSelectedItem());
	}
    }


    //----------------------- Mouse Listener Stuff --------------------------

    public void mouseClicked(MouseEvent e) {
	DefaultMutableTreeNode n;
	String selected;

	if ((e.getModifiers() == MouseEvent.BUTTON1_MASK) && 
            (e.getClickCount() == 2)) {
	    TreePath selPath = tree.getPathForLocation(e.getX(), e.getY());
	    if(selPath != null) {
		n = (DefaultMutableTreeNode)selPath.getLastPathComponent();
		if (model.getRoot() == n)
		    return; // it was the root node...
		selected = (String)n.getUserObject();
		if (selected != null) {
                    confighelper_module.openDescFrame (currdb, currdb.getByName (selected), true);
		}
	    }
	}
    }


    public void mouseEntered(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}
    public void mousePressed(MouseEvent e) {}
    public void mouseReleased(MouseEvent e) {}



    //---------------------- ConfigModuleListener Stuff ----------------------

    public void addDescDB (ConfigModuleEvent e) {
        // add new DescDB to our drop-down list.
	db_combobox.addItem (e.getDescDB().getName());
    }

    public void removeDescDB (ConfigModuleEvent e) {
        // remove removed descdb from our drop-down list.
	if (currdb == e.getDescDB())
	    selectDB ("No Selection");
	db_combobox.removeItem (e.getDescDB().getName());
    }

    public void addChunkDB (ConfigModuleEvent e) {
    }

    public void removeChunkDB (ConfigModuleEvent e) {
    }



    //---------------------- DescDBListener Stuff ----------------------------


    public void addDesc (DescDBEvent e) {
	// need to update the tree model too <sigh>
	DefaultMutableTreeNode root = (DefaultMutableTreeNode)tree.getModel().getRoot();
	DefaultMutableTreeNode n = new DefaultMutableTreeNode(e.getNewDesc().name);
	root.add(n);
	model.reload(root);
    }	



    public void removeDesc (DescDBEvent e) {
	DefaultMutableTreeNode n;
	DefaultMutableTreeNode root = (DefaultMutableTreeNode)tree.getModel().getRoot();
	Enumeration en = root.children();
	while (en.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)en.nextElement();
	    if (e.getOldDesc().name.equals((String)n.getUserObject())) {
		root.remove(n);
		model.reload(root);
		break;
	    }
	}
    }



    public void replaceDesc (DescDBEvent e) {
        System.out.println ("replacedesc event received");
	DefaultMutableTreeNode n;
	DefaultMutableTreeNode root = (DefaultMutableTreeNode)tree.getModel().getRoot();
	Enumeration en = root.children();
	while (en.hasMoreElements()) {
	    n = (DefaultMutableTreeNode)en.nextElement();
	    if (e.getOldDesc().name.equals((String)n.getUserObject())) {
	        n.setUserObject(e.getNewDesc().name);
		model.reload(n);
		return;
	    }
	}
        Core.consoleErrorMessage (component_name, "replaceDesc: old desc wasn't found in DescDB display");
    }



    public void removeAllDescs (DescDBEvent e) {
	DefaultMutableTreeNode root = (DefaultMutableTreeNode)tree.getModel().getRoot();
	root.removeAllChildren();
	model.reload(root);
    }



    //----------------------- PlugPanel Stuff --------------------------------

    public String getComponentName () {
        return component_name;
    }


    public ImageIcon getComponentIcon () {
        return null;
    }


    public boolean configure (ConfigChunk ch) {
        component_chunk = ch;
        component_name = ch.getName();

        // get pointers to the modules we need.
        Property p = ch.getPropertyFromToken ("Dependencies");
        if (p != null) {
            int i;
            int n = p.getNum();
            String s;
            VjComponent c;
            for (i = 0; i < n; i++) {
                s = p.getValue(i).toString();
                c = Core.getComponentFromRegistry(s);
                if (c != null) {
                    if (c instanceof ControlUIModule)
                        ui_module = (ControlUIModule)c;
                    if (c instanceof ConfigModule)
                        config_module = (ConfigModule)c;
                    if (c instanceof ConfigUIHelper)
                        confighelper_module = (ConfigUIHelper)c;
                }
            }
        }
        if (ui_module == null)
            ui_module = (ControlUIModule)Core.getComponentFromRegistry ("ControlUI Module");
        if (config_module == null)
            config_module = (ConfigModule)Core.getComponentFromRegistry ("Config Module");
        if (confighelper_module == null)
            confighelper_module = (ConfigUIHelper)Core.getComponentFromRegistry ("ConfigUIHelper Module");
        if ((ui_module == null) || (config_module == null) || (confighelper_module == null)) {
            Core.consoleErrorMessage (component_name, "Instantiated with unmet VjComponent Dependencies. Fatal Configuration Error!");
            return false;
        }


        descdb_filter = new SuffixFilter("Description Files (*.desc, *.dsc)", ".desc");
        descdb_filter.addSuffix(".dsc");
        descdb_filter = (SuffixFilter)ui_module.getEasyFileDialog().addFilter (descdb_filter, "ChunkDescDB");

        return true;
    }

    
    public ConfigChunk getConfiguration () {
        return component_chunk;
    }


    public boolean addConfig (ConfigChunk ch) {
        return false;
    }


    public boolean removeConfig (String s) {
        return false;
    }


    public JComponent getUIComponent () {
        return this;
    }


    public boolean initUIComponent() {
        if (!ui_initialized) {
            JPanel south_panel, side_panel; Box center_panel;

            setBorder (BorderFactory.createEmptyBorder (5,5,5,5));
            setLayout (new BorderLayout (5, 5));

            // create buttons
            load_button = new JButton ("Load",
                                       ui_module.getIcon ("open file", 0));
            save_button = new JButton ("Save",
                                       ui_module.getIcon ("save file", 0));
            new_button = new JButton ("New",
                                      ui_module.getIcon ("new file", 0));
            close_button = new JButton ("Close",
                                        ui_module.getIcon ("close file", 0));
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
            //	buildDBList();
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
	
            //can't use selectDB yet...
            model = new DefaultTreeModel (new DefaultMutableTreeNode ("no selection"));
            tree.setModel (model);

            config_module.addConfigModuleListener (this);

            buildDBList();

            ui_initialized = true;
        }
        return ui_initialized;
    }


    public void destroy () {
        if (ui_initialized) {
            config_module.removeConfigModuleListener (this);
            if (currdb != null)
                currdb.removeDescDBListener (this);
        }
    }


    public void rebuildDisplay () {
    }


}




