

package VjWizards;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.URL;
import java.util.*;

import VjWizards.*;
import VjConfig.*;
import VjComponents.UI.*;
import VjComponents.ConfigEditor.ConfigModule;
import VjControl.Core;

public class WizardPanel extends JPanel implements PlugPanel, ActionListener {

    protected String component_name;
    protected ConfigChunk component_chunk;
    protected boolean ui_initialized;

    protected java.util.List subpanels;
    protected java.util.List subpanel_gfx;
    protected java.util.List subpanel_labels;

    protected int current_subpanel_index;

    protected JLabel current_gfxpanel;
    protected WizardSubPanel current_subpanel;

    protected JButton help_button;
    protected JButton prev_button;
    protected JButton next_button;
    protected JButton finished_button;

    protected ConfigChunkDB initial_chunkdb;

    public WizardPanel () {
        super();
        component_name = "Unconfigured WizardPanel";
        component_chunk = null;
        ui_initialized = false;

        current_gfxpanel = null;
        current_subpanel = null;
        current_subpanel_index = 0;

        subpanels = new ArrayList();
        subpanel_gfx = new ArrayList();
        subpanel_labels = new ArrayList();

        initial_chunkdb = null;
    }

    public boolean addSubPanel (WizardSubPanel p, ImageIcon gfx, String label) {
        if (subpanels.isEmpty())
            p.setInitialDB (initial_chunkdb);
        subpanels.add(p);
        subpanel_gfx.add(gfx);
        subpanel_labels.add (label);

        return true;
    }

    public boolean configure (ConfigChunk ch) {
        component_chunk = ch;
        component_name = ch.getName();

        return true;
    }


    public void setButtonStates () {
        if (current_subpanel_index == -1) {
            help_button.setEnabled(false);
            prev_button.setEnabled(false);
            next_button.setEnabled(false);
            finished_button.setEnabled(false);
        }
        else {
            help_button.setEnabled(true);
            prev_button.setEnabled(current_subpanel_index > 0);
            next_button.setEnabled(current_subpanel_index < subpanels.size()-1);
            finished_button.setEnabled(current_subpanel_index == subpanels.size()-1);
        }
    }


    /********************** PlugPanel Stuff **************************/

    public String getComponentName () {
        return component_name;
    }


    public ImageIcon getComponentIcon () {
        return null;
    }


//      public boolean configure (ConfigChunk ch) {
//          component_chunk = ch;
//          component_name = ch.getName();
//          return true;
//      }

    
    public ConfigChunk getConfiguration () {
        return component_chunk;
    }


    public boolean addConfig (ConfigChunk ch) {
        return false;
    }


    public boolean removeConfig (String name) {
        return false;
    }


    public JComponent getUIComponent() {
        return this;
    }

    public boolean initUIComponent() {
        if (!ui_initialized) {
//          if (textarea == null) {
//              setLayout (new BorderLayout (5, 5));
//              textarea = new JTextArea (tmpbuffer.toString());
//              textarea.setEditable (false);
//              add (sp = new JScrollPane (textarea), "Center");
//          }

            // really the first thing we need to do here is get the chunkdb
            // which will form the basis of all our defaults...
            initial_chunkdb = new ConfigChunkDB();
            try {
                URL init_url = ClassLoader.getSystemResource ("VjWizards/data/SimWizardDB.config");
                ConfigIO.readConfigChunkDB (init_url.openStream(), 
                                            initial_chunkdb, ConfigIO.GUESS);  
            }
            catch (IOException e) {
                return false;
            }

            setLayout (new BorderLayout (5,5));

            // buttons panel
            JPanel p = new JPanel();
            p.setLayout (new FlowLayout (FlowLayout.TRAILING));
            help_button = new JButton ("  Help  ");
            prev_button = new JButton ("Previous");
            next_button = new JButton ("  Next  ");
            finished_button = new JButton ("Finished");
            p.add (help_button);
            p.add (prev_button);
            p.add (next_button);
            p.add (finished_button);
            add (p, "South");

            // create subpanels...
            ImageIcon icon = new ImageIcon (ClassLoader.getSystemResource("VjWizards/gfx/corner.png"));
            //addSubPanel (new BeginnerSubPanel1(), new ImageIcon (ClassLoader.getSystemResource("VjWizards/gfx/beginner_1.png")));
            addSubPanel (new DisplayWindowPanel(), icon, "Step 1: Display Window");
            addSubPanel (new InputWindowPanel(), icon, "Step 2: Input Windows");
            addSubPanel (new InputDevicePanel(), icon, "Step 3: Input Devices");
            addSubPanel (new MiscInfoPanel(), icon, "Step 4: Final Setup");
            addSubPanel (new SavePanel(), icon, "Step 5: Save File");
            
            // init and display first subpanel...
            setSubPanel (0);

            //setButtonStates();
            
            // setup event handling
            help_button.addActionListener (this);
            prev_button.addActionListener (this);
            next_button.addActionListener (this);
            finished_button.addActionListener (this);

            ui_initialized = true;
        }
        return ui_initialized;
    }

