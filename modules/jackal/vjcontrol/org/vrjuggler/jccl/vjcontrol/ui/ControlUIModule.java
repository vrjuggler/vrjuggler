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


package VjComponents.UI;

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.Vector;
import java.util.List;
import java.util.HashMap;

import VjComponents.UI.*;
import VjComponents.UI.Widgets.*;
import VjControl.*;
import VjConfig.*;



/** Base UI for VjControl.
 * 
 *  ControlUIModule creates the main VjControl window.  It includes a menu
 *  system, an attachment point for PlugPanel components, and a configurable
 *  system for selecting which PlugPanel to display (by attaching a 
 *  PlugPanelContainer object to self).
 *  <p>
 *  It is possible to create multiple instances of ControlUIModule - ie 
 *  creating multiple juggler windows for a single instance of the app,
 *  each with its own set of panels.  But why would you really want to?
 *  <p>
 *  The utilities provided by ControlUIModule include an IconFactory,
 *  the EasyMenuBar, and an EasyFileDialog.
 *
 *  @author Christopher Just
 *  @version $Revision$
 */
public class ControlUIModule
    extends JFrame 
    implements ActionListener, 
               WindowListener, 
               ChildFrameParent, 
	       LogMessageListener, 
               ChunkDBListener,  
               CoreModule {


    protected JPanel      main_panel;

    protected String component_name;
    protected ConfigChunk component_chunk;

    protected IconFactory icon_factory;

    protected EasyMenuBar menubar;

    protected EasyFileDialog filedialog;
    protected SuffixFilter jarfile_filter;

    JMenuItem   quit_mi, 
                load_extensions_mi,
                load_extensions_url_mi;
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

    /** All the frames (ConfigChunkFrames, mainly, but not exclusively). */
    Vector child_frames;

    /** All the PlugPanels that're attached to self */
    Vector child_panels;

    /** A UI element where we'll display all the Panel children. */
    PlugPanelContainer panel_container;


    public ControlUIModule () {
	super ("VR Juggler Control Program");

        component_name = "Unconfigured ControlUIModule";
        component_chunk = null;

	JComponent p;  // temp panel pointer

        icon_factory = new IconFactory();

        filedialog = new EasyFileDialog();
        jarfile_filter = new SuffixFilter("VjControl Extensions (*.jar)", 
                                          ".jar");
        filedialog.addFilter (jarfile_filter, "VjControl Extension");

	child_panels = new Vector();
	child_frames = new Vector();

	main_panel = new JPanel();
	main_panel.setLayout (new BorderLayout (5,5));
	getContentPane().add (main_panel);

        panel_container = null;

	status_label = new JLabel (" ");
	main_panel.add (status_label, "South");

	//-------------------------- MENUS ----------------------------------

        menubar = new EasyMenuBar();
        menubar.addMenuItem ("File");
        menubar.addMenuItem ("Edit");
        menubar.addMenuItem ("Options");
        menubar.addMenuItem ("Help");

        load_extensions_mi = 
            menubar.addMenuItem ("File/Load Extension .jars...");
        load_extensions_url_mi =
            menubar.addMenuItem ("File/Load Extension URL...");
        quit_mi =
            menubar.addMenuItem ("File/Quit");

        helpindex_mi =
            menubar.addMenuItem ("Help/Help Index");
        hboverview_mi = 
            menubar.addMenuItem ("Help/Configuration Basics/Configuration Files");
        hbchunks_mi =
            menubar.addMenuItem ("Help/Configuration Basics/ConfigChunks");
        hbapps_mi = 
            menubar.addMenuItem ("Help/Configuration Basics/Configuring Applications");
        helpabout_mi =
            menubar.addMenuItem ("Help/Using VjControl/About VjControl");
        helpstart_mi =
            menubar.addMenuItem ("Help/Using VjControl/Getting Started");
        helpcmdline_mi =
            menubar.addMenuItem ("Help/Using VjControl/Command Line Arguments");
        helpchunk_mi =
            menubar.addMenuItem ("Help/Using VjControl/Working with Config Files");

        helpdesc_mi = 
            menubar.addMenuItem ("Help/Using VjControl/Working with ChunkDesc Files");
        helporgtree_mi =
            menubar.addMenuItem ("Help/Using VjControl/Working with ChunkOrgTrees");
        helpdynamic_mi = 
            menubar.addMenuItem ("Help/Using VjControl/Dynamic Reconfiguration");
        helpprefs_mi =
            menubar.addMenuItem ("Help/Using VjControl/VjControl Preferences");
        helpbugs_mi =
            menubar.addMenuItem ("Help/Using VjControl/Known Bugs");
        descformat_mi =
            menubar.addMenuItem ("Help/Technical Information/ChunkDesc File Format");
        chunkformat_mi =
            menubar.addMenuItem ("Help/Technical Information/Config File Format");
        orgtreeformat_mi =
            menubar.addMenuItem ("Help/Technical Information/ChunkOrgTree File Format");

        setJMenuBar (menubar.getJMenuBar());

	quit_mi.addActionListener (this);
        load_extensions_mi.addActionListener (this);
        load_extensions_url_mi.addActionListener (this);

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

	// --------------------- FINAL WIN STUFF -----------------

	addWindowListener (this);
	setSize(750,500);

	//show();

	//Core.vjcontrol_chunkdb.addChunkDBListener (this);
	//Core.addLogMessageListener (this);

        icon_factory.registerIcon ("new file", 0,
                                   ClassLoader.getSystemResource ("VjFiles/new.gif"));
	icon_factory.registerIcon ("open file", 0,
                                   ClassLoader.getSystemResource ("VjFiles/open.gif"));
	icon_factory.registerIcon ("save file", 0,
                                   ClassLoader.getSystemResource ("VjFiles/save.gif"));
	icon_factory.registerIcon ("close file", 0,
                                   ClassLoader.getSystemResource ("VjFiles/close.gif"));


    }


    //---------------------- ControlUIModule Services ------------------------


    /** Returns a reference to this UI's icon factory. */
    public IconFactory getIconFactory () {
        return icon_factory;
    }



    /** Shortcut for getIconFactory().getIcon(). */
    public ImageIcon getIcon (String name, int size) {
        return icon_factory.getIcon (name, size);
    }


    /** Provides components with access to ControlUI's menubar.
     *  Other GUI components can use this to add menu items. They
     *  should _NOT_ try to get ControlUI module's menubar directly
     *  using any of the JComponent/JFrame interfaces.
     */
    public EasyMenuBar getEasyMenuBar () {
        return menubar;
    }


    /** gives other components access to our menus (shortcut for easymenu) */
    public JMenuItem addMenuItem (String item) {
        return menubar.addMenuItem (item);
    }


    /** Provides components with access to a FileDialog.
     *  Other components can use this as an easy way to display a 
     *  file dialog with a consistent look & feel, filters, etc.
     */
    public EasyFileDialog getEasyFileDialog () {
        return filedialog;
    }


    //----------------------- VjComponent Stuff -----------------------------

    public String getComponentName () {
        return component_name;
    }


    public void setComponentName (String _name) {
        component_name = _name;
    }


    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        component_chunk = ch;
        component_name = ch.getName();

        // also, let's set ourselves up with the global prefs.  oh yeah...
	List v = Core.vjcontrol_chunkdb.getOfDescToken ("vjcontrol");
        if (v.size() > 0) {
            ConfigChunk chunk = (ConfigChunk)v.get(0);
            reconfigure (chunk);
        }
    }



    public ConfigChunk getConfiguration () {
        return component_chunk;
    }


    public void initialize () throws VjComponentException {
        // GUI initialization from constructor should go here.
	Core.vjcontrol_chunkdb.addChunkDBListener (this);
	Core.addLogMessageListener (this);
        show();
    }


    public boolean addConfig (ConfigChunk ch) {

        try {
            String classname = ch.getValueFromToken ("classname", 0).getString();
            if (classname == null) {
                Core.consoleErrorMessage (component_name, "Not a component instance chunk:" + ch.getName());
                return false;
            }
            else if (Core.component_factory.classSupportsInterface (classname, "VjComponents.UI.PlugPanel")) {
                PlugPanel pp = (PlugPanel) Core.component_factory.createComponent(classname);
                pp.setConfiguration (ch);
                pp.initialize ();
                child_panels.add (pp);
                if (panel_container != null)
                    panel_container.insertPanel(pp);
                Core.registerComponent (pp);
                return true;
            }
            else if (Core.component_factory.classSupportsInterface (classname, "VjComponents.UI.PlugPanelContainer")) {
                PlugPanelContainer pc = (PlugPanelContainer) Core.component_factory.createComponent (classname);
                pc.setConfiguration (ch);
                pc.initialize ();
                if (panel_container != null) {
                    ((JComponent)panel_container).removeAll();
                    main_panel.remove ((Component)panel_container);
                }
                panel_container = pc;
                int n = child_panels.size();
                for (int i = 0; i < n; i++) {
                    PlugPanel pp = (PlugPanel)child_panels.get(i);
                    pc.insertPanel (pp);
                }
                main_panel.add ((Component)panel_container, "Center");
                return true;
            }
            else {
                Core.consoleErrorMessage ("ControlUI", "Unrecognized component: " + classname);
                return false;
            }
        }
        catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }



    public boolean removeConfig (String name) {
        Core.consoleErrorMessage ("ControlUIModule removeConfig not implemented!!!");
        return false;
    }



    public void destroy () {
        // should remove children here

	Core.vjcontrol_chunkdb.removeChunkDBListener (this);
	Core.removeLogMessageListener (this);
        dispose();
    }


    //----------------------------------------------------------------------

    protected void quit() {
        Core.getComponentFromRegistry("VjControl Core").destroy();
        //dispose();
    }



    /** Kludge to force subpanels to redraw.
     */
    public void rebuildAllPanels() {
        if (panel_container != null)
            panel_container.rebuildDisplays();
    }
    


    /** Adds a child frame (e.g. a ConfigChunkFrame or HTMLFrame).
     */
    public void addChildFrame (ChildFrame f) {
	child_frames.add(f);
    }

    public void removeChildFrame (ChildFrame f) {
	child_frames.remove (f);
	f.destroy();
    }

    public void removeChildFramesMatching (String cl, Object db, Object o) {
	ChildFrame f;
        int i, n = child_frames.size();
	for (i = 0; i < n; ) {
	    f = (ChildFrame)child_frames.get(i);
	    if (f.matches (cl, db, o)) {
		child_frames.remove(i);
		f.destroy();
	    }
	    else
		i++;
	}
    }

    public ChildFrame getChildFrameMatching (String cl, Object db, Object o) {
	ChildFrame f;
        int i, n = child_frames.size();
	for (i = 0; i < n; i++) {
	    f = (ChildFrame)child_frames.get(i);
	    if (f.matches (cl, db, o))
		return f;
	}
	return null;
    }




    public void actionPerformed (ActionEvent e) {
	Object o = e.getSource(); 
	if (o == quit_mi)
	    quit();
        else if (o == load_extensions_mi) {
            File f = filedialog.requestOpenFile (null, this, jarfile_filter);
            if (f != null)
                Core.file.loadExtensions(f);
        }
        else if (o == load_extensions_url_mi) {
            String url = 
                JOptionPane.showInputDialog (this, "Enter URL of .jar file:", 
                                             "Load extension from URL", 
                                             JOptionPane.QUESTION_MESSAGE);
            if (url != null && !url.equals(""))
                Core.component_factory.registerJarURL (url);
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
	else
            Core.consoleErrorMessage (component_name, "Unidentified event!");
    }



    /** Creates an HTML frame displaying the named helpfile. */
    public void loadHelp (String s) {
	System.out.println ("loadhelp: " + s);
	URL url = ClassLoader.getSystemResource (s);
	HTMLFrame help_frame = new HTMLFrame (this, "VjControl Help", url);
        help_frame.setContentsURL (ClassLoader.getSystemResource ("VjFiles/VjControlIndex.html"));
	child_frames.add (help_frame);
	help_frame.show();
    }
    


    /** Callback when one of ControlUI's children is closed.
     */
    public void closeChild (ChildFrame frame) {
	if (frame instanceof HTMLFrame) {
	    child_frames.remove (frame);
	    frame.destroy();
	}
    }



    /** Callback when one of ControlUI's children is applied.
     */
    public void applyChild (ChildFrame frame) {
	if (frame instanceof HTMLFrame) {
            ;
	}
        else {
            Core.consoleErrorMessage (component_name, "Has a child frame that it doesn't know what to do about in applyChild().");
        }                             
    }


    //--------------------- UI & Font handling stuff -------------------------


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
            int i;
	    UIManager.setLookAndFeel (lnf);
	    SwingUtilities.updateComponentTreeUI (this);
	    for (i = 0; i < child_frames.size(); i++) {
		((ChildFrame)child_frames.elementAt(i)).updateUI();
	    }
//             for (i = 0; i < child_panels.size(); i++) {
//                 ((JComponent)child_panels.elementAt(i)).updateUI();
//             }
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
	changeMenuBarFont (menubar.getJMenuBar(), newfont);

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


    protected static void changeFont(Component cmp, Font font) {
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



    /** Update UI based on VjControl global prefs. 
     *  The VjC global prefs is currently a kludge looking for a home,
     *  but nevertheless here it is & here it'll stay until after the next
     *  revision of the config file format.
     */
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



    //-------------------- WindowListener Stuff ----------------------------

    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
        quit();
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}



    //---------------------- LogMessageListener stuff ------------------------

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



    //------------------------- ChunkDBListener stuff ------------------------

    /** Listening to Core.vjcontrol_chunkdb. */
    public void addChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeChunk (ChunkDBEvent e) {;}
    public void replaceChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeAllChunks (ChunkDBEvent e) {;}

}



