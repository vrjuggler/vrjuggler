/* A panel for editing a single ChunkDesc... */

package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.ChunkDesc;
import VjConfig.PropertyDesc;
import VjGUI.ClientGlobals;
import VjGUI.BorderedPanel;

public class ChunkDescFrame extends Frame implements ActionListener,
       WindowListener { 

  ChunkDescDBPanel parent;
  ChunkDesc desc;
  ClientGlobals core;
  Vector proppanels; // property description panels.
  Panel properties;
  GridBagLayout propertieslayout;
  GridBagConstraints constraints;
  TextField namefield;
  TextField tokenfield;
  TextField helpfield;
  Button removebutton;
  Button insertbutton;
  Button cancelbutton;
  Button okbutton;

  public ChunkDescFrame (ClientGlobals c, ChunkDescDBPanel p, ChunkDesc d) {
    super("Edit ChunkDesc: "+d.name);

    parent = p;
    ScrollPane sp;
    PropertyDescPanel t;
    Panel mainpanel, buttonspanel, northpanel;

    core = c;
    desc = d;
    proppanels = new Vector();

    setFont(core.ui.windowfont);

    mainpanel = new BorderedPanel(10,5);
    mainpanel.setLayout(new BorderLayout(10,2));
    add(mainpanel);

    /********************** North Panel **************************/
    northpanel = new Panel();
    GridBagConstraints npconstraints;
    GridBagLayout northpanellayout;
    northpanellayout = new GridBagLayout();
    northpanel.setLayout(northpanellayout);
    npconstraints = new GridBagConstraints();
    npconstraints.gridwidth = GridBagConstraints.REMAINDER;
    npconstraints.fill = GridBagConstraints.HORIZONTAL;

    northpanel.add (new Label ("ChunkDesc Token:", Label.RIGHT));
    tokenfield = new TextField (d.token, 20);
    northpanel.add (tokenfield);
    Label l = new Label ("Descriptive Name:", Label.RIGHT);
    namefield = new TextField (d.name, 20);
    northpanellayout.setConstraints (namefield, npconstraints);
    northpanel.add(l);
    northpanel.add(namefield);
    northpanel.add (new Label ("Help Text:", Label.RIGHT));
    helpfield = new TextField (d.help, 5);
    northpanellayout.setConstraints (helpfield, npconstraints);
    northpanel.add (helpfield);
    mainpanel.add(northpanel,"North");

    /********************** Center Panel **************************/
    sp = new ScrollPane(ScrollPane.SCROLLBARS_ALWAYS);
    sp.getVAdjustable().setUnitIncrement(20);
    sp.getHAdjustable().setUnitIncrement(20);
    mainpanel.add(sp,"Center");
    properties = new Panel ();
    sp.add(properties);

    propertieslayout = new GridBagLayout ();
    properties.setLayout(propertieslayout);
    constraints = new GridBagConstraints();
    constraints.gridwidth = GridBagConstraints.REMAINDER;
    constraints.fill = GridBagConstraints.HORIZONTAL;


    // make property description panels
    PropertyDesc prop;
    for (int i = 0; i < desc.props.size(); i++) {
      prop = (PropertyDesc)desc.props.elementAt(i);
      t = new PropertyDescPanel (core, this, 
				 !prop.name.equalsIgnoreCase("name"),
				 prop);
      proppanels.addElement(t);
      propertieslayout.setConstraints(t,constraints);
      properties.add(t);
    }

    /************************ South Panel ********************************/
    buttonspanel = new Panel();
    removebutton = new Button ("Remove");
    removebutton.addActionListener (this);
    buttonspanel.add (removebutton);
    insertbutton = new Button ("Insert");
    insertbutton.addActionListener (this);
    buttonspanel.add (insertbutton);
    okbutton = new Button ("OK");
    okbutton.addActionListener (this);
    buttonspanel.add (okbutton);
    cancelbutton = new Button ("Cancel");
    cancelbutton.addActionListener (this);
    buttonspanel.add (cancelbutton);

    mainpanel.add (buttonspanel, "South");

    /* this slightly convoluted process is needed in order to open a window
     * that is wide enough to show the full PropertyPanels, regardless of
     * font & label sizes etc.
     */
    //    setSize(900,500);
pack();
    setSize (getSize().width, 600);
    setVisible(true);
    Dimension dim = properties.getPreferredSize();
    //setSize(dim.width + 54, 600);
    //validate();

  }

  public void closeFrame() {
    /* for each property panel, call closeEnums() */
    for (int i = 0; i < proppanels.size(); i++) {
      ((PropertyDescPanel)proppanels.elementAt(i)).closeFrames();
    }
    parent.closingChunkDescFrame(this);
    /* tell our parent ChunkDescDBPanel we're going away */
    dispose();
  }

  public String cdName () {
    return desc.name;
  }

  public void paint (Graphics g) {
    super.paint(g);
    g.setColor(Color.lightGray);
    g.draw3DRect(3, 3, getSize().width-6, getSize().height-6, true);
    g.draw3DRect(2, 2, getSize().width-4, getSize().height-4, false);
  }


  public void actionPerformed (ActionEvent e) {
    if (e.getSource() == removebutton) {
      for (int i = 0; i < proppanels.size(); ) {
	PropertyDescPanel p = 
	  (PropertyDescPanel)proppanels.elementAt(i);
	if (p.getSelected()) {
	  Panel par = (Panel)p.getParent();
	  par.remove(p);
	  proppanels.removeElementAt(i);
	}
	else
	  i++;
      }
      validate();
    }
    if (e.getSource() == insertbutton) {
      PropertyDescPanel t = new PropertyDescPanel (core, this, true);
      proppanels.addElement(t);
      propertieslayout.setConstraints(t,constraints);
      properties.add(t);
      validate();      
    }
    if (e.getSource() == okbutton) {
	/* ok. if we've ok'd changes, we need to:
	 * 1. build up a new ChunkDesc out of what's in the PropertyDesc 
	 *    panels.
	 * 2. stick that into our ChunkDescDB, replacing the old.
	 * 3. send a message with the new ChunkDesc out of NetControl
	 * 4. close the window.
	 */
	if (tokenfield.getText().equals("")) {
	    /* this really oughtta bring up a dialog box */
	    System.err.println ("Error: Must fill in ChunkDesc token field");
	    return;
	}

	ChunkDesc d = new ChunkDesc();
	d.token = tokenfield.getText();
	d.help = helpfield.getText();
	d.name = namefield.getText();
	if (d.name.equals(""))
	    d.name = d.token;
	/* start at one to skip name field which is already there */
	for (int i = 1; i < proppanels.size(); i++) {
	    PropertyDesc p = 
		((PropertyDescPanel)proppanels.elementAt(i)).getPropertyDesc();
	    d.props.addElement(p);
	}
	core.descs.remove (desc.token);
	core.descs.addElement (d);
	core.ui.update();
	//core.net.sendChunkDesc (d);
	closeFrame ();
    }
    if (e.getSource() == cancelbutton) {
	closeFrame ();
    }
  }


  /* WindowListener Stuff */
  public void windowActivated(WindowEvent e) {}
  public void windowClosed(WindowEvent e) {}
  public void windowClosing(WindowEvent e) {
    //core.ui.closeCDFrame (cdName());
    closeFrame();
  }
  public void windowDeactivated(WindowEvent e) {}
  public void windowDeiconified(WindowEvent e) {}
  public void windowIconified(WindowEvent e) {}
  public void windowOpened(WindowEvent e) {}


}









