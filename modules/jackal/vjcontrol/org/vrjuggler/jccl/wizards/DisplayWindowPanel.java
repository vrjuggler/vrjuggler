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

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import VjWizards.*;
import VjConfig.*;
import VjControl.Core;
import VjComponents.UI.Widgets.*;
import VjComponents.ConfigEditor.ConfigUIHelper;

/**
 * Title:        VjControl Sim Wizard GUI
 * Description:
 * Copyright:    Copyright (c) 2001
 * Company:      VRAC
 * @author Christopher Just
 * @version 1.0
 */

public class DisplayWindowPanel extends JPanel implements WizardSubPanel, FocusListener, ActionListener {
    JPanel right_panel = new JPanel();
    BorderLayout borderLayout1 = new BorderLayout();
    GridLayout gridLayout1 = new GridLayout();
    JCheckBox input_check = new JCheckBox();
    JCheckBox stereo_check = new JCheckBox();
    JPanel jPanel2 = new JPanel();
    JPanel jPanel3 = new JPanel();
    JLabel jLabel1 = new JLabel();
    JComboBox viewmode_select = new JComboBox();
    JTextField interocular_field = new FloatTextField();
    JLabel jLabel2 = new JLabel();
    Box left_panel;
    WindowLayoutPanel window_layout_panel = new WindowLayoutPanel();
    JPanel jPanel4 = new JPanel();
    GridLayout gridLayout2 = new GridLayout();
    JLabel jLabel3 = new JLabel();
    JLabel jLabel4 = new JLabel();
    JLabel jLabel5 = new JLabel();
    JLabel jLabel6 = new JLabel();
    JLabel jLabel7 = new JLabel();
    JLabel jLabel8 = new JLabel();
    JLabel jLabel9 = new JLabel();
    JLabel jLabel10 = new JLabel();
    JTextField height_field = new IntegerTextField();
    JTextField ypos_field = new IntegerTextField();
    JTextField width_field = new IntegerTextField();
    JTextField xpos_field = new IntegerTextField();
    JPanel jPanel1 = new JPanel();

    boolean ui_initialized;
    boolean results_dirty;  // does results_db need to be updated from ui?

    ConfigChunkDB results_db;

    // current 'in-use' values...
    String window_name;
    int x, y, width, height;
    boolean input_source;
    boolean stereo;
    float interocular;
    int viewmode;  // 0 = 1st person; 1 = 3rd


