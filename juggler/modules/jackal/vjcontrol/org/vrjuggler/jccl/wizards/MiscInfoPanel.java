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

import java.io.*;
import java.awt.*;
import javax.swing.*;
import VjWizards.*;
import VjConfig.*;
import VjComponents.UI.Widgets.*;

/**
 * Title:        VjControl Sim Wizard GUI
 * Description:
 * Copyright:    Copyright (c) 2001
 * Company:      VRAC
 * @author Christopher Just
 * @version 1.0
 */

public class MiscInfoPanel 
    extends JPanel 
    implements WizardSubPanel {

    GridBagLayout gridBagLayout1 = new GridBagLayout();
    JCheckBox em_checkbox = new JCheckBox();
    JLabel jLabel1 = new JLabel();
    JTextField em_port_field = new IntegerTextField();
    JLabel jLabel2 = new JLabel();
    JLabel jLabel3 = new JLabel();
    JLabel jLabel4 = new JLabel();
    JTextField head_model_field = new JTextField();
    JLabel jLabel5 = new JLabel();
    JTextField wand_model_field = new JTextField();
    JLabel jLabel6 = new JLabel();

    boolean ui_initialized;
    boolean results_dirty;  // does results_db need to be updated from ui?

    ConfigChunkDB results_db;

    public MiscInfoPanel() {
        results_db = null;
        results_dirty = false;
        ui_initialized = false;

        try {
            jbInit();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }
    private void jbInit() throws Exception {
        this.setBorder (BorderFactory.createEmptyBorder (10,10,10,10));
        em_checkbox.setText("Allow Connections");
        this.setLayout(gridBagLayout1);
        jLabel1.setText("            Listen on port: ");
        em_port_field.setText("jTextField1");
        jLabel2.setText("Model files (for Performer apps):");
        jLabel3.setText("Dynamic Reconfiguration:");
        jLabel4.setText("Head Model: ");
        head_model_field.setText("jTextField2");
        jLabel5.setText("Wand Model: ");
        wand_model_field.setText("jTextField3");
        jLabel6.setText(" ");
        this.add(jLabel1, new GridBagConstraints(0, 2, 1, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
        this.add(jLabel2, new GridBagConstraints(0, 3, 2, 1, 0.0, 0.2
            ,GridBagConstraints.SOUTHWEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 82), 0, 0));
        this.add(em_port_field, new GridBagConstraints(1, 2, 1, 1, 1.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
        this.add(jLabel3, new GridBagConstraints(0, 0, 1, 1, 0.0, 0.2
            ,GridBagConstraints.SOUTHWEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
        this.add(em_checkbox, new GridBagConstraints(1, 1, 1, 1, 0.0, 0.0
            ,GridBagConstraints.WEST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
        this.add(jLabel4, new GridBagConstraints(0, 4, 1, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
        this.add(head_model_field, new GridBagConstraints(1, 4, 1, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
        this.add(jLabel5, new GridBagConstraints(0, 5, 1, 1, 0.0, 0.0
            ,GridBagConstraints.EAST, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
        this.add(wand_model_field, new GridBagConstraints(1, 5, 1, 1, 0.0, 0.0
            ,GridBagConstraints.CENTER, GridBagConstraints.HORIZONTAL, new Insets(0, 0, 0, 0), 0, 0));
        this.add(jLabel6, new GridBagConstraints(0, 6, 1, 1, 0.0, 0.2
            ,GridBagConstraints.CENTER, GridBagConstraints.NONE, new Insets(0, 0, 0, 0), 0, 0));
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

        chunks = results_db.getOfDescToken ("EnvironmentManager");
        ch = (ConfigChunk)chunks.get(0);

        em_checkbox.setSelected (ch.getValueFromToken("AcceptConnections",0).getBoolean());

        em_port_field.setText (Integer.toString(ch.getValueFromToken("Port",0).getInt()));

        chunks = results_db.getOfDescToken ("apiPerformer");
        ch = (ConfigChunk)chunks.get(0);

        head_model_field.setText (ch.getValueFromToken("SimHeadModel",0).getString());

        wand_model_field.setText (ch.getValueFromToken("SimWandModel",0).getString());

        results_dirty = false;
    }


    //----------------------- WizardSubPanel Stuff --------------------------

    public JComponent getUIComponent () {
        return this;
    }

    public boolean initUIComponent () {
        if (!ui_initialized) {
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
        if (results_db != null) {
        //if (results_dirty && (results_db != null)) {
            // query the GUI widgets to update results_db
            java.util.List chunks;
            ConfigChunk ch;
            Property p;
            String window_name;

            chunks = results_db.getOfDescToken ("EnvironmentManager");
            ch = (ConfigChunk)chunks.get(0);

            p = ch.getPropertyFromToken ("AcceptConnections");
            p.setValue (em_checkbox.getSelectedObjects() != null, 0);

            p = ch.getPropertyFromToken ("Port");
            p.setValue (new VarValue (Integer.parseInt (em_port_field.getText())), 0);

            chunks = results_db.getOfDescToken ("apiPerformer");
            ch = (ConfigChunk)chunks.get(0);

            p = ch.getPropertyFromToken ("SimHeadModel");
            p.setValue (head_model_field.getText(), 0);

            p = ch.getPropertyFromToken ("SimWandModel");
            p.setValue (wand_model_field.getText(), 0);

            results_dirty = false;
        }
        return results_db;
    }


    public void reset () {
        results_db = null;
    }



}
