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
import javax.swing.*;
import javax.swing.border.*;
import VjWizards.*;
import VjConfig.*;
import VjComponents.ConfigEditor.ConfigModule;
import VjComponents.ConfigEditor.ConfigUIHelper;
import VjComponents.UI.ControlUIModule;
//import VjComponents.UI.Widgets.*;
import VjControl.*;

/**
 * Title:        VjControl Sim Wizard GUI
 * Description:
 * Copyright:    Copyright (c) 2001
 * Company:      VRAC
 * @author Christopher Just
 * @version 1.0
 */

public class SavePanel 
    extends JPanel
    implements WizardSubPanel,
               ActionListener {

    JButton save_button = new JButton();
    JTextArea jTextArea1 = new JTextArea();
    FlowLayout flowLayout1 = new FlowLayout();

    boolean ui_initialized;
    boolean results_dirty;  // does results_db need to be updated from ui?
    boolean saved;

    ConfigChunkDB results_db;

    public SavePanel() {
        results_db = null;
        results_dirty = false;
        ui_initialized = false;
        saved = false;

        try {
            jbInit();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }
    private void jbInit() throws Exception {
        this.setBorder (BorderFactory.createEmptyBorder (20, 50, 20, 50));
        save_button.setText("Click here to save your new file");
        //this.setLayout(flowLayout1);
        this.setLayout (new BoxLayout (this, BoxLayout.Y_AXIS));
        jTextArea1.setColumns(40);
        jTextArea1.setLineWrap(false);
        jTextArea1.setBorder (BorderFactory.createBevelBorder(BevelBorder.LOWERED));
        jTextArea1.setText("Congratulations! Your configuration\n" +
                           "file is complete.  Once you save it\n" +
                           "you can use it with your VR Juggler \n" +
                           "applications and edit it using \n" +
                           "VjControl\'s \'Configure\' panel.");
        jTextArea1.setEditable(false);
        this.add(save_button, null);
        this.add(jTextArea1, null);
    }


    public void setConfigUIHelper (ConfigUIHelper uihelper_module) {
    }


    /** Sets the state of the UI components to reflect the resultsDB. 
     *  This is called when the ui is initialized or when a new input db
     *  is set.
     */
    private void setUIState () {

        results_dirty = false;
    }


    //----------------------- WizardSubPanel Stuff --------------------------

    public JComponent getUIComponent () {
        return this;
    }

    public boolean initUIComponent () {
        if (!ui_initialized) {

            save_button.addActionListener (this);

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
        saved = false;
        return true;
    }

    public ConfigChunkDB getResultDB () {
        if (results_db != null) {

            results_dirty = false;
        }
        return results_db;
    }


    public void reset () {
        results_db = null;
    }


    //-------------------------- ActionListener Stuff -----------------------

    public void actionPerformed (ActionEvent e) {
        if (e.getSource() == save_button) {
            File f = results_db.file;
            // this is inexcusable cheating, but i'm in a terrible hurry
            // BUG BUG BUG BUG BUG BUG BUG BUG!!! CJ LOOK HERE!!!
            ConfigModule config_module;
            ControlUIModule ui_module;
            ui_module = (ControlUIModule)Core.getVjComponent ("ControlUI Module");
            config_module = (ConfigModule)Core.getVjComponent ("Config Module");
            SuffixFilter chunkdb_filter = new SuffixFilter ("Config Files (*.config, *.cfg)", ".config");
            chunkdb_filter.addSuffix(".cfg");
            chunkdb_filter = (SuffixFilter)ui_module.getEasyFileDialog().addFilter (chunkdb_filter, "ConfigChunkDB");

            f = ui_module.getEasyFileDialog().requestSaveFile (f, ui_module, chunkdb_filter);
            if (f != null) {
                String s = config_module.saveChunkDBFile (results_db, f);
                System.out.println ("savedname: " + s);
                results_db.setName (s);
            }

        }
    }

}
