

/*
 * PerfAnalyzerPanel
 *
 * This panel is connected to a PerfDataCollection object.
 * It will include a chooser to select which data set in the collection
 * to show, a scrollpane for displaying text or graphics, and some other
 * gadgest to control what gets displayed.
 *
 */

package VjPerf;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.EmptyBorder;
import VjPerf.*;
import VjGUI.FileControl;

public class PerfAnalyzerPanel extends JPanel implements ActionListener {


    private class CollectorSummaryButton extends JButton {
	public PerfDataCollector collector;

	public CollectorSummaryButton (PerfDataCollector _collector) {
	    super ("Summary");
	    collector = _collector;
	}
    }


    private class AnomaliesButton extends JButton {
	public PerfDataCollector collector;
	public int phase;

	public AnomaliesButton (PerfDataCollector _collector, int _phase) {
	    super ("Anomalies");
	    collector = _collector;
	    phase = _phase;
	}
    }


    private class GraphButton extends JButton {
	public PerfDataCollector collector;
	public int phase;

	public GraphButton (PerfDataCollector _collector, int _phase) {
	    super ("Graph");
	    collector = _collector;
	    phase = _phase;
	}
    }


    private String padFloat (double f) {
	// reformats f to a string w/ 3 places after decimal
	String s = Double.toString(f);
	int i = s.lastIndexOf('.');
	if (i >= 0)
	    s = s.substring (0, i + 3);
	return s;
    }

    private void buildDataPanel () {
	int i,j;
	PerfDataCollector col;
	JLabel l;
	JButton b;
	double avg;
	data_panel.removeAll();
	Insets insets = new Insets (1,1,1,1);

	for (i = 0; i < collection.getSize(); i++) {
	    col = collection.getCollector(i);
	    col.generateAverages(preskip, postskip);
	    gbc.gridwidth = gbc.REMAINDER;
	    l = new JLabel ("----------------------------------------------");
	    gblayout.setConstraints (l, gbc);
	    data_panel.add (l);
	    l = new JLabel ("Averages report for " + col.getName());
	    b = new CollectorSummaryButton (col);
	    b.addActionListener (this);
	    gblayout.setConstraints (b, gbc);
	    data_panel.add (b);
	    gblayout.setConstraints (l, gbc);
	    data_panel.add (l);
	    for (j = 0; j < col.getNumPhases(); j++) {
		avg = col.getAverageForPhase(j);
		if (avg == 0.0)
		    continue;
		gbc.gridwidth = 1;
		l = new JLabel (j + ": " + col.getLabelForPhase(j));
		gblayout.setConstraints (l, gbc);
		data_panel.add(l);
		l = new JLabel (padFloat(avg), JLabel.RIGHT);
		gblayout.setConstraints (l, gbc);
		data_panel.add(l);
		b = new GraphButton (col, j);
		b.addActionListener (this);
		b.setMargin(insets);
		gblayout.setConstraints (b, gbc);
		data_panel.add(b);
		b = new AnomaliesButton (col, j);
		b.addActionListener (this);
		b.setMargin(insets);
		gbc.gridwidth = gbc.REMAINDER;
		gblayout.setConstraints (b, gbc);
		data_panel.add(b);
	    }
	}
    }


    JComboBox  collection_choice;
    JComboBox  display_choice;
    JTextField preskip_box;
    JTextField postskip_box;
    JButton savecontents_button;
    JButton load_button;
    JButton print_button;
    JButton print_all_button;
    JPanel data_panel;
    GridBagLayout gblayout;
    GridBagConstraints gbc;

    JScrollPane display_pane;
    JTextArea text_area;

    PerfDataCollector current_collector;
    PerfDataCollection collection;

    int preskip, postskip;
    float anomalycutoff;
    boolean doanomaly;

