package VjPerf;


/* displays a summary of a PerfDataCollector, being a bar graph sort of like
 * what xload shows, with each index separately color-coded
 */

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import VjPerf.*;
import VjGUI.util.*;

public class SummaryGraphPanel extends GenericGraphPanel implements ActionListener {

    private class SummaryDisplayPanel extends JPanel {
	// actual interior display panel...
	public Dimension cursize;
	public double heightscale;
	public int widthscale; //(widthscale is # samples per line)
	public Color colors[];
	public double maxval;
	public double newmaxval;

	public double horizmult;
	public double vertmult;

	public void setVerticalScale (double y) {
	    vertmult = y;
	    Dimension d = new Dimension (col.datalines.size() + 5, (int)(200.0 * vertmult));
	    setPreferredSize (d);
	    setMinimumSize(d);
	    setSize (d);
	    //System.out.println ("vert scale set: " + vertmult);
	    //validate();
	    repaint();

	}

	public SummaryDisplayPanel() {

	    colors = new Color[60];
	    for (int i = 0; i < 60; i+=3) {
		colors[i] = new Color (.3f + (float)i/90.0f, 0.0f, 0.0f);
		colors[i+1] = new Color (0.0f, .3f + (float)i/90.0f, 0.0f);
		colors[i+2] = new Color (0.0f, 0.0f, .3f + (float)i/90.0f);
	    }
 
	    refigureMax();

	    horizmult = 1.0;
	    vertmult = 1.0;

	    setPreferredSize (new Dimension (col.datalines.size()+5,200));


	}


	public void refigureMax() {
	    col.refreshMaxValues();
	    maxval = 0.0;
	    for (int i = 0; i < num; i++)
		if (phase_active[i])
		    maxval += col.getMaxValueForPhase(i);
	    maxval = Math.ceil(maxval/10000) * 10000;
	    newmaxval = maxval;
	}

	public void refresh() {
	    if (newmaxval < (maxval/2.0))
		maxval = Math.ceil(maxval/10000) * 10000;
	    if (newmaxval > maxval)
		maxval = Math.ceil(newmaxval/10000) * 10000;

	    repaint();
	}


	private DataLine getAverageDiffs (ListIterator li, int numsamps, int numphases) {
	    // returns a DataLine whose diffs are the averages of the
	    // diffs of the next numsamps in diffs
	    DataLine dl = new DataLine(numphases);
	    DataLine dl2;
	    int i, j;
	    for (i = 0; i < numsamps; i++) {
		if (!li.hasNext())
		    break;
		dl2 = (DataLine)li.next();
		for (j = 0; j < numphases; j++)
		    dl.diffs[j] += dl2.diffs[j];
	    }
	    if (i != 0)
		for (j = 0; j < numphases; j++)
		    dl.diffs[j] = dl.diffs[j] / (double)i;
	    return dl;
	}

	public void paint (Graphics g) {

	    cursize = getSize();
	    cursize.width = col.datalines.size();
	    setSize (cursize);
	    setPreferredSize (cursize);
	    
	    //System.out.println ("set size is: " + cursize);

	    newmaxval = 0.0;

//  	    System.out.println ("paint called");
	    g.setColor (Color.white);
	    g.fillRect (0, 0, cursize.width, cursize.height);
	    g.setColor (Color.black);
	    int i, j, k;
	    int curheight, newheight;

	    //widthscale = Math.max (1, col.datalines.size() / cursize.width);
	    widthscale = 1;
	    //widthscale = (col.datalines.size()/cursize.width) + 1;
	    heightscale = cursize.height / maxval;

//  	    System.out.println ("panel width is " + cursize.width);
//  	    System.out.println ("NumSamples is " + col.datalines.size());
//  	    System.out.println ("widthscale is " + widthscale);
	    //System.out.println ("heightscale is " + heightscale);
	    // bug - not handling pre/postskip
	    ListIterator li = col.datalines.listIterator(0);
	    for (j = 0; j < col.datalines.size()/widthscale; j++) {
		DataLine dl = getAverageDiffs (li, widthscale, col.num);
		//System.out.println (dl);
		curheight = 0;
		for (k = 0; k < col.num; k++) {
		    if (Double.isNaN(dl.diffs[k])) {
			g.setColor (Color.green);
			g.drawLine (j, cursize.height - curheight, j, 0);
			break;
		    }
		    else {
			if (phase_active[k] && (dl.diffs[k] > 0.0)) {
			    newmaxval = Math.max (newmaxval, dl.diffs[k]);
			    newheight = curheight + (int)(dl.diffs[k]*heightscale);
			    g.setColor (colors[k]);
			    g.drawLine (j, cursize.height - curheight,
					j, cursize.height - newheight);
			    curheight = newheight;
			}
		    }
		}
	    }

	    // draw some markings...
	    float horizbarsinterval;
	    if (maxval > 200000)
		horizbarsinterval = 20000.0f;
	    else if (maxval > 20000)
		horizbarsinterval = 10000.0f;
	    else if (maxval > 5000)
		horizbarsinterval = 2000.0f;
	    else
		horizbarsinterval = 500.0f;

	    for (float horizbar = horizbarsinterval; horizbar < maxval; horizbar += horizbarsinterval) {
		int y = (int)(horizbar * heightscale);
		g.setColor (Color.black);
		g.drawLine (0, cursize.height - y, cursize.width, cursize.height-y);
		int val = (int)(horizbar/1000.0f);
		g.drawString (Integer.toString(val) + " ms", 5, cursize.height - y - 1);
	    }
	} // end of paint()

    }

