
package VjGUI.chunkdesc;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import VjConfig.*;
import VjGUI.*;
import VjGUI.chunkdesc.*;
import VjGUI.util.*;

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
public class DescEnumElemPanel extends JPanel implements MouseListener {

  static AbstractBorder select_border=null;
  static AbstractBorder unselect_border=null;

  JTextField namef;
  JComboBox namechoice;
  JTextField valf;
  boolean selected;


  /* I'm going to let the parent frame deal with building the actual desc
   * enum & catch any possible numberformat exceptions.  we'll just return
   * bits and pieces.
   */
  public String getName() {
    if (namechoice != null)
      return (String)(namechoice.getSelectedItem());
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
    JLabel l = new JLabel (s);
    l.addMouseListener(this);
    add (l);
  }



  public DescEnumElemPanel (DescEnum e, ValType t) {
    super();

    if (select_border == null) {
      select_border = new CompoundBorder ( new BevelBorder (BevelBorder.LOWERED),
					   new EmptyBorder (5,5,5,5));
    }
    if (unselect_border == null) {
      unselect_border = new CompoundBorder ( new BevelBorder (BevelBorder.RAISED),
					     new EmptyBorder (5,5,5,5));
    }
    
    setBorder (unselect_border);

    int i, j;
    ChunkDescDB db;

    selected = false;
    addMouseListener (this);

    namechoice = null;
    valf = null;
    namef = null;

    /* next bit is specific on valtype of the propertydesc */
    if (t.equals(ValType.t_string)) {
      addLabel ("Name: ");
      namef = new JTextField (e.str, 20);
      add (namef);
    }
    else if (t.equals(ValType.t_chunk)) {
      addLabel ("Accept chunks of type: ");
      namechoice = new JComboBox();
      for (i = 0; i < Core.descdbs.size(); i++) {
	db = (ChunkDescDB)Core.descdbs.elementAt(i);
	for (j = 0; j < db.size(); j++)
	  namechoice.addItem(((ChunkDesc)db.elementAt(j)).name);
      }
      add (namechoice);
      namechoice.setSelectedItem(e.str);
    }
    else {
      addLabel ("Name: ");
      namef = new JTextField (e.str, 20);
      add (namef);
      addLabel ("Value: ");
      if (t.equals(ValType.t_int))
	valf = new IntegerTextField (e.val.toString(), 10);
      else if (t.equals(ValType.t_float))
	valf = new FloatTextField (e.val.toString(), 10);
      else
	valf = new StringTextField (e.val.toString(), 10);
      add (valf);
    }

  }

  public DescEnumElemPanel (ValType t) {
    super();

    int i, j;
    ChunkDescDB db;

    if (select_border == null) {
      //border = new BevelBorder(BevelBorder.LOWERED);
      select_border = new CompoundBorder ( new BevelBorder (BevelBorder.LOWERED),
				    new EmptyBorder (5,5,5,5));
    }
    if (unselect_border == null) {
      //border = new BevelBorder(BevelBorder.RAISED);
      unselect_border = new CompoundBorder ( new BevelBorder (BevelBorder.RAISED),
				    new EmptyBorder (5,5,5,5));
    }
    
    setBorder (unselect_border);

    selected = false;
    addMouseListener (this);

    /* next bit is specific on valtype of the propertydesc */
    if (t.equals(ValType.t_string)) {
      addLabel ("Name: ");
      namef = new JTextField ("", 20);
      add (namef);
    }
    else if (t.equals(ValType.t_chunk)) {
      addLabel ("Accept chunks of type: ");
      namechoice = new JComboBox();
      for (i = 0; i < Core.descdbs.size(); i++) {
	db = (ChunkDescDB)Core.descdbs.elementAt(i);
	for (j = 0; j < db.size(); j++)
	  namechoice.addItem(((ChunkDesc)db.elementAt(j)).name);
      }
      add (namechoice);
    }
    else {
      addLabel ("Name: ");
      namef = new JTextField ("", 20);
      add (namef);
      addLabel ("Value: ");
      if (t.equals(ValType.t_int))
	valf = new /*int*/JTextField (10);
      else if (t.equals(ValType.t_float))
	valf = new /*Float*/JTextField (10);
      else
	valf = new /*Integer*/JTextField (10);

      add (valf);
    }

  }


  public void setSelected(boolean v) {
    selected = v;
    if (selected)
      super.setBorder(select_border);
    else
      super.setBorder(unselect_border);
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




