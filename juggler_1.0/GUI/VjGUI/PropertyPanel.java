
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.Property;
import VjConfig.PropertyDesc;
import VjConfig.VarValue;
import VjGUI.ConfigChunkFrame;
import VjGUI.VarValuePanel;
import VjGUI.BorderedPanel;
import VjConfig.DescEnum;

public class PropertyPanel extends BorderedPanel implements ActionListener {

    public Property prop;
    public ConfigChunkFrame parent;
    Vector valuepanels;  // vector of VarValuePanel
    Panel centerpanel, eastpanel;
    Button addbutton;
    ClientGlobals core;
    int maxlabellen;

    GridBagLayout eastpanellayout;
    GridBagConstraints eastpanelconstraints;

    public PropertyPanel (Property pr, ConfigChunkFrame par, 
			  ClientGlobals c) {
	super(3,2,3,2);
	core = c;
	prop = pr;
	parent = par;
	int i;
	String label;
	int newlen;

	maxlabellen = 0;

	/* bother. We need to set the maxlabellen */
	if (pr.desc.valuelabels.size() > 0) {
	  for (i = 0; i < pr.desc.valuelabels.size(); i++) {
	    newlen = ((DescEnum)pr.desc.valuelabels.elementAt(i)).str.length();
	    maxlabellen = (maxlabellen > newlen)? maxlabellen : newlen;
	  }
	}

	setLayout(new BorderLayout (1,1));
	valuepanels = new Vector();

	Label l1 = new Label (pr.name, Label.LEFT);
	l1.setFont (core.ui.windowfontbold);
	add (l1,"West");

	if (pr.desc.num == -1) {
	    addbutton = new Button ("Add Value");
	    add(addbutton, "Center");
	    addbutton.addActionListener(this);
	}

	/* add help message, if there is one */
	if (!pr.desc.help.equals("")) {
	    /*
	    TextArea helparea;
	    if (pr.desc.help.length() < 50)
		helparea = new TextArea(pr.desc.help,2,10,
					TextArea.SCROLLBARS_NONE);
	    else
		helparea = new TextArea(pr.desc.help,1,10,
					TextArea.SCROLLBARS_HORIZONTAL_ONLY);
	    helparea.setEditable(false);
	    add(helparea, "South");
	    */
	    TextField helpfield = new TextField (pr.desc.help);
	    helpfield.setEditable(false);
	    add(helpfield, "South");
	}
	
	/* east panel holds the individual PropertyPanels */
	eastpanel = new Panel();
	eastpanellayout = new GridBagLayout();
	eastpanelconstraints = new GridBagConstraints();
	eastpanelconstraints.gridwidth = GridBagConstraints.REMAINDER;
	eastpanelconstraints.fill = GridBagConstraints.HORIZONTAL;
	eastpanel.setLayout(eastpanellayout);
	for (i = 0; i < pr.vals.size(); i++) {
	    if (pr.desc.valuelabels.size() > i)
	      label = ((DescEnum)pr.desc.valuelabels.elementAt(i)).str;
	    else
	      label = "";
	    VarValuePanel p = new VarValuePanel(this, pr.desc, core, label, maxlabellen);
	    p.setValue((VarValue)pr.vals.elementAt(i));
	    valuepanels.addElement(p);
	    eastpanellayout.setConstraints(p,eastpanelconstraints);
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
	    VarValuePanel p = new VarValuePanel(this, prop.desc, core, "", maxlabellen);
	    valuepanels.addElement(p);
	    eastpanellayout.setConstraints(p,eastpanelconstraints);
	    eastpanel.add (p);
	    parent.validate();
	}
    }

}





