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


package VjComponents.ConfigEditor.ChunkDescUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;

import VjConfig.*;
import VjComponents.UI.Widgets.*;


/** Panel for displaying a single Property of a ConfigChunk.
 *
 * @author Christopher Just
 * @version $Revision$
 */
public class PropertyDescPanel extends JPanel 
    implements ItemListener, MouseListener, ActionListener {
    
    Vector                newenum;
    Vector                newvalues; // list of string
    ChunkDescPanel        parent;
    PropertyDesc          desc;
    JTextField             namefield;
    JTextField             helpfield;
    JTextField             tokenfield;
    JCheckBox              varbox;
    JTextField             numfield;
    JComboBox                typechoice;
    JButton                enumsbutton;
    JButton                valuelabelsbutton;
    GridBagLayout         gblayout;
    DescEnumFrame         enumsframe;
    ValueLabelFrame         valuesframe;
    boolean               selected;
  
    static AbstractBorder select_border=null;
    static AbstractBorder unselect_border=null;


    public PropertyDesc getPropertyDesc() {
	/* returns a PropertyDesc based on the current values
	 * of the fields in the panel.
	 */
	PropertyDesc pd = new PropertyDesc();
	pd.name = namefield.getText();
	pd.token = tokenfield.getText();
	pd.help = helpfield.getText();
	pd.valtype = new ValType((String)typechoice.getSelectedItem());
	if (varbox.isSelected())
	    pd.num = -1;
	else
	    pd.num = Integer.parseInt(numfield.getText());
	pd.setEnumerations (newenum);
	pd.setValueLabels (newvalues);

	if (pd.name == null || pd.name.equals(""))
	    pd.name = pd.token;
	if (pd.token == null || pd.token.equals (""))
	    return null;
	return pd;
    }



    public PropertyDescPanel (ChunkDescPanel p, 
			      boolean editable) {
	/* creates a "new" property panel, without a PropertyDesc */
	super();
	init (new PropertyDesc(), p, editable);
    }



    public PropertyDescPanel (ChunkDescPanel p,
			      boolean editable,
			      PropertyDesc d ) {
	/* Creates a new PropertyDescPanel with its components
	 * initialized with the values in d
	 */
	super();

	int i;
	init (d, p, editable);
        // for safety's sake we copy all the descenums, just so that we
        // don't accidentally modify the old PropertyDesc.
        DescEnum[] e = d.getEnumerations();
	for (i = 0; i < e.length; i++)
	    newenum.add(new DescEnum(e[i]));
        String[] s = d.getValueLabels();
	for (i = 0; i < s.length; i++) 
	    newvalues.add(s[i]);
    }



    private void addLabel (String s) {
	/* just a convenience used below */
	JLabel l = new JLabel (s);
	l.addMouseListener(this);
	add (l);
    }



    private void init (PropertyDesc d, 
		       ChunkDescPanel p, 
		       boolean editable) {
	/* Does common initialization used by all constructors */

	if (select_border == null) {
	    select_border = new CompoundBorder ( new BevelBorder (BevelBorder.LOWERED),
						 new EmptyBorder (5,5,5,5));
	}
	if (unselect_border == null) {
	    unselect_border = new CompoundBorder ( new BevelBorder (BevelBorder.RAISED),
						   new EmptyBorder (5,5,5,5));
	}
    
	setBorder (unselect_border);

	newenum = new Vector();
	newvalues = new Vector();

	GridBagConstraints gbc = new GridBagConstraints();
	desc = d;
	parent = p;
	selected = false;
	addMouseListener(this);
	enumsframe = null;
	addLabel ("Name");
	desc = d;
	gblayout = new GridBagLayout();
	setLayout (gblayout);

	namefield = new StringTextField (d.name, 15);
	gbc.gridwidth = 2;
	gblayout.setConstraints(namefield,gbc);
	add (namefield);
	addLabel ("Token");
	tokenfield = new NoSpaceTextField (d.token, 15);
	gbc.gridwidth = GridBagConstraints.RELATIVE;
	gblayout.setConstraints(tokenfield,gbc);
	add (tokenfield);
	
	gbc.gridwidth = GridBagConstraints.REMAINDER;
	valuelabelsbutton = new JButton ("Edit Value Labels");
	gblayout.setConstraints (valuelabelsbutton, gbc);
	valuelabelsbutton.addActionListener(this);
	add (valuelabelsbutton); 
	
	varbox = new JCheckBox ("Var Arguments", (d.num == -1));
	add (varbox);
	varbox.addItemListener(this);
	addLabel ("Number");
	numfield = new IntegerTextField (Integer.toString(d.num), 3);
	if (d.num == -1)
	    numfield.setEnabled(false);
	add (numfield);
	addLabel ("Type");
	typechoice = new JComboBox();
	typechoice.addItem ("Int");
	typechoice.addItem ("Float");
	typechoice.addItem ("Bool");
	typechoice.addItem ("String");
	typechoice.addItem ("Chunk");
	typechoice.addItem ("EmbeddedChunk");
	typechoice.setSelectedItem ( d.valtype.toString());
	add (typechoice);
	typechoice.addActionListener (this);

	enumsbutton = new JButton ("Edit Enumerations");
	setEnumsButton ((String)typechoice.getSelectedItem());
	enumsbutton.addActionListener(this);
	gblayout.setConstraints(enumsbutton,gbc);
	add(enumsbutton);
	
	addLabel ("Help Text");
	helpfield = new StringTextField (d.help, 45);
	gbc.gridwidth = GridBagConstraints.REMAINDER;
	gbc.fill = GridBagConstraints.HORIZONTAL;
	gblayout.setConstraints(helpfield,gbc);
	add (helpfield);
	
	if (editable == false) {
	    namefield.setEditable(false);
	    tokenfield.setEditable(false);
	    numfield.setEditable(false);
	    helpfield.setEditable(false);
	    varbox.setEnabled(false);
	    typechoice.setEnabled(false);
	    enumsbutton.setEnabled(false);
	    valuelabelsbutton.setEnabled(false);
	}

	Dimension d1 = getPreferredSize();
	Dimension d2 = getMaximumSize();
	d2.height = d1.height;
	setMaximumSize(d2);

    }



    public void setSelected(boolean v) {
	selected = v;
	if (selected)
	    setBorder(select_border);
	else
	    setBorder(unselect_border);
	repaint();
    }



    public void toggleSelected() {
	if (!desc.name.equalsIgnoreCase("Name"))
	    setSelected (!selected);
    }



    public boolean getSelected() {
	return selected;
    }



    public void itemStateChanged (ItemEvent e) {
	if (e.getSource() == varbox) {
	  numfield.setEnabled (!varbox.isSelected());
	}
    }



    private void setEnumsButton (String s) {
	if (s.equalsIgnoreCase ("EmbeddedChunk")) {
	    enumsbutton.setText ("Inner Chunk Type");
	}
	else if (s.equalsIgnoreCase ("Chunk")) {
	    enumsbutton.setText ("Set Allowed Types");
	}
	else {
	    enumsbutton.setText ("Edit Enumerations");
	}
    }



    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == typechoice) {
	    setEnumsButton ((String)typechoice.getSelectedItem());
	}
	if (e.getSource() == valuelabelsbutton) {
	    if ((valuesframe == null) || valuesframe.closed) {
		if (varbox.isSelected()) {
		    valuesframe = new ValueLabelFrame (this, newvalues,
						     namefield.getText(),
						     true, 1
						     );
		}
		else {
		    valuesframe = new ValueLabelFrame (this, newvalues,
						     namefield.getText(),
						     false, Integer.parseInt(numfield.getText())
						     );
		}
	    }
	}
	else if (e.getSource() == enumsbutton) {
	    if ((enumsframe == null) || enumsframe.closed) {
		if (((String)typechoice.getSelectedItem()).equalsIgnoreCase 
		    ("EmbeddedChunk")) {
		    enumsframe = new DescEnumFrame (this, newenum,
						    namefield.getText(),
						    new ValType((String)typechoice.getSelectedItem()),
						    false, 1);
		}
		else {
		    enumsframe = new DescEnumFrame (this, newenum,
						    namefield.getText(),
						    new ValType((String)typechoice.getSelectedItem()),
						    true, 0);
		}
	    }
	}
    }



    public void closeFrames () {
	if (enumsframe != null) {
	    enumsframe.closeFrame();
	    enumsframe = null;
	}
	if (valuesframe != null) {
	    valuesframe.closeFrame();
	    valuesframe = null;
	}
    }



    /* MouseListener stuff */
    
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


}



