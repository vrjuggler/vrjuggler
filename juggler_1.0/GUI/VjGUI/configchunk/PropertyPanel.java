/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 */


/*
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

public class PropertyPanel extends JPanel implements ActionListener, VarValuePanel.VarValuePanelParent {

    
    protected static AbstractBorder border=null;
    public Property prop;
    public ConfigChunkFrame parent;
    Vector valuepanels;  // vector of VarValuePanel
    JPanel centerpanel, eastpanel;
    JButton addbutton;
    protected static String valuelabelpad = "          ";

    protected GridBagLayout eastpanellayout;
    protected GridBagConstraints c;


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

	JPanel westpanel = new JPanel();
	westpanel.setLayout (new BoxLayout (westpanel, BoxLayout.Y_AXIS));

	JLabel l1 = new JLabel (pr.name, JLabel.LEFT);
	//l1.setFont (core.ui.windowfontbold);
	westpanel.add (l1);

	if (pr.desc.num == -1) {
	    addbutton = new JButton ("Add Value");
	    westpanel.add(addbutton);
	    addbutton.addActionListener(this);
	}
	add (westpanel, "West");

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
		lj = new JLabel (valuelabelpad + ((DescEnum)pr.desc.valuelabels.elementAt(i)).str);
	    else
		lj = new JLabel (valuelabelpad);
	    c.gridwidth = GridBagConstraints.RELATIVE;
	    c.weightx = 0;
	    eastpanellayout.setConstraints (lj, c);
	    eastpanel.add (lj);
	    c.weightx = 1;
	    c.gridwidth = GridBagConstraints.REMAINDER;

	    VarValuePanel p;
	    p = makeVarValuePanel (pr, i);
	    p.setValue((VarValue)pr.vals.elementAt(i));
	    valuepanels.addElement(p);
	    eastpanellayout.setConstraints(p, c);
	    eastpanel.add (p);
	}
	add(eastpanel,"Center");
	validate();
	eastpanel.validate();
    }



    private VarValuePanel makeVarValuePanel (Property pr, int valindex) {
	if (pr.desc.valtype.equals (ValType.t_embeddedchunk)) {
	    ConfigChunk ch;
	    if (valindex < pr.vals.size())
		ch = ((VarValue)pr.vals.elementAt(valindex)).getEmbeddedChunk();
	    else
		ch = ChunkFactory.createChunk (pr.embeddesc);
		//ch = new ConfigChunk (pr.embeddesc, pr.descdb);
	    if (pr.desc.valuelabels.size() > valindex)
		ch.setName (((DescEnum)pr.desc.valuelabels.elementAt(valindex)).str);
	    else
		ch.setName (pr.desc.name + " " + valindex);
	    boolean useminipanel = true;
	    if (ch.desc.props.size() > 4)
		useminipanel = false;
	    PropertyDesc pd;
	    int nvals = 0;
	    for (int i = 0; i < ch.desc.props.size(); i++) {
		pd = (PropertyDesc)ch.desc.props.elementAt(i);
		if (pd.num == -1)
		    useminipanel = false;
		else
		    nvals += pd.num;
	    }
	    if (nvals > 4)
		useminipanel = false;
	    // basically, if the chunk has more than 3 values, or has a var
	    // args thing anywhere, we can't use the minipanel.  
	    // Actually, that should be 3 _values_ - change later.
	    if (useminipanel)
		return new VarValueMiniChunkPanel (this, pr, ch);
	    else
		return new VarValueBigChunkPanel (this, pr, ch);
	}
	else
	    return new VarValueStandardPanel(this, pr.desc);
    }


    public void removePanel (VarValuePanel p) {
	valuepanels.removeElement(p);
	// annoying but true: we need to remove p and the label component left of it
	Component comps[] = eastpanel.getComponents();
	for (int i = 0; i < comps.length; i++) {
	    if (comps[i] == p) {
		eastpanel.remove (comps[i-1]);
		eastpanel.remove(p);
		JPanel pr = (JPanel)getParent();
		pr.validate();
		pr.repaint (pr.getBounds());
		return;
	    }
	}
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
	    int i;
	    JLabel lj;

	    i = valuepanels.size();
	    if (prop.desc.valuelabels.size() > i)
		lj = new JLabel (valuelabelpad + ((DescEnum)prop.desc.valuelabels.elementAt(i)).str);
	    else
		lj = new JLabel (valuelabelpad);
	    lj.setBackground (Color.green);
	    c.gridwidth = GridBagConstraints.RELATIVE;
	    c.weightx = 0;
	    eastpanellayout.setConstraints (lj, c);
	    eastpanel.add (lj);
	    c.weightx = 1;
	    c.gridwidth = GridBagConstraints.REMAINDER;

	    VarValuePanel p = makeVarValuePanel(prop, i);
	    valuepanels.addElement(p);
	    //c.gridwidth = GridBagConstraints.REMAINDER;
	    eastpanellayout.setConstraints(p,c);
	    eastpanel.add (p);
	    validate();
	    parent.validate();
	}
    }

}





