
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.io.*;

public class CfgFileRequester extends Dialog implements ActionListener,
TextListener, KeyListener {

  Frame parent;
  TextField f;
  Button cancelb;
  String directory;  // directory to list files from.
  String suffix;     // suffix displayed files should have.
  boolean addfile;   // if true, have a text box to add a name.
  List filelist;
  String retval;
  Panel pa;

  public CfgFileRequester (Frame p, String dir, String suf, boolean ad) {
    super (p, "File Request", true);
    parent = p;
    directory = dir;
    suffix = suf;
    addfile = ad;

    pa = new Panel();
    pa.setLayout(new BorderLayout(10,10));

    filelist = new List(10);
    pa.add("North", filelist);
    filelist.addActionListener(this);

    File d = new File (directory);
    String[] files = d.list();
    for (int i = 0; i < files.length; i++)
      filelist.add(files[i]);

    if (addfile) {
      f = new TextField ();
      f.addKeyListener(this);
      pa.add("Center",f);
    }
    cancelb = new Button ("Cancel");
    pa.add("South", cancelb);
    cancelb.addActionListener(this);

    add(pa);
    pack();

  }


  public String getFile() {
    return retval;
  }

  public void actionPerformed (ActionEvent e) {
    System.out.println ("action event");
    if (e.getSource() == cancelb) {
      retval = "";
    }
    else if (e.getSource() == filelist) {
      retval = filelist.getSelectedItem();
    }
    else if (e.getSource() == f) {
      retval = f.getText();
    }
    setVisible(false);
  }



  public void keyPressed(KeyEvent e) {}
  public void keyReleased(KeyEvent e) {
    System.out.println("Key event: code is " + e.getKeyCode());
    if (e.getKeyCode() == KeyEvent.VK_ENTER) {
       retval = f.getText();
       setVisible(false);
    }
} 
  public void keyTyped(KeyEvent e) {

  }




  public void textValueChanged (TextEvent e) {
    System.out.println ("textvalue changed");
    Object obj = e.getSource();
    System.out.println(obj.toString());
    //    retval = f.getText();
    //hide();
  }
}

  

  
    


