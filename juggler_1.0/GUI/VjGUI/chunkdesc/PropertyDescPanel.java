/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


/*
 * Author: Christopher Just
 */

package VjGUI.chunkdesc;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;
import VjConfig.*;
import VjGUI.chunkdesc.*;
import VjGUI.util.*;

public class PropertyDescPanel extends JPanel 
    implements ItemListener, MouseListener, ActionListener {
    
    Vector                newenum;
    Vector                newvalues;
    ChunkDescFrame        parent;
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
    DescEnumFrame         valuesframe;
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
	pd.enums = newenum;
	pd.valuelabels = newvalues;

	if (pd.name == null || pd.name.equals(""))
	    pd.name = pd.token;
	if (pd.token == null || pd.token.equals (""))
	    return null;
	return pd;
    }



    public PropertyDescPanel (ChunkDescFrame p, 
			      boolean editable) {
	/* creates a "new" property panel, without a PropertyDesc */
	super();
	init (new PropertyDesc(), p, editable);
    }



    public PropertyDescPanel (ChunkDescFrame p,
			      boolean editable,
			      PropertyDesc d ) {
	/* Creates a new PropertyDescPanel with its components
	 * initialized with the values in d
	 */
	super();

	int i;
	init (d, p, editable);
	for (i = 0; i < d.enums.size(); i++)
	    newenum.addElement(new DescEnum((DescEnum)d.enums.elementAt(i)));
	for (i = 0; i < d.valuelabels.size(); i++) 
	    newvalues.addElement(new DescEnum((DescEnum)d.valuelabels.elementAt(i)));
    }



    private void addLabel (String s) {
	/* just a convenience used below */
	JLabel l = new JLabel (s);
	l.addMouseListener(this);
	add (l);
    }



    private void init (PropertyDesc d, 
		       ChunkDescFrame p, 
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
		    valuesframe = new DescEnumFrame (this, newvalues,
						     namefield.getText(),
						     new ValType ("string"),
						     true, 1
						     );
		}
		else {
		    valuesframe = new DescEnumFrame (this, newvalues,
						     namefield.getText(),
						     new ValType ("string"),
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



