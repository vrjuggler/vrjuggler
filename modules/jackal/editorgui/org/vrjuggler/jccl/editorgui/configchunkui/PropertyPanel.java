/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/



package VjComponents.ConfigEditor.ConfigChunkUI;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;

import VjConfig.*;
import VjComponents.ConfigEditor.*;

/** Panel for displaying a single Property of a ConfigChunk.
 *  This panel will typical contain 0 or more VarValuePanels for the
 *  individual values of the property.
 *
 * @author Christopher Just
 * @version $Revision$
 */
public class PropertyPanel extends JPanel implements ActionListener, VarValuePanel.VarValuePanelParent {

    ConfigUIHelper uihelper_module;
    
    protected static AbstractBorder border=null;
    public Property prop;
    public JPanel parent;
    private java.util.List valuepanels;  // vector of VarValuePanel
    private JPanel centerpanel, eastpanel;
    private JButton addbutton;
    protected static String valuelabelpad = "          ";

    protected GridBagLayout eastpanellayout;
    protected GridBagConstraints c;


    public PropertyPanel (Property pr, JPanel par, 
                          ConfigUIHelper _uihelper_module) {
	super();
        uihelper_module = _uihelper_module;

	if (border == null)
	    border = new CompoundBorder ( new BevelBorder (BevelBorder.RAISED),
					  new EmptyBorder (5,5,5,5));

	prop = pr;
	parent = par;
	int i, n;
	String label;
	int newlen;

	setBorder (border);

	setLayout(new BorderLayout (1,1));
	valuepanels = new Vector();

	JPanel westpanel = new JPanel();
	westpanel.setLayout (new BoxLayout (westpanel, BoxLayout.Y_AXIS));

	JLabel l1 = new JLabel (pr.getName(), JLabel.LEFT);
	//l1.setFont (core.ui.windowfontbold);
	westpanel.add (l1);

	if (!pr.hasFixedNumberOfValues()) {
	    addbutton = new JButton ("Add Value");
	    westpanel.add(addbutton);
	    addbutton.addActionListener(this);
	}
	add (westpanel, "West");

	/* add help message, if there is one */
        String h = pr.getDesc().getHelp();
	if (!h.equals("")) {
	    
	    JTextField helparea;
//  	    if (h.length() < 50)
//  		helparea = new JTextField(h, 10);
//  	    else
            helparea = new JTextField(h,10);
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
        n = pr.getNum();
        for (i = 0; i < n; i++) {
	    if (pr.getDesc().getValueLabelsSize() > i)
		lj = new JLabel (valuelabelpad + pr.getDesc().getValueLabel(i));
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
	    p.setValue(pr.getValue(i));
	    valuepanels.add(p);
	    eastpanellayout.setConstraints(p, c);
	    eastpanel.add (p);
	}
	add(eastpanel,"Center");
	validate();
	eastpanel.validate();
    }



    private VarValuePanel makeVarValuePanel (Property pr, int valindex) {
	if (pr.getDesc().getValType().equals (ValType.t_embeddedchunk)) {
	    ConfigChunk ch;
            PropertyDesc d = pr.getDesc();
	    if (valindex < pr.getNum())
		ch = pr.getValue(valindex).getEmbeddedChunk();
	    else {
		ch = ChunkFactory.createChunk (pr.getEmbeddedDesc());
                if (valindex < d.getValueLabelsSize())
                    ch.setName (d.getValueLabel (valindex));
                else
                    ch.setName (d.getName() + " " + valindex);
            }
	    if (useMiniPanel (ch))
		return new VarValueMiniChunkPanel (this, !pr.hasFixedNumberOfValues(), ch, uihelper_module);
	    else
		return new VarValueBigChunkPanel (this, pr, ch, uihelper_module);
	}
	else
	    return new VarValueStandardPanel(this, pr.getDesc(), uihelper_module);
    }



    // helper function for makeVarValuePanel. returns true if we
    // should use the VarValueMiniPanel to handle this embedded
    // chunk, and false if we should give it a separate frame.
    private boolean useMiniPanel (ConfigChunk ch) {
        boolean useminipanel = true;
        int maxwidth = 4;

        PropertyDesc[] props = ch.getDesc().getPropertyDescs();
        if (props.length > maxwidth)
            return false;

        int nvals = 0;
        for (int i = 0; i < props.length; i++) {
            if (props[i].num == -1)
                return false;
            if (props[i].valtype.equals(ValType.t_embeddedchunk))
                return false;
            nvals += props[i].num;
        }
        return (nvals <= maxwidth);
    }



    public void removePanel (VarValuePanel p) {
	valuepanels.remove(p);
	// annoying but true: we need to remove p and the label component left of it
	Component comps[] = eastpanel.getComponents();
	for (int i = 0; i < comps.length; i++) {
	    if (comps[i] == p) {
		eastpanel.remove (comps[i-1]);
		eastpanel.remove(p);
                System.out.println ("What the fuck is going on here?");
		Box pr = (Box)getParent();
		pr.validate();
		//pr.repaint (pr.getBounds());
                pr.repaint();  // in jdk < 1.4, box isn't a JComponent.
                               // what in cthulhu's name were they thinking?
		return;
	    }
	}
    }



    public Property getValue() {
	Property p = new Property (prop);
	//p.vals.removeAllElements();
        p.clearValues(); // only affects if var # of values allowed.
        int i, n = valuepanels.size();
	for (i = 0; i < n; i++) {
	    p.setValue (((VarValuePanel)valuepanels.get(i)).getValue(), i);
	}
	return p;
    }



    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == addbutton) {
	    int i;
	    JLabel lj;

	    i = valuepanels.size();
	    if (i < prop.getDesc().getValueLabelsSize())
		lj = new JLabel (valuelabelpad + prop.getDesc().getValueLabel(i));
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
	    valuepanels.add(p);
	    //c.gridwidth = GridBagConstraints.REMAINDER;
	    eastpanellayout.setConstraints(p,c);
	    eastpanel.add (p);
	    validate();
	    parent.validate();
	}
    }

}





