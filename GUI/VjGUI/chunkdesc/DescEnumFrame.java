/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



package VjGUI.chunkdesc;

import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjGUI.*;
import VjConfig.*;



public class DescEnumFrame extends JFrame 
    implements ActionListener, WindowListener {

    ValType pdtype; // current type of the associated propertydescpanel
    JPanel panel;
    JPanel sppanel;  /* sppanel is the panel inside the scrollpane...*/
    JButton insertbutton;
    JButton removebutton;
    JButton okbutton;
    JButton cancelbutton;
    Vector elempanels; // vector of DescEnumElemPanels
    Vector data;  // vector of DescEnum
    public boolean closed;
    JPanel buttonspanel;
    boolean varnumbers;
    int numvals;


    /* 
     * ARGS: _varnumbers - if true, show insert & remove buttons
     * ARGS: _numval - # of values to show (only matters if varnumbers false)
     */
    public DescEnumFrame (PropertyDescPanel p,
			  Vector _data,
			  String pdn,
			  ValType pdt,
			  boolean _varnumbers,
			  int _numval) {

	super(pdn);
	
	closed = false;
	pdtype = pdt;

	buttonspanel = new JPanel();
	//buttonspanel.setLayout (new GridLayout (1, 4));
	//buttonspanel.setLayout (new BoxLayout (buttonspanel, BoxLayout.X_AXIS));

	data = _data;
	varnumbers = _varnumbers;
	numvals = _numval;

	elempanels = new Vector();
	panel = new JPanel(new BorderLayout (10,2));
	panel.setBorder (new EmptyBorder (10, 5, 5, 5));
	getContentPane().add(panel);
	
	sppanel = new JPanel();
	sppanel.setLayout (new BoxLayout (sppanel, BoxLayout.Y_AXIS));

	JScrollPane sp = new JScrollPane(sppanel, 
					 JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, 
					 JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	sp.getVerticalScrollBar().setUnitIncrement(40);
	sp.getHorizontalScrollBar().setUnitIncrement(40);

	panel.add (sp, "Center");

	if (varnumbers) {
	    buttonspanel.setLayout (new GridLayout (1, 4));
	    insertbutton = new JButton ("Insert");
	    insertbutton.addActionListener(this);
	    buttonspanel.add (insertbutton);
	    removebutton = new JButton ("Remove");
	    removebutton.addActionListener(this);
	    buttonspanel.add (removebutton);
	}
	else {
	    buttonspanel.setLayout (new GridLayout (1, 2));
	    insertbutton = null;
	    removebutton = null;
	}
	okbutton = new JButton ("OK");
	okbutton.addActionListener(this);
	buttonspanel.add (okbutton);
	cancelbutton = new JButton ("Cancel");
	cancelbutton.addActionListener(this);
	buttonspanel.add (cancelbutton);
	panel.add (buttonspanel, "South");

	makeItems();
	addWindowListener(this);

	setFrameSize();
	setVisible(true);
    }



    public void setFrameSize() {
	Dimension d = sppanel.getPreferredSize();
	Dimension d2 = buttonspanel.getPreferredSize();
	d.width = Math.max (d.width+42, d2.width +20);
	d.width = Math.min (d.width, Core.screenWidth);
	if (varnumbers)
	    d.height = Math.min (300, Core.screenHeight);
	else
	    d.height = Math.min (d.height + d2.height + 45, Core.screenHeight);
	setSize(d);
    }



    public void closeFrame() {
	/* closes this frame & informs it's parent - doesn't save any data or
	 * anything.
	 */
	closed = true;
	dispose();
    }



    private int makeItems() {
	/* fills up the scrollpane with all the items in enums, which is a vector
	 * of DescEnum
	 */
	int i;
	int ypos = 0;
	DescEnumElemPanel p;
	for (i = 0; (i < data.size()) && (varnumbers || (i < numvals)); i++) {
	    p = new DescEnumElemPanel((DescEnum)data.elementAt(i),
				      pdtype);
	    elempanels.addElement(p);
	    sppanel.add(p);
	}
	if (!varnumbers) {
	    for (; i < numvals; i++) {
		p = new DescEnumElemPanel(pdtype);
		elempanels.addElement(p);
		sppanel.add(p);
	    }
	}
	sppanel.validate();
	return data.size();
    }



    public void actionPerformed (ActionEvent e) {
	DescEnumElemPanel p;
	int unused,j;
	float k;
	String s = null;
	if (e.getSource() == insertbutton) {
	    p = new DescEnumElemPanel(pdtype);
	    elempanels.addElement(p);
	    sppanel.add(p);

	    if (elempanels.size() == 1) { // make sure panel wide enough
		setFrameSize();
		Dimension d = getSize();
		Dimension d2 = sppanel.getPreferredSize();
		d.width = Math.max (d.width, d2.width + 42);
		d.width = Math.min (d.width, Core.screenWidth);
		setSize (d);
	    }
	    //setFrameSize();
	    validate();
	}
	else if (e.getSource() == removebutton) {
	    for (int i = 0; i < elempanels.size(); ) {
		p = (DescEnumElemPanel)elempanels.elementAt(i);
		if (p.getSelected()) {
		    sppanel.remove(p);
		    elempanels.removeElementAt(i);
		}
		else
		    i++;
	    }
	    // validate & repaint needed to get everything repositioned & redrawn
	    validate();
	    sppanel.repaint (sppanel.getBounds());
	}
	else if (e.getSource() == cancelbutton) {
	    closeFrame();
	}
	else if (e.getSource() == okbutton) {
	    unused = 0;
	    data.removeAllElements();
	    for (int i = 0; i < elempanels.size(); i++) {
		p = (DescEnumElemPanel)elempanels.elementAt(i);
		if (pdtype.equals (ValType.t_chunk) || pdtype.equals (ValType.t_embeddedchunk)) {
		    for (j = 0; j < Core.descdbs.size(); j++) {
			s = ((ChunkDescDB)Core.descdbs.elementAt(j)).getTokenFromName (p.getName());
			if (s != null)
			    break;
		    }
		}
		else
		    s = p.getName();
		if (s == null || s.equals (""))
		    continue; 
		
		if (pdtype.equals (ValType.t_int)) {
		    try {
			j = p.getIntVal();
			unused = (unused>j)?(unused):(j+1);
		    }
		    catch (NumberFormatException ne) {
			j = unused++;
		    }
		    data.addElement(new DescEnum (s,j));
		}
		else if (pdtype.equals (ValType.t_float)) {
		    try {
			k = p.getFloatVal();
			unused = (unused>k)?(unused):((new Float(k)).intValue() +1);
		    }
		    catch (NumberFormatException ne) {
			k = unused++;
		    }
		    data.addElement(new DescEnum (s,k));
		}
		else if (pdtype.equals (ValType.t_string) ||
			 pdtype.equals (ValType.t_chunk) ||
			 pdtype.equals (ValType.t_embeddedchunk)) {
		    data.addElement(new DescEnum (s,0));
		}
	    }
	    closeFrame();
	}
    }



    /* WindowListener Stuff */
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
	closeFrame();
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}



}




