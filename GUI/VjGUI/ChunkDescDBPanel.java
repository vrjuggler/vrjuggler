
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.ChunkDesc;
import VjConfig.PropertyDesc;
import VjGUI.MainWindowPanel;
import VjGUI.AppCore;

public class ChunkDescDBPanel extends MainWindowPanel 
implements ActionListener {

    ClientGlobals core;
    List list;
    Button savebutton, loadbutton, 
	removebutton, insertbutton, clearbutton;
    Vector cdframes; // vector storing all the ChunkDescPanels...
    Menu filemenu, networkmenu;



    public ChunkDescDBPanel (ClientGlobals c) {
	super(); 
	/* watch his every move - superconstructor
	 * orchestrate illusions - superconstructor
	 */
	cdframes = new Vector();
	core = c;
	
	setLayout (new BorderLayout());
	
	add (new Label ("Available Chunk Descriptions:", Label.LEFT), "North");
	
	list = new List (10, false);
	list.addActionListener(this);
	add(list, "Center");
	
	Panel buttonpanel = new BorderedPanel(20,5,0,0);
	GridBagLayout buttonlayout = new GridBagLayout();
	buttonpanel.setLayout(buttonlayout);
	GridBagConstraints buttonconstraints = new GridBagConstraints();
	buttonconstraints.insets.top = 10;
	buttonconstraints.gridwidth = GridBagConstraints.REMAINDER;
	buttonconstraints.anchor = GridBagConstraints.WEST;
	buttonconstraints.fill = GridBagConstraints.HORIZONTAL;
	
	add(buttonpanel,"East");
	
	insertbutton = new Button ("Insert");
	insertbutton.addActionListener(this);
	buttonlayout.setConstraints (insertbutton, buttonconstraints);
	buttonpanel.add(insertbutton);
	removebutton = new Button ("Remove");
	removebutton.addActionListener(this);
	buttonlayout.setConstraints (removebutton, buttonconstraints);
	buttonpanel.add (removebutton);
	
	clearbutton = new Button ("Clear");
	clearbutton.addActionListener(this);
	buttonlayout.setConstraints (clearbutton, buttonconstraints);
	buttonpanel.add (clearbutton);
	
	loadbutton = new Button ("Load");
	loadbutton.addActionListener(this);
	buttonlayout.setConstraints (loadbutton, buttonconstraints);
	buttonpanel.add (loadbutton);
	savebutton = new Button ("Save");
	savebutton.addActionListener(this);
	buttonlayout.setConstraints (savebutton, buttonconstraints);
	buttonpanel.add(savebutton);
      
	/* build menus - but don't add them until "activate" */
	filemenu = new Menu("File");
	filemenu.addActionListener(this);
	if (core.multilevelchunkdescenabled) {
	    filemenu.add("Open User Descriptions");
	    filemenu.add("Save User Descriptions");
	    filemenu.add("Save User Descriptions As...");
	    filemenu.add("Open Global Descriptions");
	    filemenu.add("Save Global Descriptions");
	    filemenu.add("Save Global Descriptions As...");
	}
	else {
	    filemenu.add ("Open Descriptions...");
	    filemenu.add ("Save Descriptions");
	    filemenu.add ("Save Descriptions As...");
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

    public boolean update () {
	if (list.getItemCount() != 0)
	    list.removeAll();
	//System.out.println ("update: list has " + core.descs.size() + 
	//	    " items.");
	for (int i = 0; i < core.descs.size(); i++) {
	    list.addItem (((ChunkDesc)core.descs.elementAt(i)).name);
	}
	return true;
    }


    private boolean inList (String name) {
	int n = list.getItemCount();
	for (int i = 0; i < n; i++)
	    if (name.equals (list.getItem(i)))
		return true;
	return false;
    }



    public void actionPerformed (ActionEvent e) {
	//System.out.println ("panel event called");
	int i;

	if (e.getSource() == savebutton) {
	    System.out.println ("saving file");
	    core.fileio.saveUserChunkDescDB();
	}
	else if (e.getSource() == loadbutton) {
	    //System.out.println ("loading file");
	    core.fileio.loadUserChunkDescDB();
	    update();
	}
	else if (e.getSource() == removebutton) {
	    System.out.println ("pushed remove");
	    int ind[] = list.getSelectedIndexes();
	    // something w/ net? or just remove now??????????
	    // should just remove locally until we press OK or APPLY
	    for (i = 0; i < ind.length; i++ ) {
		core.descs.remove (list.getItem(ind[i]));
		list.delItem(ind[i]);
	    }
	}
	else if (e.getSource() == insertbutton) {
	    String name;
	    int num = 1;
	    do {
		name = "unnamed" + Integer.toString(num++);
	    } while (inList(name));
	    System.out.println ("pushed insert");
	    core.descs.addElement (new ChunkDesc(name));
	    list.addItem(name);
	    /* because on at least one system, this can make the scroller on
	     * the list show up & make a mess of things:
	     */
	    validate();
	}
	else if (e.getSource() == clearbutton) {
	    core.descs.removeAll();
	    update();
	}
	else if (e.getSource() == list) {
	    String name = list.getSelectedItem();
	    for (int j = 0; j < cdframes.size(); j++) {
		if (((ChunkDescFrame)cdframes.elementAt(j)).cdName().equals(name))
		    return; // cuz this window is already open.
	    }

	    ChunkDescFrame cdf = 
		new ChunkDescFrame (core, this, 
				    core.descs.getByName(name),
				    core.mode == core.FILE_EDITOR);
	    cdf.addWindowListener (cdf);
	    cdframes.addElement(cdf);
	    //cdf.setSize (550,350);
	    //cdf.show();
	}
	/* menu stuff */
	else if ((e.getSource() == filemenu) || 
		 (e.getSource() == networkmenu)) {
	    String s = e.getActionCommand();
	    
	    if (s.equalsIgnoreCase ("save user descriptions")) {
		core.fileio.saveUserChunkDescDB();
	    }
	    else if (s.equalsIgnoreCase ("save descriptions")) {
		core.fileio.saveUserChunkDescDB();
	    }
	    else if (s.equalsIgnoreCase ("save user descriptions as...")) {
		core.fileio.saveAsUserChunkDescDB();
	    }
	    else if (s.equalsIgnoreCase ("save descriptions as...")) {
		core.fileio.saveAsUserChunkDescDB();
	    }
	    else if (s.equalsIgnoreCase ("open user descriptions")) {
		core.fileio.loadUserChunkDescDB();
		update();
	    }
	    else if (s.equalsIgnoreCase ("open descriptions")) {
		core.fileio.loadUserChunkDescDB();
		update();
	    }
	    if (s.equalsIgnoreCase ("save global descriptions")) {
		core.fileio.saveBaseChunkDescDB();
	    }
	    else if (s.equalsIgnoreCase ("save global descriptions as...")) {
		core.fileio.saveAsBaseChunkDescDB();
	    }
	    else if (s.equalsIgnoreCase ("open global descriptions")) {
		core.fileio.loadBaseChunkDescDB();
		update();
	    }
	    else if (s.equalsIgnoreCase ("exit")) {
		core.ui.quit();
	    }
	}
    }



  public void closingChunkDescFrame (ChunkDescFrame f) {
    /* informs self that the argument ChunkDescFrame is closing,
     * and should be removed from the list of open ChunkDescFrames.
     * This should only be called by the ChunkDescFrame in question 
     * in its closeFrame() method.
     */
    for (int j = 0; j < cdframes.size(); j++)
      if (((ChunkDescFrame)cdframes.elementAt(j)) == f) {
	cdframes.removeElementAt(j);
      }
  }

  public void closeAllCDFrames () {
    for (int j = 0; j < cdframes.size(); j++)
      ((ChunkDescFrame)cdframes.elementAt(j)).closeFrame();
  }

}





