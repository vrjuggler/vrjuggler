

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
import VjPerf.*;
import VjGUI.FileControl;

public class PerfAnalyzerPanel extends JPanel implements ActionListener {

    JComboBox  collection_choice;
    JComboBox  display_choice;
    JTextField preskip_box;
    JTextField postskip_box;
    JButton savecontents_button;
    JButton load_button;
    JButton print_button;
    JButton print_all_button;

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

	JPanel epanel = new JPanel ();
	epanel.setLayout (new BoxLayout (epanel, BoxLayout.Y_AXIS));
	add (epanel, "East");

	load_button = new JButton ("Load");
	epanel.add (load_button);
	load_button.addActionListener (this);

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
	nargpanel.setLayout (new GridLayout (6,6,5,5));
	npanel.add (nargpanel);

	nargpanel.add (new JLabel ("Preskip"));
	nargpanel.add (preskip_box = new JTextField ("   " + preskip));
	preskip_box.addActionListener (this);

	nargpanel.add (new JLabel ("Postskip"));
	nargpanel.add (postskip_box = new JTextField ("   " + postskip));
	postskip_box.addActionListener (this);

	nargpanel.add (savecontents_button = new JButton ("Save Contents"));
	savecontents_button.addActionListener (this);

	display_pane = new JScrollPane (JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

	setCollectionList();

	add (display_pane, "Center");

    }


    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == display_choice)
	    refreshDisplay();
	else if (e.getSource() == collection_choice) {
	    current_collector = (PerfDataCollector)collection_choice.getSelectedItem();
	    refreshDisplay();
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
	String s;
	text_area = null;
	if (display_choice.getSelectedItem().equals ("Averages with discrepencies")) {
	    if (current_collector != null)
		s = current_collector.dumpAverages (preskip, postskip, true, anomalycutoff);
	    else
		s = "";
	    text_area = new JTextArea(s);
	    display_pane.getViewport().remove (display_pane.getViewport().getView());
	    display_pane.getViewport().add(text_area);
	}
	else if (display_choice.getSelectedItem().equals ("Averages")) {
	    if (current_collector != null)
		s = current_collector.dumpAverages (preskip, postskip, false, anomalycutoff);
	    else
		s = "";
	    text_area = new JTextArea(s);
	    display_pane.getViewport().add(text_area);
	}
	display_pane.validate();
	display_pane.repaint();
//  	for (int j = 0; j < current_collector.num; j++) {
//  	    JFrame f = (new GraphFrame (current_collector, j));
//  	    f.show();
//  	}
    }

    public void setCollectionList() {
	while (collection_choice.getItemCount() != 0) 
	    collection_choice.removeItemAt(0);
	//collection_choice.removeAllItems();
	for (int i = 0; i < collection.collectors.size(); i++) {
	    collection_choice.addItem (collection.collectors.elementAt(i));
	}
    }

    public void refresh() {
	setCollectionList();
	refreshDisplay();
    }

}



