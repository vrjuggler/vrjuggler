/*
 * ConfigChunkDBPanel.java
 *
 * Author: Christopher Just
 */

package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.ConfigChunk;
import VjConfig.ChunkDesc;
import VjGUI.MainWindowPanel;
import VjGUI.ConfigChunkFrame;
import VjGUI.AppCore;

public class ConfigChunkDBPanel extends MainWindowPanel 
    implements ActionListener, ItemListener {

    ClientGlobals core;
    List list;
    Button savebutton, loadbutton, removebutton, insertbutton, clearbutton;
    Vector chunkframes; // vector storing all the ChunkDescPanels...
    Choice insertchoice;
    Menu filemenu, networkmenu;
    Panel buttonspanel;
    String nullchoice;
    Label helplabel;
Panel centerpanel;


    public ConfigChunkDBPanel (ClientGlobals c ) {
	super(); 

	BorderedPanel      p;
	GridBagLayout      bp_layout;
	GridBagConstraints bp_constraints;

	nullchoice = "(select Chunk type)";
	chunkframes = new Vector();
	core = c;

	setLayout (new BorderLayout());

	add (new Label ("Available ConfigChunks:", Label.LEFT), 
	     "North");

	/*************** Center Panel ****************/
centerpanel = new Panel();
centerpanel.setLayout (new BorderLayout());
	list = new List (10, false);
	list.addActionListener(this);
	list.addItemListener(this);
	centerpanel.add(list,"Center");
add (centerpanel, "Center");
	/*************** East Panel *****************/
	buttonspanel = new BorderedPanel (20, 5, 10, 10, 0, 0);
	bp_layout = new GridBagLayout();
	bp_constraints = new GridBagConstraints();
	buttonspanel.setLayout(bp_layout);

       	bp_constraints.insets.top = 10;
	bp_constraints.gridwidth = 1;

	insertbutton = new Button ("Insert");
	insertbutton.addActionListener(this);
	bp_layout.setConstraints(insertbutton,bp_constraints);
	buttonspanel.add (insertbutton);

	bp_constraints.gridwidth = GridBagConstraints.REMAINDER;
	bp_constraints.fill = GridBagConstraints.HORIZONTAL;
	insertchoice = new Choice();
	insertchoice.add (nullchoice);
	for (int i = 0; i < core.descs.size(); i++) {
	    insertchoice.add(((ChunkDesc)core.chunks.elementAt(i)).name);
	}
	bp_layout.setConstraints(insertchoice,bp_constraints);
	buttonspanel.add(insertchoice);

	bp_constraints.gridwidth = 2;
	removebutton = new Button ("Remove");
	removebutton.addActionListener(this);
	bp_layout.setConstraints(removebutton,bp_constraints);
	buttonspanel.add (removebutton);

	clearbutton = new Button ("Clear");
	clearbutton.addActionListener(this);
	bp_layout.setConstraints (clearbutton, bp_constraints);
	buttonspanel.add (clearbutton);

	loadbutton = new Button (" Load ");
	loadbutton.addActionListener(this);
	bp_layout.setConstraints(loadbutton,bp_constraints);
	buttonspanel.add (loadbutton);

	savebutton = new Button (" Save ");
	savebutton.addActionListener(this);
	bp_layout.setConstraints(savebutton,bp_constraints);
	buttonspanel.add(savebutton);
    
	add(buttonspanel,"South");

	/***************** South Panel *******************/
	helplabel = new Label ("  ");
	centerpanel.add (helplabel, "South");

	/***************** Menus *********************/
	filemenu = new Menu("File");
	filemenu.addActionListener(this);
	if (core.multilevelconfigenabled) {
	    filemenu.add("Open User Config...");
	    filemenu.add("Save User Config");
	    filemenu.add("Save User Config As...");
	    filemenu.add("Open Global Config...");
	    filemenu.add("Save Global Config");
	    filemenu.add("Save Global Config As...");
	}
	else {
	    filemenu.add ("Open Config...");
	    filemenu.add ("Save Config");
	    filemenu.add ("Save Config As...");
	}
	filemenu.add("Exit");

	networkmenu = new Menu("File");
	networkmenu.addActionListener(this);
	networkmenu.add("Disconnect");
	networkmenu.add("Exit");
    }
    


    public void activate() {
	if (core.isConnected())
	    core.ui.menubar.add(networkmenu);
	else
	    core.ui.menubar.add(filemenu);
    }



    public void deactivate() {
	/* is it safe to remove a menu that may not have been added? */
	core.ui.menubar.remove(filemenu);
	core.ui.menubar.remove(networkmenu);
    }



    private void updateList() {
	ConfigChunk c;
	if (list.getItemCount() != 0)
	    list.removeAll();
	for (int i = 0; i < core.chunks.size(); i++) {
	    c = (ConfigChunk)core.chunks.elementAt(i);
	    if ((i == 0) || (!c.desc.name.equals (((ConfigChunk)core.chunks.elementAt(i-1)).desc.name)))
		list.addItem (c.desc.name);
	    list.addItem ("  " + c.name);
	}
    }



    public boolean update () {	
	int i,n;

	updateList();
	String s = insertchoice.getSelectedItem();
	n = insertchoice.getItemCount(); 
	for (i = 1; i < n; i++)
	insertchoice.remove(1);
	//insertchoice.setFont(core.ui.windowfont);

	for (i = 0; i < core.descs.size(); i++) {
	    insertchoice.addItem(((ChunkDesc)core.descs.elementAt(i)).name);
	}

	if (s != null)
	    insertchoice.select(s);

	updateHelpLabel();

	//validate();
	return true;
    }



    private boolean inList (String name) {
	int n = list.getItemCount();
	for (int i = 0; i < n; i++)
	    if (name.equals (list.getItem(i)))
		return true;
	return false;
    }
    


    private void updateHelpLabel() {
	centerpanel.remove (helplabel);

	String name = list.getSelectedItem();
	if (name == null) 
	    helplabel = new Label ("  ");
	else if (name.charAt(0) != ' ') {
	    // its a type
	    ChunkDesc d = core.descs.getByName(name);
	    helplabel = new Label (d.help);
	}
	else {
	    name = name.trim();
	    ConfigChunk c = core.chunks.get(name);
	    helplabel = new Label (c.desc.help);
	}
	centerpanel.add (helplabel,"South");
	validate();
    }



    public void actionPerformed (ActionEvent e) {
	int i;

	/***************** Buttons *********************/
	if (e.getSource() == savebutton) {
	    System.out.println ("saving file");
	    core.fileio.saveUserConfigChunkDB();
	}
	else if (e.getSource() == loadbutton) {
	    System.out.println ("loading file");
	    core.fileio.loadUserConfigChunkDB();
	}
	else if (e.getSource() == insertbutton) {
	    String name;
	    ChunkDesc d = core.descs.getByName(insertchoice.getSelectedItem());
	    if (d == null)
		return;
	    name = core.chunks.getNewName(d.name);
	    core.chunks.insertOrdered (new ConfigChunk (d, name));
	    //list.addItem("  " + name);
	    updateList();
	}
	else if (e.getSource() == removebutton) {
	    System.out.println ("pushed remove");
	    int ind[] = list.getSelectedIndexes();
	    for (i = 0; i < ind.length; i++ ) {
		core.chunks.remove (list.getItem(ind[i]).trim());
		updateList();
	    }      
	}
	else if (e.getSource() == clearbutton) {
	    System.out.println ("pressed clear");
	    core.chunks.removeAll();
	    updateList();
	}
	/*********************** List ***********************/
	else if (e.getSource() == list) {
	    String name = list.getSelectedItem();
	    if (name.charAt(0) != ' ')
		return;
	    openConfigChunkFrame(name);
	}

	/********************* Menus ****************************/
	else {
	    String s = e.getActionCommand();
	    //System.out.println ("got menu command: " + s);
	    if (s.equalsIgnoreCase ("save user config")) {
		core.fileio.saveUserConfigChunkDB();
	    }
	    else if (s.equalsIgnoreCase ("save config")) {
		core.fileio.saveUserConfigChunkDB();
	    }
	    else if (s.equalsIgnoreCase ("save user config as...")) {
		core.fileio.saveAsUserConfigChunkDB();
	    }
	    else if (s.equalsIgnoreCase ("save config as...")) {
		core.fileio.saveAsUserConfigChunkDB();
	    }
	    else if (s.equalsIgnoreCase ("open user config...")) {
		core.fileio.loadUserConfigChunkDB();
		update();
	    }
	    else if (s.equalsIgnoreCase ("open config...")) {
		core.fileio.loadUserConfigChunkDB();
		update();
	    }
	    else if (s.equalsIgnoreCase ("open global config...")) {
		core.fileio.loadBaseConfigChunkDB();
	    }
	    else if (s.equalsIgnoreCase ("save global config")) {
		core.fileio.saveBaseConfigChunkDB();
	    }
	    else if (s.equalsIgnoreCase ("save global config as...")) {
		core.fileio.saveAsBaseConfigChunkDB();
	    }
	    /* shared menu items */
	    else if (s.equalsIgnoreCase ("exit")) {
		core.ui.quit();
	    }
	}
    }



    public void itemStateChanged (ItemEvent e) {
	if (e.getSource() == list)
	    updateHelpLabel();
    }



  public void closedChunkFrame (ConfigChunkFrame fr) {
    for (int j = 0; j < chunkframes.size(); j++)
      if (((ConfigChunkFrame)chunkframes.elementAt(j)) == fr) {
	chunkframes.removeElementAt(j);
      }
  }



  public void closeAllChunkFrames () {
    for (int j = 0; j < chunkframes.size(); j++)
      ((ConfigChunkFrame)chunkframes.elementAt(j)).dispose();
    chunkframes.removeAllElements();
  }


    public boolean openConfigChunkFrame (String name) {
	name = name.trim();
	for (int j = 0; j < chunkframes.size(); j++) {
	    if (((ConfigChunkFrame)chunkframes.elementAt(j))
		.getName().equals(name))
		return false; // cuz this window is already open.
	}
	ConfigChunkFrame ccf = new ConfigChunkFrame (core, this,
						     core.chunks.get(name));
	chunkframes.addElement(ccf);
	ccf.show();
	return true;
    }

}


