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



package VjComponents.PerfMonitor;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.util.*;
import java.io.File;
import javax.swing.tree.*;

import VjComponents.PerfMonitor.*;
import VjControl.*;
import VjComponents.UI.*;
import VjConfig.*;


/** PlugPanel for displaying data from PerformanceModule
 *  This panel is connected to a PerfDataCollection object.
 *  It will include a chooser to select which data set in the collection
 *  to show, a scrollpane for displaying text or graphics, and some other
 *  gadgest to control what gets displayed.
 *
 *  @see VjComponents.PerfMonitor.PerformanceModule
 * 
 *  @version $Revision$
 */
public class PerfAnalyzerPanel 
    extends JPanel 
    implements PlugPanel, 
               ActionListener,
               PerformanceModuleListener {


    protected class NumberedPanelButton extends JButton {
	public PerfDataCollector collector;
	public int phase;

	public NumberedPanelButton (PerfDataCollector _collector, 
                               int _phase,
                               String text) {
	    super (text);
	    collector = _collector;
	    phase = _phase;
	}
    }

    protected class LabeledPanelButton extends JButton {
        public LabeledPerfDataCollector collector;
        public LabeledPerfDataCollector.IndexInfo index_info;

        public LabeledPanelButton (LabeledPerfDataCollector _collector, 
                                   LabeledPerfDataCollector.IndexInfo _ii, 
                                   String text) {
            super (text);
            collector = _collector;
            index_info = _ii;
        }
    }

    protected interface DataPanelElem 
        extends ActionListener {

	public void initialize (JPanel panel, 
			      GridBagLayout gblayout, GridBagConstraints gbc);

        public void destroy ();

	public void update();

        public PerfDataCollector getPerfDataCollector();
    }


    protected class NumberedDataPanelElem 
        implements DataPanelElem {

	public NumberedPerfDataCollector col;
	public JLabel sep_label;
	public JLabel colname_label;
	public NumberedPanelButton colsummary_button;
	public JLabel avgs_label;
	public JLabel[] phase_labels;
	public JLabel[] avg_labels;
	public NumberedPanelButton[] graph_buttons;
	public NumberedPanelButton[] anomalies_buttons;
	public int num;

        public NumberedDataPanelElem (NumberedPerfDataCollector _col) {
            col = _col;
        }

	public void initialize (JPanel panel, 
			      GridBagLayout gblayout, GridBagConstraints gbc) {
	    Insets insets = new Insets (1,1,1,1);
	    num = col.getNumPhases();
	    phase_labels = new JLabel[num];
	    avg_labels = new JLabel[num];
	    graph_buttons = new NumberedPanelButton[num];
	    anomalies_buttons = new NumberedPanelButton[num];

	    col.generateAverages(preskip, postskip);
	    gbc.gridwidth = gbc.REMAINDER;
	    sep_label = new JLabel ("----------------------------------------------");
	    gblayout.setConstraints (sep_label, gbc);
	    panel.add (sep_label);

	    colname_label = new JLabel (col.getName());
	    gbc.gridwidth = 1;
	    gblayout.setConstraints (colname_label, gbc);
	    panel.add (colname_label);

	    colsummary_button = new NumberedPanelButton (col, -1, "Graph");
            colsummary_button.setActionCommand ("Graph");
	    colsummary_button.addActionListener (PerfAnalyzerPanel.this);
	    gbc.gridwidth = gbc.REMAINDER;
	    gblayout.setConstraints (colsummary_button, gbc);
	    panel.add (colsummary_button);

	    avgs_label = new JLabel ("Average times in milliseconds:");
	    gblayout.setConstraints(avgs_label, gbc);
	    panel.add (avgs_label);

	    JLabel l;
	    JButton b;
	    for (int j = 0; j < num; j++) {
		double avg = col.getAverageForPhase(j);
		// this below will cause trouble
		if (avg == 0.0)
		    continue;
		gbc.gridwidth = 1;
		l = phase_labels[j] = new JLabel (j + ": " + col.getLabelForPhase(j));
		gblayout.setConstraints (l, gbc);
		panel.add(l);
		l = avg_labels[j] = new JLabel (padFloat(avg/1000.0), JLabel.RIGHT);
		gblayout.setConstraints (l, gbc);
		panel.add(l);
		b = graph_buttons[j] = new NumberedPanelButton (col, j, "Graph");
                b.setActionCommand ("Graph");
		b.addActionListener (PerfAnalyzerPanel.this);
		b.setMargin(insets);
		gblayout.setConstraints (b, gbc);
		panel.add(b);
		b = anomalies_buttons[j] = new NumberedPanelButton (col, j, "Anomalies");
                b.setActionCommand ("Anomalies");
		b.setEnabled(false);
		b.addActionListener (PerfAnalyzerPanel.this);
		b.setMargin(insets);
		gbc.gridwidth = gbc.REMAINDER;
		gblayout.setConstraints (b, gbc);
		panel.add(b);
	    }

            col.addActionListener (this);
	}

        public void destroy () {
            col.removeActionListener (this);
        }

	public void update() {
	    col.generateAverages(preskip, postskip);
	    for (int i = 0; i < num; i++) {
		double avg = col.getAverageForPhase(i);
		if (avg_labels[i] != null)
		    avg_labels[i].setText(padFloat(avg/1000.0));
		else
		    ; // we should probably add something in this case...
	    }
	}


        public PerfDataCollector getPerfDataCollector() {
            return col;
        }


        public void actionPerformed (ActionEvent e) {
            if (e.getActionCommand().equals ("Update")) {
                update();
            }
        }

    }



    protected class LabeledDataPanelElem 
        implements DataPanelElem {

	public LabeledPerfDataCollector col;
	public JLabel sep_label;
	public JLabel colname_label;
	public LabeledPanelButton colsummary_button;
	public JLabel avgs_label;
	public java.util.List index_labels;       // list of JLabel
	public java.util.List avg_labels;         // list of JLabel
	public java.util.List graph_buttons;      // list of LabeledPanelButton
	public java.util.List anomalies_buttons;  // list of LabeledPanelButton
        public JPanel panel;
	public DefaultMutableTreeNode root;

        public LabeledDataPanelElem (LabeledPerfDataCollector _col) {
            col = _col;
            index_labels = new ArrayList();
            avg_labels = new ArrayList();
            graph_buttons = new ArrayList();
            anomalies_buttons = new ArrayList();
	    root = new DefaultMutableTreeNode();
	    root.setUserObject (new TreeElem ("Root", null));
        }

	public void initialize (JPanel _panel, 
			      GridBagLayout gblayout, GridBagConstraints gbc) {
            panel = _panel;

	    col.generateAverages(preskip, postskip);
	    gbc.gridwidth = gbc.REMAINDER;
	    sep_label = new JLabel ("----------------------------------------------");
	    gblayout.setConstraints (sep_label, gbc);
	    panel.add (sep_label);

	    colname_label = new JLabel (col.getName());
	    gbc.gridwidth = 1;
	    gblayout.setConstraints (colname_label, gbc);
	    panel.add (colname_label);

	    colsummary_button = new LabeledPanelButton (col, null, "Graph");
            colsummary_button.setActionCommand ("Graph");
	    colsummary_button.addActionListener (PerfAnalyzerPanel.this);
	    gbc.gridwidth = gbc.REMAINDER;
	    gblayout.setConstraints (colsummary_button, gbc);
	    panel.add (colsummary_button);

	    avgs_label = new JLabel ("Average times in milliseconds:");
	    gblayout.setConstraints(avgs_label, gbc);
	    panel.add (avgs_label);

            Iterator i = col.indexIterator();
            while (i.hasNext()) {
                LabeledPerfDataCollector.IndexInfo ii = (LabeledPerfDataCollector.IndexInfo)i.next();
                addInitial (ii);
 	    }

            col.addActionListener (this);
	}

        public void destroy () {
            col.removeActionListener (this);
        }

	protected class TreeElem {
	    public String sublabel;
	    public LabeledPerfDataCollector.IndexInfo ii; // null for folders
	    public JComponent component;
	    public TreeElem (String _sublabel, LabeledPerfDataCollector.IndexInfo _ii) {
		sublabel = sublabel;
		ii = _ii;
		component = new JLabel (sublabel);
	    }
	}

	public void addToTree (LabeledPerfDataCollector.IndexInfo ii) {
	    MutableTreeNode node = root;
	    MutableTreeNode new_node;
	    int i, j, n;  // i = index into ii.label_components
	    i = 0;
	    // first, find the parent
	    for (i = 0; i < ii.label_components.size() - 1; i++) {
		new_node = null;
		for (j = 0; j < node.getChildCount(); j++) {
		    DefaultMutableTreeNode mtn = (DefaultMutableTreeNode)node.getChildAt(j);
		    if (((TreeElem)mtn.getUserObject()).sublabel.equals((String)ii.label_components.get(i))) {
			new_node = mtn;
		    }
		}
		if (new_node == null) {
		    // didn't find it, create folder node
		    new_node = new DefaultMutableTreeNode();
		    new_node.setUserObject (new TreeElem ((String)ii.label_components.get(i), null));
		    node.insert (new_node, node.getChildCount());
		}
		node = new_node;
	    }
	    // add ii as a child node of node.
	    new_node = new DefaultMutableTreeNode();
	    new_node.setUserObject (new TreeElem ((String)ii.label_components.get(ii.label_components.size()-1), ii));
	    node.insert (new_node, node.getChildCount());
	}

        public void addInitial (LabeledPerfDataCollector.IndexInfo ii) {
	    System.out.println ("addInitial: " + ii.index);
	    JLabel l;
	    JButton b;
	    Insets insets = new Insets (1,1,1,1);
            double avg = ii.getAverage();
//             // this below will cause trouble
//             if (avg == 0.0)
//                 continue;
            gbc.gridwidth = 1;
            l = new JLabel (/*i + ": " + */ii.index);
            index_labels.add(l);
            gblayout.setConstraints (l, gbc);
            panel.add(l);
            l = new JLabel (padFloat(avg/1000.0), JLabel.RIGHT);
            avg_labels.add(l);
            gblayout.setConstraints (l, gbc);
            panel.add(l);
            b = new LabeledPanelButton (col, ii, "Graph");
            graph_buttons.add(b);
            b.setActionCommand ("Graph");
            b.addActionListener (PerfAnalyzerPanel.this);
            b.setMargin(insets);
            gblayout.setConstraints (b, gbc);
//          panel.add(b);
//  	    panel.revalidate();
// 	    b = anomalies_buttons[j] = new AnomaliesButton (col, j);
// 	    b.setEnabled(false);
// 	    b.addActionListener (PerfAnalyzerPanel.this);
// 	    b.setMargin(insets);
	    gbc.gridwidth = gbc.REMAINDER;
	    gblayout.setConstraints (b, gbc);
	    panel.add(b);
        }

	public void update() {
 	    col.generateAverages(preskip, postskip);
            int num = index_labels.size();
            Iterator i = col.indexIterator();
            int j = 0;
            while (i.hasNext()) {
                LabeledPerfDataCollector.IndexInfo ii = (LabeledPerfDataCollector.IndexInfo)i.next();
		System.out.println ("updating " + ii.index);
                if (j < num) {
                    JLabel l = (JLabel)avg_labels.get(j);
                    l.setText (padFloat(ii.getAverage()/1000.0));
                }
                else
                    addInitial (ii);
                j++;
 	    }
	}


        public PerfDataCollector getPerfDataCollector() {
            return col;
        }


        public void actionPerformed (ActionEvent e) {
            if (e.getActionCommand().equals ("Update")) {
                update();
            }
        }

    }





    JComboBox  display_choice;
    JComboBox  max_samples_box;
    JTextField preskip_box;
    JTextField postskip_box;
    JButton savecontents_button;
    JButton load_button;
    JButton print_all_button;
    JPanel data_panel;
    GridBagLayout gblayout;
    GridBagConstraints gbc;
    boolean ui_initialized;

    JScrollPane display_pane;
    JTextArea text_area;

    PerformanceModule perf_module;
    protected String component_name;
    protected ConfigChunk component_chunk;

    ControlUIModule ui_module;
    protected SuffixFilter perf_filter;

    Vector child_frames;  // vector of GenericGraphFrames...
    Vector datapanel_elems;

    int preskip, postskip;
    float anomalycutoff;
    boolean doanomaly;


    public PerfAnalyzerPanel () {
	super();

        component_name = "Unconfigured PerfAnalyzerPanel";
        component_chunk = null;

	child_frames = new Vector();
	datapanel_elems = new Vector();
	perf_module = null;
        ui_module = null;
	text_area = null;

	preskip = 20;
	postskip = 20;
	doanomaly = true;
	anomalycutoff = 1.0f;

        ui_initialized = false;

    }

    /** Utility method for various printing routines. */
    private String padFloat (double f) {
	// reformats f to a string w/ 3 places after decimal
	String s = Double.toString(f);
	if (s.indexOf('E') != -1)
	    return s;
	int i = s.lastIndexOf('.');
	if ((i >= 0) && (i+5 < s.length()))
	    s = s.substring (0, i + 5);
	return s;
    }



    public DataPanelElem getDataPanelElem (PerfDataCollector col) {
	DataPanelElem dpe;
	for (int i = 0; i < datapanel_elems.size(); i++) {
	    dpe = (DataPanelElem)datapanel_elems.elementAt(i);
	    if (col == dpe.getPerfDataCollector())
		return dpe;
	}
	return null;
    }



    public void addDataPanelElem (PerfDataCollector col) {
        DataPanelElem dpe;
        if (col instanceof NumberedPerfDataCollector)
            dpe = new NumberedDataPanelElem ((NumberedPerfDataCollector)col);
        else
            dpe = new LabeledDataPanelElem ((LabeledPerfDataCollector)col);
        datapanel_elems.add(dpe);
        if (ui_initialized)
            dpe.initialize (data_panel, gblayout, gbc);
    }



    public void actionPerformed (ActionEvent e) {
        Object source = e.getSource();

        if (source instanceof GenericGraphFrame) {
            GenericGraphFrame f = (GenericGraphFrame)source;
            child_frames.removeElement(f);
            f.destroy();
        }
        else if (source instanceof NumberedPanelButton) {
            NumberedPanelButton b = (NumberedPanelButton)e.getSource();
            if (e.getActionCommand().equals ("Graph")) {
                GenericGraphPanel gp = new SummaryGraphPanel ((NumberedPerfDataCollector)b.collector, b.phase);
                String title = "Graph of " + b.collector.getName();
                if (b.phase >= 0)
                    title = title + " phase " + b.phase;
                GenericGraphFrame gf = new GenericGraphFrame (gp, title);
                gf.addActionListener (this);
                
                child_frames.addElement(gf);
                gf.show();
            }
            else if (e.getActionCommand().equals ("Anomalies")) {
                System.out.println ("Not implemented");
            }
        }
        else if (source instanceof LabeledPanelButton) {
            LabeledPanelButton b = (LabeledPanelButton)e.getSource();
            if (e.getActionCommand().equals ("Graph")) {
                GenericGraphPanel gp = new LabeledSummaryGraphPanel (b.collector, b.index_info);
                String title = "Graph of " + b.collector.getName();
                if (b.index_info != null)
                    title = title + " label = " + b.index_info.index;
                GenericGraphFrame gf = new GenericGraphFrame (gp, title);
                gf.addActionListener (this);
                child_frames.addElement (gf);
                gf.show();
            }
        }
	else if (source == max_samples_box) {
	    int numsamps;
	    String s = (String)max_samples_box.getSelectedItem();
	    if (s.equalsIgnoreCase("<Infinite>"))
		numsamps = -1;
	    else
		numsamps = Integer.parseInt(s);
	    //System.out.println ("setting max samples to " + numsamps);
	    perf_module.setMaxSamples(numsamps);
	}
	else if (source == display_choice)
	    refreshDisplay();
	else if (source == preskip_box) {
	    preskip = Integer.parseInt (preskip_box.getText().trim());
	    refreshDisplay();
	}
	else if (source == postskip_box) {
	    postskip = Integer.parseInt (postskip_box.getText().trim());
	    refreshDisplay();
	}
	else if (source == savecontents_button) {
	    //collection.savePerfDataFile ();
            File f = ui_module.getEasyFileDialog().requestSaveFile(perf_module.getFile(), ui_module, perf_filter);
            perf_module.savePerfDataFile (f);
	}
	else if (source == load_button) {
            File f = ui_module.getEasyFileDialog().requestOpenFile (perf_module.getFile().getParentFile(), ui_module, perf_filter);
            if (f != null) {
                String name = perf_module.loadNewPerfDataFile (f);
                if (name != null)
                    refreshDisplay();
            }
	}
	else if (source == print_all_button) {
	    String s = "";
	    PerfDataCollector col;
	    for (int i = 0; i < perf_module.getSize(); i++) {
		col = perf_module.getCollector(i);
		s += col.dumpAverages (preskip, postskip, false, anomalycutoff);
	    }
	    System.out.println (s);
	}
    }



    public void refreshDisplay() {
        if (ui_initialized) {
            DataPanelElem dpe;
            int n = datapanel_elems.size();
            for (int i = 0; i < n; i++) {
                dpe = (DataPanelElem)datapanel_elems.elementAt(i);
                dpe.update();
            }
        }
    }



    public void removeAllData() {
        if (ui_initialized) {
            int i, n;

            for (i = 0; i < child_frames.size(); i++) {
                GenericGraphFrame f = (GenericGraphFrame)child_frames.get(i);
                f.destroy();
            }
            child_frames.removeAllElements();

            DataPanelElem dpe;
            n = datapanel_elems.size();
            for (i = 0; i < n; i++) {
                dpe = (DataPanelElem)datapanel_elems.elementAt(i);
                dpe.destroy();
            }
            datapanel_elems.clear();
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
                    if (c instanceof ControlUIModule)
                        ui_module = (ControlUIModule)c;
                    if (c instanceof PerformanceModule)
                        perf_module = (PerformanceModule)c;
                }
            }
        }
    }


    public void initialize () throws VjComponentException {
        if (perf_module == null || ui_module == null)
            throw new VjComponentException (component_name + ": Instantiated with unmet dependencies.");

	perf_module.addPerformanceModuleListener (this);
        // get all the alread-added perf modules...
        for (int i = 0; i < perf_module.getSize(); i++) {
            addDataPanelElem (perf_module.getCollector(i));
        }

        perf_filter = new SuffixFilter ("Perf Data Files (*.perf)", ".perf");
        ui_module.getEasyFileDialog().addFilter (perf_filter, "PerfData");

        refreshDisplay();
    }


    public VjComponent addConfig (ConfigChunk ch) throws VjComponentException {
        throw new VjComponentException (component_name + " does not support child component: " + ch.getName());
    }


    public boolean removeConfig (String name) {
        return false;
    }

    public JComponent getUIComponent () {
        return this;
    }

    public boolean initUIComponent() {
        if (!ui_initialized) {
            setLayout (new BorderLayout (5, 5));
            setBorder (new EmptyBorder (5,5,5,5));

            data_panel = new JPanel();
            data_panel.setLayout (gblayout = new GridBagLayout());
            gbc = new GridBagConstraints();
            gbc.insets = new Insets (0, 4, 0, 4);
            gbc.fill = gbc.HORIZONTAL;
            

            JPanel epanel = new JPanel ();
            epanel.setLayout (new GridLayout (10, 1, 2, 0));
            add (epanel, "East");

            load_button = new JButton ("Load");
            epanel.add (load_button);
            load_button.addActionListener (this);

            epanel.add (savecontents_button = new JButton ("Save Data"));
            savecontents_button.addActionListener (this);

            print_all_button = new JButton ("Print");
            print_all_button.addActionListener (this);
            epanel.add (print_all_button);
       

            JPanel npanel = new JPanel ();
            npanel.setLayout (new BoxLayout (npanel, BoxLayout.Y_AXIS));
            add (npanel, "North");
            JPanel ntoppanel = new JPanel();
            ntoppanel.setLayout (new BoxLayout (ntoppanel, BoxLayout.X_AXIS));
            npanel.add (ntoppanel);
            
            ntoppanel.add (new JLabel ("Maximum stored samples"));
            ntoppanel.add (max_samples_box = new JComboBox());
            max_samples_box.addItem ("100");
            max_samples_box.addItem ("500");
            max_samples_box.addItem ("1000");
            max_samples_box.addItem ("5000");
            max_samples_box.addItem ("<Infinite>");
            max_samples_box.setSelectedIndex(1);
            max_samples_box.addActionListener (this);

            display_pane = new JScrollPane (data_panel, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
            
            add (display_pane, "Center");

            for (int i = 0; i < datapanel_elems.size(); i++) {
                DataPanelElem dpe = (DataPanelElem)datapanel_elems.get(i);
                dpe.initialize (data_panel, gblayout, gbc);
            }

            ui_initialized = true;
        }
        return true;
    }


    public void destroy () {
        removeAllData ();
    }


    public void rebuildDisplay () {
    }


    //------------------ PerformanceModuleListener Stuff ---------------------

    public void perfDataCollectorAdded (PerformanceModuleEvent e) {
        addDataPanelElem (e.getPerfDataCollector());
    }

    public void perfDataCollectorRemoved (PerformanceModuleEvent e) {
        // not implemented
    }

    public void perfDataCollectorsCleared (PerformanceModuleEvent e) {
        removeAllData ();
    }

}
