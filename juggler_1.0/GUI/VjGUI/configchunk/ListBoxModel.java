
package VjGUI.configchunk;

import javax.swing.*;
import javax.swing.event.*;
import java.util.Vector;

public class ListBoxModel implements ComboBoxModel  {

  Vector v;
  Object selected;

  ListBoxModel() {
    v = new Vector();
    selected = null;
  }

  public void addObject (Object a) {
    v.addElement(a);
  }

  public Object getSelectedItem() {
    return selected;
  }

  public void setSelectedItem (Object s) {
    selected = s;
  }

  public void addListDataListener (ListDataListener l) {
    //System.out.println ("addListDataListener not implemented yet");
  }

  public Object getElementAt (int i) {
    return v.elementAt(i);
  }

  public int getSize() {
    return v.size();
  }

  public void removeListDataListener (ListDataListener l) {
  }

}

