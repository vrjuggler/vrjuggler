package VjGUI;

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import javax.swing.*;
import javax.swing.tree.*;
import javax.swing.event.*;
import VjGUI.*;
import VjGUI.configchunk.ConfigChunkFrame;
import VjConfig.*;

public class ControlUI  extends JFrame 
  implements ActionListener, WindowListener, ConfigChunkFrame.ConfigChunkFrameParent {


    JPanel main_panel;
    ConfigurePane configure_pane;
    ConnectionPane connection_pane;
    DescDBPanel descdb_pane;
    ConsolePane console_pane;
    ChunkOrgTreePane orgtree_pane;

    JMenuBar main_menubar;
    JMenu file_menu, options_menu, help_menu, looknfeel_menu, preferences_menu, helptech_menu;
    JMenuItem quit_mi, lnf_win_mi, lnf_java_mi, lnf_motif_mi, lnf_mac_mi;
    JMenuItem lnf_organic1_mi;
    JMenuItem editprefs_mi, saveprefs_mi;
    JMenuItem helpbugs_mi, helpabout_mi, helpstart_mi, helpchunk_mi, helpdesc_mi, helpcmdline_mi, descformat_mi, chunkformat_mi, orgtreeformat_mi, helpprefs_mi;
    JLabel status_label;
    ConfigChunkFrame prefs_frame;
    HTMLFrame help_frame;


    public ControlUI () {
	super ("VR Juggler Control Program");


	prefs_frame = null;
	help_frame = null;

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

	// --------------------- MENUS -------------------------------------------

	main_menubar = new JMenuBar();

	main_menubar.add (file_menu = new JMenu("File"));
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
	help_menu.add (helpabout_mi = new JMenuItem ("About VjControl"));
	help_menu.add (helpstart_mi = new JMenuItem ("Getting Started"));
	help_menu.add (helpcmdline_mi = new JMenuItem ("Command Line Arguments"));
	help_menu.add (helpchunk_mi = new JMenuItem ("Working with Config Files"));
	help_menu.add (helpdesc_mi = new JMenuItem ("Working with ChunkDesc Files"));
	help_menu.add (helpprefs_mi = new JMenuItem ("VjControl Preferences"));
	help_menu.add (helpbugs_mi = new JMenuItem ("Known Bugs"));

	help_menu.add (helptech_menu = new JMenu ("Technical Information"));
	helptech_menu.add (descformat_mi = new JMenuItem ("ChunkDesc File Format"));
	helptech_menu.add (chunkformat_mi = new JMenuItem ("Config File Format"));
	helptech_menu.add (orgtreeformat_mi = new JMenuItem ("ChunkOrgTree File Format"));

	setJMenuBar (main_menubar);

	quit_mi.addActionListener (this);
	lnf_win_mi.addActionListener (this);
	lnf_java_mi.addActionListener (this);
	lnf_motif_mi.addActionListener (this);
	lnf_mac_mi.addActionListener (this);
	lnf_organic1_mi.addActionListener (this);
	editprefs_mi.addActionListener (this);
	saveprefs_mi.addActionListener (this);
	helpabout_mi.addActionListener (this);
	helpstart_mi.addActionListener (this);
	helpchunk_mi.addActionListener (this);
	helpdesc_mi.addActionListener (this);
	helpprefs_mi.addActionListener (this);
	helpbugs_mi.addActionListener (this);
	helpcmdline_mi.addActionListener (this);
	descformat_mi.addActionListener (this);
	chunkformat_mi.addActionListener (this);
	orgtreeformat_mi.addActionListener (this);

	// --------------------- FINAL WIN STUFF ---------------------------------
	
	totalSetFont ("", 12);

	addWindowListener (this);
	setSize(600,450);

	//show();
	//configure_pane.setDividerLocation (0.5);
    }



    public void addChunkDBTree (ChunkDBTreeModel dbt) {
	configure_pane.addChunkDBTree (dbt);
    }



    public void removeChunkDBTree (ChunkDBTreeModel dbt) {
	configure_pane.removeChunkDBTree (dbt);
    }



    public void removeDescDB (String name) {
	descdb_pane.removeDescDB (name);
    }



    public void addDescDB (ChunkDescDB db) {
	descdb_pane.addDescDB (db.name);
	configure_pane.updateInsertTypes();
    }



    public void quit() {
        dispose();
        System.exit(0);
    }


    public void selectLeftDB (String name) {
	configure_pane.selectLeftDB (name);
    }
    public void selectRightDB (String name) {
	configure_pane.selectRightDB (name);
    }

    public void selectDescDB (String name) {
	descdb_pane.selectDB (name);
    }


    public void actionPerformed (ActionEvent e) {
	//JMenuItem quit_mi, lnf_win_mi, lnf_java_mi, lnf_motif_mi;
	Object o = e.getSource(); 
	if (o == quit_mi)
	    quit();
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
	else if (o == descformat_mi)
	    loadHelp ("VjFiles/ChunkDescFormat.html");
	else if (o == chunkformat_mi)
	    loadHelp ("VjFiles/ConfigChunkFormat.html");
	else if (o == orgtreeformat_mi)
	    loadHelp ("VjFiles/ChunkOrgTreeFormat.html");
    }




    public void loadHelp (String s) {
	//System.out.println ("loadhelp: " + s);
	URL url = ClassLoader.getSystemResource (s);
	if (help_frame == null)
	    help_frame = new HTMLFrame ("VjControl Help", url);
	else
	    help_frame.setURL (url);
	help_frame.show();
    }
    


    public void closedChunkFrame (ConfigChunkFrame f, boolean ok) {
	if (ok) {
	    ConfigChunk newc = f.getValue();
	    Core.gui_chunkdb.replace (Core.vjcontrol_preferences, newc);
	    Core.vjcontrol_preferences = newc;
	    Core.reconfigure();
	}
	prefs_frame = null;
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

	System.out.println ("setting look n feel to : " + lnf);
	
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
	for (int i = 0; i < keys.length; i++) {
	    UIManager.put (keys[i], newfont);
	} 
	changeFont (this, newfont);

	if (help_frame != null)
	    help_frame.setFont (newfont);
	// for some reason, i need to do menuitems by hand...

	/*
	file_menu.setFont (newfont);
	options_menu.setFont (newfont);
	looknfeel_menu.setFont (newfont);
	preferences_menu.setFont (newfont);
	quit_mi.setFont (newfont);
	lnf_win_mi.setFont (newfont);
	lnf_java_mi.setFont (newfont);
	lnf_motif_mi.setFont (newfont);
	lnf_mac_mi.setFont (newfont);
	lnf_organic1_mi.setFont (newfont);
	editprefs_mi.setFont (newfont);
	saveprefs_mi.setFont (newfont);
	*/
	changeFont (file_menu, newfont);
	changeFont (options_menu, newfont);
	changeFont (help_menu, newfont);
	validate();
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



}







