
package VjPerf;

import java.awt.*;
import javax.swing.*;
import java.util.Vector;
import VjPerf.DataLine;
import VjPerf.PerfDataCollector;

public class GraphFrame extends JFrame {

    public PerfDataCollector dc;
    int numtouse;
    int numsamples;
    double max;
    int height, width;
    float heightscale;
    JScrollPane display_pane;
    GraphCanvas canvas;

    public class GraphCanvas extends Canvas {
	public GraphFrame gf;
	public GraphCanvas (GraphFrame _gf) {
	    super();
	    gf = _gf;
	    setSize (gf.width, gf.height);
	    //setMinimumSize (new Dimension (gf.width, gf.height));
	}
	public void paint (Graphics g) {
	    g.setColor (Color.white);
	    g.fillRect (0, 0, gf.width, gf.height);
	    g.setColor (Color.black);
	    int i = 0, j = 0;
	    for (; i < gf.width; i++) {
		double f = ((DataLine)gf.dc.datalines.elementAt(i)).diffs[gf.numtouse];
		if (!Double.isNaN(f)) {
		    g.drawLine (j, 200, j, (int)(gf.heightscale * ((DataLine)gf.dc.datalines.elementAt(i)).diffs[gf.numtouse]));
		    j++;
		}
	    }
	}
    }

    public GraphFrame (PerfDataCollector _dc, int _num) {
	super(_dc.name+_num);

	numtouse = _num;
	dc = _dc;
	numsamples = dc.numsamps[numtouse];
	max = dc.maxvals[numtouse];

	setSize (500,220);

	width = numsamples;
	height = 200;
	heightscale = (float)(height/max);

	getContentPane().add(display_pane = new JScrollPane (canvas = new GraphCanvas (this), JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS));
    }

}

	

