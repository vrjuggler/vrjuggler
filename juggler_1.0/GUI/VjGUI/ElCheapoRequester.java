
package VjGUI;

import java.awt.*;
import java.awt.event.*;

public class ElCheapoRequester extends Dialog implements ActionListener {

  Frame parent;
  TextField f;
  Button okb;

  public ElCheapoRequester (Frame p) {
    super (p, "File Request", true);
    parent = p;

    add (new Label ("Enter file name:"), "North");
    f = new TextField ();
    add(f,"Center");
    okb = new Button ("OK");
    add(okb, "South");
    okb.addActionListener(this);

    setSize(200,100);
  }

  public String getFile() {
    return f.getText();
  }

  public void actionPerformed (ActionEvent e) {
    setVisible(false);
  }

}

  

  
    

