
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import VjGUI.AppCore;
import VjGUI.TrackerDataDisplayPanel;

public class ClientUI extends Frame 
implements ActionListener, WindowListener, ItemListener {

  ClientGlobals core;
  Panel cardpanel;
  CardLayout layout;
  public Font windowfont;
  public Font windowfontbold;

  MainWindowPanel descspanel, chunkspanel, connectpanel, activepanel,
      positionupdatepanel;
  List panellist; // list of panels in the cardlayout to display
  Panel mp;
  public MenuBar menubar;

  public ClientUI (ClientGlobals c) {
    super("C2 Configuration Editor");
    core = c;

    windowfont = new Font ("Courier", Font.PLAIN, 16);
    windowfontbold = new Font ("Courier", Font.BOLD, 16);
    setFont (windowfont);

    Menu hmen = new Menu ("Help");
    hmen.add ("Manual");

    menubar = new MenuBar();
    menubar.setFont (windowfont);
    menubar.setHelpMenu (hmen);
    setMenuBar (menubar);


    mp = new Panel();
    mp.setLayout(new BorderLayout(2,2));

    BorderedPanel listpanel = new BorderedPanel (4,4,0,0);
    listpanel.setLayout(new BorderLayout());

    listpanel.add(new Label ("Panels:", Label.LEFT), "North");
    panellist = new List (10,false);
    panellist.addItem ("Connection");
    panellist.addItem ("Descriptions");
    panellist.addItem ("Chunks");
    panellist.addItem ("Position Data");
    //panellist.addActionListener(this);
    panellist.addItemListener(this);
    listpanel.add(panellist,"Center");

    mp.add (listpanel, "West");
    
    cardpanel = new Panel();
    layout = new CardLayout();
    descspanel = new ChunkDescDBPanel(core);
    chunkspanel = new ConfigChunkDBPanel(core);
    connectpanel = new ConnectionPanel(core);
    positionupdatepanel = new TrackerDataDisplayPanel(core);
    cardpanel.setLayout(layout);

    cardpanel.add(descspanel,"Descriptions");
    cardpanel.add(chunkspanel,"Chunks");
    cardpanel.add(connectpanel,"Connection");
    cardpanel.add(positionupdatepanel, "Position Data");

    layout.layoutContainer(cardpanel);
    mp.add (cardpanel, "Center");

    add(mp);

    resize (750,490);
    show();
    addWindowListener(this);
    layout.show(cardpanel, "Connection");
    activepanel = connectpanel;
  }


    public boolean showPanel (String s) {
	activepanel.deactivate();
	
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
	else if (s.equalsIgnoreCase("Position Data")) {
	    layout.show (cardpanel, "Position Data");
	    activepanel = positionupdatepanel;
	    positionupdatepanel.activate();
	    update();
	    return true;
	}
	else
	    return false;
    }
    


  public boolean update () {
    activepanel.update();
    return true;
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







