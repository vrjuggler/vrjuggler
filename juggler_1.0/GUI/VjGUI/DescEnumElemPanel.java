
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import VjConfig.DescEnum;
import VjConfig.ValType;
import VjConfig.ChunkDesc;

/* DescEnumElemPanel - a small bordered panel for editing a single entry
 * in the enums vector.  Exactly how this ought to look depends on the
 * type of the Property these enums are for:
 *
 *  t_int, t_float: TextField for name, TextField for val
 *  t_string: TextField for name
 *  t_chunk: Choice with names of available chunks
 *
 * the more cool I make this GUI, the more it gets all interconnected together
 * <sigh>.  I guess sometimes you really do want global variables ;)
 */
public class DescEnumElemPanel extends BorderedPanel implements MouseListener {

  TextField namef;
  Choice namechoice;
  TextField valf;
  boolean selected;

  //  PropertyDesc desc;
  ClientGlobals core;

  /* I'm going to let the parent frame deal with building the actual desc
   * enum & catch any possible numberformat exceptions.  we'll just return
   * bits and pieces.
   */
  public String getName() {
    if (namechoice != null)
      return namechoice.getSelectedItem();
    else
      return namef.getText();
  }
  public int getIntVal() throws NumberFormatException {
    if (valf == null)
      return 0;
    else
      return Integer.parseInt(valf.getText());
  }
  public float getFloatVal() throws NumberFormatException {
    if (valf == null)
      return 0;
    else 
      return Float.valueOf(valf.getText()).floatValue();
  }


  private void addLabel (String s) {
    /* just a convenience used below */
    Label l = new Label (s);
    l.addMouseListener(this);
    add (l);
  }

  public DescEnumElemPanel (DescEnum e, ValType t, ClientGlobals c) {
    super(4,4,3,2);
    core = c;
    selected = false;
    addMouseListener (this);

    namechoice = null;
    valf = null;
    namef = null;

    /* next bit is specific on valtype of the propertydesc */
    if (t.equals(ValType.t_string)) {
      addLabel ("Name: ");
      namef = new TextField (e.str, 20);
      add (namef);
    }
    else if (t.equals(ValType.t_chunk)) {
      addLabel ("Accept chunks of type: ");
      namechoice = new Choice();
      for (int i = 0; i < core.descs.size(); i++) 
	namechoice.add(((ChunkDesc)core.descs.elementAt(i)).name);
      add (namechoice);
      namechoice.select(core.descs.getNameFromToken(e.str));
    }
    else {
      addLabel ("Name: ");
      namef = new TextField (e.str, 20);
      add (namef);
      addLabel ("Value: ");
      valf = new TextField (e.val.toString(), 20);
      add (valf);
    }

  }

  public DescEnumElemPanel (ValType t, ClientGlobals c) {
    super(4,4,BorderedPanel.RAISED ,2);
    selected = false;
    core = c;
    addMouseListener (this);

    /* next bit is specific on valtype of the propertydesc */
    if (t.equals(ValType.t_string)) {
      addLabel ("Name: ");
      namef = new TextField ("", 20);
      add (namef);
    }
    else if (t.equals(ValType.t_chunk)) {
      addLabel ("Accept chunks of type: ");
      namechoice = new Choice();
      for (int i = 0; i < core.descs.size(); i++) 
	namechoice.add(((ChunkDesc)core.descs.elementAt(i)).name);
      add (namechoice);
    }
    else {
      addLabel ("Name: ");
      namef = new TextField ("", 20);
      add (namef);
      addLabel ("Value: ");
      valf = new TextField ("", 20);
      add (valf);
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
    setSelected (!selected);
  }

  public boolean getSelected() {
    return selected;
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




