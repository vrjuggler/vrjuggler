
/* VarValuePanel is just a little panel that we'll stick several of 
 * in a PropertyPanel.
 *
 *     -----------------------------------------------------
 *     |                                                   |
 *     | Type(optional) valuegadget  removebutton(optional)|
 *     |                                                   |
 *     -----------------------------------------------------
 *
 * the value gadget is going to be either a TextArea or a multiselect
 * with the acceptable values in it.  removebutton will be there if the
 * associated property has var numbers of values.
 */

package VjGUI;

import java.awt.*;
import java.awt.event.*;
import VjConfig.PropertyDesc;
import VjConfig.ValType;
import VjConfig.ConfigChunk;
import VjConfig.DescEnum;
import VjConfig.VarValue;

public class VarValuePanel extends BorderedPanel implements ActionListener {

  PropertyPanel parent; // the parent is going to be a listener on the remove button
  PropertyDesc desc;
  TextField text;
  Choice choice;   // note that only one of text,choice will be used.
  Button removebutton;
  ClientGlobals core;


  public VarValuePanel(PropertyPanel par, PropertyDesc d, ClientGlobals c,
		       String label, int maxlabellen) {

    super(0,0);
    core = c;
    parent = par;
    desc = d;
    int i,j;

    /* this is the most god-awful ridiculous hack you're ever likely to see */
    while (label.length() < maxlabellen) {
      switch (maxlabellen - label.length()) {
      case 0:
	break;
      case 2:
      case 1:
	label = label + " ";
	break;
      case 4:
      case 3:
	label = label + "   ";
	break;
      default:
	label = label + "     ";
	break;
      }
    }

    setLayout(new BorderLayout (1,1));

    if (desc.valtype.equals(ValType.t_bool)) {
      choice = new Choice();
      choice.add("True");
      choice.add("False");
      add (new Label (label), "West");
      add (choice,"East");
    }
    else if (desc.valtype.equals(ValType.t_chunk)) {
      /* we build up a choice of all chunks in the db of allowed types */
      ConfigChunk ch;
      String s;

      choice = new Choice();
      if (desc.enums.size() == 0) {
	/* no listed types we use as a wildcard - show all chunks */
	for (j = 0; j < core.chunks.size(); j++) {
	  ch = (ConfigChunk)core.chunks.elementAt(j);
	  choice.add(ch.name);
	}
      }
      else {
	for (i = 0; i < desc.enums.size(); i++) {
	  s = ((DescEnum)desc.enums.elementAt(i)).str;
	  for (j = 0; j < core.chunks.size(); j++) {
	    ch = (ConfigChunk)core.chunks.elementAt(j);
	    if (ch.desc.token.equalsIgnoreCase(s))
	      choice.add(ch.name);
	  }
	}
      }
      add (new Label (label), "West");
      add(choice,"Center");
    }
    else if (desc.enums.size() > 0) {
      /* just present the enumeration names as choices */
      add (new Label (label), "West");
      choice = new Choice();
      for (i = 0; i < desc.enums.size(); i++)
	choice.add(((DescEnum)desc.enums.elementAt(i)).str);
      add(choice, "Center");
    }
    else {
      /* just add a type label & the text string */
      add(new Label (label + " ("+desc.valtype.strVal()+")", Label.RIGHT), "West");
      text = new TextField (25);
      add(text,"Center");
    }

    if (desc.num == -1) {
      /* then it's a variable # of values */
      removebutton = new Button("Remove");
      add (removebutton,"East");
      removebutton.addActionListener(this);
    }
  }



    public void setValue (VarValue v) {
	// sets the displayed value.
	ValType tp = v.getValType();
	if (tp.equals(ValType.t_bool)) {
	    choice.select(((Boolean)v.get()).booleanValue()?"True":"False");
	}
	else if (choice != null) {
	    /* then we're looking at a multiselect enumeration thing */
	    if (tp.equals(ValType.t_int))
		choice.select(desc.getEnumString(v.getInt()));
	    else if (tp.equals(ValType.t_float))
		choice.select(desc.getEnumString(v.getFloat()));
	    else if (tp.equals(ValType.t_string))
		choice.select((String)v.get());
	    else if (tp.equals(ValType.t_chunk))
		choice.select((String)v.get());
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
	v.set(desc.getEnumValue(choice.getSelectedItem()));
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
    /* note - this is going to listen for the removebutton's event, &
     * tell the parent to remove us.
     */
    if (e.getSource() == removebutton) {
      parent.removePanel(this);
      //parent.validate();
    }
  }

}
