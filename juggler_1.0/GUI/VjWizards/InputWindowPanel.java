package VjWizards;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;
import javax.swing.border.*;
import VjWizards.*;
import VjConfig.*;
import VjControl.*;
import VjComponents.UI.Widgets.*;

/**
 * Title:        VjControl Sim Wizard GUI
 * Description:
 * Copyright:    Copyright (c) 2001
 * Company:      VRAC
 * @author Christopher Just
 * @version 1.0
 */

public class InputWindowPanel 
    extends JPanel 
    implements WizardSubPanel,
               ActionListener, 
               FocusListener,
               ListSelectionListener {

    BorderLayout borderLayout1 = new BorderLayout();
    JPanel left_panel = new JPanel();
    BorderLayout borderLayout2 = new BorderLayout();
    WindowLayoutPanel window_layout_panel = new WindowLayoutPanel();
    JPanel left_buttons_panel = new JPanel();
    JScrollPane windows_scrollpane = new JScrollPane();
    JButton remove_button = new JButton();
    JButton add_button = new JButton();
    JPanel right_panel = new JPanel();
    TitledBorder titledBorder1;
    Border border1;
    TitledBorder titledBorder2;
    Box right_panel_interior;
    JPanel right_panel_1 = new JPanel();
    JPanel right_panel_3 = new JPanel();
    JPanel right_panel_2 = new JPanel();
    JTextField name_field = new JTextField();
    JLabel jLabel1 = new JLabel();
    JSlider mouse_slider = new JSlider();
    JLabel jLabel2 = new JLabel();
    GridLayout gridLayout1 = new GridLayout();
    JLabel jLabel3 = new JLabel();
    JLabel jLabel4 = new JLabel();
    JLabel jLabel5 = new JLabel();
    JLabel jLabel6 = new JLabel();
    JLabel jLabel7 = new JLabel();
    JLabel jLabel8 = new JLabel();
    JLabel jLabel9 = new JLabel();
    JLabel jLabel10 = new JLabel();
    JTextField height_field = new IntegerTextField();
    JTextField width_field = new IntegerTextField();
    JTextField ypos_field = new IntegerTextField();
    JTextField xpos_field = new IntegerTextField();

    JList windows_jlist = new JList();
    DefaultComboBoxModel windows_jlist_model; // this is such a cheat...

    private class InputWindowData {
        public String proxy_name;
        public int x;
        public int y;
        public int width;
        public int height;
        public float mouse_sensitivity;
        public ConfigChunk window_chunk;
        public ConfigChunk proxy_chunk;
        public boolean dirty;

        public InputWindowData () {
        }

        public String toString () {
            return proxy_name;
        }
    }

    boolean ui_initialized;
    boolean results_dirty;  // does results_db need to be updated from ui?
    ArrayList windows_list;
    InputWindowData current_editing_window;

    ConfigChunkDB results_db;


    public InputWindowPanel() {
        results_db = null;
        results_dirty = false;
        ui_initialized = false;
        windows_list = new ArrayList();
        current_editing_window = null;

        try {
            jbInit();
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }
    private void jbInit() throws Exception {
        titledBorder1 = new TitledBorder("");
        border1 = new EtchedBorder(EtchedBorder.RAISED,Color.white,new Color(142, 142, 142));
        titledBorder2 = new TitledBorder(BorderFactory.createEtchedBorder(Color.white,new Color(142, 142, 142)),"Edit window properties");
        right_panel_interior = Box.createVerticalBox();
        this.setLayout(borderLayout1);
        left_panel.setLayout(borderLayout2);
        window_layout_panel.setMinimumSize(new Dimension(10, 150));
        window_layout_panel.setPreferredSize(new Dimension(10, 150));
        remove_button.setText("Remove");
        add_button.setText("Add");
        right_panel.setBorder(titledBorder2);
        right_panel.setLayout (new BoxLayout (right_panel, BoxLayout.Y_AXIS));
        borderLayout1.setHgap(5);
        borderLayout1.setVgap(5);
        jLabel1.setText("Window name");
        jLabel2.setText("Mouse sensitivity");
        name_field.setMinimumSize(new Dimension(15, 19));
        name_field.setPreferredSize(new Dimension(25, 19));
        name_field.setColumns(13);
        mouse_slider.setPreferredSize(new Dimension(100, 16));
        this.setMinimumSize(new Dimension(500, 207));
        this.setPreferredSize(new Dimension(600, 188));
        right_panel_2.setLayout(gridLayout1);
        gridLayout1.setRows(4);
        gridLayout1.setColumns(3);
        jLabel3.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel3.setText("Position");
        jLabel4.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel4.setText("Height  ");
        jLabel6.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel6.setText("Width  ");
        jLabel7.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel7.setText("Size");
        jLabel8.setToolTipText("");
        jLabel8.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel8.setText("Y  ");
        jLabel10.setHorizontalAlignment(SwingConstants.TRAILING);
        jLabel10.setText("X  ");
        xpos_field.setColumns(5);
        this.add(left_panel, BorderLayout.CENTER);
        left_panel.add(window_layout_panel, BorderLayout.NORTH);
        left_panel.add(left_buttons_panel, BorderLayout.SOUTH);
        left_buttons_panel.add(add_button, null);
        left_buttons_panel.add(remove_button, null);
        left_panel.add(windows_scrollpane, BorderLayout.CENTER);
        windows_scrollpane.getViewport().add(windows_jlist, null);
        this.add(right_panel, BorderLayout.EAST);
        right_panel.add(right_panel_interior, null);
        right_panel_interior.add(right_panel_1, null);
        right_panel_1.add(jLabel1, null);
        right_panel_1.add(name_field, null);
        right_panel_interior.add(right_panel_2, null);
        right_panel_2.add(jLabel3, null);
        right_panel_2.add(jLabel10, null);
        right_panel_2.add(xpos_field, null);
        right_panel_2.add(jLabel9, null);
        right_panel_2.add(jLabel8, null);
        right_panel_2.add(ypos_field, null);
        right_panel_2.add(jLabel7, null);
        right_panel_2.add(jLabel6, null);
        right_panel_2.add(width_field, null);
        right_panel_2.add(jLabel5, null);
        right_panel_2.add(jLabel4, null);
        right_panel_2.add(height_field, null);
        right_panel_interior.add(right_panel_3, null);
        right_panel_3.add(jLabel2, null);
        right_panel_3.add(mouse_slider, null);
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
        int x, y, width, height;
        String window_name;
        InputWindowData iw;
        Iterator it;

        window_layout_panel.clear();
        window_layout_panel.setWindowScreenSize (java.awt.Toolkit.getDefaultToolkit().getScreenSize());

        chunks = results_db.getOfDescToken ("displayWindow");
        it = chunks.iterator();
        while (it.hasNext()) {
            ch = (ConfigChunk)it.next();
            x = ch.getValueFromToken ("origin",0).getInt();
            y = ch.getValueFromToken ("origin",1).getInt();
            width = ch.getValueFromToken ("size",0).getInt();
            height = ch.getValueFromToken ("size",1).getInt();
            window_name = ch.getName();
            window_layout_panel.addWindow (window_name, x, y, width, height, Color.red);
        }

        // what i need to do here is:
        // for any IWData not in results_db, add window to layout & results
        // for any KeyBoard chunk not in windows_list, add to list & layout
        it = windows_list.iterator();
        while (it.hasNext())
            ((InputWindowData)it.next()).dirty = true;
        chunks = results_db.getOfDescToken ("KeyboardProxy");
        it = chunks.iterator();
        while (it.hasNext()) {
            ch = (ConfigChunk)it.next();
            processKeyboardChunk (ch, null);
        }
        it = windows_list.iterator();
        while (it.hasNext()) {
            iw = (InputWindowData)it.next();
            if (iw.dirty) {
                iw.dirty = false;
                results_db.add (iw.proxy_chunk);
                results_db.add (iw.window_chunk);
                window_layout_panel.addWindow (iw.proxy_name, iw.x, iw.y, iw.width, iw.height, Color.blue);
            }
        }
    }


    // util for setUIState
    private void processKeyboardChunk (ConfigChunk proxy_chunk, ConfigChunk window_chunk) {
        int x, y, width, height;
        String window_name;
        String proxy_name;
        InputWindowData iw;
        boolean do_insert;

        proxy_name = proxy_chunk.getName();
        window_name = proxy_chunk.getValueFromToken ("device",0).getString();
        if (window_chunk == null)
            window_chunk = results_db.get(window_name);
        if (window_chunk.getDescToken().equalsIgnoreCase("displayWindow"))
            return; // we don't care about it.

        x = window_chunk.getValueFromToken ("origin",0).getInt();
        y = window_chunk.getValueFromToken ("origin",1).getInt();
        width = window_chunk.getValueFromToken ("width",0).getInt();
        height = window_chunk.getValueFromToken ("height",0).getInt();

        window_layout_panel.addWindow (proxy_name, x, y, width, height, Color.blue);
        iw = getInputWindowData (proxy_name);
        if (iw == null) {
            iw = new InputWindowData();
            do_insert = true;
        }
        else
            do_insert = false;
        iw.proxy_name = proxy_name;
        iw.x = x;
        iw.y = y;
        iw.width = width;
        iw.height = height;
        iw.mouse_sensitivity = window_chunk.getValueFromToken ("msens",0).getFloat();
        iw.proxy_chunk = proxy_chunk;
        iw.window_chunk = window_chunk;
        iw.dirty = false;
        
        if (do_insert) {
            windows_list.add (iw);
            windows_jlist_model.addElement (iw);
        }
    }


    private void setEditingWindow (InputWindowData iw) {
        if (iw != current_editing_window) {
            current_editing_window = iw;
            if (iw == null) {
                name_field.setText("");
                xpos_field.setText("");
                ypos_field.setText("");
                width_field.setText("");
                height_field.setText("");
                name_field.setEditable(false);
                xpos_field.setEditable(false);
                ypos_field.setEditable(false);
                width_field.setEditable(false);
                height_field.setEditable(false);
            }
            else {
                name_field.setText(iw.proxy_name);
                xpos_field.setText(Integer.toString(iw.x));
                ypos_field.setText(Integer.toString(iw.y));
                width_field.setText(Integer.toString(iw.width));
                height_field.setText(Integer.toString(iw.height));
                name_field.setEditable(true);
                xpos_field.setEditable(true);
                ypos_field.setEditable(true);
                width_field.setEditable(true);
                height_field.setEditable(true);
            }
        }
    }

    private InputWindowData getInputWindowData (String name) {
        InputWindowData iw;
        int i, n = windows_list.size();
        for (i = 0; i < n; i++) {
            iw = (InputWindowData)windows_list.get(i);
            if (iw.proxy_name.equals(name))
                return iw;
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
            windows_jlist_model = new DefaultComboBoxModel();
            windows_jlist.setModel(windows_jlist_model);

            add_button.addActionListener(this);
            remove_button.addActionListener(this);
            windows_jlist.addListSelectionListener (this);

            // since we start off with nothing selected, can't edit fields.
            name_field.setEditable(false);
            xpos_field.setEditable(false);
            ypos_field.setEditable(false);
            width_field.setEditable(false);
            height_field.setEditable(false);

            // listen for these events so we can update the visual display of
            // the window.
            name_field.addFocusListener (this);
            xpos_field.addFocusListener (this);
            ypos_field.addFocusListener (this);
            width_field.addFocusListener (this);
            height_field.addFocusListener (this);

            name_field.addActionListener (this);
            xpos_field.addActionListener (this);
            ypos_field.addActionListener (this);
            width_field.addActionListener (this);
            height_field.addActionListener (this);

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
        // this should return false if: 
        //    a) there are no input windows (and display isn't an input source)
        //    b) there are input windows with the same name
        //    c) any window has null dimensions
        if (windows_list.isEmpty()) {
            JOptionPane.showMessageDialog (
                null, "No Input Sources have been defined.", 
                "Wizard Error", JOptionPane.ERROR_MESSAGE);
            return false;
        }
        InputWindowData iw, iw2;
        int i, j, n = windows_list.size();
        for (i = 0; i < n; i++) {
            iw = (InputWindowData)windows_list.get(i);
            if ((iw.width == 0) || (iw.height == 0)) {
                JOptionPane.showMessageDialog (
                    null, "Window " + iw.proxy_name + " has width or height of 0.", 
                    "Wizard Error", JOptionPane.ERROR_MESSAGE);
                return false;
            }
            for (j = i+1; j < n; j++) {
                iw2 = (InputWindowData)windows_list.get(j);
                if (iw.proxy_name.equalsIgnoreCase(iw2.proxy_name)) {
                    JOptionPane.showMessageDialog (
                        null, "Windows must have unique names.", 
                        "Wizard Error", JOptionPane.ERROR_MESSAGE);
                    return false;
                }
            }
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
        if (results_dirty && (results_db != null)) {
            // query the GUI widgets to update results_db
            java.util.List chunks;
            ConfigChunk ch;
            Property p;
            InputWindowData iw;
            
            Iterator it = windows_list.iterator();
            while (it.hasNext()) {
                iw = (InputWindowData)it.next();
                ch = iw.proxy_chunk;

                ch.setName (iw.proxy_name);
                String window_name = iw.proxy_name + "_window";
                p = ch.getPropertyFromToken ("device");
                p.setValue (window_name, 0);

                ch = iw.window_chunk;
                ch.setName (window_name);

                p = ch.getPropertyFromToken ("origin");
                p.setValue (new VarValue(iw.x), 0);
                p.setValue (new VarValue(iw.y), 1);

                p = ch.getPropertyFromToken ("width");
                p.setValue (new VarValue(iw.width), 0);
                p = ch.getPropertyFromToken ("height");
                p.setValue (new VarValue(iw.height), 0);

                p = ch.getPropertyFromToken ("msens");
                p.setValue (new VarValue(iw.mouse_sensitivity), 0);

                // we depend on chunk default values etc. to set the rest 
                // of these things...
            }

            results_dirty = false;
        }
        return results_db;
    }


    public void reset () {
        window_layout_panel.clear();
        windows_list.clear();
        windows_jlist_model.removeAllElements();
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
        Object source = e.getSource();
        if (source == add_button) {
            InputWindowData iw = new InputWindowData();
            ConfigChunk proxy_chunk = ChunkFactory.createChunkWithDescToken ("KeyboardProxy");
            ConfigChunk window_chunk = ChunkFactory.createChunkWithDescToken ("Keyboard");
            String proxy_name = results_db.getNewName ("Input Window");
            System.out.println ("creating new input: '" + proxy_name + "'");
            String window_name = proxy_name + "_window";
            proxy_chunk.setName (proxy_name);
            window_chunk.setName (window_name);
            Property p = proxy_chunk.getPropertyFromToken ("device");
            p.setValue (window_name, 0);
            processKeyboardChunk (proxy_chunk, window_chunk);
            results_db.add (proxy_chunk);
            results_db.add (window_chunk);
        }
        else if (source == remove_button) {
            Object[] selects = windows_jlist.getSelectedValues();
            // go backwards so that we don't shift later indices by
            // deleting earlier ones first.
            for (int i = selects.length - 1; i >= 0; i--) {
                InputWindowData iw = (InputWindowData)selects[i];
                windows_jlist_model.removeElement (iw);
                windows_list.remove (iw);
                results_db.remove (iw.proxy_chunk);
                results_db.remove (iw.window_chunk);
                window_layout_panel.removeWindow (iw.proxy_name);
                if (iw == current_editing_window)
                    setEditingWindow (null);
            }
        }
        else
            fieldChanged (source);
    }

    //----------------------- ListSelectionListener Stuff ------------------

    public void valueChanged (ListSelectionEvent e) {
        InputWindowData selected = null;
        int i = e.getFirstIndex();
        int n = e.getLastIndex();
        for (; i <= n; i++) {
            if (windows_jlist.isSelectedIndex(i)) {
                selected = (InputWindowData)windows_jlist_model.getElementAt(i);
                break;
            }
        }
        if (selected != null) {
            setEditingWindow (selected);
        }
    }


    //----------------------- Focus/Action Utilities -----------------------


    /** Called when a field sends ActionEvent or FocusLost.
     *  Note that this means the field _may_ have changed; we should check
     *  before acting.
     */
    private void fieldChanged (Object source) {
        if (current_editing_window == null)
            return;

        if (source == name_field) {
            String newname = name_field.getText();
            if (!newname.equals(current_editing_window.proxy_name)) {
                if (newname.equals("")) {
                    Core.consoleErrorMessage ("Wizard", "Name must have a value.");
                    name_field.setText (current_editing_window.proxy_name);
                }
                else {
                    window_layout_panel.removeWindow (current_editing_window.proxy_name);
                    current_editing_window.proxy_name = newname;
                    windows_jlist.repaint(); // pick up new name... i hope
                    window_layout_panel.addWindow (
                        current_editing_window.proxy_name, 
                        current_editing_window.x, 
                        current_editing_window.y, 
                        current_editing_window.width, 
                        current_editing_window.height,
                        Color.blue);
                    results_dirty = true;
                }
            }
        }
        else if (source == xpos_field) {
            int newxpos = Integer.parseInt (xpos_field.getText());
            if (newxpos != current_editing_window.x) {
                if (newxpos < 0) {
                    Core.consoleErrorMessage ("Wizard", "Invalid X position.");
                    xpos_field.setText (Integer.toString(current_editing_window.x));
                }
                else {
                    current_editing_window.x = newxpos;
                    window_layout_panel.moveWindow (
                        current_editing_window.proxy_name, 
                        current_editing_window.x, 
                        current_editing_window.y, 
                        current_editing_window.width, 
                        current_editing_window.height);
                    results_dirty = true;
                }
            }
        }
        else if (source == ypos_field) {
            int newypos = Integer.parseInt (ypos_field.getText());
            if (newypos != current_editing_window.y) {
                if (newypos < 0) {
                    Core.consoleErrorMessage ("Wizard", "Invalid Y position.");
                    ypos_field.setText (Integer.toString(current_editing_window.y));
                }
                else {
                    current_editing_window.y = newypos;
                    window_layout_panel.moveWindow (
                        current_editing_window.proxy_name, 
                        current_editing_window.x, 
                        current_editing_window.y, 
                        current_editing_window.width, 
                        current_editing_window.height);
                    results_dirty = true;
                }
            }
        }
        else if (source == width_field) {
            int newwidth = Integer.parseInt (width_field.getText());
            if (newwidth != current_editing_window.width) {
                if (newwidth <= 0) {
                    Core.consoleErrorMessage ("Wizard", "Invalid width.");
                    width_field.setText (Integer.toString(current_editing_window.width));
                }
                else {
                    current_editing_window.width = newwidth;
                    window_layout_panel.moveWindow (
                        current_editing_window.proxy_name, 
                        current_editing_window.x, 
                        current_editing_window.y, 
                        current_editing_window.width, 
                        current_editing_window.height);
                    results_dirty = true;
                }
            }
        }
        else if (source == height_field) {
            int newheight = Integer.parseInt (height_field.getText());
            if (newheight != current_editing_window.height) {
                if (newheight <= 0) {
                    Core.consoleErrorMessage ("Wizard", "Invalid height.");
                    height_field.setText (Integer.toString(current_editing_window.height));
                }
                else {
                    current_editing_window.height = newheight;
                    window_layout_panel.moveWindow (
                        current_editing_window.proxy_name, 
                        current_editing_window.x, 
                        current_editing_window.y, 
                        current_editing_window.width, 
                        current_editing_window.height);
                    results_dirty = true;
                }
            }
        }
    }

}
