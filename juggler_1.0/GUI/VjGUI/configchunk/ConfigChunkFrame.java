/* 
 * ConfigChunkFrame.java
 *
 * Author: Christopher Just
 *
 */

package VjGUI.configchunk;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;
import VjConfig.*;
import VjGUI.*;
import VjGUI.configchunk.*;
import VjGUI.util.*;

public class ConfigChunkFrame extends JFrame 
    implements ActionListener, WindowListener {

    ConfigChunk   chunk;
    Vector proppanels; // property description panels.
    JPanel properties;
    JButton cancelbutton;
    JButton okbutton;
    JTextField namef;
    JTextField helpfield;
    JScrollPane sp;
    ConfigChunkFrameParent parent;

    GridBagLayout playout;
    GridBagConstraints pconstraints;

  public interface ConfigChunkFrameParent {
    public void closedChunkFrame (ConfigChunkFrame f, boolean ok);
  };

    public ConfigChunkFrame ( 
			     ConfigChunkFrameParent par, 
			     ConfigChunk ch) {
	super("Edit " + ch.getDescName() + ": " + ch.getName());

	Core.consoleTempMessage ("Opening ConfigChunk Window: " + ch.getName());

	//	Point p = c.ui.getLocation();
	//c.ui.waitdialog.setLocation(p.x + 100,p.y + 100);
	//c.ui.waitdialog.show();

	JPanel northpanel, southpanel, centerpanel;

	proppanels = new Vector();
	PropertyPanel t;

	parent = par;
	chunk = ch;
	JPanel mainp = new JPanel();
	mainp.setBorder (new EmptyBorder (5, 5, 0, 5));
	mainp.setLayout (new BorderLayout (5,5));
	getContentPane().add (mainp);

	JPanel p1;
	northpanel = new JPanel();
	northpanel.setLayout (new BoxLayout (northpanel, BoxLayout.Y_AXIS));
	p1 = new JPanel();
	p1.setLayout (new BoxLayout (p1, BoxLayout.X_AXIS));
	p1.add (new JLabel ("Instance Name: "));
	p1.add (namef = new StringTextField (chunk.getName(), 25));
	northpanel.add (p1);
	if (!chunk.desc.help.equals ("")) {
	    helpfield = new JTextField (chunk.desc.help);
	    helpfield.setEditable (false);
	    northpanel.add (helpfield);
	}

	mainp.add(northpanel, "North");
	
	properties = new JPanel ();
	properties.setLayout (new BoxLayout (properties, BoxLayout.Y_AXIS));

	sp = new JScrollPane(properties);
	sp.getVerticalScrollBar().setUnitIncrement(40);
	sp.getHorizontalScrollBar().setUnitIncrement(40);

	mainp.add(sp,"Center");
	
	// make property description panels
	for (int i = 0; i < chunk.props.size(); i++) {
	    t = new PropertyPanel ((Property)chunk.props.elementAt(i), this);
	    proppanels.addElement(t);
	    properties.add(t);
	}

	southpanel = new JPanel();
	okbutton = new JButton ("  OK  ");
	okbutton.addActionListener (this);
	southpanel.add (okbutton);
	cancelbutton = new JButton ("Cancel");
	cancelbutton.addActionListener (this);
	southpanel.add (cancelbutton);
	mainp.add(southpanel, "South");

	addWindowListener(this);

	/* this slightly convoluted process is needed in 
	 * order to open a window that is wide enough to 
	 * show the full PropertyPanels, regardless of
	 * font & label sizes etc.
	 *
	 * Window.pack() doesn't always handle the width 
	 * of scrollpanels as well as I'd like.
	 */
	pack();
	Dimension d = properties.getPreferredSize();
	if (d.height > 400) {
	  // the extra width is to make room for a scrollbar... but getsize
	  // on it doesn't seem to work how ye'd like...
	  //int sbwidth = sp.getVerticalScrollBar().getMaximumSize().width;
	  //System.out.println ("sbwidth: " + sbwidth);
	  setSize (d.width+42, 450);
	}
	setVisible(true);

	Core.consoleTempMessage ("");
  }



    public String getName () {
	return chunk.getName();
    }



  public ConfigChunk getOldValue() {
    return chunk;
  }

    public ConfigChunk getValue() {
	/* returns a configchunk based on the values current 
	 * in this window */
	ConfigChunk c = new ConfigChunk (chunk.desc, Core.descdb, 
					 namef.getText());
	c.props.removeAllElements();
	for (int i = 0; i < chunk.props.size(); i++) {
	    c.props.addElement (((PropertyPanel)proppanels.elementAt(i)).getValue());
	}
	return c;
    }



    public void closeFrame(boolean ok) {
	parent.closedChunkFrame(this, ok);
	dispose();
    }



    public void actionPerformed (ActionEvent e) {
	ConfigChunk newc;
	if (e.getSource() == cancelbutton) {
	    closeFrame(false);
	}
	else if (e.getSource() == okbutton) {
	    closeFrame(true);
	}
    }



    /* WindowListener Stuff */
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
	closeFrame(false);
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}
    
    
}





