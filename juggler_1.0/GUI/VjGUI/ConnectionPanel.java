
/* ConnectionPanel.java */

package VjGUI;

import java.awt.*;
import java.awt.event.*;
import VjGUI.MainWindowPanel;
import VjGUI.ClientGlobals;

public class ConnectionPanel extends MainWindowPanel 
implements ActionListener {

  ClientGlobals core;
  Button connectbutton;
  Button disconnectbutton;
  TextField hostfield;
  TextField portfield;
    Menu filemenu, networkmenu;

  public ConnectionPanel (ClientGlobals c) {
    super();
    core = c;
    
    Panel p1, p2;

    GridLayout l = new GridLayout(2,1);
    l.setVgap(10);
    l.setHgap(10);
    setLayout (l);
    
    p1 = new Panel();
    p2 = new Panel();
    p1.add (new Label("Host Name:"));
    hostfield = new TextField (core.net.remoteName, 30);
    p1.add(hostfield);
    p1.add (new Label("Port:"));
    portfield = new TextField (Integer.toString(core.net.port), 5);
    p1.add(portfield);
    connectbutton = new Button("Connect");
    connectbutton.addActionListener(this);
    p2.add(connectbutton);
    disconnectbutton = new Button ("Disconnect");
    disconnectbutton.addActionListener(this);
    p2.add(disconnectbutton);

    add(p1);
    add(p2);

    /* build menus - but don't add them until "activate" */
    filemenu = new Menu("File");
    filemenu.addActionListener(this);
    filemenu.add("Exit");

    networkmenu = new Menu("File");
    networkmenu.addActionListener(this);
    networkmenu.add("Disconnect");
    networkmenu.add("Exit");

  }



    public void activate () {
	/* basically just adds menus to core.ui.menubar */
	core.ui.menubar.add(filemenu);
	validate();
    }



    public void deactivate () {
	/* removes menus from core.ui.menubar */
	core.ui.menubar.remove(filemenu);
    }



    public boolean update () {
	return true;
    }



    public void actionPerformed (ActionEvent e) {
	System.out.println ("connection panel event called");
	int i;
	
	if (e.getSource() == connectbutton) {
	    System.out.println ("pushed connect - not implemented");
	    int portnum = Integer.parseInt(portfield.getText());
	    core.net.connect(hostfield.getText(), portnum);
	    core.net.getChunkDescs();
	    core.net.getChunks();
	}
	else if (e.getSource() == disconnectbutton) {
	    System.out.println ("disconnect");
	    core.net.disconnect();
	}
	/* menu stuff */
	else if ((e.getSource() == filemenu) || 
		 (e.getSource() == networkmenu)) {
	    String s = e.getActionCommand();
	    if (s.equalsIgnoreCase ("exit")) {
		core.ui.quit();
	    }
	}
    }  // actionPerformed()

    
}
