
/* VarValuePanel is just a little panel that we'll stick several of 
 * in a PropertyPanel.
 *
 *     -----------------------------------------------------
 *     |                                                   |
 *     | Type(optional) valuegadget  removebutton(optional)|
 *     |                                                   |
 *     -----------------------------------------------------
 *
 * the value gadget is going to be either a TextArea or a multisetSelectedItem
 * with the acceptable values in it.  removebutton will be there if the
 * associated property has var numbers of values.
 */

package VjGUI.configchunk;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import VjConfig.*;
import VjGUI.*;
import VjGUI.configchunk.*;
import VjGUI.util.*;

public class VarValuePanel extends JPanel implements ActionListener {

  PropertyPanel parent; // the parent is going to be a listener on the remove button
  PropertyDesc desc;
  JTextField text;
  JComboBox choice;   // note that only one of text,choice will be used.
  JButton removebutton;


  public VarValuePanel(PropertyPanel par, PropertyDesc d) {
    super();
    parent = par;
    desc = d;
    int i, j, k;

    setLayout(new BorderLayout (1,1));

    if (desc.valtype.equals(ValType.t_bool)) {
      choice = new JComboBox();
      choice.addItem("True");
      choice.addItem("False");
      add (choice,"East");
    }
    else if (desc.valtype.equals(ValType.t_chunk)) {
      /* we build up a choice of all chunks in the db of allowed types */
      ConfigChunkDB db;
      ConfigChunk ch;
      String s;

      choice = new JComboBox();
      if (desc.enums.size() == 0) {
	/* no listed types we use as a wildcard - show all chunks */
	choice = new JComboBox();
	ListBoxModel bm = new ListBoxModel();
	for (i = 0; i < Core.chunkdbs.size(); i++) {
	  db = (ConfigChunkDB)Core.chunkdbs.elementAt(i);
	  for (j = 0; j < db.size(); j++) {
	    ch = (ConfigChunk)db.elementAt(j);
	    bm.addObject (db.getName() + ": " + ch.getName());
	  }
	}
	choice.setModel(bm);
	add(choice, "Center");
      }
      else {
	choice = new JComboBox();
	ListBoxModel bm = new ListBoxModel();
	for (i = 0; i < Core.chunkdbs.size(); i++) {
	  db = ((ChunkDBTreeModel)Core.chunkdbs.elementAt(i)).chunkdb;
	  for (j = 0; j < db.size(); j++) {
	    ch = (ConfigChunk)db.elementAt(j);
	    for (k = 0; k < desc.enums.size(); k++) {
	      if (ch.getDescName().equalsIgnoreCase (((DescEnum)desc.enums.elementAt(k)).str)) {
		bm.addObject (db.getName() + ": " + ch.getName());
		break;
	      }
	    }
	  }
	}
	choice.setModel(bm);
	add(choice, "Center");
      }
    }
    else if (desc.enums.size() > 0) {
      /* just present the enumeration names as choices */
      //add (new JLabel (label), "West");
      choice = new JComboBox();
      ListBoxModel bm = new ListBoxModel();
      for (i = 0; i < desc.enums.size(); i++)
	bm.addObject(((DescEnum)desc.enums.elementAt(i)).str);
      choice.setModel(bm);
      add(choice, "Center");
    }
    else {
      /* just add a type label & the text string */
      add (new JLabel ("(" + desc.valtype.strVal() + ")"), "West");
      if (desc.valtype.equals(ValType.t_int))
	text = new IntegerTextField (20);
      else if (desc.valtype.equals(ValType.t_float))
	text = new FloatTextField (20);
      else
	  text = new StringTextField (25);
      add(text,"Center");
    }

    if (desc.num == -1) {
      /* then it's a variable # of values */
      removebutton = new JButton("Remove");
      Insets in = new Insets (0,0,0,0);
      removebutton.setMargin (in);
      add (removebutton,"East");
      removebutton.addActionListener(this);
    }
  }



    public void setValue (VarValue v) {
	// sets the displayed value.
	ValType tp = v.getValType();
	if (tp.equals(ValType.t_bool)) {
	    choice.setSelectedItem(((Boolean)v.get()).booleanValue()?"True":"False");
	}
	else if (choice != null) {
	    /* then we're looking at a multisetSelectedItem enumeration thing */
	    if (tp.equals(ValType.t_int))
		choice.setSelectedItem(desc.getEnumString(v.getInt()));
	    else if (tp.equals(ValType.t_float))
		choice.setSelectedItem(desc.getEnumString(v.getFloat()));
	    else if (tp.equals(ValType.t_string))
		choice.setSelectedItem((String)v.get());
	    else if (tp.equals(ValType.t_chunk))
		choice.setSelectedItem((String)v.get());
	    else 
		System.err.println ("VarValuePanel.setValue is confused");    
	}
	else
	    /* set text in textbox */
	    text.setText(v.toString());
    }



  public VarValue getValue () {
    VarValue v = new VarValue(desc.valtype);
    if (desc.valtype.equals(ValType.t_bool)) {
      v.set((choice.getSelectedIndex() == 0)?true:false);
    }
    else if (choice != null) {
      /* enumeration */
      String s = desc.getEnumValue((String)choice.getSelectedItem()).toString();
      // remove the "filename: " prefix if it's there...
      int i = s.indexOf(": ");
      if (i != -1)
	s = s.substring(i+2);
      v.set(s);
    }
    else if (desc.valtype.equals(ValType.t_int)) {
      v.set(Integer.parseInt(text.getText()));
    }
    else if (desc.valtype.equals(ValType.t_float)) {
      v.set(Float.valueOf(text.getText()).floatValue());
    }
    else
      v.set(text.getText());
    
    return v;
  }



    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == removebutton)
	    parent.removePanel(this);
    }



}

