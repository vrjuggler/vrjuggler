
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.ChunkDesc;
import VjConfig.ConfigChunk;
import VjConfig.Property;
import VjGUI.MainWindowPanel;
import VjGUI.AppCore;

public class TrackerDataDisplayPanel extends MainWindowPanel 
    implements ActionListener {

    ClientGlobals core;
    Menu filemenu, networkmenu;
    TextArea dataarea;

    public TrackerDataDisplayPanel (ClientGlobals c) {
	super(); 
	core = c;

	setLayout (new BorderLayout());
	
	add (new Label ("Current Tracker Data", Label.LEFT), "North");

	dataarea = new TextArea();
	add (dataarea, "Center");

	/* build menus - but don't add them until "activate" */
	filemenu = new Menu("File");
	filemenu.addActionListener(this);
	filemenu.add("Exit");

	networkmenu = new Menu("File");
	networkmenu.addActionListener(this);
	networkmenu.add("Disconnect");
	networkmenu.add("Exit");
    }

    public void activate() {
	if (core.isConnected()) {
	    core.ui.menubar.add(networkmenu);
	    core.net.startTimedUpdate ("Position", 300);
	}
	else
	    core.ui.menubar.add(filemenu);
    }

    public void deactivate() {
	/* is it safe to remove a menu that may not have been added? */
	if (core.isConnected())
	    core.net.stopTimedUpdate ("Position");
	core.ui.menubar.remove(filemenu);
	core.ui.menubar.remove(networkmenu);
    }

    public boolean update () {

	/* what we need to do here is get all chunks of type
	 * position_timed_update
	 * and then put there content into dataarea
	 */
	ConfigChunk c;
	Property p;
	Vector v = core.chunks.getOfType ("position_timed_update");

	dataarea.setText("");
	for (int i = 0; i < v.size(); i++) {
	    c = (ConfigChunk)v.elementAt(i);
	    dataarea.append(c.name + "\n");
	    p = c.getProperty("Position");
	    dataarea.append("    Position: \t\t" 
			    + p.vals.elementAt(0) + "\t"
			    + p.vals.elementAt(1) + "\t"
			    + p.vals.elementAt(2) + "\n");
	    p = c.getProperty("Orientation");
	    dataarea.append("    Orientation: \t"
			    + p.vals.elementAt(0) + "\t"
			    + p.vals.elementAt(1) + "\t"
			    + p.vals.elementAt(2) + "\n\n");
	}

	/*	ConfigChunk c = core.chunks.get ("position_timed_update");
	if (c == null)
	    return false;

	if (list.getItemCount() != 0)
	    list.removeAll();
	System.out.println ("update: list has " + core.descs.size() + 
			    " items.");
	for (int i = 0; i < core.descs.size(); i++) {
	    list.addItem (((ChunkDesc)core.descs.elementAt(i)).name);
	}
	*/
	return true;
    }



    public void actionPerformed (ActionEvent e) {
	System.out.println ("panel event called");
	int i;
	/* menu stuff */
	if ((e.getSource() == filemenu) || (e.getSource() == networkmenu)) {
	    String s = e.getActionCommand();
	    if (s.equalsIgnoreCase ("exit")) {
		core.ui.quit();
	    }
	    else if (s.equalsIgnoreCase ("disconnect")) {
		core.net.disconnect();
	    }
	}
    }

}








