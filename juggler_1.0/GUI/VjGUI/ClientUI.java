
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.ChunkDesc;
import VjGUI.AppCore;
import VjGUI.ProcessFrame;
import VjGUI.DataDisplayFrame;

public class ClientUI extends Frame 
implements ActionListener, WindowListener, ItemListener {

    ClientGlobals         core;
    Panel                 cardpanel,
                          mp;
    CardLayout            layout;
    public Font           windowfont;
    public Font           windowfontbold;
    BorderedPanel         listpanel;
    ProcessFrame          procframe;
    Vector                datadisplayframes;
    MainWindowPanel       descspanel, 
	                  chunkspanel, 
                          connectpanel, 
                          activepanel;
    List                  panellist; // list of panels in the cardlayout
    public MenuBar        menubar;



    public ClientUI (ClientGlobals c) {
	super("VR Juggler Configuration Editor");
	core = c;

	windowfont = new Font ("Courier", Font.PLAIN, 16);
	windowfontbold = new Font ("Courier", Font.BOLD, 16);
	setFont (windowfont);

	datadisplayframes = new Vector();
	Menu hmen = new Menu ("Help");
	hmen.add ("Manual");

	menubar = new MenuBar();
	menubar.setFont (windowfont);
	menubar.setHelpMenu (hmen);
	setMenuBar (menubar);

	mp = new Panel();
	mp.setLayout(new BorderLayout(2,2));

	listpanel = new BorderedPanel (4,4,0,0);
	listpanel.setLayout(new BorderLayout());

	listpanel.add(new Label ("Panels:", Label.LEFT), "North");
	panellist = new List (10,false);
	panellist.addItem ("Connection");
	panellist.addItem ("Descriptions");
	panellist.addItem ("Chunks");
	panellist.addItem ("Processes");
	panellist.addItemListener(this);
	listpanel.add(panellist,"Center");
	mp.add (listpanel, "West");
    
	cardpanel = new Panel();
	layout = new CardLayout();
	descspanel = new ChunkDescDBPanel(core);
	chunkspanel = new ConfigChunkDBPanel(core);
	connectpanel = new ConnectionPanel(core);
	cardpanel.setLayout(layout);
	cardpanel.add(descspanel,"Descriptions");
	cardpanel.add(chunkspanel,"Chunks");
	cardpanel.add(connectpanel,"Connection");

	layout.layoutContainer(cardpanel);
	mp.add (cardpanel, "Center");

	add(mp);
	addWindowListener(this);
	layout.show(cardpanel, "Connection");
	activepanel = connectpanel;

	procframe = new ProcessFrame (core, windowfont);

	pack();
	setVisible(true);
    }



    public boolean showPanel (String s) {
	activepanel.deactivate();
	
	if (s.equalsIgnoreCase("Processes")) {
	    procframe.update();
	    procframe.setVisible(true);
	    return true;
	}
	if (s.equalsIgnoreCase("Descriptions")) {
	    layout.show (cardpanel, "Descriptions");
	    activepanel = descspanel;
	    descspanel.activate();
	    update();
	    return true;
	}
	else if (s.equalsIgnoreCase("Chunks")) {
	    layout.show (cardpanel, "Chunks");
	    activepanel = chunkspanel;
	    chunkspanel.activate();
	    update();
	    return true;
	}
	else if (s.equalsIgnoreCase("Connection")) {
	    layout.show (cardpanel, "Connection");
	    activepanel = connectpanel;
	    connectpanel.activate();
	    update();
	    return true;
	}
	else {
	    /* check if it's a datadisplayframe */
	    int i;
	    DataDisplayFrame f;
	    for (i = 0; i < datadisplayframes.size(); i++) {
		f = (DataDisplayFrame)datadisplayframes.elementAt(i);
		if (f.getName().equalsIgnoreCase(s)) {
		    f.update();
		    f.setVisible(true);
		    return true;
		}
	    }
	}
	return false;
    }
    


    public boolean update () {
	/* for starters, we need to go thru all the datadisplay chunks &
	 * add frames for any new ones.
	 */
	int                 i, 
                            j;
	boolean             found;
	Vector              v;
	ChunkDesc           d;
	DataDisplayFrame    f;

	v = core.descs.getTokenBegins ("vj_timedupdate");
	for (i = 0; i < v.size(); i++) {
	    d = (ChunkDesc)v.elementAt(i);
	    for (j = 0, found = false; j < datadisplayframes.size(); j++) {
		f = (DataDisplayFrame)datadisplayframes.elementAt(j);
		if (d.name.equalsIgnoreCase(f.getName())) {
		    found = true;
		    break;
		}
	    }
	    if (!found) {
		f = new DataDisplayFrame (core, d);
		datadisplayframes.addElement(f);
		panellist.addItem(f.getName());
	    }
	}

	/* Well _that_ was fun :(
	 * Now we just have to call update of everything else that's visible
	 */
	for (j = 0; j < datadisplayframes.size(); j++) {
	    f = (DataDisplayFrame)datadisplayframes.elementAt(j);
	    f.update();
	}
	if (procframe.isVisible())
	    procframe.update();
	activepanel.update();
	return true;
    }



    public boolean openConfigChunkFrame (String name) {
	return ((ConfigChunkDBPanel)chunkspanel).openConfigChunkFrame (name);
    }



    // event handler

    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == panellist) {
	    showPanel (panellist.getSelectedItem());
	}
    }



    public void itemStateChanged (ItemEvent e) {
	if (e.getSource() == panellist) {
	    showPanel (panellist.getSelectedItem());
	}
    }



    public void quit() {
	dispose();
	System.exit(0);
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







