
package VjPerf;

import java.io.*;
import java.util.Vector;
import VjPerf.DataLine;
import VjConfig.*;
import VjGUI.Core;

// read for a PerfDataCollector

public class PerfDataCollector {
    // stores all performance data coming from a particular buffer
    public String name;
    public Vector datalines; // vector of DataLines & MissingCounts
    public int num; // # of different indices used for datapoints.
    public int numsamps[];
    public double maxvals[];
    int prevplace = -1;
    double prevval = 0.0;
    DataLine dl;
    int place;

    public PerfDataCollector(String _name, int _num) {
        datalines = new Vector();
	name = _name;
	num = _num;
	numsamps = new int[num];
	maxvals = new double[num];
	for (int i = 0; i < num; i++) {
	    numsamps[i] = 0;
	    maxvals[i] = 0.0;
	}
	dl = new DataLine (num);
	place = 0;
    }

    public String toString () {
	return name;
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


    private String padFloat (double f) {
	// reformats f to a string w/ 7 spaces before decimal, and
	// 3 after...
	String s = Double.toString(f);
	int i = s.lastIndexOf('.');
	if (i >= 0)
	    s = s.substring (0, i + 3);
	while (s.length() < 11)
	    s = ' ' + s;
	return s;
    }


    public String dumpAverages (int preskip, int postskip, boolean doanomoly, double cutoff) {
	DataLine dl,dl2;
	double sums[] = new double[num];
	//int numsamps[] = new int[num];
	double total = 0.0;
	int totalsamps = 0;
	int i,j;
	Property labelsprop = null;
	String label;

	ConfigChunk ch = Core.findPrefixMatchChunk (name);
	if (ch != null) {
	    System.out.println ("woo-hoo I found a chunk");
	    labelsprop = ch.getProperty ("labels");
	}
	else
	    System.out.println ("foo. no chunk");


	if (preskip < 1)
	    preskip = 1;

	for (i = 0; i < num; i++) {
	    sums[i] = 0.0;
	    maxvals[i] = 0.0;
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
		    if (dl.diffs[i] > maxvals[i])
			maxvals[i] = dl.diffs[i];
		    sums[i] += dl.diffs[i];
		    //totalsamps++;
		    //total += dl.diffs[i];
		}
	    }
	}
	String s = name + ":  averages Report per Cycle\n";
	double avg;
	for (i = 0; i < num; i++) {
	    avg = (sums[i]/numsamps[i]);
	    if (avg == 0.0)
		continue;
	    label = "";
	    if (labelsprop != null) {
		if ( i < labelsprop.getNum())
		    label = labelsprop.getValue(i).getString();
	    }
	    s += "  part " + i + ":  " + padFloat(avg) + " us\t"
		+ label + "\n";
	}

	s += "  Total: " + (total/totalsamps) + " us\n";

	double sum, diff;
	int numpoints;
	
	/* we'll handle each index reading separately */
	if (doanomoly) {
	s += "Anomolies report for " + name + "\n";
	for ( i = 0; i < num; i++) {
	    
	    avg = sums[i]/numsamps[i];
	    s += "index " + i + ": avg value is " + avg + "\n";
	    
	    for (j = preskip; j < datalines.size() - postskip; j++) {
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
    int i, j;
    //int place; // position in current DataLine
    int index;
    double val;
    int lastlost;
    double lastval;


    try {
	//place = 0;
      //dl = new DataLine (num);
      for (;;) {
	  st.nextToken();
	  index = (int)st.nval;
	  //System.out.println ("index  is " + index);
	  st.nextToken();
	  //System.out.println ("index is " + index + "\nval is " + st.nval);
	  if (index == -1) {
	      if ((int)st.nval == 0)
		  break; // no data lost, no reason to muck with things
	      for (; place < num; place++)
		  dl.vals[place] = Double.NaN;
	      dl.numlost = (int)st.nval;
	      prevplace = -1;
	      datalines.addElement(dl);
	      dl = new DataLine(num);
	      place = 0;
	      break;
	  }
	  //System.out.println ("place1 is " + place); 
	  val = (double)st.nval;
	  for (; place != index; place++) {
	      //System.out.println ("place1 is " + place);
	      if (place >= num) {
		  datalines.addElement(dl);
		  dl = new DataLine (num);
		  place = 0;
	      }
	      dl.vals[place] = 0.0;
	      dl.diffs[place] = 0.0;
	      if (place == index)
		  break; //kludge
	      //if (prevplace < place)
	  }
	  place = index;
	  //System.out.println ("place2 is " + place); 
	  dl.vals[place] = val;
	  if (prevplace != -1) {
	      //if (prevplace < index)
	      //for (j = prevplace +1; j < index; j++) 
	      //dl.diffs[j] = 0.0;
	      dl.diffs[place] = dl.vals[place] - prevval;
	  }
	  else
	      dl.diffs[place] = Double.NaN;
	  prevplace = place;
	  prevval = val;
	  place = (place+1);
	  if (place >= num) {
	      //dl.numlost = 0;
	      datalines.addElement(dl);
	      dl = new DataLine(num);
	      place = 0;
	  }
      }
    }
    catch (IOException e) {
      System.out.println ("Read failed:\n" + e);
    }

    // now we need to go thru the vector datalines & calculate diffs all
    // over the place.

    //slightly funny case of datalines[0];
//     dl = (DataLine)(datalines.elementAt(0));
//     dl.diffs[0] = Double.NaN;
//     for (j = 1; j < num; j++) {
//       if ((dl.vals[j-1] != Double.NaN) && (dl.vals[j] != Double.NaN))
// 	dl.diffs[j] = dl.vals[j] - dl.vals[j-1];
//       else
// 	dl.diffs[j] = Double.NaN;
//     }
//     for (i = 1; i < datalines.size(); i++) {
//       lastval = dl.vals[num-1];
//       lastlost = dl.numlost;
//       dl = (DataLine)(datalines.elementAt(i));
//       if ((lastval != Double.NaN) && (dl.vals[0] != Double.NaN) && (lastlost == 0))
// 	dl.diffs[0] = dl.vals[0] - lastval;
//       else
// 	dl.diffs[0] = Double.NaN;
//       for (j = 1; j < num; j++) {
// 	if ((dl.vals[j-1] != Double.NaN) && (dl.vals[j] != Double.NaN))
// 	  dl.diffs[j] = dl.vals[j] - dl.vals[j-1];
// 	else
// 	  dl.diffs[j] = Double.NaN;
//       }
//     }

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






