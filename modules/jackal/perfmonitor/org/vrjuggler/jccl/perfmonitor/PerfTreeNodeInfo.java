
package VjComponents.PerfMonitor;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import VjComponents.PerfMonitor.LabeledPerfDataCollector; 

public class PerfTreeNodeInfo implements ActionListener {

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

    public String sublabel;
    public LabeledPerfDataCollector.IndexInfo ii; // null for folders
    protected LabeledPerfDataCollector mCollector;
    protected JComponent mComponent;
    protected JLabel mValueLabel;
    protected JButton mGraphButton;

    public PerfTreeNodeInfo (String _sublabel, LabeledPerfDataCollector.IndexInfo _ii, LabeledPerfDataCollector col) {
	sublabel = _sublabel;
	ii = _ii;
	mCollector = col;

        mComponent = new JPanel();
        mComponent.setLayout (new BoxLayout (mComponent, BoxLayout.X_AXIS));

	if (ii != null) {
	    JLabel l = new JLabel(_sublabel);
	    mComponent.add (l);
            mComponent.add (Box.createHorizontalGlue());
	    mValueLabel = new JLabel (padFloat(ii.getAverage()/1000.0), JLabel.RIGHT);
	    mComponent.add (mValueLabel);
	    mGraphButton = new LabeledPanelButton (col, ii, "Graph");
	    mGraphButton.setActionCommand ("Graph");
	    mGraphButton.addActionListener (this);
	    //b.addActionListener (PerfAnalyzerPanel.this);
	    Insets insets = new Insets (1,1,1,1);
	    mGraphButton.setMargin(insets);
	    mComponent.add (mGraphButton);
	}
	else {
	    mComponent.add(new JLabel ("<i>" + sublabel + "</i>"));
            mComponent.add (Box.createHorizontalGlue());
	    mGraphButton = new LabeledPanelButton (col, null, "Graph");
	    mGraphButton.setActionCommand ("Graph");
	    mGraphButton.addActionListener (this);
	    //b.addActionListener (PerfAnalyzerPanel.this);
	    Insets insets = new Insets (1,1,1,1);
	    mGraphButton.setMargin(insets);
	    mComponent.add (mGraphButton);
	}
    }

    public String toString () {
	return sublabel;
    }
    
    public JComponent getComponent() {
	return mComponent;
    }

    public LabeledPerfDataCollector.IndexInfo getIndexInfo() {
	return ii;
    }

    public LabeledPerfDataCollector getCollector() {
	return mCollector;
    }

    public JButton getGraphButton () {
	return mGraphButton;
    }

    public void update() {
	if (ii != null) {
	    if (mValueLabel != null)
		mValueLabel.setText (padFloat(ii.getAverage()/1000.0));
	}
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

    public void actionPerformed (ActionEvent e) {
	System.out.println ("actionevent...");
    }

}
