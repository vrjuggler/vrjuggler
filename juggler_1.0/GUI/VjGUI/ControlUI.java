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
import VjGUI.configchunk.ConfigChunkFrame;
import VjConfig.*;
import VjPerf.*;

// BUG: this code doesn't now deal with adding to the helpdesc_menu... code
// such as this for when we add to the global DescDB...
//  	for (i = 0; i < db.size(); i++) {
//  	    d = (ChunkDesc)db.elementAt(i);
//  	    helpdesc_menu.add (newmenu = new JMenuItem (d.getName()));
//  	    newmenu.addActionListener(this);


public class ControlUI  extends JFrame 
    implements ActionListener, WindowListener, JFrameParent, LogMessageListener {


    JPanel      main_panel;
    JComponent configure_pane;
    ConnectionPane connection_pane;
    JComponent descdb_pane;
    JComponent console_pane;
    JComponent orgtree_pane;
    PerfAnalyzerPanel perfanalyze_pane;

    JMenuBar    main_menubar;
    JMenu       file_menu, 
                options_menu, 
                help_menu, 
                looknfeel_menu, 
                preferences_menu, 
                helptech_menu, 
                helpdesc_menu;
    JMenuItem   quit_mi, 
	        network_refresh_mi,
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
                descformat_mi, 
                chunkformat_mi, 
                orgtreeformat_mi, 
                helpprefs_mi;

    JLabel      status_label;

    ConfigChunkFrame prefs_frame;

    Vector help_frames;


    public ControlUI () {
	super ("VR Juggler Control Program");

	help_frames = new Vector();
	prefs_frame = null;

	JTabbedPane tabpane;
	main_panel = new JPanel();
	main_panel.setLayout (new BorderLayout (5,5));
	getContentPane().add (main_panel);

	tabpane = new JTabbedPane();
	main_panel.add(tabpane, "Center");

	status_label = new JLabel (" ");
	main_panel.add (status_label, "South");

	configure_pane = new ConfigurePane();
	tabpane.add ("Configure", configure_pane);

	connection_pane = new ConnectionPane();
	tabpane.add ("Connection", connection_pane);
	
	descdb_pane = new DescDBPanel();
	tabpane.add ("Descriptions", descdb_pane);
	
	orgtree_pane = new ChunkOrgTreePane();
	tabpane.add ("Org Tree", orgtree_pane);

	console_pane = new ConsolePane();
	tabpane.add ("Messages", console_pane);

	perfanalyze_pane = new PerfAnalyzerPanel(Core.perf_collection);
	tabpane.add ("Performance", perfanalyze_pane);

	// --------------------- MENUS -------------------------------------------

	main_menubar = new JMenuBar();

	main_menubar.add (file_menu = new JMenu("File"));
	file_menu.add (network_refresh_mi = new JMenuItem ("Refresh Active Config"));
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
	help_menu.add (helpabout_mi = new JMenuItem ("About VjControl"));
	help_menu.add (helpstart_mi = new JMenuItem ("Getting Started"));
	help_menu.add (helpcmdline_mi = new JMenuItem ("Command Line Arguments"));
	help_menu.add (helpchunk_mi = new JMenuItem ("Working with Config Files"));
	help_menu.add (helpdesc_mi = new JMenuItem ("Working with ChunkDesc Files"));
	help_menu.add (helporgtree_mi = new JMenuItem ("Working with ChunkOrgTrees"));
	help_menu.add (helpprefs_mi = new JMenuItem ("VjControl Preferences"));
	help_menu.add (helpbugs_mi = new JMenuItem ("Known Bugs"));

	help_menu.add (helptech_menu = new JMenu ("Technical Information"));
	helptech_menu.add (descformat_mi = new JMenuItem ("ChunkDesc File Format"));
	helptech_menu.add (chunkformat_mi = new JMenuItem ("Config File Format"));
	helptech_menu.add (orgtreeformat_mi = new JMenuItem ("ChunkOrgTree File Format"));
	help_menu.add (helpdesc_menu = new JMenu ("Chunk-Specific Help"));

	setJMenuBar (main_menubar);

	quit_mi.addActionListener (this);
	network_refresh_mi.addActionListener (this);
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
	descformat_mi.addActionListener (this);
	chunkformat_mi.addActionListener (this);
	orgtreeformat_mi.addActionListener (this);

	// --------------------- FINAL WIN STUFF ---------------------------------
	
	totalSetFont ("", 12);

	addWindowListener (this);
	setSize(750,550);

	//show();
	//configure_pane.setDividerLocation (0.5);

	Core.addLogMessageListener (this);
    }





//      public void addDescDB (ChunkDescDB db) {
//  	int i;
//  	ChunkDesc d;
//  	JMenuItem newmenu;

//  	descdb_pane.addDescDB (db.name);
//  	configure_pane.updateInsertTypes();
//  	orgtree_pane.updateInsertTypes();
//  	// update our helpdesc_menu
//  	for (i = 0; i < db.size(); i++) {
//  	    d = (ChunkDesc)db.elementAt(i);
//  	    helpdesc_menu.add (newmenu = new JMenuItem (d.getName()));
//  	    newmenu.addActionListener(this);
//  	}
//      }



    public void quit() {
        dispose();
        System.exit(0);
    }


    public void actionPerformed (ActionEvent e) {
	//JMenuItem quit_mi, lnf_win_mi, lnf_java_mi, lnf_motif_mi;
	Object o = e.getSource(); 
	if (o == quit_mi)
	    quit();
	else if (o == network_refresh_mi)
	    Core.net.getChunks();
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
	    if (prefs_frame == null) {
		prefs_frame = new ConfigChunkFrame (this, Core.vjcontrol_preferences);
	    }
	    else
		prefs_frame.show();
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
	// where does desc help come from???
	URL url;
	url = getFileURL (System.getProperty ("user.dir") + "/.vjconfig/DescHelp/" + s + ".html");
	if (url == null)
	    url = getFileURL (System.getProperty ("VJ_BASE_DIR") + "/Data/DescHelp/" + s + ".html");
	if (url == null)
	    url = getFileURL ("Data/DescHelp/" +s + ".html");
	if (url == null)
	    url = ClassLoader.getSystemResource ("VjFiles/DescHelp/" + s + ".html");
	if (url == null)
	    url = ClassLoader.getSystemResource ("VjFiles/NoHelpAvailable.html");
	if (url == null)
	    return;

	HTMLFrame help_frame = new HTMLFrame (this, "VjControl Help", url);
	help_frames.addElement (help_frame);
	help_frame.show();
    }

	

    public void loadHelp (String s) {
	//System.out.println ("loadhelp: " + s);
	URL url = ClassLoader.getSystemResource (s);
	HTMLFrame help_frame = new HTMLFrame (this, "VjControl Help", url);
	help_frames.addElement (help_frame);
	help_frame.show();
    }
    

    public void closedChild (JFrame f, boolean ok) {
	if (f instanceof HTMLFrame) {
	    help_frames.removeElement (f);
	    f.dispose();
	}
	else if (f instanceof ConfigChunkFrame) {
	    if (ok) {
		ConfigChunk newc = ((ConfigChunkFrame)f).getValue();
		Core.gui_chunkdb.replace (Core.vjcontrol_preferences, newc);
		Core.vjcontrol_preferences = newc;
		Core.reconfigure();
	    }
	    f.dispose();
	    prefs_frame = null;
	}
    }



    public void setLookNFeel (String s) {
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
	}
	catch (Exception ex) {
	    Core.consoleErrorMessage ("GUI", "Couldn't change look and feel: " + ex);
	}
    }



    public void totalSetFont (String name, int size) {
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


    public void refreshPerfData() {
	perfanalyze_pane.refresh();
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


    /* WindowListener Stuff */
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
        quit();
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}


    /* LogMessageListener stuff */
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

}







