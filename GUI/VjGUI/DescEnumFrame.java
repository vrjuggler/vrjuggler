
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.DescEnum;
import VjConfig.ValType;
import VjGUI.BorderedPanel;

public class DescEnumFrame extends Frame 
  implements ActionListener, WindowListener {

  //  PropertyDesc desc;
  ClientGlobals core;
  ValType pdtype; // current type of the associated propertydescpanel

  PropertyDescPanel par;
  BorderedPanel panel;
  Panel sppanel;  /* sppanel is the panel inside the scrollpane...*/
  Button insertbutton;
  Button removebutton;
  Button okbutton;
  Button cancelbutton;
  GridBagLayout splayout;
  GridBagConstraints spconstraints;
  Vector elempanels; // vector of DescEnumElemPanels
  Vector data;  // vector of DescEnum
  public boolean closed;

  //  public DescEnumFrame (PropertyDescPanel p, PropertyDesc d,
  //			ClientGlobals c) {
  public DescEnumFrame (ClientGlobals c,
			PropertyDescPanel p,
			Vector _data,
			String pdn,
			ValType pdt) {
    super(pdn);

    closed = false;
    core = c;
    pdtype = pdt;
    par = p;

    setFont(core.ui.windowfont);

    Panel buttonspanel = new Panel();

    data = _data;

    elempanels = new Vector();
    panel = new BorderedPanel(10,5);
    panel.setLayout (new BorderLayout (10,2));
    add(panel);
    ScrollPane sp = new ScrollPane(ScrollPane.SCROLLBARS_ALWAYS);
    sp.getVAdjustable().setUnitIncrement(20);
    sp.getHAdjustable().setUnitIncrement(20);
    sppanel = new Panel();

    splayout = new GridBagLayout ();
    sppanel.setLayout(splayout);
    spconstraints = new GridBagConstraints();
    spconstraints.gridwidth = GridBagConstraints.REMAINDER;
    spconstraints.fill = GridBagConstraints.HORIZONTAL;

    sp.add(sppanel);
    panel.add (sp, "Center");

    insertbutton = new Button ("Insert");
    insertbutton.addActionListener(this);
    buttonspanel.add (insertbutton);
    removebutton = new Button ("Remove");
    removebutton.addActionListener(this);
    buttonspanel.add (removebutton);
    okbutton = new Button ("OK");
    okbutton.addActionListener(this);
    buttonspanel.add (okbutton);
    cancelbutton = new Button ("Cancel");
    cancelbutton.addActionListener(this);
    buttonspanel.add (cancelbutton);
    panel.add (buttonspanel, "South");

    setSize (450,270);
    makeItems();
    addWindowListener(this);
    show();
  }

  public void closeFrame() {
    /* closes this frame & informs it's parent - doesn't save any data or
     * anything.
     */
    closed = true;
    dispose();
  }

  private int makeItems() {
    /* fills up the scrollpane with all the items in enums, which is a vector
     * of DescEnum
     */
    int ypos = 0;
    DescEnumElemPanel p;
    for (int i = 0; i < data.size(); i++) {
      p = new DescEnumElemPanel((DescEnum)data.elementAt(i),
				pdtype, core);
      elempanels.addElement(p);
      splayout.setConstraints(p,spconstraints);
      sppanel.add(p);
    }
    sppanel.validate();
    return data.size();
  }


  public void actionPerformed (ActionEvent e) {
    DescEnumElemPanel p;
    int unused,j;
    float k;
    String s;
    if (e.getSource() == insertbutton) {
      p = new DescEnumElemPanel(pdtype,core);
      elempanels.addElement(p);
      splayout.setConstraints(p,spconstraints);
      sppanel.add(p);
      validate();
    }
    else if (e.getSource() == removebutton) {
      for (int i = 0; i < elempanels.size(); ) {
	p = (DescEnumElemPanel)elempanels.elementAt(i);
	if (p.getSelected()) {
	  Panel par = (Panel)p.getParent();
	  par.remove(p);
	  elempanels.removeElementAt(i);
	}
	else
	  i++;
      }
      validate();
    }
    else if (e.getSource() == cancelbutton) {
      closeFrame();
    }
    else if (e.getSource() == okbutton) {
      unused = 0;
      data.removeAllElements();
      for (int i = 0; i < elempanels.size(); i++) {
	p = (DescEnumElemPanel)elempanels.elementAt(i);
	if (pdtype.equals (ValType.t_chunk))
	    s = core.descs.getTokenFromName (p.getName());
	else
	    s = p.getName();
	if (s.equals (""))
	  continue; /* no sense adding an empty label */

	if (pdtype.equals (ValType.t_int)) {
	  try {
	    j = p.getIntVal();
	    unused = (unused>j)?(unused):(j+1);
	  }
	  catch (NumberFormatException ne) {
	    j = unused++;
	  }
	  data.addElement(new DescEnum (s,j));
	}
	else if (pdtype.equals (ValType.t_float)) {
	  try {
	    k = p.getFloatVal();
	    unused = (unused>k)?(unused):((new Float(k)).intValue() +1);
	  }
	  catch (NumberFormatException ne) {
	    k = unused++;
	  }
	  data.addElement(new DescEnum (s,k));
	}
	else if (pdtype.equals (ValType.t_string) ||
		 pdtype.equals (ValType.t_chunk)) {
	  data.addElement(new DescEnum (s,0));
	}
      }
      closeFrame();
    }

  }


  /* WindowListener Stuff */
  public void windowActivated(WindowEvent e) {}
  public void windowClosed(WindowEvent e) {}
  public void windowClosing(WindowEvent e) {
    closeFrame();
  }
  public void windowDeactivated(WindowEvent e) {}
  public void windowDeiconified(WindowEvent e) {}
  public void windowIconified(WindowEvent e) {}
  public void windowOpened(WindowEvent e) {}



}




