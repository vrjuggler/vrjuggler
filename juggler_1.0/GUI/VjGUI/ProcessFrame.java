
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjGUI.ProcGroupPanel;
import VjGUI.BorderedPanel;
import VjConfig.ConfigChunk;
import VjConfig.VarValue;
import VjConfig.Property;

public class ProcessFrame extends Frame 
implements ActionListener, WindowListener {

    ClientGlobals          core;
    ConfigChunk            chunk;
    Vector                 procpanels; // procgrouppanels
    Panel                  procs;
    Button                 editbutton;
    Button                 startbutton;
    Button                 stopbutton;
    ScrollPane             sp;
    GridBagLayout          procgrouplayout;
    GridBagConstraints     constraints;


    public ProcessFrame (ClientGlobals c, Font windowfont) {
	super("VR Juggler Process Control");
	
	Panel northpanel, southpanel, centerpanel;
	ProcGroupPanel t;

	core = c;
	procpanels = new Vector();

	setFont (windowfont);
    
	Panel mainp = new BorderedPanel(5,5);
	mainp.setLayout (new BorderLayout (5,5));
	add (mainp);
    
	northpanel = new Panel();
	mainp.add(northpanel, "North");

	sp = new ScrollPane(ScrollPane.SCROLLBARS_ALWAYS);
	sp.getVAdjustable().setUnitIncrement(20);
	sp.getHAdjustable().setUnitIncrement(20);
	mainp.add(sp,"Center");

	procs = new Panel ();
	procgrouplayout = new GridBagLayout ();
	procs.setLayout(procgrouplayout);
	constraints = new GridBagConstraints();
	constraints.gridwidth = GridBagConstraints.REMAINDER;
	constraints.fill = GridBagConstraints.HORIZONTAL;
	
	sp.add(procs);

	update();

	southpanel = new Panel();
	startbutton = new Button ("Start");
	startbutton.addActionListener (this);
	southpanel.add (startbutton);
	stopbutton = new Button ("Stop");
	stopbutton.addActionListener (this);
	southpanel.add (stopbutton);
	editbutton = new Button ("Edit");
	editbutton.addActionListener (this);
	southpanel.add (editbutton);
	mainp.add(southpanel, "South");

	addWindowListener(this);
	
	/* this slightly convoluted process is needed in order to open a window
	 * that is wide enough to show the full PropertyPanels, regardless of
	 * font & label sizes etc.
	 *
	 * Window.pack() doesn't always handle the width of scrollpanels as
	 * well as I'd like.
	 */
	setSize(300,600);
	//setVisible(true);
	//Dimension d = procs.getPreferredSize();
	//setSize(d.width + 50, 600);
	
	//System.out.println ("panel size: " + procs.getPreferredSize());
    }



    public void update() {
	/* this needs work - shouldn't add new panels for everything. 
	 * just modify what exists 
	 */
	int i, j;
	Property prop;
	ProcGroupPanel t;
	ConfigChunk chunk = core.chunks.get ("vj process status");

	// make procgroup panels
	if (chunk != null) {
	    for (i = 0; i < chunk.props.size(); i++) {
		prop = (Property)chunk.props.elementAt(i);
		for (j = 0; j <procpanels.size(); j++) {
		    t = (ProcGroupPanel)procpanels.elementAt(j);
		    if (t.prop.name.equalsIgnoreCase (prop.name)) {
			t.update (prop);
			break;
		    }
		}
		if (j == procpanels.size()) {
		    t = new ProcGroupPanel (prop, core);
		    procpanels.addElement(t);
		    procgrouplayout.setConstraints(t,constraints);
		    procs.add(t);
		}
	    }
	}

    }


    
    public void setVisible(boolean b) {
	super.setVisible(b);
	setSize(procs.getPreferredSize().width + 50, getSize().height);
    }



    public void actionPerformed (ActionEvent e) {
	ProcGroupPanel p;
	int i;
	
	if (e.getSource() == startbutton) {
	    for (i = 0; i < procpanels.size(); i++) {
		p = (ProcGroupPanel)procpanels.elementAt(i);
		p.startActives();
	    }
	}
	else if (e.getSource() == stopbutton) {
	    for (i = 0; i < procpanels.size(); i++) {
		p = (ProcGroupPanel)procpanels.elementAt(i);
		p.stopActives();
	    }
	}
	else if (e.getSource() == editbutton) {
	    for (i = 0; i < procpanels.size(); i++) {
		p = (ProcGroupPanel)procpanels.elementAt(i);
		p.editActives();
	    }
	}
    }



  /* WindowListener Stuff */
  public void windowActivated(WindowEvent e) {}
  public void windowClosed(WindowEvent e) {}
  public void windowClosing(WindowEvent e) {
    hide();
  }
  public void windowDeactivated(WindowEvent e) {}
  public void windowDeiconified(WindowEvent e) {}
  public void windowIconified(WindowEvent e) {}
  public void windowOpened(WindowEvent e) {}


}












