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


package VjGUI;

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import java.util.Vector;

import VjGUI.*;
import VjGUI.util.JFrameParent;
import VjGUI.util.ChildFrame;
import VjGUI.util.PlugPanel;
import VjGUI.configchunk.ConfigChunkFrame;
import VjGUI.chunkdesc.ChunkDescFrame;
import VjConfig.*;


public class ControlUI  
    extends JFrame 
    implements ActionListener, WindowListener, JFrameParent, 
	       LogMessageListener, DescDBListener, ChunkDBListener, CoreDBListener {


    private JPanel      main_panel;

    JMenuBar    main_menubar;
    JMenu       file_menu, 
                options_menu, 
                help_menu, 
                looknfeel_menu, 
                preferences_menu, 
                helptech_menu,
                helpvjc_menu,
                helpbasic_menu,
                helpdesc_menu;
    JMenuItem   quit_mi, 
	        network_refresh_mi,
                tree_rebuild_kludge_mi,
                lnf_win_mi, 
                lnf_java_mi, 
                lnf_motif_mi, 
                lnf_mac_mi;
    JMenuItem   lnf_organic1_mi;
    JMenuItem   editprefs_mi, 
                saveprefs_mi;
    JMenuItem   helpindex_mi,
	        helpbugs_mi, 
                helpabout_mi, 
                helpstart_mi, 
                helpchunk_mi, 
                helpdesc_mi, 
                helporgtree_mi, 
                helpcmdline_mi, 
                helpdynamic_mi,
                descformat_mi, 
                chunkformat_mi, 
                orgtreeformat_mi, 
                helpprefs_mi;
    JMenuItem   hboverview_mi,
                hbchunks_mi,
                hbapps_mi;

    JLabel      status_label;

    Vector child_frames;

    JTabbedPane tabpane;


    public ControlUI () {
	super ("VR Juggler Control Program");

	JComponent p;  // temp panel pointer

	//help_frames = new Vector();
	child_frames = new Vector();

	main_panel = new JPanel();
	main_panel.setLayout (new BorderLayout (5,5));
	getContentPane().add (main_panel);

	tabpane = new JTabbedPane();
	main_panel.add(tabpane, "Center");

	status_label = new JLabel (" ");
	main_panel.add (status_label, "South");

	// ------------------- SUBPANELS ----------------------
	// eventually, this will need to know to load components from somewhere

        Vector panelclasses = new Vector();
        panelclasses.addElement ("VjGUI.ConfigurePane");
        panelclasses.addElement ("VjGUI.ConnectionPane");
        panelclasses.addElement ("VjGUI.DescDBPanel");
        panelclasses.addElement ("VjGUI.ChunkOrgTreePane");
        panelclasses.addElement ("VjGUI.ConsolePane");
        panelclasses.addElement ("VjPerf.PerfAnalyzerPanel");

        for (int i = 0; i < panelclasses.size(); i++) {

            try {
                p = (JComponent)Core.factory.createPlugPanel((String)panelclasses.elementAt(i));
                tabpane.add (p.getName(), p);
            }
            catch (Exception e) {
                Core.consoleErrorMessage ("VjControl", e.toString());
            }
        }


	// --------------------- MENUS -------------------------------------------

	main_menubar = new JMenuBar();

	main_menubar.add (file_menu = new JMenu("File"));
	file_menu.add (network_refresh_mi = new JMenuItem ("Refresh Active Config"));
        file_menu.add (tree_rebuild_kludge_mi = new JMenuItem ("Rebuild ChunkDB Trees"));
	file_menu.add (quit_mi = new JMenuItem ("Quit"));

	main_menubar.add (options_menu = new JMenu("Options"));
	//	options_menu.add (looknfeel_menu = new JMenu ("Look & Feel"));
	looknfeel_menu = new JMenu ("Look & Feel");
	looknfeel_menu.add (lnf_win_mi = new JMenuItem ("Windows"));
	looknfeel_menu.add (lnf_java_mi = new JMenuItem ("Java"));
	looknfeel_menu.add (lnf_motif_mi = new JMenuItem ("Motif"));
	looknfeel_menu.add (lnf_mac_mi = new JMenuItem ("Macintosh"));
	looknfeel_menu.add (lnf_organic1_mi = new JMenuItem ("organic 1"));

	options_menu.add (preferences_menu = new JMenu ("Preferences"));
	preferences_menu.add (editprefs_mi = new JMenuItem ("Edit Preferences"));
	preferences_menu.add (saveprefs_mi = new JMenuItem ("Save Preferences"));

	main_menubar.add (help_menu = new JMenu ("Help"));
	help_menu.add (helpindex_mi = new JMenuItem ("Help Index"));

        help_menu.add (helpbasic_menu = new JMenu ("Configuration Basics"));
        helpbasic_menu.add (hboverview_mi = new JMenuItem ("Configuration Files"));
        helpbasic_menu.add (hbchunks_mi = new JMenuItem ("ConfigChunks"));
        helpbasic_menu.add (hbapps_mi = new JMenuItem ("Configuring Applications"));

        help_menu.add (helpvjc_menu = new JMenu ("Using VjControl"));

	helpvjc_menu.add (helpabout_mi = new JMenuItem ("About VjControl"));
	helpvjc_menu.add (helpstart_mi = new JMenuItem ("Getting Started"));
	helpvjc_menu.add (helpcmdline_mi = new JMenuItem ("Command Line Arguments"));
	helpvjc_menu.add (helpchunk_mi = new JMenuItem ("Working with Config Files"));
	helpvjc_menu.add (helpdesc_mi = new JMenuItem ("Working with ChunkDesc Files"));
	helpvjc_menu.add (helporgtree_mi = new JMenuItem ("Working with ChunkOrgTrees"));
        helpvjc_menu.add (helpdynamic_mi = new JMenuItem ("Dynamic Reconfiguration"));
	helpvjc_menu.add (helpprefs_mi = new JMenuItem ("VjControl Preferences"));
	helpvjc_menu.add (helpbugs_mi = new JMenuItem ("Known Bugs"));

	help_menu.add (helptech_menu = new JMenu ("Technical Information"));
	helptech_menu.add (descformat_mi = new JMenuItem ("ChunkDesc File Format"));
	helptech_menu.add (chunkformat_mi = new JMenuItem ("Config File Format"));
	helptech_menu.add (orgtreeformat_mi = new JMenuItem ("ChunkOrgTree File Format"));
	help_menu.add (helpdesc_menu = new JMenu ("Chunk-Specific Help"));

	setJMenuBar (main_menubar);

	quit_mi.addActionListener (this);
	network_refresh_mi.addActionListener (this);
        tree_rebuild_kludge_mi.addActionListener (this);

	lnf_win_mi.addActionListener (this);
	lnf_java_mi.addActionListener (this);
	lnf_motif_mi.addActionListener (this);
	lnf_mac_mi.addActionListener (this);
	lnf_organic1_mi.addActionListener (this);
	editprefs_mi.addActionListener (this);
	saveprefs_mi.addActionListener (this);

	helpindex_mi.addActionListener (this);
	helpabout_mi.addActionListener (this);
	helpstart_mi.addActionListener (this);
	helpchunk_mi.addActionListener (this);
	helpdesc_mi.addActionListener (this);
	helporgtree_mi.addActionListener (this);
	helpprefs_mi.addActionListener (this);
	helpbugs_mi.addActionListener (this);
	helpcmdline_mi.addActionListener (this);
        helpdynamic_mi.addActionListener (this);

	descformat_mi.addActionListener (this);
	chunkformat_mi.addActionListener (this);
	orgtreeformat_mi.addActionListener (this);

        hboverview_mi.addActionListener (this);
        hbchunks_mi.addActionListener (this);
        hbapps_mi.addActionListener (this);

	// initial helpdesc menu stuff...
 	for (int i = 0; i < Core.descdb.size(); i++) {
	    JMenuItem newmenu;
 	    ChunkDesc d = (ChunkDesc)Core.descdb.elementAt(i);
 	    helpdesc_menu.add (newmenu = new JMenuItem (d.getName()));
 	    newmenu.addActionListener(this);
	}

	// --------------------- FINAL WIN STUFF ---------------------------------

	addWindowListener (this);
	setSize(750,550);

	//show();
	//configure_pane.setDividerLocation (0.5);

	Core.gui_chunkdb.addChunkDBListener (this);
	Core.addLogMessageListener (this);
	Core.descdb.addDescDBListener (this);
	Core.addCoreDBListener (this);
    }



    public void quit() {
        dispose();
        System.exit(0);
    }



    public void rebuildAllPanels() {
        PlugPanel p;
        for (int i = 0; i < tabpane.getTabCount(); i++) {
            p = (PlugPanel)tabpane.getComponentAt(i);
            p.rebuildDisplay();
        }
    }
    


    public void addChildFrame (ChildFrame f) {
	child_frames.addElement(f);
    }

    public void removeChildFrame (ChildFrame f) {
	child_frames.removeElement (f);
	f.destroy();
    }

    public void removeChildFramesMatching (String cl, Object db, Object o) {
	ChildFrame f;
	for (int i = 0; i < child_frames.size(); ) {
	    f = (ChildFrame)child_frames.elementAt(i);
	    if (f.matches (cl, db, o)) {
		child_frames.removeElementAt(i);
		f.destroy();
	    }
	    else
		i++;
	}
    }

    public ChildFrame getChildFrameMatching (String cl, Object db, Object o) {
	ChildFrame f;
	for (int i = 0; i < child_frames.size(); i++) {
	    f = (ChildFrame)child_frames.elementAt(i);
	    if (f.matches (cl, db, o))
		return f;
	}
	return null;
    }




    public void actionPerformed (ActionEvent e) {
	//JMenuItem quit_mi, lnf_win_mi, lnf_java_mi, lnf_motif_mi;
	Object o = e.getSource(); 
	if (o == quit_mi)
	    quit();
	else if (o == network_refresh_mi)
	    Core.net.getChunks();
        else if (o == tree_rebuild_kludge_mi) {
            rebuildAllPanels();
        }
	else if (o == lnf_win_mi)
	    setLookNFeel("windows");
	else if (o == lnf_java_mi)
	    setLookNFeel ("java");
	else if (o == lnf_motif_mi)
	    setLookNFeel ("motif");
	else if (o == lnf_mac_mi)
	    setLookNFeel ("mac");
	else if (o == lnf_organic1_mi) {
	    //totalSetFont ("", 20);
	    //javax.swing.plaf.organic.OrganicLookAndFeel.setCurrentTheme(new javax.swing.plaf.organic.OrganicDefaultTheme());
	    //setLookNFeel ("javax.swing.plaf.organic.OrganicLookAndFeel");
	    //javax.swing.plaf.metal.MetalLookAndFeel.setCurrentTheme(
	    //              new javax.swing.plaf.metal.ContrastMetalTheme());
	    //setLookNFeel ("javax.swing.plaf.metal.MetalLookAndFeel");

	}
	else if (o == editprefs_mi) {
	    Vector v = Core.gui_chunkdb.getOfDescToken ("vjcontrol");
	    if (v.size() >= 1) {
		ConfigChunkFrame f = (ConfigChunkFrame)getChildFrameMatching ("VjGUI.configchunk.ConfigChunkFrame", Core.gui_chunkdb, 
									      v.elementAt(0));
		if (f == null) {
		    f = new ConfigChunkFrame (this, 
					      (ConfigChunk)(v.elementAt(0)), 
					      Core.gui_chunkdb);
		    addChildFrame (f);
		}
		else
		    f.show();
	    }

	}
	else if (o == saveprefs_mi) {
	    //System.out.println ("need to add save prefs");
	    FileControl.saveChunkDBFile (Core.gui_chunkdb);
	}
	else if (o == helpindex_mi)
	    loadHelp ("VjFiles/VjControlIndex.html");
	else if (o == helpbugs_mi)
	    loadHelp ("VjFiles/bugs.html");
	else if (o == helpabout_mi)
	    loadHelp ("VjFiles/about.html");
	else if (o == helpcmdline_mi)
	    loadHelp ("VjFiles/cmdline.html");
	else if (o == helpstart_mi)
	    loadHelp ("VjFiles/GettingStarted.html");
	else if (o == helpprefs_mi)
	    loadHelp ("VjFiles/Preferences.html");
	else if (o == helpchunk_mi)
	    loadHelp ("VjFiles/ChunkDBPanel.html");
	else if (o == helpdesc_mi)
	    loadHelp ("VjFiles/DescDBPanel.html");
	else if (o == helporgtree_mi)
	    loadHelp ("VjFiles/OrgTreePanel.html");
        else if (o == helpdynamic_mi)
            loadHelp ("VjFiles/DynamicReconfiguration.html");
        else if (o == hboverview_mi)
            loadHelp ("VjFiles/cfgfiles_intro.html");
        else if (o == hbchunks_mi)
            loadHelp ("VjFiles/chunkprimer.html");
        else if (o == hbapps_mi)
            loadHelp ("VjFiles/configApps.html");
	else if (o == descformat_mi)
	    loadHelp ("VjFiles/ChunkDescFormat.html");
	else if (o == chunkformat_mi)
	    loadHelp ("VjFiles/ConfigChunkFormat.html");
	else if (o == orgtreeformat_mi)
	    loadHelp ("VjFiles/OrgTreeFormat.html");
	else if (o instanceof JMenuItem) {
	    // the default case - we'll assume the menuitem is a chunk-
	    // specific help request. it's name will be a chunkdesc name
	    String name = ((JMenuItem)o).getText();
	    ChunkDesc d = Core.descdb.getByName (name);
	    if (d != null)
		loadDescHelp (d.getToken());
	}
    }



    //: utility for loadDescHelp()
    private URL getFileURL (String fname) {
	URL url = null;
	try {
	    url = new URL ("file", "localhost", -1, fname);
	    // make sure it's really there.
	    if (url.openConnection().getContentLength() == -1)
		url = null;
	}
	catch (Exception e) {
	    url = null;
	}
	return url;
    }



    public void loadDescHelp (String s) {
        String url = "DescHelp/" + s + ".html";
	HTMLFrame help_frame = new HTMLFrame (this, "VjControl Help", null);
        help_frame.setURL (url);
        help_frame.setContentsURL (ClassLoader.getSystemResource ("VjFiles/VjControlIndex.html"));
	child_frames.addElement (help_frame);
	help_frame.show();
    }

	

    //: Creates an HTML frame displaying the named helpfile
    public void loadHelp (String s) {
	//System.out.println ("loadhelp: " + s);
	URL url = ClassLoader.getSystemResource (s);
	HTMLFrame help_frame = new HTMLFrame (this, "VjControl Help", url);
        help_frame.setContentsURL (ClassLoader.getSystemResource ("VjFiles/VjControlIndex.html"));
	child_frames.addElement (help_frame);
	help_frame.show();
    }
    


    /**
     * Callback when one of ControlUI's children is closed (this sucks).
     */
    public void closedChild (ChildFrame frame, boolean ok) {
	if (frame instanceof HTMLFrame) {
	    child_frames.removeElement (frame);
	    frame.destroy();
	}
	else if (frame instanceof ChunkDescFrame) {
	    ChunkDescFrame f = (ChunkDescFrame)frame;

	    if (ok) {
		ChunkDesc oldc = f.getOldValue();
		ChunkDesc newc = f.getNewValue();
		ChunkDescDB descdb = f.getDescDB();
		if (oldc != null) {
		    descdb.replace (oldc, newc);
		}
	    }
	    child_frames.removeElement(f);
	    f.destroy();
	}
	else if (frame instanceof ConfigChunkFrame) {
	    child_frames.removeElement(frame);
	    frame.destroy();
	}
    }


    /**
     * Callback when one of ControlUI's children is applied (this sucks)
     */
    public void applyChild (ChildFrame frame) {
	if (frame instanceof HTMLFrame) {
            ;
	}
	else if (frame instanceof ChunkDescFrame) {
	    ChunkDescFrame f = (ChunkDescFrame)frame;
            ChunkDesc oldc = f.getOldValue();
            ChunkDesc newc = f.getNewValue();
            ChunkDescDB descdb = f.getDescDB();
            if (oldc != null) {
                descdb.replace (oldc, newc);
            }
	}
	else if (frame instanceof ConfigChunkFrame) {
	    ConfigChunkFrame f = (ConfigChunkFrame)frame;
            ConfigChunk newc, oldc;
            oldc = f.getOldValue();
            newc = f.getNewValue();
            ConfigChunkDB chunkdb = f.getChunkDB();
            if (chunkdb == Core.active_chunkdb) {
                if (oldc.getName().equals (newc.getName()))
                    Core.net.sendChunk(newc);
                else {
                    Core.net.removeChunk (oldc);
                    Core.net.sendChunk (newc);
                }
            }
            else {
                chunkdb.replace (oldc, newc);
            }
	}
    }



    protected void setLookNFeel (String s) {
	String lnf = "";
	
	if (s.equalsIgnoreCase ("java"))
	    lnf = "javax.swing.plaf.metal.MetalLookAndFeel";
	else if (s.equalsIgnoreCase ("motif"))
	    lnf = "com.sun.java.swing.plaf.motif.MotifLookAndFeel";
	else if (s.equalsIgnoreCase ("windows"))
	    lnf = "com.sun.java.swing.plaf.windows.WindowsLookAndFeel";
	else if (s.equalsIgnoreCase ("mac"))
	    lnf = "javax.swing.plaf.mac.MacLookAndFeel";
	else return;

	//System.out.println ("setting look n feel to : " + lnf);
	
	try {
	    UIManager.setLookAndFeel (lnf);
	    SwingUtilities.updateComponentTreeUI (this);
	    for (int i = 0; i < child_frames.size(); i++) {
		((ChildFrame)child_frames.elementAt(i)).updateUI();
	    }
	}
	catch (Exception ex) {
	    Core.consoleErrorMessage ("GUI", "Couldn't change look and feel: " + ex);
	}
    }



    protected void totalSetFont (String name, int size) {
	//System.out.println (UIManager.getDefaults());
	//There ought to be a better way to do this...
	Font newfont, oldfont;
	int i;
	ChunkDBTreeModel dbt;
	String keys[] = {
	    "RadioButtonMenuItem.acceleratorFont",
	    "Menu.font",
	    "PopupMenu.font",
	    "ScrollPane.font",
	    "CheckBoxMenuItem.font",
	    "TextArea.font",
	    "TitledBorder.font",
	    "MenuItem.acceleratorFont",
	    "MenuBar.font",
	    "MenuItem.font",
	    "ComboBox.font",
	    "InternalFrame.font",
	    "Menu.acceleratorFont",
	    "TextPane.font",
	    "Label.font",
	    "ToolBar.font",
	    "RadioButton.font",
	    "ToolTip.font",
	    "RadioButtonMenuItem.font",
	    "Tree.font",
	    "DesktopIcon.font",
	    "Panel.font",
	    "OptionPane.font",
	    "InternalFrame.titleFont",
	    "TabbedPane.font",
	    "CheckBoxMenuItem.acceleratorFont",
	    "PasswordField.font",
	    "ColorChooser.font",
	    "Viewport.font",
	    "ProgressBar.font",
	    "List.font",
	    "CheckBox.font",
	    "Table.font",
	    "TextField.font",
	    "Button.font",
	    "ToggleButton.font",
	    "EditorPane.font",
	    "TableHeader.font",
	};

	oldfont = UIManager.getFont ("Button.font");
	if (name == null || name.equals (""))
	    name = oldfont.getFamily();
	if (size < 5)
	    size = oldfont.getSize();
	newfont = new Font (name, oldfont.getStyle(), size);
	for (i = 0; i < keys.length; i++) {
	    UIManager.put (keys[i], newfont);
	} 
	changeFont (this, newfont);
	changeMenuBarFont (main_menubar, newfont);

	validate();
    }



    protected void changeMenuBarFont (JMenuBar bar, Font font) {
	bar.setFont (font);
	int i, c;
	JMenu m;
	c = bar.getMenuCount();
	for (i = 0; i < c; i++) {
	    m = bar.getMenu (i);
	    changeMenuFont (m, font);
	}
    }



    protected void changeMenuFont (JMenu menu, Font font) {
	JMenuItem item;
	int i, c;
	menu.setFont (font);
	c = menu.getItemCount();
	for (i = 0; i < c; i++) {
	    item = menu.getItem (i);
	    if (item instanceof JMenu)
		changeMenuFont ((JMenu)item, font);
	    else
		item.setFont (font);
	}
    }


    public static void changeFont(Component cmp, Font font) {
	cmp.setFont(font);
	cmp.invalidate();
	if (cmp instanceof Container) {
	    Component[] kids = ((Container)cmp).getComponents();
	    for (int i = 0 ; i < kids.length ; i++) {
		changeFont(kids[i], font);
	    }
	}
	return;
    }



    protected void reconfigure (ConfigChunk ch) {
	// called whenever vjcontrol_preferences changes
	String looknfeel = null;
	int fontsize = -1;
	String fontname = "";

	try {
	    looknfeel = 
		ch.getPropertyFromToken ("looknfeel").getValue(0).getString();
	    fontsize = 
		ch.getPropertyFromToken("fontsize").getValue(0).getInt();
	    fontname = 
		ch.getPropertyFromToken("fontname").getValue(0).getString();
	}
	catch (Exception e) {
	    Core.consoleInfoMessage ("vjControl", "Old vjcontrol preferences file - please check preferences and re-save");
	}

	if (fontname != "" && fontsize != -1)
	    totalSetFont (fontname, fontsize);
	setLookNFeel (looknfeel);

    }


    /******************** WindowListener Stuff ***********************/
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
        quit();
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}


    /******************* LogMessageListener stuff *********************/
    public void logMessage (LogMessageEvent e) {
	String source = e.getSourceName();
	String s = e.getMessage();
	String message = "";

	if (source != null)
	    message = "(" + source + "): ";
	if (s != null)
	    message += s;

	switch (e.getStyle()) {
	case LogMessageEvent.TEMPORARY_MESSAGE:
	    status_label.setText (message);
	    status_label.setForeground (UIManager.getColor ("Label.foreground"));
	    //Core.ui.status_label.repaint((long)1);
	    paint (getGraphics());
	    break;
	case LogMessageEvent.PERMANENT_MESSAGE:
	    status_label.setText (message);
	    status_label.setForeground (UIManager.getColor ("Label.foreground"));
	    break;
	case LogMessageEvent.PERMANENT_ERROR:
	    status_label.setText ("Error " + message);
	    status_label.setForeground (Color.red);
	    break;
	}
    }


    /************************ DescDBListener stuff *************************/

    //: When the global descDB adds an item, add it to the chunk-specific-
    //+ help menu
    public void addDesc (DescDBEvent e) { 
	JMenuItem newmenu;
	ChunkDesc d = e.getNewDesc();
	helpdesc_menu.add (newmenu = new JMenuItem (d.getName()));
	newmenu.addActionListener(this);

    }

    public void removeDesc (DescDBEvent e) {}
    public void replaceDesc (DescDBEvent e) {}
    public void removeAllDescs (DescDBEvent e) {}


    /************************** ChunkDBListener stuff **********************/
    //: ControlUI only listens to ChunkDB events from the GUI chunkdb
    public void addChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeChunk (ChunkDBEvent e) {;}
    public void replaceChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeAllChunks (ChunkDBEvent e) {;}


    /************************ CoreDBListener Stuff ************************/
    // These are mostly in here so we can close child frames when an 
    // associated db is unloaded.
    public void addChunkDB (CoreDBEvent e) {
    }
    public void removeChunkDB (CoreDBEvent e) {
	removeChildFramesMatching (null, e.getChunkDB(), null);
    }
    public void addDescDB (CoreDBEvent e) {
    }
    public void removeDescDB (CoreDBEvent e) {
	removeChildFramesMatching (null, e.getDescDB(), null);
    }

}







