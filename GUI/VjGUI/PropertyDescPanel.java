/* PropertyDescPanel... */

package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.PropertyDesc;
import VjConfig.ValType;
import VjConfig.DescEnum;

public class PropertyDescPanel extends BorderedPanel implements ItemListener, 
MouseListener, ActionListener {

  ClientGlobals core;
  Vector newenum;
  Vector newvalues;
  ChunkDescFrame parent;
  PropertyDesc desc;
  TextField namefield;
  TextField helpfield;
  TextField tokenfield;
  Checkbox  varbox;
  TextField numfield;
  Choice    typechoice;
  Button    enumsbutton;
  Button    valuelabelsbutton;
  GridBagLayout gblayout;
  DescEnumFrame enumsframe;
  DescEnumFrame valuesframe;
  boolean   selected;
  
  public PropertyDesc getPropertyDesc() {

    PropertyDesc pd = new PropertyDesc();
    pd.name = namefield.getText();
    pd.token = tokenfield.getText();
    pd.help = helpfield.getText();
    pd.valtype = new ValType(typechoice.getSelectedItem());
    if (varbox.getState())
      pd.num = -1;
    else
      pd.num = Integer.parseInt(numfield.getText());
    pd.enums = newenum;
    pd.valuelabels = newvalues;
    return pd;
  }

  public PropertyDescPanel (ClientGlobals c, ChunkDescFrame p, 
			    boolean editable) {
    /* creates a "new" property panel, without a PropertyDesc */
    super(4,4,BorderedPanel.RAISED,2);
    core = c;
    newenum = new Vector();
    newvalues = new Vector();
    init (new PropertyDesc(), p, editable);
  }

  public PropertyDescPanel (ClientGlobals c, 
			    ChunkDescFrame p,
			    boolean editable,
			    PropertyDesc d ) {
    super(4,4,BorderedPanel.RAISED,2);
    core = c;
    int i;
    newenum = new Vector(); //d.enums;
    newvalues = new Vector();
    init (d, p, editable);
    for (i = 0; i < d.enums.size(); i++)
      newenum.addElement(new DescEnum((DescEnum)d.enums.elementAt(i)));
    for (i = 0; i < d.valuelabels.size(); i++) 
      newvalues.addElement(new DescEnum((DescEnum)d.valuelabels.elementAt(i)));
System.out.println ("newvalues size is " + newvalues.size());
  }

  private void addLabel (String s) {
    /* just a convenience used below */
    Label l = new Label (s);
    l.addMouseListener(this);
    add (l);
  }

  private void init (PropertyDesc d, ChunkDescFrame p, boolean editable) {
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

    namefield = new TextField (d.name, 15);
    gbc.gridwidth = 2;
    gblayout.setConstraints(namefield,gbc);
    add (namefield);
    addLabel ("Token");
    tokenfield = new TextField (d.token, 15);
    gbc.gridwidth = GridBagConstraints.RELATIVE;
    gblayout.setConstraints(tokenfield,gbc);
    add (tokenfield);

    gbc.gridwidth = GridBagConstraints.REMAINDER;
    valuelabelsbutton = new Button ("Edit Value Labels");
    gblayout.setConstraints (valuelabelsbutton, gbc);
    valuelabelsbutton.addActionListener(this);
    add (valuelabelsbutton); 

    varbox = new Checkbox ("Var Arguments", (d.num == -1));
    add (varbox);
    varbox.addItemListener(this);
    addLabel ("Number");
    numfield = new TextField (Integer.toString(d.num), 3);
    add (numfield);
    addLabel ("Type");
    typechoice = new Choice();
    typechoice.add ("Int");
    typechoice.add ("Float");
    typechoice.add ("Bool");
    typechoice.add ("String");
    typechoice.add ("Chunk");
    typechoice.select ( d.valtype.strVal());
    add (typechoice);
    enumsbutton = new Button ("Edit Enumerations");
    enumsbutton.addActionListener(this);
    gblayout.setConstraints(enumsbutton,gbc);
    add(enumsbutton);

    addLabel ("Help Text");
    helpfield = new TextField (d.help, 45);
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
  }



  public void setSelected(boolean v) {
    selected = v;
    if (selected)
      super.setBorderStyle(BorderedPanel.DEPRESSED);
    else
      super.setBorderStyle(BorderedPanel.RAISED);
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
      if (varbox.getState())
	numfield.setEnabled(false);
      else
	numfield.setEnabled(true);
    }
  }



  public void actionPerformed (ActionEvent e) {
    if (e.getSource() == valuelabelsbutton) {
      System.out.println ("got valuelabelsbutton action");
      if ((valuesframe == null) || valuesframe.closed) {
	/* ^--- thank goodness for short circuit evaluation */
	valuesframe = new DescEnumFrame (core, this, newvalues,
					namefield.getText(),
					new ValType ("string")
					);
      }
    }
    else if (e.getSource() == enumsbutton) {
      System.out.println ("got enumsbutton action");
      if ((enumsframe == null) || enumsframe.closed) {
	enumsframe = new DescEnumFrame (core, this, newenum,
					namefield.getText(),
					new ValType(typechoice.
						    getSelectedItem())
					);
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



