/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */



package VjPerf;

import java.awt.*;
import javax.swing.*;
import java.util.Vector;
import VjPerf.DataLine;
import VjPerf.PerfDataCollector;
import VjGUI.Core;
import VjGUI.util.*;

public class GraphFrame extends JFrame {

    public PerfDataCollector dc;
    int numtouse;
    int numsamples;
    double max;
    int height, width;
    float heightscale;
    JScrollPane display_pane;
    GraphCanvas canvas;
    HorizBarCanvas horizbarcanvas;
    int preskip, postskip;

    public class GraphCanvas extends JPanel {
	public GraphFrame gf;
	int width, height;
	public GraphCanvas (GraphFrame _gf) {
	    super();
	    gf = _gf;
	    height = gf.height;
	    width = gf.width;
	    //setSize (width, height);
	    //setMinimumSize (new Dimension (width, height));
	    setPreferredSize (new Dimension (width, height));
	    //System.out.println ("set size: " + width + ", " + height);
	}
	public void paint (Graphics g) {
	    System.out.println ("paint called");
	    g.setColor (Color.white);
	    g.fillRect (0, 0, width, height);
	    g.setColor (Color.black);
	    int i = preskip, j = 0;

	    // bug - not handling pre/postskip
	    ListIterator li = gf.dc.datalines.listIterator(0);
	    while (li.hasNext()) {
		DataLine dl = (DataLine)li.next();
		double f = dl.diffs[gf.numtouse];
		if (!Double.isNaN(f)) {
		    g.setColor (Color.black);
		    g.drawLine (j, height, j, height - (int)(gf.heightscale * (dl.diffs[gf.numtouse])));
		    j++;
		}
		else {
		    g.setColor (Color.red);
		    g.drawLine (j, height, j, 0);
		    j++;
		}
	    }
	}
    }



    public class HorizBarCanvas extends JPanel {
	public GraphFrame gf;
	int width, height;

	public HorizBarCanvas (GraphFrame _gf) {
	    super();
	    gf = _gf;
	    height = 25;
	    width = gf.width;
	    setPreferredSize (new Dimension (width, height));
	    setMinimumSize (new Dimension (width, height));
	}

	public void paint (Graphics g) {
	    System.out.println ("paint called");
	    g.setColor (Color.red);
	    g.fillRect (0, 0, width, height);
	}
    }



    public GraphFrame (PerfDataCollector _dc, int _num, int _preskip, int _postskip) {
	super(_dc.name+ " phase " + _num + " " + _dc.getLabelForPhase (_num));

	preskip = _preskip;
	postskip = _postskip;
	numtouse = _num;
	dc = _dc;
	numsamples = dc.numsamps[numtouse];
	max = dc.maxvals[numtouse];

	int winwidth = Math.min (numsamples + 40, Core.screenWidth);
	int winheight = Math.min (370, Core.screenHeight);

	setSize (winwidth, winheight);

	width = numsamples - preskip - postskip;
	height = winheight - 40;
	heightscale = (float)((height-5)/max);

	canvas = new GraphCanvas (this);
	horizbarcanvas = new HorizBarCanvas (this);

	getContentPane().add(display_pane = new JScrollPane (JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS));
	//display_pane.setRowHeaderView (horizbarcanvas);
	display_pane.setViewportView (canvas);
    }

}

	


