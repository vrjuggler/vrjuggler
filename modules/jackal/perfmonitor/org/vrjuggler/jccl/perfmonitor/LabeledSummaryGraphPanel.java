/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
package org.vrjuggler.jccl.perfmonitor;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.border.*;

/** displays a summary of a PerfDataCollector, being a bar graph sort of like
 * what xload shows, with each index separately color-coded
 */
public class LabeledSummaryGraphPanel 
    extends GenericGraphPanel 
    implements ActionListener {

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
	    System.out.println ("vert scale set: " + vertmult);
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
            IndexVis iv;
            Iterator it = indices.iterator();
            while (it.hasNext()) {
                iv = (IndexVis)it.next();
                if (iv.isActive())
                    maxval += iv.index_info.getMaximum();
            }
  	    maxval = Math.ceil(maxval/10000) * 10000;
	    newmaxval = maxval;
	}

	public void refresh() {
	    if (newmaxval < (maxval/2.0))
		maxval = Math.ceil(maxval/10000) * 10000;
	    if (newmaxval > maxval)
		maxval = Math.ceil(newmaxval/10000) * 10000;
	    System.out.println ("repaint called for this panel");
	    repaint();
	}


	private double[] getAverageDiffs (Iterator li, int numsamps) {
	    // returns an ArrayList of doubles whose values are the averages
	    // of the deltas of the next numsamps in the collector.
	    double[] results = new double[col.getNumIndices()];
	    LabeledPerfDataCollector.DataLine dl;
	    LabeledPerfDataCollector.DataElem de;
	    LabeledPerfDataCollector.IndexInfo ordered_ii;
	    Iterator ordered_it, dl_it;
  	    int i, j, k;
	    for (i = 0; i < results.length; i++)
		results[i] = 0.0;
	    for (i = 0; (i < numsamps) && li.hasNext(); i++) {
		dl = (LabeledPerfDataCollector.DataLine)li.next();
		j = k = 0; // j is in array, k is in ii
		// this is broken as it doesn't deal with changes in
		// the order/presence of labels.
		dl_it = dl.iterator();
		ordered_it = col.indexIterator();
		while (dl_it.hasNext()) {
		    de = (LabeledPerfDataCollector.DataElem)dl_it.next();
                    ordered_it = col.indexIterator();
                    ordered_ii = (LabeledPerfDataCollector.IndexInfo)ordered_it.next();
                    j = 0;
                    while (de.index_info != ordered_ii) {
                        ordered_ii = (LabeledPerfDataCollector.IndexInfo)ordered_it.next();
                        j++;
                    }

		    results[j] += de.delta;
		}
	    }

	    for (i = 0; i < results.length; i++) 
		results[i] = results[i] / numsamps;

	    return results;
	}

	public void paint (Graphics g) {

	    cursize = getSize();
	    if (cursize.width < col.datalines.size()) {
		//System.out.println ("resizing...");
		cursize.width = col.datalines.size();
		setSize (cursize);
		setPreferredSize (cursize);
		return; // cuz resizing will trigger another paint
	    }	    

	    //System.out.println ("set size is: " + cursize);

	    newmaxval = 0.0;

//  	    System.out.println ("paint called");
	    g.setColor (Color.white);
	    g.fillRect (0, 0, cursize.width, cursize.height);
	    g.setColor (Color.black);
	    int i, j, k;
	    int x, y, new_y;
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
	    x = 0;
	    IndexVis iv;
	    Iterator iv_it; // IndexVis Iterator
	    Iterator li = col.datalines.listIterator(0);
	    while (li.hasNext()) {
		double[] deltas = getAverageDiffs (li, widthscale);
		curheight = 0;
		// ok, just need to find the right column and draw lines.
		iv_it = indices.iterator();
		y = 0; // vert position
		i = 0; // index of iv, so we can lookup in deltas.
		while (iv_it.hasNext()) {
		    iv = (IndexVis)iv_it.next();
		    if (iv.isActive() && deltas[i] > 0.0) {
			newmaxval = Math.max (newmaxval, deltas[i]);
			new_y = y + (int)(deltas[i]*heightscale);
			g.setColor (iv.getColor());
			g.drawLine (x, cursize.height - y,
				    x, cursize.height - new_y);
			y = new_y;
		    }
                    i++;
		}
                x++;
	    }

	    // draw some markings...
	    float horizbarsinterval;
            //horizbarsinterval = ((float)maxval/2.0f);
            if (maxval > 1000000)
                horizbarsinterval = 100000.0f;
	    else if (maxval > 200000)
		horizbarsinterval = 20000.0f;
	    else if (maxval > 20000)
		horizbarsinterval = 10000.0f;
	    else if (maxval > 5000)
		horizbarsinterval = 2000.0f;
	    else if (maxval > 500)
		horizbarsinterval = 500.0f;
            else
                horizbarsinterval = 100.0f;

	    for (float horizbar = horizbarsinterval; horizbar < maxval; horizbar += horizbarsinterval) {
		y = (int)(horizbar * heightscale);
		g.setColor (Color.black);
		g.drawLine (0, cursize.height - y, cursize.width, cursize.height-y);
		int val = (int)(horizbar/1000.0f);
		g.drawString (Integer.toString(val) + " ms", 5, cursize.height - y - 1);
	    }
	} // end of paint()

    }

    // -------------- end child class

    Color[] graph_colors = {Color.red, Color.yellow, Color.blue, Color.orange,
                            Color.green};

    protected class IndexVis {
	public JCheckBox checkbox;
	public LabeledPerfDataCollector.IndexInfo index_info;
	public Color color;

	public IndexVis (LabeledPerfDataCollector.IndexInfo _ii, 
			 boolean _active,
			 Color _color) {
	    index_info = _ii;
	    color = _color;
	    checkbox = new JCheckBox (index_info.index, _active);
	    checkbox.setBackground (color);
	}
	public JCheckBox getCheckBox () {
	    return checkbox;
	}
	public Color getColor () {
	    return color;
	}
	public boolean isActive () {
	    return checkbox.isSelected();
	}
    }

    LabeledPerfDataCollector col;
    SummaryDisplayPanel summarypanel;
    JComboBox vertscale_box;
    java.util.List indices;  // List of IndexVis
    int num;

    public LabeledSummaryGraphPanel (LabeledPerfDataCollector _col) {
	init (_col, null);
    }

    public LabeledSummaryGraphPanel (LabeledPerfDataCollector _col, 
				     LabeledPerfDataCollector.IndexInfo ii) {
	init (_col, ii);
    }

    private void init (LabeledPerfDataCollector _col,
		       LabeledPerfDataCollector.IndexInfo requested_ii) {
	int i,j;

	setBorder (BorderFactory.createEmptyBorder (5,5,5,5));
	setLayout (new BorderLayout (5,5));

	col = _col;
	num = col.getNumIndices();

	indices = new ArrayList();

	JPanel checkboxpanel = new JPanel();
	checkboxpanel.setBorder (BorderFactory.createTitledBorder (BorderFactory.createEtchedBorder(), "Display Indices", TitledBorder.LEFT, TitledBorder.TOP));
	Iterator it = col.indexIterator();
	i = 0;
	while (it.hasNext()) {
	    LabeledPerfDataCollector.IndexInfo ii = (LabeledPerfDataCollector.IndexInfo)it.next();
	    IndexVis iv = new IndexVis (ii, (requested_ii == null) || 
					(ii == requested_ii),
					graph_colors[i%graph_colors.length]);
	    indices.add (iv);
	    checkboxpanel.add (iv.getCheckBox());
	    iv.getCheckBox().addActionListener (this);
	    i++;
	}

	checkboxpanel.setLayout (new GridLayout (indices.size(), 1));//((j+1)/2, 2));
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

        col.addActionListener (this);
    }


    public void refresh () {
	summarypanel.refresh();
    }


    public void destroy () {
        col.removeActionListener (this);
    }


    public void actionPerformed (ActionEvent e) {
        if (e.getSource() == col) {
            summarypanel.refresh();
        }
	else if (e.getSource() == vertscale_box) {
	    summarypanel.setVerticalScale ((new Double((String)vertscale_box.getSelectedItem())).doubleValue());
	}
	else if (e.getSource() instanceof JCheckBox) {
	    // one of the "active?" checkboxes was hit.
	    refresh();
	}
    }


}
