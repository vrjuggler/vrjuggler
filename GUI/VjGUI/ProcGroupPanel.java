/* This is a small panel for displaying a group of related processes.
 * eg. you'd have one ProcGroupPanel for Input devices and another
 * for displays
 *
 *    |------------------------------------------------|
 *    | Group Name                                     |
 *    |      |--------------------------------------|  |
 *    |      |  First Process            Active     |  |
 *    |      |--------------------------------------|  |
 *    |      |  2nd  Proc                Inactive   |  |
 *    |      |--------------------------------------|  |
 *    |________________________________________________|
 *
 * Where the little boxes are 'selectable' subpanels.
 */

package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.Property;
import VjConfig.ConfigChunk;
import VjConfig.DescEnum;

public class ProcGroupPanel extends BorderedPanel {

    /******************* Internal ProcLine Class **************************/
    class ProcLine implements MouseListener {

	public String           name;
	public int              state;
        public BorderedPanel    panel;
	private boolean         selected;

	public ProcLine(String n, int s) {
	    selected = false;
	    name = n;
	    state = s;
	    panel = new BorderedPanel (4,4,BorderedPanel.RAISED,2);
	    Label l = new Label (name, Label.LEFT);
	    panel.add(l);
	    l.addMouseListener(this);
	    l = new Label ( ((state==0)?"          Inactive":"        Active"),
				 Label.RIGHT);
	    panel.add(l);
	    panel.addMouseListener(this);
	    l.addMouseListener(this);
	}

	public void setSelected(boolean v) {
	    selected = v;
	    if (selected)
		panel.setBorderStyle(BorderedPanel.DEPRESSED);
	    else
		panel.setBorderStyle(BorderedPanel.RAISED);
	    panel.repaint();
	}
	
	public void toggleSelected() {
	    setSelected (!selected);
	}

	public void mouseClicked (MouseEvent e) {
	    toggleSelected();
	} 	
	public void mouseEntered (MouseEvent e) {
	}
	public void mouseExited (MouseEvent e) {
	}
	public void mousePressed (MouseEvent e) {
	}
	public void mouseReleased (MouseEvent e) {
	}


    };
    /***************** End Internal ProcLine Class ************************/



    Vector               procs;
    Property             prop;
    ClientGlobals        core;
    Panel                centerpanel;
    GridBagLayout        cplayout;
    GridBagConstraints   cpconstraints;


    public ProcGroupPanel (Property p, ClientGlobals _core) {
	super (5,6,3,2);

	ProcLine prl;
	String s;
	Vector v;
	ConfigChunk chunk;
	prop = p;
	core = _core;
	procs = new Vector();

	setLayout( new BorderLayout() );
	add (new Label (prop.name, Label.LEFT), "North");

	add (new Label ("   "), "West"); // this adds space on left side

	centerpanel = new Panel();
	add (centerpanel, "East");

	cplayout = new GridBagLayout ();
	centerpanel.setLayout(cplayout);
	cpconstraints = new GridBagConstraints();
	cpconstraints.gridwidth = GridBagConstraints.REMAINDER;
	cpconstraints.fill = GridBagConstraints.HORIZONTAL;

	update(prop);

    }



    public void update (Property p) {
	int i, j;
	ProcLine prl;
	String s;
	Vector v;
	ConfigChunk chunk;

	for (i = 0; i < procs.size(); i++) {
	    prl = (ProcLine)procs.elementAt(0);
	    centerpanel.remove(prl.panel);
	    procs.removeElementAt(0);
	}

	for (i = 0; i < prop.desc.enums.size(); i++) {
	    s = ((DescEnum)prop.desc.enums.elementAt(i)).str;
	    v = core.chunks.getOfDescName(s);
	    for (j = 0; j < v.size(); j++) {
		/* this is getting ugly */
		chunk = (ConfigChunk)v.elementAt(j);
		prl = new ProcLine (chunk.name, 
				    prop.containsValue(chunk.name)?1:0);
		procs.addElement (prl);
		cplayout.setConstraints (prl.panel, cpconstraints);
		centerpanel.add (prl.panel);
	    }
	}
	validate();
    }



    public void remove() {
	/* eliminates the selected ProcLines */
	int j;
	ProcLine prl;

	for (j = 0; j < procs.size(); j++) {
	    prl = (ProcLine)procs.elementAt(j);
	    if (prl.selected) {
		remove(prl.panel);
		procs.removeElementAt(j);
	    }
	}
    }



    public void editActives() {
	int j;
	ProcLine prl;

	for (j = 0; j < procs.size(); j++) {
	    prl = (ProcLine)procs.elementAt(j);
	    if (prl.selected) {
		core.ui.openConfigChunkFrame(prl.name);
	    }
	}
    }



    public void startActives() {
	System.out.println ("startActives -- not implemented");
    }

    public void stopActives() {
	System.out.println ("stopActives -- not implemented");
    }

    public void actionPerformed (ActionEvent e) {
    }



}

