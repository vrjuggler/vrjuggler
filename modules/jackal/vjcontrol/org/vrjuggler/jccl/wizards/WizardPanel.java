/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


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
import VjComponents.ConfigEditor.ConfigUIHelper;
import VjControl.*;


public class WizardPanel extends JPanel implements PlugPanel, ActionListener {

    protected String component_name;
    protected ConfigChunk component_chunk;
    protected boolean ui_initialized;

    protected ConfigUIHelper uihelper_module;

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

        uihelper_module = null;

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
        p.setConfigUIHelper (uihelper_module);
        subpanels.add(p);
        subpanel_gfx.add(gfx);
        subpanel_labels.add (label);

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


    public void setComponentName (String _name) {
        component_name = _name;
    }


    public ImageIcon getComponentIcon () {
        return null;
    }

    
    public ConfigChunk getConfiguration () {
        return component_chunk;
    }


    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        component_chunk = ch;
        component_name = ch.getName();

        // get pointers to the modules we need.
        Property p = ch.getPropertyFromToken ("Dependencies");
        if (p != null) {
            int i;
            int n = p.getNum();
            String s;
            VjComponent c;
            for (i = 0; i < n; i++) {
                s = p.getValue(i).toString();
                c = Core.getVjComponent (s);
                if (c != null) {
                    if (c instanceof ConfigUIHelper)
                        uihelper_module = (ConfigUIHelper)c;
                }
            }
        }
    }


    public void initialize () throws VjComponentException {
        if (uihelper_module == null)
            throw new VjComponentException (component_name + ": Initialized with unmet dependencies.");
    }


    public VjComponent addConfig (ConfigChunk ch) throws VjComponentException {
        throw new VjComponentException (component_name + " does not support child component: " + ch.getName());
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
                config_module = (ConfigModule)Core.getVjComponent ("Config Module");
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
