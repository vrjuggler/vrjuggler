/*
 * ChunkDescFrame.java
 *
 * A window for viewing/editing a single ChunkDesc
 *
 * Author: Christopher Just
 *
 */

package VjGUI.chunkdesc;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;
import VjConfig.*;
import VjGUI.chunkdesc.*;
import VjGUI.Core;
import VjGUI.util.JFrameParent;

public class ChunkDescFrame 
    extends JFrame 
    implements ActionListener, WindowListener { 

    JFrameParent parent;
    ChunkDesc desc;
    Vector proppanels; // property description panels.
    JPanel properties;
    JTextField namefield;
    JTextField tokenfield;
    JTextField helpfield;
    JButton removebutton;
    JButton insertbutton;
    JButton cancelbutton;
    JButton okbutton;
    boolean editable;
    JPanel mainpanel, buttonspanel, northpanel;


    public ChunkDescFrame (JFrameParent p, 
			   ChunkDesc _desc,
			   boolean _editable) {
	super("Edit ChunkDesc: "+_desc.name);

	JScrollPane sp;
	PropertyDescPanel t;
	
	parent = p;
	editable = _editable;
	desc = _desc;
	proppanels = new Vector();
	
	//setFont(core.ui.windowfont);
	
	mainpanel = new JPanel();
	mainpanel.setBorder (new EmptyBorder (10,5,10,5));
	mainpanel.setLayout(new BorderLayout(10,2));
	getContentPane().add(mainpanel);
	
	/******************* North Panel **********************/
	northpanel = new JPanel();
        northpanel.setLayout(new BoxLayout (northpanel, BoxLayout.Y_AXIS));

        JPanel p1, p2;
        p1 = new JPanel();
        p2 = new JPanel();
        northpanel.add (p1);
        northpanel.add (p2);

        p1.add (new JLabel ("ChunkDesc Token:", 
                                   JLabel.RIGHT));
        tokenfield = new JTextField (desc.token, 15);
        p1.add (tokenfield);
        JLabel l = new JLabel ("Descriptive Name:", Label.RIGHT);
        namefield = new JTextField (desc.name, 15);
        p1.add(l);
        p1.add(namefield);
        p2.add (new JLabel ("Help Text:", Label.RIGHT));
        helpfield = new JTextField (desc.help, 50);
        p2.add (helpfield);

        mainpanel.add(northpanel,"North");

	/******************* Center Panel ********************/
	properties = new JPanel ();
	sp = new JScrollPane(properties, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
			     JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        sp.getVerticalScrollBar().setUnitIncrement(40);
        sp.getHorizontalScrollBar().setUnitIncrement(40);
	mainpanel.add(sp,"Center");
	
	properties.setLayout (new BoxLayout (properties, BoxLayout.Y_AXIS));

	// make property description panels
	PropertyDesc prop;
	for (int i = 0; i < desc.props.size(); i++) {
	    prop = (PropertyDesc)desc.props.elementAt(i);
	    t = new PropertyDescPanel (this, 
				       !prop.name.equalsIgnoreCase("name") && editable,
				       prop);
	    proppanels.addElement(t);
	    //	    propertieslayout.setConstraints(t,constraints);
	    properties.add(t);
	}

	/****************** South Panel ***********************/
	if (editable) {
	    buttonspanel = new JPanel();
	    removebutton = new JButton ("Remove");
	    removebutton.addActionListener (this);
	    buttonspanel.add (removebutton);
	    insertbutton = new JButton ("Insert");
	    insertbutton.addActionListener (this);
	    buttonspanel.add (insertbutton);
	    okbutton = new JButton ("OK");
	    okbutton.addActionListener (this);
	    buttonspanel.add (okbutton);
	    cancelbutton = new JButton ("Cancel");
	    cancelbutton.addActionListener (this);
	    buttonspanel.add (cancelbutton);
	}
	else { /* not editable */
	    buttonspanel = new JPanel();
	    cancelbutton = new JButton ("Close");
	    cancelbutton.addActionListener (this);
	    buttonspanel.add (cancelbutton);
	}
	
	mainpanel.add (buttonspanel, "South");
	
	addWindowListener (this);

	//setReasonableSize();
	//pack();
	//Dimension d = properties.getPreferredSize();
	//if (d.height > Core.screenHeight - 100)
	//    setSize (d.width+42, 550);
	
	Dimension d = properties.getPreferredSize();
	Dimension d2 = buttonspanel.getPreferredSize();
	//Dimension d3 = northpanel.getPreferredSize();
	d.width = Math.max (d.width+42, d2.width +20);
	d.width = Math.min (d.width, Core.screenWidth);
	d.height = Math.min (550, Core.screenHeight);
	setSize(d);


        setVisible(true);

    }


    public void setReasonableSize() {
	Dimension d1, d2, d3, d4, d;

	validate();
	d = new Dimension ();
	d1 = northpanel.getPreferredSize();
	d2 = buttonspanel.getPreferredSize();
	d3 = properties.getPreferredSize();
	d.height = d1.height + d2.height + d3.height +65;
	d4 = getSize();
	d.width = d4.width;

	if (d.height > Core.screenHeight - 100) {
	    d.height = Core.screenHeight;
	}
	if (d.width > Core.screenWidth) {
	    d.width = Core.screenWidth;
	}

	System.out.println ("setting size to " + d);
	setSize (d);
	validate();
    }



    public void closeFrame(boolean ok) {
	/* for each property panel, call closeEnums() */
	for (int i = 0; i < proppanels.size(); i++) {
	    ((PropertyDescPanel)proppanels.elementAt(i)).closeFrames();
	}
	parent.closedChild(this, ok);
	/* tell our parent ChunkDescDBPanel we're going away */
    }



    public String getName () {
	return desc.name;
    }



    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == removebutton) {
	    for (int i = 0; i < proppanels.size(); ) {
		PropertyDescPanel p = 
		    (PropertyDescPanel)proppanels.elementAt(i);
		if (p.getSelected()) {
		    properties.remove(p);
		    proppanels.removeElementAt(i);
		}
		else
		    i++;
	    }
	    //setReasonableSize();
	    validate();
	}
	if (e.getSource() == insertbutton) {
	    PropertyDescPanel t = new PropertyDescPanel (this, true);
	    proppanels.addElement(t);
	    properties.add(t); 
	    //setReasonableSize();
	    validate();
	}
	if (e.getSource() == okbutton) {
	  closeFrame (true);
	}
	if (e.getSource() == cancelbutton) {
	  closeFrame (false);
	}
    }



  public ChunkDesc getOldValue() {
    return desc;
  }



  public ChunkDesc getNewValue() {
    // returns the value currently shown in the panel...
    if (tokenfield.getText().equals("")) {
      /* this really oughtta bring up a dialog box */
      System.err.println ("Error: Must fill in ChunkDesc token field");
      return null;
    }
	    
    ChunkDesc d = new ChunkDesc();
    d.token = tokenfield.getText();
    d.help = helpfield.getText();
    d.name = namefield.getText();
    if (d.name.equals(""))
      d.name = d.token;

    /* start at one to skip instance name field which is already there */
    for (int i = 1; i < proppanels.size(); i++) {
      PropertyDesc p = 
	((PropertyDescPanel)proppanels.elementAt(i)).getPropertyDesc();
      if (p != null)
	  d.props.addElement(p);
    }

    return d;
  }


  /* WindowListener Stuff */
  public void windowActivated(WindowEvent e) {}
  public void windowClosed(WindowEvent e) {}
  public void windowClosing(WindowEvent e) {
    closeFrame (false);
  }
  public void windowDeactivated(WindowEvent e) {}
  public void windowDeiconified(WindowEvent e) {}
  public void windowIconified(WindowEvent e) {}
  public void windowOpened(WindowEvent e) {}


}



