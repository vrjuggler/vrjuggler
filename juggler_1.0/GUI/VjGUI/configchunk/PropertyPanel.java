/*
 * PropertyPanel.java
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

public class PropertyPanel extends JPanel implements ActionListener {

    
  static AbstractBorder border=null;
    public Property prop;
    public ConfigChunkFrame parent;
    Vector valuepanels;  // vector of VarValuePanel
    JPanel centerpanel, eastpanel;
    JButton addbutton;

    GridBagLayout eastpanellayout;
    GridBagConstraints c;

    public PropertyPanel (Property pr, ConfigChunkFrame par) {
	super();

	if (border == null)
	    border = new CompoundBorder ( new BevelBorder (BevelBorder.RAISED),
					  new EmptyBorder (5,5,5,5));


	prop = pr;
	parent = par;
	int i;
	String label;
	int newlen;

	setBorder (border);

	setLayout(new BorderLayout (1,1));
	valuepanels = new Vector();

	JLabel l1 = new JLabel (pr.name, JLabel.LEFT);
	//l1.setFont (core.ui.windowfontbold);
	add (l1,"West");

	if (pr.desc.num == -1) {
	    addbutton = new JButton ("Add Value");
	    add(addbutton, "Center");
	    addbutton.addActionListener(this);
	}

	/* add help message, if there is one */
	if (!pr.desc.help.equals("")) {
	    
	    JTextField helparea;
	    if (pr.desc.help.length() < 50)
		helparea = new JTextField(pr.desc.help, 10);
	    else
		helparea = new JTextField(pr.desc.help,10);
	    helparea.setEditable(false);
	    add(helparea, "South");
	}
	
	/* east panel holds the individual PropertyPanels */
	eastpanel = new JPanel();

	eastpanellayout = new GridBagLayout();
	c = new GridBagConstraints();
	eastpanel.setLayout (eastpanellayout);
	c.fill = GridBagConstraints.BOTH;
	c.weightx = 1.0;
	JLabel lj;
        for (i = 0; i < pr.vals.size(); i++) {
	    if (pr.desc.valuelabels.size() > i)
		lj = new JLabel (((DescEnum)pr.desc.valuelabels.elementAt(i)).str);
	    else
		lj = new JLabel (" ");

	    c.gridwidth = GridBagConstraints.RELATIVE;
	    eastpanellayout.setConstraints (lj, c);
	    eastpanel.add (lj);
	    c.gridwidth = GridBagConstraints.REMAINDER;

	    VarValuePanel p = new VarValuePanel(this, pr.desc);
	    p.setValue((VarValue)pr.vals.elementAt(i));
	    valuepanels.addElement(p);
	    eastpanellayout.setConstraints(p, c);
	    eastpanel.add (p);
	}
	add(eastpanel,"East");
    }



    public void removePanel (VarValuePanel p) {
	valuepanels.removeElement(p);
	eastpanel.remove(p);
	parent.validate();
    }



    public Property getValue() {
	Property p = new Property (prop.desc);
	p.vals.removeAllElements();
	for (int i = 0; i < valuepanels.size(); i++) {
	    p.vals.addElement (((VarValuePanel)valuepanels.elementAt(i)).getValue());
	}
	return p;
    }



    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == addbutton) {
	    VarValuePanel p = new VarValuePanel(this, prop.desc);
	    valuepanels.addElement(p);
	    c.gridwidth = GridBagConstraints.REMAINDER;
	    eastpanellayout.setConstraints(p,c);
	    eastpanel.add (p);
	    validate();
	    parent.validate();
	}
    }

}





