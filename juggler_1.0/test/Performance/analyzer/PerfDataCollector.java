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


import java.io.*;
import java.util.Vector;

// read for a PerfDataCollector

public class PerfDataCollector {
    // stores all performance data coming from a particular buffer
    public String name;
    public Vector datalines; // vector of DataLines & MissingCounts
    public int num; // # of different indices used for datapoints.

    public PerfDataCollector(String _name, int _num) {
        datalines = new Vector();
	name = _name;
	num = _num;
    }


    public String prebuff (double d, int b) {
	String s = Double.toString(d);
	while (s.length() < b)
	    s = " " + s;
	return s;
    }

    public String dumpData() {
	String s = name + "\n";
	DataLine d;
	int i,j;
	for (j = 0; j < num; j++)
	    s = s + prebuff (j, (j==0)?9:18);
	s += "               Total\n";
	for (i = 0; i < datalines.size(); i++) {
	    d = (DataLine)datalines.elementAt(i);
	    for (j = 0; j < num; j++) {
		s = s + prebuff(d.diffs[j],(j==0)?9:18);
	    }
	    if (d.numlost > 0)
		s = s + prebuff(d.linetotal,18) + "\t(lost " + d.numlost + ")\n";
	    else
		s += "\n";
	}

	return s;
    }



    public String dumpAverages (int preskip, int postskip, boolean doanomoly, double cutoff) {
	DataLine dl,dl2;
	double sums[] = new double[num];
	int numsamps[] = new int[num];
	double total = 0.0;
	int totalsamps = 0;
	int i,j;

	if (preskip < 1)
	    preskip = 1;

	for (i = 0; i < num; i++) {
	    sums[i] = 0.0;
	    numsamps[i] = 0;
	}
	for (j = preskip; j < datalines.size()-postskip; j++) {
	    dl = (DataLine)datalines.elementAt(j);
	    if (j < datalines.size()-1) {
		dl2 = (DataLine)datalines.elementAt(j+1);
		if (dl.numlost == 0) {
		    double t = dl2.vals[0] - dl.vals[0];
		    if (!Double.isNaN (t)) {
			totalsamps++;
			total += t;
		    }
		}
	    }
	    for (i = 0; i < num; i++) {
		if (!Double.isNaN(dl.diffs[i])) {
		    numsamps[i]++;
		    sums[i] += dl.diffs[i];
		    //totalsamps++;
		    //total += dl.diffs[i];
		}
	    }
	}
	String s = "Averages Report per Cycle\n";
	for (i = 0; i < num; i++) {
	    s += "  part " + i + ":  " + (sums[i]/numsamps[i]) + " us\n";
	}

	s += "  Total: " + (total/totalsamps) + " us\n";

	double sum, avg, diff;
	int numpoints;
	
	/* we'll handle each index reading separately */
	if (doanomoly) {
	s += "Anomolies report for " + name + "\n";
	for ( i = 0; i < num; i++) {
	    
	    avg = sums[i]/numsamps[i];
	    s += "index " + i + ": avg value is " + avg + "\n";
	    
	    for (j = 0; j < datalines.size(); j++) {
		dl = (DataLine)datalines.elementAt(j);
		if (Double.isNaN(dl.diffs[i]))
		    continue;
		// what's the best diff function?
		diff = Math.abs(dl.diffs[i]-avg);
		if (diff > (avg*cutoff)) {
		    s += prebuff(dl.diffs[i], 10) + "       at time " + 
			dl.vals[i]/1000000.0 + " seconds\n";
		}
	    }
	    s += "-------------------------------------------\n";
	}
	}
	return s;
    }



  public void read (StreamTokenizer st) {
    // we'll assume that the initial id stuff has already been read
    // and we can dig straight into the data lines.
    DataLine dl;
    int i, j;
    int place; // position in current DataLine
    int index;
    double val;
    int lastlost;
    double lastval;

    try {
      place = 0;
      dl = new DataLine (num);
      for (;;) {
	st.nextToken();
	index = (int)st.nval;
	st.nextToken();
	//System.out.println ("index is " + index + "\nval is " + st.nval);
	if (index == -1) {
	  for (; place < num; place++)
	    dl.vals[place] = Double.NaN;
	  dl.numlost = (int)st.nval;
	  datalines.addElement(dl);
	  dl = new DataLine(num);
	  place = 0;
	  break;
	}
	for (; place < index; place++)
	  dl.vals[place] = Double.NaN;
	dl.vals[place] = st.nval;
	place = (place+1)%num;
	if (place == 0) {
	  dl.numlost = 0;
	  datalines.addElement(dl);
	  dl = new DataLine(num);
	  continue;
	}
      }
    }
    catch (IOException e) {
      System.out.println ("Read failed:\n" + e);
    }

    // now we need to go thru the vector datalines & calculate diffs all
    // over the place.

    //slightly funny case of datalines[0];
    dl = (DataLine)(datalines.elementAt(0));
    dl.diffs[0] = Double.NaN;
    for (j = 1; j < num; j++) {
      if ((dl.vals[j-1] != Double.NaN) && (dl.vals[j] != Double.NaN))
	dl.diffs[j] = dl.vals[j] - dl.vals[j-1];
      else
	dl.diffs[j] = Double.NaN;
    }
    for (i = 1; i < datalines.size(); i++) {
      lastval = dl.vals[num-1];
      lastlost = dl.numlost;
      dl = (DataLine)(datalines.elementAt(i));
      if ((lastval != Double.NaN) && (dl.vals[0] != Double.NaN) && (lastlost == 0))
	dl.diffs[0] = dl.vals[0] - lastval;
      else
	dl.diffs[0] = Double.NaN;
      for (j = 1; j < num; j++) {
	if ((dl.vals[j-1] != Double.NaN) && (dl.vals[j] != Double.NaN))
	  dl.diffs[j] = dl.vals[j] - dl.vals[j-1];
	else
	  dl.diffs[j] = Double.NaN;
      }
    }

    // calculate linetotals
    for (i = 0; i < datalines.size(); i++) {
      dl = (DataLine)datalines.elementAt(i);
      dl.linetotal = 0;
      for (j = 0; j < num; j++)
	if (!Double.isNaN(dl.diffs[j]))
	  dl.linetotal += dl.diffs[j];
    }

  }


};