    public void setSubPanel (int i) {
        if (current_gfxpanel == null) {
            current_gfxpanel = new JLabel ("", JLabel.LEADING);
            current_gfxpanel.setBackground (Color.black);
            current_gfxpanel.setHorizontalAlignment (JLabel.LEFT);
            current_gfxpanel.setFont(new Font ("Helvetica", Font.ITALIC, 24));
            current_gfxpanel.setForeground (Color.white);
            //add (current_gfxpanel, "North");
            JPanel foo = new JPanel();
            foo.setLayout (new GridLayout (1,1));
            foo.add (current_gfxpanel);
            foo.setBackground (Color.black);
            add (foo, "North");
        }
        if (current_subpanel != null) {
            remove (current_subpanel.getUIComponent());
        }
        WizardSubPanel old_subpanel = current_subpanel;
        //current_north = new JLabel ((ImageIcon)subpanel_gfx.get(i));
        current_gfxpanel.setIcon ((ImageIcon)subpanel_gfx.get(i));
        current_gfxpanel.setText ((String)subpanel_labels.get(i));
        current_subpanel = (WizardSubPanel)subpanels.get(i);
        current_subpanel.initUIComponent();
//         if (i == 0)
//             current_subpanel.setInitialDB (initial_chunkdb);
        if (i > current_subpanel_index) {
            current_subpanel.setInitialDB (old_subpanel.getResultDB());
        }
        add (current_subpanel.getUIComponent(), "Center");
        current_subpanel_index = i;
        setButtonStates();
        validate();
        repaint();
    }

    public void destroy () {
        //Core.removeLogMessageListener(this);
    }


    public void rebuildDisplay () {
    }

    //----------------------- ActionListener Stuff --------------------------

    public void actionPerformed (ActionEvent e) {
        Object o = e.getSource();
        if (o == next_button) {
            if (current_subpanel.checkState()) {
                setSubPanel (current_subpanel_index+1);
            }
        }
        else if (o == prev_button) {
            setSubPanel (current_subpanel_index-1);
        }
        else if (o == help_button) {
        }
        else if (o == finished_button) {
            if (current_subpanel.checkState()) {
                // copy results db into config module...

                ConfigChunkDB results = current_subpanel.getResultDB();
                System.out.println(results.xmlRep());

                // this is inexcusable cheating, but i'm in a terrible hurry
                // BUG BUG BUG BUG BUG BUG BUG BUG!!! CJ LOOK HERE!!!
                ConfigModule config_module;
                config_module = (ConfigModule)Core.getComponentFromRegistry ("Config Module");
                config_module.addChunkDB (results);

                // reset the wizard
                for (int i = 0; i < subpanels.size(); i++)
                    ((WizardSubPanel)subpanels.get(i)).reset();

                ((WizardSubPanel)subpanels.get(0)).setInitialDB 
                    (initial_chunkdb);
                setSubPanel (0);
            }
        }
    }
}
