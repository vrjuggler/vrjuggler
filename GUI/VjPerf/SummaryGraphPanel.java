package VjPerf;


/* displays a summary of a PerfDataCollector, being a bar graph sort of like
 * what xload shows, with each index separately color-coded
 */

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import VjPerf.*;
import VjGUI.util.*;

public class SummaryGraphPanel extends JPanel implements ActionListener {

    private class SummaryDisplayPanel extends JPanel {
	// actual interior display panel...
	public Dimension cursize;
	public double heightscale;
	public int widthscale; //(widthscale is # samples per line)
	public Color colors[];
	public double maxval;

	public double horizmult;
	public double vertmult;

	public void setVerticalScale (double y) {
	    vertmult = y;
	    Dimension d = new Dimension (col.datalines.size() + 2, (int)(200.0 * vertmult));
	    setPreferredSize (d);
	    setSize (d);
	    //System.out.println ("vert scale set: " + vertmult);
	    //validate();
	    repaint();

	}

	public SummaryDisplayPanel() {
	    colors = new Color[50];
	    for (int i = 0; i < 50; i++) 
		colors[i] = new Color ((float)i * .02f, 0.0f, 1.0f - (float)i * .02f);
	    maxval = 70000;

	    horizmult = 1.0;
	    vertmult = 1.0;

	    setPreferredSize (new Dimension (col.datalines.size(),200));


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
	    System.out.println ("get size is: " + cursize);


	    System.out.println ("paint called");
	    g.setColor (Color.white);
	    g.fillRect (0, 0, cursize.width, cursize.height);
	    g.setColor (Color.black);
	    int i, j, k;
	    int curheight, newheight;

	    //widthscale = Math.max (1, col.datalines.size() / cursize.width);
	    widthscale = (col.datalines.size()/cursize.width) + 1;
	    heightscale = cursize.height / maxval;

	    System.out.println ("panel width is " + cursize.width);
	    System.out.println ("NumSamples is " + col.datalines.size());
	    System.out.println ("widthscale is " + widthscale);
	    System.out.println ("heightscale is " + heightscale);
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
			if (dl.diffs[k] == 0.0)
			    continue;
			newheight = curheight + (int)(dl.diffs[k]*heightscale);
			g.setColor (colors[k]);
			g.drawLine (j, cursize.height - curheight,
				    j, cursize.height - newheight);
			curheight = newheight;
		    }
		}
	    }

	    // draw some markings...
	    float horizbarsinterval = 10000.0f;
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

    public SummaryGraphPanel (PerfDataCollector _col) {

	setBorder (BorderFactory.createEmptyBorder (5,5,5,5));
	setLayout (new BorderLayout (5,5));

	col = _col;

	JPanel northpanel = new JPanel();
	add (northpanel, "North");
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
    }

}