    public PerfAnalyzerPanel (VjPerf.PerfDataCollection _data_collection) {
	super();

	collection = _data_collection;
	current_collector = null;
	text_area = null;

	preskip = 20;
	postskip = 20;
	doanomaly = true;
	anomalycutoff = 1.0f;

	setLayout (new BorderLayout (5, 5));
	setBorder (new EmptyBorder (5,5,5,5));

	data_panel = new JPanel();
	data_panel.setLayout (gblayout = new GridBagLayout());
	gbc = new GridBagConstraints();
	gbc.insets = new Insets (0, 4, 0, 4);
	gbc.fill = gbc.HORIZONTAL;


	JPanel epanel = new JPanel ();
	epanel.setLayout (new BoxLayout (epanel, BoxLayout.Y_AXIS));
	add (epanel, "East");

	load_button = new JButton ("Load");
	epanel.add (load_button);
	load_button.addActionListener (this);

	epanel.add (savecontents_button = new JButton ("Save Contents"));
	savecontents_button.addActionListener (this);

	print_button = new JButton ("Print");
	print_button.addActionListener (this);
	epanel.add (print_button);

	print_all_button = new JButton ("Print All");
	print_all_button.addActionListener (this);
	epanel.add (print_all_button);
       

	JPanel npanel = new JPanel ();
	npanel.setLayout (new BoxLayout (npanel, BoxLayout.Y_AXIS));
	add (npanel, "North");
	JPanel ntoppanel = new JPanel();
	ntoppanel.setLayout (new BoxLayout (ntoppanel, BoxLayout.X_AXIS));
	npanel.add (ntoppanel);

	collection_choice = new JComboBox();
	display_choice = new JComboBox();

	display_choice.addItem ("Averages");
	display_choice.addItem ("Averages with discrepencies");

	collection_choice.addActionListener (this);
	display_choice.addActionListener (this);
	ntoppanel.add (collection_choice);
	ntoppanel.add (display_choice);


	JPanel nargpanel = new JPanel();
	nargpanel.setLayout (new GridLayout (4,6,5,5));
	npanel.add (nargpanel);

	nargpanel.add (new JLabel ("Preskip"));
	nargpanel.add (preskip_box = new JTextField ("   " + preskip));
	preskip_box.addActionListener (this);

	nargpanel.add (new JLabel ("Postskip"));
	nargpanel.add (postskip_box = new JTextField ("   " + postskip));
	postskip_box.addActionListener (this);

	display_pane = new JScrollPane (data_panel, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

	setCollectionList();
	setCurrentCollector (null);

	add (display_pane, "Center");

    }


    public void setCurrentCollector (PerfDataCollector c) {
	if (c == null) {
	    current_collector = null;
	    savecontents_button.setEnabled (false);
	    print_button.setEnabled (false);
	    print_all_button.setEnabled (false);
	}
	else {
	    current_collector = c;
	    savecontents_button.setEnabled (true);
	    print_button.setEnabled (true);
	    print_all_button.setEnabled (true);
	}
    }

    public void actionPerformed (ActionEvent e) {
	if (e.getSource() instanceof AnomaliesButton) {
	    System.out.println ("not implemented");
	}
	else if (e.getSource() instanceof GraphButton) {
	    GraphButton b = (GraphButton)e.getSource();
	    GraphFrame gf = new GraphFrame (b.collector, b.phase, preskip, postskip);
	    gf.show();
	}
	else if (e.getSource() instanceof CollectorSummaryButton) {
	    JFrame jf = new JFrame ("foo");
	    jf.getContentPane().add (new SummaryGraphPanel (((CollectorSummaryButton)e.getSource()).collector));
	    jf.setSize (500,300);
	    jf.show();
	}
	if (e.getSource() == display_choice)
	    refreshDisplay();
	else if (e.getSource() == collection_choice) {
	    Object o = collection_choice.getSelectedItem();
	    if (o instanceof String)
		setCurrentCollector (null);
	    else 
		setCurrentCollector ((PerfDataCollector)collection_choice.getSelectedItem());
	    //current_collector = (PerfDataCollector)collection_choice.getSelectedItem();
	    //refreshDisplay();
	}
	else if (e.getSource() == preskip_box) {
	    preskip = Integer.parseInt (preskip_box.getText().trim());
	    refreshDisplay();
	}
	else if (e.getSource() == postskip_box) {
	    postskip = Integer.parseInt (postskip_box.getText().trim());
	    refreshDisplay();
	}
	else if (e.getSource() == savecontents_button) {
	    if (text_area != null) {
		System.out.println (text_area.getText());
	    }
	}
	else if (e.getSource() == load_button) {
	    String name = FileControl.loadNewPerfDataFile ("", true);
	    setCollectionList();
	}
	else if (e.getSource() == print_button) {
	     String s = current_collector.dumpAverages (preskip, postskip, true, anomalycutoff);
	     System.out.println (s);
	}
	else if (e.getSource() == print_all_button) {
	    String s = "";
	    PerfDataCollector col;
	    for (int i = 0; i < collection.getSize(); i++) {
		col = collection.getCollector(i);
		s += col.dumpAverages (preskip, postskip, false, anomalycutoff);
	    }
	    System.out.println (s);
	}
    }


    public void refreshDisplay() {
	buildDataPanel();
    }

    public void setCollectionList() {
	while (collection_choice.getItemCount() != 0) 
	    collection_choice.removeItemAt(0);
	//collection_choice.removeAllItems();
	collection_choice.addItem ("None Selected");
	for (int i = 0; i < collection.collectors.size(); i++) {
	    collection_choice.addItem (collection.collectors.elementAt(i));
	}
    }

    public void refresh() {
	setCollectionList();
	refreshDisplay();
    }

}