    public DisplayWindowPanel() {
        results_db = null;
        results_dirty = false;
        ui_initialized = false;
        viewmode = 0;

        try {
            jbInit();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }
    private void jbInit() throws Exception {
        left_panel = Box.createVerticalBox();
        this.setLayout(borderLayout1);
        right_panel.setLayout(gridLayout1);
        gridLayout1.setRows(7);
        gridLayout1.setColumns(1);
        input_check.setText("Use as Input Source");
        input_check.setToolTipText("Should we register keystrokes and mouse movement on this window?");
        stereo_check.setText("Stereo View");
        stereo_check.setToolTipText("Should the window try to display in stereo?");
        jLabel1.setText("View mode");
        interocular_field.setToolTipText("Distance between the user\'s eyes.");
        interocular_field.setColumns(7);
        jLabel2.setText("Interocular Distance");
        viewmode_select.setToolTipText("Select a first-person (over-the-shoulder) view, or a third-person " +
    "view with a separate camera.");
        jPanel4.setLayout(gridLayout2);
        gridLayout2.setRows(4);
        gridLayout2.setColumns(3);
        gridLayout2.setVgap(3);
        jLabel3.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel3.setText("Position");
        jLabel4.setToolTipText("Width of display window.");
        jLabel4.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel4.setText("Width  ");
        jLabel6.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel6.setText("Height  ");
        jLabel7.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel7.setText("Size");
        jLabel8.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel8.setText("y  ");
        jLabel10.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel10.setText("x  ");
        height_field.setToolTipText("Height of display window.");
        xpos_field.setToolTipText("X-position of window\'s lower-left corner (from lower-left of screen)");
        xpos_field.setColumns(10);
        window_layout_panel.setMinimumSize(new Dimension(300, 100));
        window_layout_panel.setPreferredSize(new Dimension(200, 100));
        this.add(right_panel, BorderLayout.EAST);
        right_panel.add(input_check, null);
        right_panel.add(stereo_check, null);
        right_panel.add(jPanel2, null);
        jPanel2.add(jLabel2, null);
        jPanel2.add(interocular_field, null);
        //right_panel.add(jPanel3, null);
        //jPanel3.add(jLabel1, null);
        //jPanel3.add(viewmode_select, null);
        this.add(left_panel, BorderLayout.CENTER);
        left_panel.add(window_layout_panel, null);
        left_panel.add(jPanel4, null);
        jPanel4.add(jLabel3, null);
        jPanel4.add(jLabel10, null);
        jPanel4.add(xpos_field, null);
        jPanel4.add(jLabel9, null);
        jPanel4.add(jLabel8, null);
        jPanel4.add(ypos_field, null);
        jPanel4.add(jLabel7, null);
        jPanel4.add(jLabel4, null);
        jPanel4.add(width_field, null);
        jPanel4.add(jLabel5, null);
        jPanel4.add(jLabel6, null);
        jPanel4.add(height_field, null);
        left_panel.add(jPanel1, null);
    }

    public void setConfigUIHelper (ConfigUIHelper _uihelper_module) {
    }

    /** Sets the state of the UI components to reflect the resultsDB. 
     *  This is called when the ui is initialized or when a new input db
     *  is set.
     */
    private void setUIState () {
        // just for the moment, i'm gonna assume that setUIState hasn't
        // already been called... just to get the set code in place.
        ConfigChunk ch;
        java.util.List chunks;

        window_layout_panel.clear();
        window_layout_panel.setWindowScreenSize (java.awt.Toolkit.getDefaultToolkit().getScreenSize());

        viewmode = 0;

        chunks = results_db.getOfDescToken ("displayWindow");
        ch = (ConfigChunk)chunks.get(0);
        x = ch.getValueFromToken ("origin",0).getInt();
        y = ch.getValueFromToken ("origin",1).getInt();
        width = ch.getValueFromToken ("size",0).getInt();
        height = ch.getValueFromToken ("size",1).getInt();
        window_name = ch.getName();//ch.getValueFromToken ("name",0).getString();
        input_source = ch.getValueFromToken ("act_as_keyboard_device",0).getBoolean();
        stereo = ch.getValueFromToken ("stereo",0).getBoolean();

        ch = ch.getValueFromToken ("sim_viewports",0).getEmbeddedChunk();
        //view_mode = ch.getValueFromToken ("view",0).getInt();
        //view_mode = (view_mode == 2)?1:0;

        String user_name = ch.getValueFromToken ("user",0).getString();
        ch = results_db.get(user_name);
        interocular = ch.getValueFromToken ("interocular_distance",0).getFloat();

        // now that we got all that information from the chunkdb, let's
        // put it into our ui widgets.
        window_layout_panel.addWindow (window_name, x, y, width, height, Color.red);
        xpos_field.setText (Integer.toString(x));
        ypos_field.setText (Integer.toString(y));
        height_field.setText (Integer.toString(height));
        width_field.setText (Integer.toString(width));

        stereo_check.setSelected(stereo);
        input_check.setSelected(input_source);
        interocular_field.setText (Float.toString (interocular));
        viewmode_select.setSelectedIndex (viewmode);

        results_dirty = false;
    }

    //----------------------- WizardSubPanel Stuff --------------------------

    public JComponent getUIComponent () {
        return this;
    }

    public boolean initUIComponent () {
        if (!ui_initialized) {
            viewmode_select.addItem ("First-person");
            viewmode_select.addItem ("Third-person");

            // listen for these events so we can update the visual display of
            // the window.
            xpos_field.addFocusListener (this);
            ypos_field.addFocusListener (this);
            width_field.addFocusListener (this);
            height_field.addFocusListener (this);
            stereo_check.addFocusListener (this);
            input_check.addFocusListener (this);
            interocular_field.addFocusListener (this);
            viewmode_select.addFocusListener (this);

            xpos_field.addActionListener (this);
            ypos_field.addActionListener (this);
            width_field.addActionListener (this);
            height_field.addActionListener (this);
            stereo_check.addActionListener (this);
            input_check.addActionListener (this);
            interocular_field.addActionListener (this);
            viewmode_select.addActionListener (this);

            if (results_db != null)
                setUIState();
            
            ui_initialized = true;
        }
        return ui_initialized;
    }

    public File getHelpFile () {
        return null;
    }

    public boolean checkState () {
        return true;
    }

    public boolean setInitialDB (ConfigChunkDB db) {
        results_db = (ConfigChunkDB)db.clone();
        if (ui_initialized && (results_db != null))
            setUIState();
        return true;
    }

    public ConfigChunkDB getResultDB () {
        if (results_dirty && (results_db != null)) {
            // query the GUI widgets to update results_db
            java.util.List chunks;
            ConfigChunk ch;
            Property p;
            String window_name;

            chunks = results_db.getOfDescToken ("displayWindow");
            ch = (ConfigChunk)chunks.get(0);
            window_name = ch.getName();

            p = ch.getPropertyFromToken ("origin");
            p.setValue (new VarValue(x), 0);
            p.setValue (new VarValue(y), 1);

            p = ch.getPropertyFromToken ("size");
            p.setValue (new VarValue(width), 0);
            p.setValue (new VarValue(height), 1);

            ch.setName (window_name);

            p = ch.getPropertyFromToken ("act_as_keyboard_device");
            p.setValue (input_source, 0);
           
            p = ch.getPropertyFromToken ("stereo");
            p.setValue (stereo, 0);

            ch = ch.getValueFromToken ("sim_viewports", 0).getEmbeddedChunk();

            p = ch.getPropertyFromToken ("view");
            p.setValue (new VarValue((stereo==true)?3:1), 0);

            String user_name = ch.getValueFromToken ("user",0).getString();
            ch = results_db.get(user_name);
            p = ch.getPropertyFromToken ("interocular_distance");
            p.setValue (new VarValue (interocular), 0);

            // create or delete a keyboard proxy as needed
            ch = results_db.get (window_name + "_input");
            if (input_source) {
                if (ch == null) {
                    ch = ChunkFactory.createChunkWithDescToken ("KeyboardProxy");
                    ch.setName (window_name + "_input");
                    p = ch.getPropertyFromToken ("device");
                    p.setValue (window_name, 0);
                    results_db.add (ch);
                }
            }
            else {
                if (ch != null) 
                    results_db.remove (ch);
            }

            results_dirty = false;
        }
        return results_db;
    }


    public void reset () {
        results_db = null;
    }


    //------------------------ FocusListener Stuff -------------------------

    public void focusGained (FocusEvent e) {
        ;
    }

    public void focusLost (FocusEvent e) {
        fieldChanged (e.getSource());
    }

    //----------------------- ActionListener Stuff -------------------------

    public void actionPerformed (ActionEvent e) {
        fieldChanged (e.getSource());
    }

    //----------------------- Focus/Action Utilities -----------------------


    /** Called when a field sends ActionEvent or FocusLost.
     *  Note that this means the field _may_ have changed; we should check
     *  before acting.
     */
    private void fieldChanged (Object source) {
        if (source == xpos_field) {
            int newxpos = Integer.parseInt (xpos_field.getText());
            if (newxpos != x) {
                if (newxpos < 0) {
                    Core.consoleErrorMessage ("Wizard", "Invalid X position.");
                    xpos_field.setText (Integer.toString(x));
                }
                else {
                    window_layout_panel.moveWindow (window_name, newxpos, y, width, height);
                    x = newxpos;
                    results_dirty = true;
                }
            }
        }
        else if (source == ypos_field) {
            int newypos = Integer.parseInt (ypos_field.getText());
            if (newypos != y) {
                if (newypos < 0) {
                    Core.consoleErrorMessage ("Wizard", "Invalid Y position.");
                    ypos_field.setText (Integer.toString(y));
                }
                else {
                    y = newypos;
                    window_layout_panel.moveWindow (window_name, x, y, width, height);
                    results_dirty = true;
                }
            }
        }
        else if (source == width_field) {
            int newwidth = Integer.parseInt (width_field.getText());
            if (newwidth != width) {
                if (newwidth <= 0) {
                    Core.consoleErrorMessage ("Wizard", "Invalid width.");
                    width_field.setText (Integer.toString(width));
                }
                else {
                    width = newwidth;
                    window_layout_panel.moveWindow (window_name, x, y, width, height);
                    results_dirty = true;
                }
            }
        }
        else if (source == height_field) {
            int newheight = Integer.parseInt (height_field.getText());
            if (newheight != height) {
                if (newheight <= 0) {
                    Core.consoleErrorMessage ("Wizard", "Invalid height.");
                    height_field.setText (Integer.toString(height));
                }
                else {
                    height = newheight;
                    window_layout_panel.moveWindow (window_name, x, y, width, height);
                    results_dirty = true;
                }
            }
        }
        else if (source == stereo_check) {
            boolean newstereo = (stereo_check.getSelectedObjects() != null);
            if (newstereo != stereo) {
                stereo = newstereo;
                results_dirty = true;
            }
        }
        else if (source == input_check) {
            boolean newinput = (input_check.getSelectedObjects() != null);
            if (newinput != input_source) {
                input_source = newinput;
                results_dirty = true;
            }
        }
        else if (source == interocular_field) {
            try {
                float newinterocular = Float.parseFloat (interocular_field.getText());
                if (newinterocular != interocular) {
                    if (newinterocular < 0.0f) {
                        Core.consoleErrorMessage ("Wizard", "Invalid interocular distance.");
                        interocular_field.setText(Float.toString(interocular));
                    }
                    else {
                        interocular = newinterocular;
                        results_dirty = true;
                    }
                }
            }
            catch (Exception e) {
                Core.consoleErrorMessage ("Wizard", "Invalid interocular distance.");
                interocular_field.setText(Float.toString(interocular));
            }
        }
        else if (source == viewmode_select) {
            int newviewmode = viewmode_select.getSelectedIndex();
            if (newviewmode != viewmode) {
                viewmode = newviewmode;
                results_dirty = true;
            }
        }
    }


}
