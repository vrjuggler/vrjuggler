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
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import java.io.File;

import VjControl.*;
import VjConfig.*;
import VjComponents.UI.*;
import VjComponents.UI.Widgets.*;
import VjComponents.ConfigEditor.ConfigCommunicator;
import VjComponents.ConfigEditor.ConfigModule;
import VjComponents.ConfigEditor.ConfigModuleEvent;
import VjComponents.ConfigEditor.ConfigModuleListener;
import VjComponents.ConfigEditor.ConfigUIHelper;

/** An individual subpanel of ConfigurePane.  
 *  ChunkDBPanel views and edits a single ChunkDB (at a time).  It has
 *  methods to copy chunks to another ChunkDBPanel.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class ChunkDBPanel 
    extends JPanel
    implements PlugPanel, 
               ActionListener, 
               MouseListener, 
               ConfigModuleListener { 


    protected ConfigModule config_module;
    protected ConfigUIHelper confighelper_module;
    protected ControlUIModule ui_module;
    protected String component_name;
    protected ConfigChunk component_chunk;

    protected SuffixFilter chunkdb_filter;

    protected int controls_on_side; // 0 for left, 1 for right;
    public static final int CONTROLS_ON_RIGHT = 0;
    public static final int CONTROLS_ON_LEFT = 1;

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

    protected boolean ui_initialized;

    public ChunkDBPanel () {
	super();

        component_name = "Unconfigured ChunkDBPanel";
        component_chunk = null;

        config_module = null;
        ui_module = null;
        confighelper_module = null;

        ui_initialized = false;

	current_font = null;
	chunkdb = null;
	dbt = null;
	sendacross_target = null;

	controls_on_side = CONTROLS_ON_RIGHT;
    }



    public void updateUI () {
        super.updateUI();
        if (ui_initialized) {
            if (dbt != null)
                dbt.updateUI();
        }
    }


    public void setControlsOnSide (int _controls_on_side) {
        controls_on_side = _controls_on_side;
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
        String[] names = config_module.getChunkDBNames();
        for (i = 0; i < names.length; i++)
            db_combobox.addItem (names[i]);
    }



    public void selectDB (String name) {
	/* changes the currently displayed db in the panel to name... */
	ConfigChunkDB newchunkdb = config_module.getChunkDB (name);
	if (newchunkdb == chunkdb)
	    return;
	chunkdb = newchunkdb;

	if (dbt != null) {
	    dbt.tree.removeMouseListener (this);
	    confighelper_module.releaseChunkDBTreeModel (dbt);
	}
	    
	dbt = confighelper_module.getChunkDBTreeModel (chunkdb);
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



    protected String getDBName() {
        return (chunkdb != null)? chunkdb.name: null;
    }



    // typically, one chunkdbpanel will call this on another to perform a 
    // sendacross action
    protected void addAll (ConfigChunkDB newdb) {
	if (chunkdb == null)
	    return;
        else
            config_module.addChunks (chunkdb, newdb);
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

        String[] desc_names = config_module.getDescNames();
        for (i = 0; i < desc_names.length; i++)
            insert_type.addItem (desc_names[i]);
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
		    if (ni.isChunkNode())
			v.add (ni.getChunk());
		}
	    }
	    if (sendacross_target != null)
		sendacross_target.addAll (v);
	}
	else if (source == send_all_button) {
	    if (sendacross_target != null)
		sendacross_target.addAll (chunkdb);
	}
	else if (source == new_button) {
	    selectDB (config_module.addChunkDB (new ConfigChunkDB()));
	}
	else if (source == close_button) {
	    config_module.closeChunkDB (chunkdb);
	}
	else if (source == checkdepend_button) {
	    java.util.List vec = chunkdb.getDependencies();
	    for (i = 0; i < config_module.chunkdbs.size(); i++) {
		ConfigChunkDB db = (ConfigChunkDB)config_module.chunkdbs.elementAt(i);
		if (db != chunkdb)
		    db.searchDependencies (vec);
	    }
            DependencyFrame dependency_frame = (DependencyFrame)ui_module.getChildFrameMatching ("VjComponents.ConfigEditor.DependencyFrame", chunkdb, null);
            if (dependency_frame != null)
                dependency_frame.refreshData (vec);
            else {
		dependency_frame = new DependencyFrame(this, chunkdb, vec);
                ui_module.addChildFrame (dependency_frame);
	    }
            dependency_frame.show();
	}
	else if (source == remove_button) {
	    tp = dbt.tree.getSelectionPaths();
	    if (tp == null)
		return;
            ConfigChunkDB db = new ConfigChunkDB ();
            for (i = 0; i < tp.length; i++) {
                ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
                if (ni.isChunkNode()) {
                    db.add (ni.getChunk());
                }
            }
            config_module.removeChunks (chunkdb, db);
	}
	else if (source == insert_button) {
	    ch = ChunkFactory.createChunkWithDescName ((String)insert_type.getSelectedItem());
	    if (ch != null) {
		ch.setName(chunkdb.getNewName(ch.getDescName()));
		chunkdb.add (ch);
	    }
	}
	else if (source == chunkhelp_button) {
            String token = config_module.getDescTokenFromName ((String)insert_type.getSelectedItem());
            confighelper_module.loadDescHelp (token);
	}
	else if (source == duplicate_button) {
	    tp = dbt.tree.getSelectionPaths();
	    if (tp == null)
		return;
	    for (i = 0; i < tp.length; i++) {
		ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)tp[i].getLastPathComponent()).getUserObject());
		if (ni.isChunkNode()) {
		    // create a copy of this node...
		    ch = new ConfigChunk (ni.getChunk());
		    ch.setName (chunkdb.getNewName("copy of " + ch.getName()));
		    chunkdb.add (ch);
		}
	    }
	}
	else if (source == load_button) {
            File default_dir = (chunkdb == null)?null:chunkdb.file.getParentFile();
            File f = ui_module.getEasyFileDialog().requestOpenFile (default_dir, ui_module, chunkdb_filter);
            if (f != null) {
                s = config_module.loadNewChunkDBFile(f);
                if (s != null)
                    selectDB(s);
            }
	}
	else if (source == save_button) {
	    if (chunkdb != null) {
                File f = chunkdb.file;
                f = ui_module.getEasyFileDialog().requestSaveFile (f, ui_module, chunkdb_filter);
                if (f != null) {
                    s = config_module.saveChunkDBFile (chunkdb, f);
                    selectDB(s);
                }
            }
	}
	else if (source == db_combobox) {
	    selectDB ((String)db_combobox.getSelectedItem());
	}
	else if (source == help1_mi || source == help2_mi) {
	    ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());
	    if (ni.isDescNode())
                confighelper_module.loadDescHelp (config_module.getDescTokenFromName (ni.toString()));
	    else if (ni.isChunkNode())
		confighelper_module.loadDescHelp (ni.getChunk().getDescToken());
	}
	else if (source == remove_mi) {
	    ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());
	    if (ni.isChunkNode()) {
                config_module.removeChunk (chunkdb, ni.getChunk());
            }
	}
	else if (source == insert_mi) {
	    ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());
	    if (ni.isDescNode()) {
		ch = ChunkFactory.createChunkWithDescName (ni.toString());
		ch.setName (chunkdb.getNewName(ni.toString()));
		chunkdb.add (ch);
	    }
	    dbt.tree.expandPath(treeitem_menu_path);
	}
    }



    public void mouseClicked(MouseEvent e) {
	if (dbt == null)
	    return;

	int selRow = dbt.tree.getRowForLocation(e.getX(), e.getY());
	if (selRow == -1)
	    return;
	treeitem_menu_path = dbt.tree.getPathForLocation(e.getX(), e.getY());
	ChunkTreeNodeInfo ni = ((ChunkTreeNodeInfo)((DefaultMutableTreeNode)treeitem_menu_path.getLastPathComponent()).getUserObject());


	int mod = e.getModifiers();
	if (e.getClickCount() == 1) {
	    if (ni.isChunkNode()) {
                ConfigChunk ch = ni.getChunk();
                String h = ch.getDescHelp();
                if (h.equals(""))
                    h = "No help available";
		Core.consoleTempMessage (ch.getDescName(), h);
	    }
	    else if (ni.isDescNode()) {
		ChunkDesc d = Core.descdb.getByName (ni.toString());
                String h = ((d == null) || d.help.equals(""))?"No help available":d.help;
                String n = (d == null)?"Undefined ChunkDesc":d.getName();
                Core.consoleTempMessage (n, h);
	    }
	}
	else if ((mod == MouseEvent.BUTTON1_MASK) &&
                 (e.getClickCount() == 2)) {
	    openChunkFrame (ni.getChunk());
	}
    }



    protected void openChunkFrame (String name) {
	ConfigChunk ch = chunkdb.get (name);
	openChunkFrame (ch);
    }



    protected void openChunkFrame (ConfigChunk ch) {
	if (ch != null)
            confighelper_module.openChunkFrame (chunkdb, ch);
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



    /****************** ConfigModuleListener stuff *********************/
    public void addChunkDB (ConfigModuleEvent e) {
	db_combobox.addItem (e.getChunkDB().getName());
    }

    public void removeChunkDB (ConfigModuleEvent e) {
	if (chunkdb == e.getChunkDB())
	    selectDB ("");
	db_combobox.removeItem (e.getChunkDB().getName());
    }

    public void addDescDB (ConfigModuleEvent e) {
	updateInsertTypes();
    }

    public void removeDescDB (ConfigModuleEvent e) {
	updateInsertTypes();
    }


    /*********************** PlugPanel Stuff ***************************/

    public String getComponentName () {
        return component_name;
    }


    public void setComponentName (String _name) {
        component_name = _name;
    }

    
    public ImageIcon getComponentIcon () {
        return null;
    }


    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
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
                c = Core.getVjComponent (s);
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
    }


    public void setConfigModule(ConfigModule m) {
        config_module = m;
    }
    public void setControlUIModule (ControlUIModule m) {
        ui_module = m;
    }
    public void setConfigUIHelper (ConfigUIHelper m) {
        confighelper_module = m;
    }


    public void initialize () throws VjComponentException {
        if (ui_module == null || config_module == null || confighelper_module == null)
            throw new VjComponentException (component_name + ": Initialize called with unmet dependences.");

        chunkdb_filter = new SuffixFilter ("Config Files (*.config, *.cfg)", ".config");
        chunkdb_filter.addSuffix(".cfg");
        chunkdb_filter = (SuffixFilter)ui_module.getEasyFileDialog().addFilter (chunkdb_filter, "ConfigChunkDB");

    }


    public ConfigChunk getConfiguration () {
        return component_chunk;
    }


    public VjComponent addConfig (ConfigChunk ch) throws VjComponentException {
        throw new VjComponentException (component_name + " does not support child component: " + ch.getName());
    }


    public boolean removeConfig (String s) {
        return false;
    }


    public JComponent getUIComponent () {
        return this;
    }


    public boolean initUIComponent () {
        if (!ui_initialized) {
            setBorder (BorderFactory.createEmptyBorder (5,5,5,5));
            setLayout (new BorderLayout (5, 5));

            // side panel - add/load etc. buttons

            load_button = new JButton ("Load", 
                                       ui_module.getIcon ("open file", 0));
            save_button = new JButton ("Save", 
                                       ui_module.getIcon ("save file", 0));
            new_button = new JButton ("New", 
                                      ui_module.getIcon ("new file", 0));
            close_button = new JButton ("Close", 
                                        ui_module.getIcon ("close file", 0));

            Insets ins = new Insets (0,0,0,0);
            load_button.setMargin (ins);
            save_button.setMargin (ins);
            new_button.setMargin (ins);
            close_button.setMargin (ins);

            JPanel side_panel = new JPanel();
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

            // south: insert types selector
            JPanel south_panel = new JPanel();
            south_panel.setLayout (new BoxLayout (south_panel, BoxLayout.X_AXIS));
            south_panel.add (new JLabel ("Insert type:"));
            insert_type = new JComboBox();
            south_panel.add (insert_type);
            chunkhelp_button = new JButton ("Help");
            chunkhelp_button.setMargin (new Insets (0,0,0,0));
            south_panel.add (chunkhelp_button);

            add (south_panel, "South");

            // center: scrolled pane w/ tree
            Box center_panel = new Box (BoxLayout.Y_AXIS);
            center_panel.add (db_combobox = new JComboBox());

            scroll_pane = new JScrollPane (JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
                                           JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

            center_panel.add (scroll_pane);
            add (center_panel, "Center");

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


            // set up initial tree viewing stuff. can't use selectDB() for this
            // because the chunkdb is already null and it decides not to do
            // anything.
            dbt = confighelper_module.getChunkDBTreeModel (chunkdb);
            scroll_pane.setViewportView (dbt.tree);
            dbt.tree.addMouseListener(this);
            db_combobox.setSelectedItem (dbt.getName());
            setButtonsEnabled (false);

            buildDBList();

            // listen for new/delete chunkdb events from Core
            config_module.addConfigModuleListener (this);

            // get initial set of chunkdesc names for the type selector box.
            updateInsertTypes();

            ui_initialized = true;
        }
        return ui_initialized;
    }


    public void destroy () {
        if (ui_initialized)
            config_module.removeConfigModuleListener (this);
    }


    public void rebuildDisplay () {
        if (ui_initialized) {
            String s = getDBName();
            selectDB (null);
            selectDB (s);
        }
    }
}




