package VjWizards;

import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.*;
import VjWizards.*;
import VjConfig.*;
import VjControl.*;
import VjComponents.ConfigEditor.ConfigChunkUI.*;
import VjComponents.UI.Widgets.*;

/**
 * Title:        VjControl Sim Wizard GUI
 * Description:
 * Copyright:    Copyright (c) 2001
 * Company:      VRAC
 * @author Christopher Just
 * @version 1.0
 */

public class InputDevicePanel
    extends JPanel 
    implements WizardSubPanel,
               ActionListener, 
               FocusListener,
               MouseListener,
               ListSelectionListener {

    BorderLayout borderLayout1 = new BorderLayout();
    JPanel jPanel1 = new JPanel();
    JPanel jPanel2 = new JPanel();
    BorderLayout borderLayout2 = new BorderLayout();
    JScrollPane devices_scrollpane = new JScrollPane();
    JPanel jPanel3 = new JPanel();
    GridLayout gridLayout1 = new GridLayout();
    JButton add_button = new JButton();
    JButton remove_button = new JButton();
    TitledBorder titledBorder1;
    JPanel box1 = new JPanel();
    GridBagLayout right_panel_layout = new GridBagLayout();
    GridBagConstraints gbc = new GridBagConstraints();
    JPanel keys_panel = new JPanel();
    JPanel defaults_panel = new JPanel();
    JPanel source_panel = new JPanel();
    JPanel name_panel = new JPanel();
    JLabel devtype_name = new JLabel();
    JLabel jLabel1 = new JLabel();
    JTextField name_field = new JTextField();
    JComboBox source_select = new JComboBox();
    JLabel jLabel2 = new JLabel();
    TitledBorder titledBorder2;
    TitledBorder titledBorder3;
    JScrollPane keys_scrollpane = new JScrollPane();
    JPanel keys_panel_int = new JPanel();
    GridBagLayout keys_panel_int_layout = new GridBagLayout();
    JPanel devices_panel_int = new JPanel();

    JPopupMenu add_menu;
    JMenuItem add_head_mi;
    JMenuItem add_firstp_camera_mi;
    JMenuItem add_thirdp_camera_mi;
    JMenuItem add_wand3_mi;
    JMenuItem add_digital_mi;
    JMenuItem add_analog_mi;
    JMenuItem add_position_mi;
    JMenuItem add_relative_mi;

    JList devices_jlist = new JList();
    DefaultComboBoxModel devices_jlist_model; // this is such a cheat...

    DefaultComboBoxModel source_select_model;

    // simrelativeposition defaults panel
    JComponent relative_defaults_panel = null;
    JComboBox relative_base;
    JComboBox relative_device;
    DefaultComboBoxModel relative_base_model;
    DefaultComboBoxModel relative_device_model;

    // simanalog defaults panel
    JComponent analog_defaults_panel = null;
    JTextField analog_delta_field;
    JTextField analog_min_field;
    JTextField analog_max_field;


    // device types:
    final int DIGITAL = 1;
    final int ANALOG = 2;
    final int POSITION = 3;
    final int FIRST_PERSON_CAMERA = 4;
    final int THIRD_PERSON_CAMERA = 5;
    final int USER_HEAD_TRACKING = 6;
    final int RELATIVE_POSITION = 7;

    private class InputDeviceData {
        public String proxy_name;
        public String device_name; // just proxy_name + "_device" if owns
        public int device_unit;
        public int device_type;
        public String input_source;

        public ConfigChunk proxy_chunk;
        public ConfigChunk device_chunk;
        public boolean dirty;

        public InputDeviceData () {
        }

        public String toString () {
            return proxy_name;
        }

        public boolean isDeviceOwner () {
            switch (device_type) {
            case FIRST_PERSON_CAMERA:
                return false;
            default:
                return true;
            }
        }
    }

    boolean ui_initialized;
    boolean results_dirty;  // does results_db need to be updated from ui?
    ArrayList devices_list;
    InputDeviceData current_editing_device;

    ArrayList keys_panel_list;

    ConfigChunkDB results_db;

    public InputDevicePanel() {
        results_db = null;
        results_dirty = false;
        ui_initialized = false;
        devices_list = new ArrayList();
        current_editing_device = null;
        keys_panel_list = new ArrayList();

        try {
            jbInit();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }
    private void jbInit() throws Exception {
        this.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));
        titledBorder1 = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"Edit Device");
        //box1 = Box.createVerticalBox();
        titledBorder2 = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"Initial Value");
        titledBorder3 = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"Key Bindings");
        this.setLayout(borderLayout1);
        jPanel1.setLayout(borderLayout2);
        jPanel3.setLayout(gridLayout1);
        gridLayout1.setColumns(2);
        gridLayout1.setHgap(5);
        gridLayout1.setVgap(5);
        add_button.setToolTipText("Add a simulated input device");
        add_button.setText("Add");
        remove_button.setToolTipText("Remove an input device");
        remove_button.setText("Remove");
        borderLayout2.setHgap(5);
        borderLayout2.setVgap(5);
        /*jPanel2*/box1.setBorder(titledBorder1);
        devtype_name.setText("Device: <None Selected>");
        jLabel1.setText("Name ");
        name_field.setToolTipText("Name of this simulated device");
        name_field.setColumns(17);
        jLabel2.setText("Input Source ");
        //source_select.setMinimumSize(new Dimension(106, 24));
        //source_select.setPreferredSize(new Dimension(106, 24));
        source_select.setToolTipText("Input window that supplies keystrokes to this simulator");
        defaults_panel.setBorder(titledBorder2);
        defaults_panel.setLayout (new BorderLayout());
        keys_panel.setBorder(titledBorder3);
        borderLayout1.setHgap(5);
        borderLayout1.setVgap(5);
        this.add(jPanel1, BorderLayout.WEST);
        jPanel1.add(devices_scrollpane, BorderLayout.CENTER);
        devices_scrollpane.getViewport().add(devices_jlist, null);
        jPanel1.add(jPanel3, BorderLayout.SOUTH);
        jPanel3.add(add_button, null);
        jPanel3.add(remove_button, null);
        this.add(box1, BorderLayout.CENTER);
        //jPanel2.add(box1, null);
        box1.setLayout (right_panel_layout);
        gbc.gridwidth = gbc.REMAINDER;
        //gbc.fill = gbc.HORIZONTAL;
        gbc.anchor = gbc.WEST;
        right_panel_layout.setConstraints (devtype_name, gbc);
        box1.add(devtype_name, null);
        right_panel_layout.setConstraints (name_panel, gbc);
        box1.add(name_panel, null);
        name_panel.add(jLabel1, null);
        name_panel.add(name_field, null);
        right_panel_layout.setConstraints (source_panel, gbc);
        box1.add(source_panel, null);
        source_panel.setLayout (new BorderLayout());
        source_panel.add(jLabel2, BorderLayout.WEST);
        source_panel.add(source_select, BorderLayout.CENTER);
        gbc.fill = gbc.BOTH;
        gbc.weightx = 1.0;
        gbc.weighty = 0.5;
        right_panel_layout.setConstraints (defaults_panel, gbc);
        box1.add(defaults_panel, null);
        right_panel_layout.setConstraints (keys_panel, gbc);
        box1.add(keys_panel, null);
        keys_panel.setLayout (new BorderLayout (5,5));
        keys_panel.add(keys_scrollpane, BorderLayout.CENTER);
        keys_scrollpane.getViewport().add(keys_panel_int, null);
        keys_panel_int.setLayout (keys_panel_int_layout);
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
        int i, n;
        String device_name;
        InputDeviceData id;
        Iterator it;

        // get list of input devices for source_select combobox
        source_select_model.removeAllElements();
        chunks = results_db.getOfDescToken ("KeyboardProxy");
        it = chunks.iterator();
        while (it.hasNext()) {
            ch = (ConfigChunk)it.next();
            source_select_model.addElement (ch.getName());
        }


        // what i need to do here is:
        // for any IDData not in results_db, add chunk to results
        // for any device chunk not in devices_list, add to list
        it = devices_list.iterator();
        while (it.hasNext())
            ((InputDeviceData)it.next()).dirty = true;
        // eeks... i need somethin clever here to get all devices :(
        chunks = results_db.getOfDescToken ("PosProxy");
        it = chunks.iterator();
        while (it.hasNext()) {
            ch = (ConfigChunk)it.next();
            processDeviceChunk (ch, null);
        }
        chunks = results_db.getOfDescToken ("DigProxy");
        it = chunks.iterator();
        while (it.hasNext()) {
            ch = (ConfigChunk)it.next();
            processDeviceChunk (ch, null);
        }        
        chunks = results_db.getOfDescToken ("AnaProxy");
        it = chunks.iterator();
        while (it.hasNext()) {
            ch = (ConfigChunk)it.next();
            processDeviceChunk (ch, null);
        }        
        it = devices_list.iterator();
        while (it.hasNext()) {
            id = (InputDeviceData)it.next();
            if (id.dirty) {
                id.dirty = false;
                results_db.add (id.proxy_chunk);
                results_db.add (id.device_chunk);
            }
        }
    }


    // util for setUIState
    /** Utility for setUIState and adding new devices.
     *  Processes the ConfigChunks proxy_chunk and device_chunk, and
     *  adds a new InputDeviceData entry.
     *  If device_chunk is null, it assumes that proxy_chunk's device 
     *  property is valid and checks the resultsDB for the device's chunk.
     */
    private void processDeviceChunk (ConfigChunk proxy_chunk, ConfigChunk device_chunk) {
        String proxy_name, device_name;
        int device_unit;
        InputDeviceData id;
        boolean do_insert;
        String input_source;

        proxy_name = proxy_chunk.getName();

//          if (proxy_name.endsWith ("_helper")) // it doesn't warrant it's own 
//              return;                          // mention

        device_name = proxy_chunk.getValueFromToken ("device",0).getString();
        device_unit = proxy_chunk.getValueFromToken ("unit", 0).getInt();

        if (device_chunk == null)
            device_chunk = results_db.get(device_name);

        VarValue v = device_chunk.getValueFromToken ("keyboardProxy",0);
        if (v == null)
            input_source = "";
        else
            input_source = v.getString();

        id = getInputDeviceData (proxy_name);
        if (id == null) {
            id = new InputDeviceData();
            do_insert = true;
        }
        else
            do_insert = false;

        String proxy_token = proxy_chunk.getDescToken();
        if (proxy_token.equalsIgnoreCase("DigProxy"))
            id.device_type = DIGITAL;
        else if (proxy_token.equalsIgnoreCase("AnaProxy"))
            id.device_type = ANALOG;
        else if (proxy_token.equalsIgnoreCase("PosProxy")) {
            if (proxy_name.equalsIgnoreCase ("SimCamera")) {
                if (device_name.equalsIgnoreCase (proxy_name + "_device"))
                    id.device_type = THIRD_PERSON_CAMERA;
                else {
                    id.device_type = FIRST_PERSON_CAMERA;
                }
            }
            else if (proxy_name.equalsIgnoreCase ("VJHead")) {
                id.device_type = USER_HEAD_TRACKING;
            }
            else if (device_chunk.getDescToken().equalsIgnoreCase("SimRelativePosition")) {
                id.device_type = RELATIVE_POSITION;
            }
            else
                id.device_type = POSITION;
        }

        id.proxy_name = proxy_name;
        id.device_name = device_name;
        id.device_unit = device_unit;
        id.input_source = input_source;

        id.proxy_chunk = proxy_chunk;
        id.device_chunk = device_chunk;

        id.dirty = false;
        
        if (do_insert) {
            devices_list.add (id);
            devices_jlist_model.addElement (id);
        }
    }



    private void updateEditingDeviceValues () {
        if (current_editing_device == null)
            return;

        // get key values for old current device
        VarValueMiniChunkPanel vp;
        Property p;
        ConfigChunk ch;

        String new_input_source = (String)source_select.getSelectedItem();
        if (!new_input_source.equalsIgnoreCase("<No Selection>"))
            current_editing_device.input_source = new_input_source;

        // get defaults

        switch (current_editing_device.device_type) {
        case ANALOG:
            updateAnalogDefaults (current_editing_device);
            break;
        case FIRST_PERSON_CAMERA:
            break;
        case RELATIVE_POSITION:
            updateRelativeDefaults (current_editing_device);
            break;
        case DIGITAL:
            break;
        case THIRD_PERSON_CAMERA:
        case POSITION:
        case USER_HEAD_TRACKING:
            updatePositionDefaults (current_editing_device);
            break;
        }

        // get key bindings
        switch (current_editing_device.device_type) {
        case ANALOG:
            p = current_editing_device.device_chunk.getPropertyFromToken ("KeyPairsInc");
            ch = p.getValue(0).getEmbeddedChunk();
            vp = (VarValueMiniChunkPanel)keys_panel_list.get(0);
            p.setValue (vp.getValue(), 0);

            p = current_editing_device.device_chunk.getPropertyFromToken ("KeyPairsDec");
            ch = p.getValue(0).getEmbeddedChunk();
            vp = (VarValueMiniChunkPanel)keys_panel_list.get(1);
            p.setValue (vp.getValue(), 0);
            break;
        case FIRST_PERSON_CAMERA:
            break;
        case RELATIVE_POSITION:
            break;
        case THIRD_PERSON_CAMERA:
        case POSITION:
        case USER_HEAD_TRACKING:
        default:
            p = current_editing_device.device_chunk.getPropertyFromToken ("KeyPairs");
            int i, n = p.getNum();
            for (i = 0; i < n; i++) {
                ch = p.getValue(i).getEmbeddedChunk();
                vp = (VarValueMiniChunkPanel)keys_panel_list.get(i);
                p.setValue (vp.getValue(), i);
            }
        }

        System.out.println ("updated stuff:\n" + current_editing_device.proxy_chunk.xmlRep() + current_editing_device.device_chunk.xmlRep());
    }


    private void setEditingDevice (InputDeviceData id) {
        if (id != current_editing_device) {
            updateEditingDeviceValues();

            // set new device...
            current_editing_device = id;
            if (id == null) {
                name_field.setText("");
                devtype_name.setText("Device: <None Selected>");
                keys_panel_int.removeAll();
                keys_panel_int.repaint();
                keys_panel_list.clear();
                defaults_panel.removeAll();
                defaults_panel.repaint();

                name_field.setEditable (false);
                //source_select.setEditable (false);
            }
            else {

                name_field.setEditable (true);
                //source_select.setEditable (true);

                switch (id.device_type) {
                case DIGITAL:
                    devtype_name.setText("Device: Digital");
                    break;
                case ANALOG:
                    devtype_name.setText("Device: Analog");
                    break;
                case POSITION:
                    devtype_name.setText("Device: Position");
                    break;
                case FIRST_PERSON_CAMERA:
                    devtype_name.setText("Device: First-person Camera");
                    break;
                case THIRD_PERSON_CAMERA:
                    devtype_name.setText("Device: Third-person Camera");
                    break;
                case USER_HEAD_TRACKING:
                    devtype_name.setText("Device: User head tracking");
                    break;
                case RELATIVE_POSITION:
                    devtype_name.setText("Device: Relative position");
                    break;
                }

                // name field
                name_field.setText(id.proxy_name);
                switch (id.device_type) {
                case ANALOG:
                case DIGITAL:
                case POSITION:
                case RELATIVE_POSITION:
                    name_field.setEditable(true);
                    break;
                default:
                    name_field.setEditable(false);
                    break;
                }

                // source-select field
                source_select.setSelectedItem (id.input_source);

                // defaults_panel stuff.....
                defaults_panel.removeAll();
                switch (id.device_type) {
                case ANALOG:
                    defaults_panel.add (getAnalogDefaults(id), "Center");
                    break;
                case RELATIVE_POSITION:
                    defaults_panel.add (getRelativeDefaults(id), "Center");
                    break;
                case POSITION:
                case THIRD_PERSON_CAMERA:
                case USER_HEAD_TRACKING:
                    defaults_panel.add (getPositionDefaults(id), "Center");
                    break;
                default:
                }

                // keys_panel_int
                keys_panel_int.removeAll();
                keys_panel_list.clear();
                GridBagConstraints gbc = new GridBagConstraints();
                gbc.fill = gbc.HORIZONTAL;
                //gbc.weighty = 0.0;
                Property p;
                ConfigChunk ch;
                VarValueMiniChunkPanel vp;
                JLabel l;

                switch (id.device_type) {
                case ANALOG:
                    p = id.device_chunk.getPropertyFromToken ("KeyPairsInc");
                    ch = p.getValue(0).getEmbeddedChunk();
                    gbc.gridwidth = gbc.RELATIVE;
                    gbc.weightx = 0.0;
                    l = new JLabel ("Increment");
                    keys_panel_int_layout.setConstraints (l, gbc);
                    keys_panel_int.add (l);
                    gbc.gridwidth = gbc.REMAINDER;
                    gbc.weightx = 1.0;
                    vp = new VarValueMiniChunkPanel (null, false, ch);
                    vp.setValue (p.getValue(0)); // yech... this api sucks
                    keys_panel_int_layout.setConstraints (vp, gbc);
                    keys_panel_int.add (vp);
                    keys_panel_list.add (vp);

                    p = id.device_chunk.getPropertyFromToken ("KeyPairsDec");
                    ch = p.getValue(0).getEmbeddedChunk();
                    gbc.gridwidth = gbc.RELATIVE;
                    gbc.weightx = 0.0;
                    l = new JLabel ("Decrement");
                    keys_panel_int_layout.setConstraints (l, gbc);
                    keys_panel_int.add (l);
                    gbc.gridwidth = gbc.REMAINDER;
                    gbc.weightx = 1.0;
                    vp = new VarValueMiniChunkPanel (null, false, ch);
                    vp.setValue (p.getValue(0)); // yech... this api sucks
                    keys_panel_int_layout.setConstraints (vp, gbc);
                    keys_panel_int.add (vp);
                    keys_panel_list.add (vp);

                    break;
                case FIRST_PERSON_CAMERA:
                    break;
                case RELATIVE_POSITION:
                    break;
                case THIRD_PERSON_CAMERA:
                case POSITION:
                case USER_HEAD_TRACKING:
                case DIGITAL:
                default:
                    p = id.device_chunk.getPropertyFromToken ("KeyPairs");
                    int i, n = p.getNum();
                    for (i = 0; i < n; i++) {
                        ch = p.getValue(i).getEmbeddedChunk();
                        gbc.gridwidth = gbc.RELATIVE;
                        gbc.weightx = 0.0;
                        l = new JLabel (p.getDesc().getValueLabel(i));
                        keys_panel_int_layout.setConstraints (l, gbc);
                        keys_panel_int.add (l);
                        gbc.gridwidth = gbc.REMAINDER;
                        gbc.weightx = 1.0;
                        vp = new VarValueMiniChunkPanel (null, false, ch);
                        vp.setValue (p.getValue(i)); // yech... this api sucks
                        keys_panel_int_layout.setConstraints (vp, gbc);
                        keys_panel_int.add (vp);
                        keys_panel_list.add (vp);

                    }
                }
                keys_scrollpane.validate();
                keys_scrollpane.repaint(); // needed if we just removed stuff

            }
        }
    }

    private InputDeviceData getInputDeviceData (String name) {
        InputDeviceData id;
        int i, n = devices_list.size();
        for (i = 0; i < n; i++) {
            id = (InputDeviceData)devices_list.get(i);
            if (id.proxy_name.equals(name))
                return id;
        }
        return null;
    }


    //----------------------- WizardSubPanel Stuff --------------------------

    public JComponent getUIComponent () {
        return this;
    }

    public boolean initUIComponent () {
        if (!ui_initialized) {
            // set up anything jbuilder didn't - listeners & such
            devices_jlist_model = new DefaultComboBoxModel();
            devices_jlist.setModel(devices_jlist_model);

            source_select_model = new DefaultComboBoxModel();
            source_select.setModel (source_select_model);

            keys_panel.setPreferredSize (new Dimension (300, 100));

            add_menu = new JPopupMenu();
            add_menu.add (add_head_mi = new JMenuItem ("User head tracking"));
            add_menu.add (add_firstp_camera_mi = new JMenuItem ("First-person camera view"));
            add_menu.add (add_thirdp_camera_mi = new JMenuItem ("Third-person camera view"));
            add_menu.add (add_wand3_mi = new JMenuItem ("3-button wand"));
            add_menu.add (add_digital_mi = new JMenuItem ("Digital device"));
            add_menu.add (add_analog_mi = new JMenuItem ("Analog device"));
            add_menu.add (add_position_mi = new JMenuItem ("Position device"));
            add_menu.add (add_relative_mi = new JMenuItem ("Relative Position device"));

            name_field.setEditable (false);
            source_select.setEditable (false);

            add_head_mi.addActionListener (this);
            add_firstp_camera_mi.addActionListener (this);
            add_thirdp_camera_mi.addActionListener (this);
            add_wand3_mi.addActionListener (this);
            add_digital_mi.addActionListener (this);
            add_analog_mi.addActionListener (this);
            add_position_mi.addActionListener (this);
            add_relative_mi.addActionListener (this);

            add_button.addMouseListener (this);
            add_button.addActionListener(this);
            remove_button.addActionListener(this);
            devices_jlist.addListSelectionListener (this);

            // listen for these events so we can update the visual display of
            // the window.
            name_field.addFocusListener (this);
//             xpos_field.addFocusListener (this);
//             ypos_field.addFocusListener (this);
//             width_field.addFocusListener (this);
//             height_field.addFocusListener (this);

            name_field.addActionListener (this);
//             xpos_field.addActionListener (this);
//             ypos_field.addActionListener (this);
//             width_field.addActionListener (this);
//             height_field.addActionListener (this);

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
        if (getInputDeviceData ("VJHead") == null) {
            JOptionPane.showMessageDialog (
                null, "No Head Tracking device has been defined.", 
                "Wizard Error", JOptionPane.ERROR_MESSAGE);
            return false;
        }
        if (getInputDeviceData ("SimCamera") == null) {
            JOptionPane.showMessageDialog (
                null, "No Camera has been defined.", 
                "Wizard Error", JOptionPane.ERROR_MESSAGE);
            return false;
        }

        return true;
    }

    public boolean setInitialDB (ConfigChunkDB db) {
        results_db = (ConfigChunkDB)db.clone();
        if (ui_initialized && (results_db != null))
            setUIState();
        return true;
    }

    public ConfigChunkDB getResultDB () {
        updateEditingDeviceValues();

        // the keys editor doesn't update results_dirty so we can't trust it.
        if (results_db != null) {
        //if (results_dirty && (results_db != null)) {
            // query the GUI widgets to update results_db
            java.util.List chunks;
            ConfigChunk ch;
            Property p;
            InputDeviceData id;
            
            Iterator it = devices_list.iterator();
            while (it.hasNext()) {
                id = (InputDeviceData)it.next();
                ch = id.proxy_chunk;

                ch.setName (id.proxy_name);
                
                p = ch.getPropertyFromToken ("device");
                p.setValue (id.device_name, 0);

                ch = id.device_chunk;
                ch.setName (id.device_name);

                p = ch.getPropertyFromToken ("keyboardProxy");
                if (p != null)
                    p.setValue (id.input_source, 0);

//                 p = ch.getPropertyFromToken ("origin");
//                 p.setValue (new VarValue(iw.x), 0);
//                 p.setValue (new VarValue(iw.y), 1);


                // we depend on chunk default values etc. to set the rest 
                // of these things...
            }

            results_dirty = false;
        }
        //System.out.println (results_db.xmlRep());
        return results_db;
    }

    public void reset () {
        System.out.println ("resetting inputdevicepanel");
        setEditingDevice(null);
        devices_list.clear();
        devices_jlist_model.removeAllElements();
        results_db = null;
    }


    //------------------------ MouseListener Stuff -------------------------

    public void mouseEntered(MouseEvent e) {}
    public void mouseReleased(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}
    public void mouseClicked(MouseEvent e) {}

    public void mousePressed(MouseEvent e) {
        if (e.getSource() == add_button)
            add_menu.show (add_button, e.getX(), e.getY());
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
        ConfigChunk device_chunk, proxy_chunk;
        Property p;
        InputDeviceData id;

        Object source = e.getSource();
        if (source == add_button) {
            // never mind = just get mouse pressed events on this...
            //InputDeviceData id = new InputDeviceData();
            //ConfigChunk ch = ChunkFactory.createChunkWithDescToken ("Keyboard");
            //ch.setName (results_db.getNewName ("Input Window"));
            //processDeviceChunk (ch);
            //results_db.add (ch);
        }
        else if (source == remove_button) {
            Object[] selects = devices_jlist.getSelectedValues();
            // go backwards so that we don't shift later indices by
            // deleting earlier ones first.
            for (int i = selects.length - 1; i >= 0; i--) {
                id = (InputDeviceData)selects[i];
                devices_jlist_model.removeElement (id);
                devices_list.remove (id);
                results_db.remove (id.proxy_chunk);
                if (id.isDeviceOwner())
                    results_db.remove (id.device_chunk);
                if (id == current_editing_device)
                    setEditingDevice (null);
            }
        }
        else if (source == add_head_mi) {
            id = getInputDeviceData ("VJHead");
            if (id != null) {
                Object[] options = { "OK", "CANCEL" };
                int result = JOptionPane.showOptionDialog(
                    null, 
                    "Adding a new head will replace the current head",
                    "Warning", 
                    JOptionPane.DEFAULT_OPTION,
                    JOptionPane.WARNING_MESSAGE,
                    null, options, options[0]);
                if (result != 0)
                    return;
                // remove simcamera
                devices_jlist_model.removeElement (id);
                devices_list.remove(id);
                results_db.remove (id.proxy_chunk);
                if (id.isDeviceOwner())
                    results_db.remove (id.device_chunk);
                if (id == current_editing_device)
                    setEditingDevice (null);
            }
            device_chunk = ChunkFactory.createChunkWithDescToken ("SimPosition");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("PosProxy");
            proxy_chunk.setName (results_db.getNewName ("VJHead"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);

        }
        else if (source == add_firstp_camera_mi) {
            id = getInputDeviceData ("SimCamera");
            if (id != null) {
                Object[] options = { "OK", "CANCEL" };
                int result = JOptionPane.showOptionDialog(
                    null, 
                    "Adding a new Camera will replace the current Camera",
                    "Warning", 
                    JOptionPane.DEFAULT_OPTION,
                    JOptionPane.WARNING_MESSAGE,
                    null, options, options[0]);
                if (result != 0)
                    return;
                // remove simcamera
                //InputDeviceData id = getInputDevice ("SimCamera");
                devices_jlist_model.removeElement (id);
                devices_list.remove(id);
                results_db.remove (id.proxy_chunk);
                if (id.isDeviceOwner())
                    results_db.remove (id.device_chunk);
                if (id == current_editing_device)
                    setEditingDevice (null);
            }
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("PosProxy");
            proxy_chunk.setName (results_db.getNewName ("SimCamera"));
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue ("VJHead_device", 0);
            p = proxy_chunk.getPropertyFromToken ("translate");
            p.setValue (new VarValue (-1.0f), 2);
            p = proxy_chunk.getPropertyFromToken ("etrans");
            p.setValue (true, 0);
            processDeviceChunk (proxy_chunk, null);
            results_db.add (proxy_chunk);

        }
        else if (source == add_thirdp_camera_mi) {
            id = getInputDeviceData ("SimCamera");
            if (id != null) {
                Object[] options = { "OK", "CANCEL" };
                int result = JOptionPane.showOptionDialog(
                    null, 
                    "Adding a new Camera will replace the current Camera",
                    "Warning", 
                    JOptionPane.DEFAULT_OPTION,
                    JOptionPane.WARNING_MESSAGE,
                    null, options, options[0]);
                if (result != 0)
                    return;
                // remove simcamera
                devices_jlist_model.removeElement (id);
                devices_list.remove(id);
                results_db.remove (id.proxy_chunk);
                if (id.isDeviceOwner())
                    results_db.remove (id.device_chunk);
                if (id == current_editing_device)
                    setEditingDevice (null);
            }
            device_chunk = ChunkFactory.createChunkWithDescToken ("SimPosition");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("PosProxy");
            proxy_chunk.setName (results_db.getNewName ("SimCamera"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);

        }
        else if (source == add_wand3_mi) {
            // just adds a new position called VJWandx and 3 new Digital
            device_chunk = ChunkFactory.createChunkWithDescToken ("SimPosition");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("PosProxy");
            proxy_chunk.setName (results_db.getNewName ("VJWand"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);

            device_chunk = ChunkFactory.createChunkWithDescToken ("SimDigital");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("DigProxy");
            proxy_chunk.setName (results_db.getNewName ("VJButton"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);

            device_chunk = ChunkFactory.createChunkWithDescToken ("SimDigital");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("DigProxy");
            proxy_chunk.setName (results_db.getNewName ("VJButton"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);

            device_chunk = ChunkFactory.createChunkWithDescToken ("SimDigital");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("DigProxy");
            proxy_chunk.setName (results_db.getNewName ("VJButton"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);
        }
        else if (source == add_digital_mi) {
            device_chunk = ChunkFactory.createChunkWithDescToken ("SimDigital");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("DigProxy");
            proxy_chunk.setName (results_db.getNewName ("VJDigital"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);
        }
        else if (source == add_analog_mi) {
            device_chunk = ChunkFactory.createChunkWithDescToken ("SimAnalog");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("AnaProxy");
            proxy_chunk.setName (results_db.getNewName ("VJAnalog"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);
        }
        else if (source == add_position_mi) {
            device_chunk = ChunkFactory.createChunkWithDescToken ("SimPosition");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("PosProxy");
            proxy_chunk.setName (results_db.getNewName ("VJPosition"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);
        }
        else if (source == add_relative_mi) {
            device_chunk = ChunkFactory.createChunkWithDescToken ("SimRelativePosition");
            proxy_chunk = ChunkFactory.createChunkWithDescToken ("PosProxy");
            proxy_chunk.setName (results_db.getNewName ("VJPosition"));
            device_chunk.setName (proxy_chunk.getName() + "_device");
            p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (device_chunk.getName(), 0);
            processDeviceChunk (proxy_chunk, device_chunk);
            results_db.add (proxy_chunk);
            results_db.add (device_chunk);

        }
        else
            fieldChanged (source);
    }

    //----------------------- ListSelectionListener Stuff ------------------

    public void valueChanged (ListSelectionEvent e) {
        InputDeviceData selected = null;
        int i = e.getFirstIndex();
        int n = e.getLastIndex();
        for (; i <= n; i++) {
            if (devices_jlist.isSelectedIndex(i)) {
                selected = (InputDeviceData)devices_jlist_model.getElementAt(i);
                break;
            }
        }
        if (selected != null) {
            setEditingDevice (selected);
        }
    }


    //----------------------- Focus/Action Utilities -----------------------


    /** Called when a field sends ActionEvent or FocusLost.
     *  Note that this means the field _may_ have changed; we should check
     *  before acting.
     */
    private void fieldChanged (Object source) {
        if (current_editing_device == null)
            return;

        if (source == name_field) {
            String newname = name_field.getText();
            if (!newname.equals(current_editing_device.proxy_name)) {
                if (newname.equals("")) {
                    Core.consoleErrorMessage ("Wizard", "Name must have a value.");
                    name_field.setText (current_editing_device.proxy_name);
                }
                else {
                    current_editing_device.proxy_name = newname;
                    current_editing_device.device_name = newname + "_device";
                    devices_jlist.repaint(); // pick up new name.
                    results_dirty = true;
                }
            }
        }
    }


    //--------------- Defaults Panel Stuff --------------------------------

    private JComponent getRelativeDefaults (InputDeviceData id) {
        if (relative_defaults_panel == null) {
            relative_defaults_panel = new JPanel();
            GridBagLayout gbl = new GridBagLayout();
            GridBagConstraints gbc = new GridBagConstraints();
            relative_defaults_panel.setLayout (gbl);
            gbc.gridwidth = 1;
            gbc.fill = gbc.HORIZONTAL;
            gbc.anchor = gbc.EAST;
            JLabel l = new JLabel ("Input Device: ");
            gbl.setConstraints (l, gbc);
            relative_defaults_panel.add (l);
            gbc.gridwidth = gbc.REMAINDER;
            relative_device = new JComboBox();
            gbl.setConstraints (relative_device, gbc);
            relative_defaults_panel.add (relative_device);

            gbc.gridwidth = 1;
            l = new JLabel ("Base coordinate: ");
            gbl.setConstraints (l, gbc);
            relative_defaults_panel.add (l);
            gbc.gridwidth = gbc.REMAINDER;
            relative_base = new JComboBox();
            gbl.setConstraints (relative_base, gbc);
            relative_defaults_panel.add (relative_base);

            relative_base_model = new DefaultComboBoxModel();
            relative_device_model = new DefaultComboBoxModel();
            relative_base.setModel (relative_base_model);
            relative_device.setModel (relative_device_model);
        }
        // get current stuff to put in combo box
        // hey, that'll all be in devices_jlist... yipee
        relative_base_model.removeAllElements();
        relative_device_model.removeAllElements();
        for (int i = 0; i < devices_list.size(); i++) {
            InputDeviceData id2 = (InputDeviceData)devices_list.get(i);
            switch (id2.device_type) {
            case FIRST_PERSON_CAMERA:
            case THIRD_PERSON_CAMERA:
            case USER_HEAD_TRACKING:
            case RELATIVE_POSITION:
            case POSITION:
                if (id2 != id) {
                    relative_base_model.addElement(id2);
                    relative_device_model.addElement(id2);
                }
                break;
            default:
            }
        }

        // apply input device data...
        String base_name = id.device_chunk.getValueFromToken("base_frame_proxy", 0).getString();
        InputDeviceData id2 = getInputDeviceData (base_name);
        relative_base_model.setSelectedItem (id2);
        String dev_name = id.device_chunk.getValueFromToken("relative_proxy", 0).getString();
        id2 = getInputDeviceData (dev_name);
        relative_device_model.setSelectedItem (id2);
        return relative_defaults_panel;
    }

    private void updateRelativeDefaults (InputDeviceData id) {
        Property p;
        p = id.device_chunk.getPropertyFromToken ("base_frame_proxy");
        p.setValue(relative_base_model.getSelectedItem().toString(), 0);
        p = id.device_chunk.getPropertyFromToken ("relative_proxy");
        p.setValue(relative_device_model.getSelectedItem().toString(), 0);

    }

    private JComponent getAnalogDefaults (InputDeviceData id) {
        if (analog_defaults_panel == null) {
            analog_defaults_panel = new JPanel();
            GridBagLayout gbl = new GridBagLayout();
            GridBagConstraints gbc = new GridBagConstraints();
            JLabel l;
            analog_defaults_panel.setLayout (gbl);
            gbc.gridwidth = 1;
            gbc.fill = gbc.HORIZONTAL;
            gbc.anchor = gbc.EAST;
            l = new JLabel ("Minimum Value: ");
            gbl.setConstraints (l, gbc);
            analog_defaults_panel.add (l);
            gbc.gridwidth = gbc.REMAINDER;
            gbc.weightx = 1.0;
            analog_min_field = new FloatTextField();
            gbl.setConstraints (analog_min_field, gbc);
            analog_defaults_panel.add (analog_min_field);

            gbc.gridwidth = 1;
            gbc.weightx = 0.0;
            l = new JLabel ("Maximum Value: ");
            gbl.setConstraints (l, gbc);
            analog_defaults_panel.add (l);
            gbc.gridwidth = gbc.REMAINDER;
            gbc.weightx = 1.0;
            analog_max_field = new FloatTextField();
            gbl.setConstraints (analog_max_field, gbc);
            analog_defaults_panel.add (analog_max_field);

            gbc.gridwidth = 1;
            gbc.weightx = 0.0;
            l = new JLabel ("Minimum Delta: ");
            gbl.setConstraints (l, gbc);
            analog_defaults_panel.add (l);
            gbc.gridwidth = gbc.REMAINDER;
            gbc.weightx = 1.0;
            analog_delta_field = new FloatTextField();
            gbl.setConstraints (analog_delta_field, gbc);
            analog_defaults_panel.add (analog_delta_field);

        }

        // apply input device data...
        float val;
        val = id.device_chunk.getValueFromToken ("min",0).getFloat();
        analog_min_field.setText (Float.toString(val));

        val = id.device_chunk.getValueFromToken ("max",0).getFloat();
        analog_max_field.setText (Float.toString(val));

        val = id.device_chunk.getValueFromToken ("anastep",0).getFloat();
        analog_delta_field.setText (Float.toString(val));

        return analog_defaults_panel;
    }

    private void updateAnalogDefaults (InputDeviceData id) {
        Property p;
        try {
            p = id.device_chunk.getPropertyFromToken ("min");
            p.setValue(new VarValue(Float.parseFloat(analog_min_field.getText())), 0);

            p = id.device_chunk.getPropertyFromToken ("max");
            p.setValue(new VarValue(Float.parseFloat(analog_max_field.getText())), 0);

            p = id.device_chunk.getPropertyFromToken ("anastep");
            p.setValue(new VarValue(Float.parseFloat(analog_delta_field.getText())), 0);
        }
        catch (Exception e) {
            System.out.println ("hey, who put an illegal value in my floatTextField?");
        }
    }

    JComponent position_defaults_panel = null;
    JTextField[] position_startpos = new JTextField[3];
    JTextField[] position_startrot = new JTextField[3];
    JTextField   position_transdelta;
    JTextField   position_rotdelta;
    JComboBox    position_coordsystem;

    private JComponent getPositionDefaults (InputDeviceData id) {
        String[] valuelabels = {"X", "Y", "Z"};
        int i;

        if (position_defaults_panel == null) {
            position_defaults_panel = new JPanel();
            GridBagLayout gbl = new GridBagLayout();
            GridBagConstraints gbc = new GridBagConstraints();
            JLabel l;
            position_defaults_panel.setLayout (gbl);
            gbc.gridwidth = 1;
            gbc.weightx = 1.0;
            gbc.fill = gbc.HORIZONTAL;
            gbc.anchor = gbc.WEST;

            gbc.gridwidth = gbc.REMAINDER;
            l = new JLabel ("Initial Position:");
            gbl.setConstraints (l, gbc);
            position_defaults_panel.add (l);
            for (i = 0; i < 3; i++) {
                if (i == 0)
                    gbc.weightx = 1.0;
                else
                    gbc.weightx = 0.0;
                gbc.gridwidth = 1;
                l = new JLabel (valuelabels[i], JLabel.TRAILING);
                gbl.setConstraints(l, gbc);
                position_defaults_panel.add (l);

                gbc.weightx = 0.0;
                if (i == 2)
                    gbc.gridwidth = gbc.REMAINDER;
                position_startpos[i] = new FloatTextField(7);
                gbl.setConstraints (position_startpos[i], gbc);
                position_defaults_panel.add (position_startpos[i]);
            }

            gbc.gridwidth = gbc.RELATIVE;
            gbc.weightx = 1.0;
            l = new JLabel ("Translation delta: ", JLabel.TRAILING);
            gbl.setConstraints (l, gbc);
            position_defaults_panel.add (l);
            gbc.gridwidth = gbc.REMAINDER;
            gbc.weightx = 0.0;
            position_transdelta = new FloatTextField(7);
            gbl.setConstraints (position_transdelta, gbc);
            position_defaults_panel.add (position_transdelta);

            gbc.gridwidth = gbc.REMAINDER;
            l = new JLabel ("Initial Rotation (degrees):");
            gbl.setConstraints (l, gbc);
            position_defaults_panel.add (l);
            for (i = 0; i < 3; i++) {
                if (i == 0)
                    gbc.weightx = 1.0;
                else
                    gbc.weightx = 0.0;
                gbc.gridwidth = 1;
                l = new JLabel (valuelabels[i], JLabel.TRAILING);
                gbl.setConstraints(l, gbc);
                position_defaults_panel.add (l);

                gbc.weightx = 0.0;
                if (i == 2)
                    gbc.gridwidth = gbc.REMAINDER;
                position_startrot[i] = new FloatTextField(7);
                gbl.setConstraints (position_startrot[i], gbc);
                position_defaults_panel.add (position_startrot[i]);
            }

            gbc.gridwidth = gbc.RELATIVE;
            gbc.weightx = 1.0;
            l = new JLabel ("Rotation delta: ", JLabel.TRAILING);
            gbl.setConstraints (l, gbc);
            position_defaults_panel.add (l);
            gbc.gridwidth = gbc.REMAINDER;
            gbc.weightx = 0.0;
            position_rotdelta = new FloatTextField(7);
            gbl.setConstraints (position_rotdelta, gbc);
            position_defaults_panel.add (position_rotdelta);

            String[] csvalues = {"Local", "Global"};
            position_coordsystem = new JComboBox (csvalues);
            gbc.gridwidth = 1;
            gbc.weightx = 1.0;
            l = new JLabel ("Coordinate system: ", JLabel.TRAILING);
            gbl.setConstraints (l, gbc);
            position_defaults_panel.add (l);
            gbc.gridwidth = gbc.REMAINDER;
            gbc.weightx = 0.0;
            gbl.setConstraints (position_coordsystem, gbc);
            position_defaults_panel.add (position_coordsystem);
        }

        // apply input device data...
        float val;
        Property p;

        p = id.device_chunk.getPropertyFromToken ("initialPos");
        for (i = 0; i < 3; i++) {
            val = p.getValue(i).getFloat();
            position_startpos[i].setText(Float.toString(val));
        }

        p = id.device_chunk.getPropertyFromToken ("initialRot");
        for (i = 0; i < 3; i++) {
            val = p.getValue(i).getFloat();
            position_startrot[i].setText(Float.toString(val));
        }

        val = id.device_chunk.getValueFromToken ("dtrans",0).getFloat();
        position_transdelta.setText (Float.toString(val));

        val = id.device_chunk.getValueFromToken ("drot",0).getFloat();
        position_rotdelta.setText (Float.toString(val));

        int j = id.device_chunk.getValueFromToken("transCoordSystem",0).getInt();
        position_coordsystem.setSelectedIndex (j);

        return position_defaults_panel;
    }

    private void updatePositionDefaults (InputDeviceData id) {
        Property p;
        int i;
        try {
            p = id.device_chunk.getPropertyFromToken ("initialPos");
            for (i = 0; i < 3; i++) {
                p.setValue(new VarValue(Float.parseFloat(position_startpos[i].getText())), i);
            }

            p = id.device_chunk.getPropertyFromToken ("initialRot");
            for (i = 0; i < 3; i++) {
                p.setValue(new VarValue(Float.parseFloat(position_startrot[i].getText())), i);
            }

            p = id.device_chunk.getPropertyFromToken ("dtrans");
            p.setValue (new VarValue(Float.parseFloat(position_transdelta.getText())), 0);

            p = id.device_chunk.getPropertyFromToken ("drot");
            p.setValue (new VarValue(Float.parseFloat(position_rotdelta.getText())), 0);

            p = id.device_chunk.getPropertyFromToken("transCoordSystem");
            p.setValue (new VarValue(position_coordsystem.getSelectedIndex()),0);
            p = id.device_chunk.getPropertyFromToken("rotCoordSystem");
            p.setValue (new VarValue(position_coordsystem.getSelectedIndex()),0);

        }
        catch (Exception e) {
            System.out.println ("hey, who put an illegal value in my floatTextField?");
        }
    }

}
