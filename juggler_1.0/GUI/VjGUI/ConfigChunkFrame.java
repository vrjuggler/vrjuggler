
package VjGUI;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjGUI.PropertyPanel;
import VjGUI.BorderedPanel;
import VjConfig.ConfigChunk;
import VjConfig.VarValue;
import VjConfig.Property;

public class ConfigChunkFrame extends Frame 
implements ActionListener, WindowListener {

  ClientGlobals core;
  ConfigChunk   chunk;
  Vector proppanels; // property description panels.
  Panel properties;
  Button cancelbutton;
  Button okbutton;
  TextField namef;
    TextField helpfield;
  ScrollPane sp;
  ConfigChunkDBPanel parent;

GridBagLayout playout;
GridBagConstraints pconstraints;

  public ConfigChunkFrame (ClientGlobals c, ConfigChunkDBPanel par, ConfigChunk ch) {
    super("Edit " + ch.descName() + ": " + ch.chunkName());
    Panel northpanel, southpanel, centerpanel;

    proppanels = new Vector();
    PropertyPanel t;

    setFont (c.ui.windowfont);
    
    core = c;
    parent = par;
    chunk = ch;
    Panel mainp = new BorderedPanel(5,5);
    mainp.setLayout (new BorderLayout (5,5));
    playout = new GridBagLayout();
    pconstraints = new GridBagConstraints();
    //mainp.setLayout (playout);

    add (mainp);
    //pconstraints.gridwidth = GridBagConstraints.REMAINDER;
    pconstraints.fill = GridBagConstraints.HORIZONTAL;

    northpanel = new Panel();
    northpanel.setLayout(playout);
    Label l = new Label ("Instance Name: ");
    playout.setConstraints (l, pconstraints);
    northpanel.add(l);
    namef = new TextField (chunk.chunkName(), 25);
    pconstraints.gridwidth = GridBagConstraints.REMAINDER;
    playout.setConstraints (namef, pconstraints);
    northpanel.add(namef);

    helpfield = new TextField (chunk.desc.help);
    helpfield.setEditable (false);
    playout.setConstraints (helpfield, pconstraints);
    northpanel.add (helpfield);

    mainp.add(northpanel, "North");

    sp = new ScrollPane(ScrollPane.SCROLLBARS_ALWAYS);
    sp.getVAdjustable().setUnitIncrement(20);
    sp.getHAdjustable().setUnitIncrement(20);
    mainp.add(sp,"Center");
    properties = new Panel ();

    GridBagLayout propertieslayout = new GridBagLayout ();
    properties.setLayout(propertieslayout);
    GridBagConstraints constraints = new GridBagConstraints();
    constraints.gridwidth = GridBagConstraints.REMAINDER;
    constraints.fill = GridBagConstraints.HORIZONTAL;

    sp.add(properties);

    // make property description panels
    for (int i = 0; i < chunk.props.size(); i++) {
      t = new PropertyPanel ((Property)chunk.props.elementAt(i), this, core);
      proppanels.addElement(t);
      propertieslayout.setConstraints(t,constraints);
      properties.add(t);
    }

    southpanel = new Panel();
    okbutton = new Button ("  OK  ");
    okbutton.addActionListener (this);
    southpanel.add (okbutton);
    cancelbutton = new Button ("Cancel");
    cancelbutton.addActionListener (this);
    southpanel.add (cancelbutton);
    mainp.add(southpanel, "South");

    setSize(470,700);

    validate();
    addWindowListener(this);
    show();
  }

  public String chunkName () {
    return chunk.chunkName();
  }

  public ConfigChunk getValue() {
    /* returns a configchunk based on the values current in this window */
    ConfigChunk c = new ConfigChunk (chunk.desc, namef.getText());
    c.props.removeAllElements();
    for (int i = 0; i < chunk.props.size(); i++) {
      c.props.addElement (((PropertyPanel)proppanels.elementAt(i)).getValue());
    }
    return c;
  }

  public void closeFrame() {
    parent.closedChunkFrame(this);
    dispose();
  }

  public void actionPerformed (ActionEvent e) {
    ConfigChunk newc;
    if (e.getSource() == cancelbutton) {
      closeFrame();
    }
    else if (e.getSource() == okbutton) {
      newc = getValue();
      core.chunks.replace(chunk,newc);
      parent.update();
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