    // -------------- end child class

    PerfDataCollector col;
    SummaryDisplayPanel summarypanel;
    JComboBox vertscale_box;
    JFrameParent parent;
    boolean phase_active[];
    JCheckBox phase_active_boxes[];
    int num;

    public SummaryGraphPanel (PerfDataCollector _col) {
	init (_col, -1);
    }

    public SummaryGraphPanel (PerfDataCollector _col, int phase) {
	init (_col, phase);
    }

    private void init(PerfDataCollector _col, int phase) {
	int i,j;

	setBorder (BorderFactory.createEmptyBorder (5,5,5,5));
	setLayout (new BorderLayout (5,5));

	col = _col;
	num = col.getNumPhases();

	phase_active = new boolean[num];
	for (i = 0; i < num; i++)
	    phase_active[i] = (phase == -1) || (phase == i);

	phase_active_boxes = new JCheckBox[num];
	JPanel checkboxpanel = new JPanel();
	checkboxpanel.setBorder (BorderFactory.createTitledBorder (BorderFactory.createEtchedBorder(), "Display Indices", TitledBorder.LEFT, TitledBorder.TOP));
	for (i = 0, j = 0; i < num; i++) {
	    if (col.getAverageForPhase(i) > 0.0) {
		phase_active_boxes[i] = new JCheckBox (Integer.toString(i) + " -" + col.getLabelForPhase(i), phase_active[i]);
		phase_active_boxes[i].setActionCommand (Integer.toString(i));
		phase_active_boxes[i].addActionListener(this);
		checkboxpanel.add(phase_active_boxes[i]);
		j++;
	    }
	    else
		phase_active_boxes[i] = null;
	}
	checkboxpanel.setLayout (new GridLayout (j, 1));//((j+1)/2, 2));
	add (checkboxpanel, "East");

	JPanel northpanel = new JPanel();
	//northpanel.add (checkboxpanel);
	add (northpanel, "North");

	northpanel.add (new JLabel ("Vert scale factor: "));

	vertscale_box = new JComboBox();
	vertscale_box.addItem ("1.0");
	vertscale_box.addItem ("2.0");
	vertscale_box.addItem ("5.0");
	vertscale_box.addItem ("10.0");
	vertscale_box.addItem ("100.0");
	northpanel.add (vertscale_box);
	vertscale_box.addActionListener (this);

	summarypanel = new SummaryDisplayPanel ();
	JScrollPane display_pane = new JScrollPane (summarypanel, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
	//display_pane.setRowHeaderView (horizbarcanvas);
	add (display_pane, "Center");


    }

    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == vertscale_box) {
	    summarypanel.setVerticalScale ((new Double((String)vertscale_box.getSelectedItem())).doubleValue());
	}
	if (e.getSource() instanceof JCheckBox) {
	    int i;
	    for (i = 0; i < num; i++)
		if (phase_active_boxes[i] == e.getSource())
		    break;
	    //System.out.println ("checkbox selected # " + i);
	    if (i < num) {
		phase_active[i] = phase_active_boxes[i].isSelected();
	    }
	    col.refreshMaxValues();
	    summarypanel.refigureMax();
	    summarypanel.refresh();
	}
    }


}


