
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



    public DescEnumFrame (PropertyDescPanel p,
			  Vector _data,
			  String pdn,
			  ValType pdt) {
	super(pdn);
	
	closed = false;
	pdtype = pdt;

	//setFont(core.ui.windowfont);

	buttonspanel = new JPanel();
	buttonspanel.setLayout (new GridLayout (1, 4));
	
	data = _data;

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

	insertbutton = new JButton ("Insert");
	insertbutton.addActionListener(this);
	buttonspanel.add (insertbutton);
	removebutton = new JButton ("Remove");
	removebutton.addActionListener(this);
	buttonspanel.add (removebutton);
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
	//pack();
	Dimension d = sppanel.getPreferredSize();
	Dimension d2 = buttonspanel.getPreferredSize();
	d.width = Math.max (d.width+42, d2.width +20);
	d.width = Math.min (d.width, Core.screenWidth);
	d.height = Math.min (300, Core.screenHeight);
	setSize(d);
	//if (d.height > Core.screenHeight - 50) {
	// the extra width is to make room for a scrollbar... but getsize
	// on it doesn't seem to work how ye'd like...
	//setSize (d.width+42, Core.screenHeight);
	//}
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
	int ypos = 0;
	DescEnumElemPanel p;
	for (int i = 0; i < data.size(); i++) {
	    p = new DescEnumElemPanel((DescEnum)data.elementAt(i),
				      pdtype);
	    elempanels.addElement(p);
	    sppanel.add(p);
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
		if (pdtype.equals (ValType.t_chunk)) {
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
			 pdtype.equals (ValType.t_chunk)) {
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




