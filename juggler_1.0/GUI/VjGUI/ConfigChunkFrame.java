/* 
 * ConfigChunkFrame.java
 *
 * Author: Christopher Just
 *
 */

package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjGUI.PropertyPanel;
import VjGUI.BorderedPanel;
import VjConfig.ConfigChunk;
import VjConfig.VarValue;
import VjConfig.Property;

public class ConfigChunkFrame extends Frame 
    implements ActionListener, WindowListener {

    ClientGlobals core;
    ConfigChunk   chunk;
    Vector proppanels; // property description panels.
    Panel properties;
    Button cancelbutton;
    Button okbutton;
    TextField namef;
    TextField helpfield;
    ScrollPane sp;
    ConfigChunkDBPanel parent;

    GridBagLayout playout;
    GridBagConstraints pconstraints;



    public ConfigChunkFrame (ClientGlobals c, 
			     ConfigChunkDBPanel par, 
			     ConfigChunk ch) {
	super("Edit " + ch.getDescName() + ": " + ch.getName());

	Point p = c.ui.getLocation();
	c.ui.waitdialog.setLocation(p.x + 100,p.y + 100);
	c.ui.waitdialog.show();

	Panel northpanel, southpanel, centerpanel;

	proppanels = new Vector();
	PropertyPanel t;

	setFont (c.ui.windowfont);
    
	core = c;
	parent = par;
	chunk = ch;
	Panel mainp = new BorderedPanel(5,5);
	mainp.setLayout (new BorderLayout (5,5));
	playout = new GridBagLayout();
	pconstraints = new GridBagConstraints();
	
	add (mainp);
	pconstraints.fill = GridBagConstraints.HORIZONTAL;
	
	northpanel = new Panel();
	northpanel.setLayout(playout);
	Label l = new Label ("Instance Name: ");
	playout.setConstraints (l, pconstraints);
	northpanel.add(l);
	namef = new TextField (chunk.getName(), 25);
	pconstraints.gridwidth = GridBagConstraints.REMAINDER;
	playout.setConstraints (namef, pconstraints);
	northpanel.add(namef);

	if (!chunk.desc.help.equals ("")) {
	    helpfield = new TextField (chunk.desc.help);
	    helpfield.setEditable (false);
	    playout.setConstraints (helpfield, pconstraints);
	    northpanel.add (helpfield);
	}
	
	mainp.add(northpanel, "North");
	
	sp = new ScrollPane(ScrollPane.SCROLLBARS_ALWAYS);
	sp.getVAdjustable().setUnitIncrement(20);
	sp.getHAdjustable().setUnitIncrement(20);
	mainp.add(sp,"Center");

	properties = new Panel ();
	GridBagLayout propertieslayout = new GridBagLayout ();
	properties.setLayout(propertieslayout);
	GridBagConstraints constraints = new GridBagConstraints();
	constraints.gridwidth = GridBagConstraints.REMAINDER;
	constraints.fill = GridBagConstraints.HORIZONTAL;
	
	sp.add(properties);
	
	// make property description panels
	for (int i = 0; i < chunk.props.size(); i++) {
	    t = new PropertyPanel ((Property)chunk.props.elementAt(i), this, core);
	    proppanels.addElement(t);
	    propertieslayout.setConstraints(t,constraints);
	    properties.add(t);
	}

	southpanel = new Panel();
	okbutton = new Button ("  OK  ");
	okbutton.addActionListener (this);
	southpanel.add (okbutton);
	cancelbutton = new Button ("Cancel");
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
	setSize(500,500);


	setVisible(true);
	Dimension d = properties.getPreferredSize();
	setSize(d.width + 50, 600);
	c.ui.waitdialog.hide();

  }



    public String getName () {
	return chunk.getName();
    }



    public ConfigChunk getValue() {
	/* returns a configchunk based on the values current 
	 * in this window */
	ConfigChunk c = new ConfigChunk (chunk.desc, 
					 namef.getText());
	c.props.removeAllElements();
	for (int i = 0; i < chunk.props.size(); i++) {
	    c.props.addElement (((PropertyPanel)proppanels.elementAt(i)).getValue());
	}
	return c;
    }



    public void closeFrame() {
	parent.closedChunkFrame(this);
	dispose();
    }



    public void actionPerformed (ActionEvent e) {
	ConfigChunk newc;
	if (e.getSource() == cancelbutton) {
	    closeFrame();
	}
	else if (e.getSource() == okbutton) {
	    newc = getValue();
	    core.chunks.replace(chunk,newc);
	    parent.update();
	    if (core.isConnected())
		core.net.sendChunk(newc);
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





