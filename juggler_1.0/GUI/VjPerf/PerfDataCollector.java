
package VjPerf;

import java.io.*;
import java.util.Vector;
import VjPerf.DataLine;
import VjConfig.*;
import VjGUI.Core;
import VjGUI.util.*;
import VjConfig.ConfigStreamTokenizer;

public class PerfDataCollector {
    // stores all performance data coming from a particular buffer
    public String name;
    public LinkedList datalines; // vector of DataLines & MissingCounts
    public int num; // # of different indices used for datapoints.
    public int numsamps[];
    public double totalsum;
    public int totalsamps;
    public double maxvals[];
    public double sums[];
    int prevplace = -1;
    double prevval = 0.0;
    DataLine dl;
    int place;
    ConfigChunk infochunk;
    int maxdatalines = 500;

    private void addDataLine (DataLine dl) {
	int i;

	dl.linetotal = 0.0;
	for (i = 0; i < num; i++)
	    dl.linetotal += dl.diffs[i];

	if (datalines.size() == maxdatalines) {
	    DataLine tmp = (DataLine)datalines.removeFirst();
	    for (i = 0; i < num; i++) {
		if (!Double.isNaN (tmp.diffs[i])) {
		    sums[i] -= tmp.diffs[i];
		    numsamps[i]--;
		}
	    }
	    if (!Double.isNaN(tmp.linetotal)) {
		totalsum -= tmp.linetotal;
		totalsamps--;
	    }
	}
	for (i = 0; i < num; i++) {
	    if (!Double.isNaN (dl.diffs[i])) {
		sums[i] += dl.diffs[i];
		numsamps[i]++;
	    }
	}
	if (!Double.isNaN(dl.linetotal)) {
	    totalsum += dl.linetotal;
	    totalsamps++;
	}
	datalines.add(dl);
    }




    public PerfDataCollector(String _name, int _num) {
        datalines = new LinkedList();
	name = _name;
	num = _num;
	numsamps = new int[num];
	maxvals = new double[num];
	sums = new double[num];
	totalsum = 0.0;
	totalsamps = 0;
	for (int i = 0; i < num; i++) {
	    numsamps[i] = 0;
	    maxvals[i] = 0.0;
	    sums[i] = 0.0;
	}
	dl = new DataLine (num);
	place = 0;

	System.out.println ("creating PerfDataCollector " + _name + " with " + _num + " elements.");

	infochunk = Core.findPrefixMatchChunk (name);
//  	if (infochunk != null) {
//  	    //System.out.println ("woo-hoo I found a chunk");
//  	    //labelsprop = ch.getProperty ("labels");
//  	}
//  	else
//  	    System.out.println ("foo. no chunk");

    }

    public int getNumPhases() {
	return num;
    }

    public String getName() {
	return name;
    }

    public double getAverageForPhase (int _phase) {
	return sums[_phase]/numsamps[_phase];
    }

    public String getLabelForPhase (int _phase) {
	if (infochunk == null)
	    return "";
	Property labelsprop = infochunk.getProperty ("labels");
	if (labelsprop != null) {
	    if ( _phase < labelsprop.getNum())
		return labelsprop.getValue(_phase).getString();
	}
	return "";
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
	ListIterator li;
	int i,j;
	for (j = 0; j < num; j++)
	    s = s + prebuff (j, (j==0)?9:18);
	s += "               Total\n";
	//for (i = 0; i < datalines.size(); i++) {
	li = datalines.listIterator(0);
	while (li.hasNext()) {
	    d = (DataLine)li.next();
	    //d = (DataLine)datalines.elementAt(i);
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


    public void generateAverages (int preskip, int postskip) {

//  	DataLine dl,dl2;
//  	double total = 0.0;
//  	int totalsamps = 0;
//  	int i,j;
//  	Property labelsprop = null;
//  	String label;
//  	ListIterator li;

//  	if (preskip < 1)
//  	    preskip = 1;

//  	for (i = 0; i < num; i++) {
//  	    sums[i] = 0.0;
//  	    maxvals[i] = 0.0;
//  	    numsamps[i] = 0;
//  	}

//  	// this is a sucky way of taking care of pre/postskip
//  	li = datalines.listIterator();
//  	int limit = Math.min (preskip, datalines.size());
//  	for (i = 0; i < limit; i++)
//  	    li.next();
	
//  	while (li.hasNext()) {
//  	    dl = (DataLine)li.next();
//  	    if (j < datalines.size()-1) {
//  		dl2 = (DataLine)li.next();
//  		if (dl.numlost == 0) {
//  		    double t = dl2.vals[0] - dl.vals[0];
//  		    if (!Double.isNaN (t)) {
//  			totalsamps++;
//  			total += t;
//  		    }
//  		}
//  		li.getPrev(); // undo extra getnext
//  	    }
//  	    for (i = 0; i < num; i++) {
//  		if (!Double.isNaN(dl.diffs[i])) {
//  		    numsamps[i]++;
//  		    if (dl.diffs[i] > maxvals[i])
//  			maxvals[i] = dl.diffs[i];
//  		    sums[i] += dl.diffs[i];
//  		    //totalsamps++;
//  		    //total += dl.diffs[i];
//  		}
//  	    }
//  	}

    }

    public String dumpAverages (int preskip, int postskip, boolean doanomaly, double cutoff) {
	int i;
  	Property labelsprop = null;
  	String label;

	ConfigChunk ch = Core.findPrefixMatchChunk (name);
	if (ch != null) {
	    System.out.println ("woo-hoo I found a chunk");
	    labelsprop = ch.getProperty ("labels");
	}
	else
	    System.out.println ("foo. no chunk");


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

	s += "  Total: " + (totalsum/totalsamps) + " us\n";

	return s;
    }


//      public String dumpAverages (int preskip, int postskip, boolean doanomoly, double cutoff) {
//  	DataLine dl,dl2;
//  	double total = 0.0;
//  	int totalsamps = 0;
//  	int i,j;
//  	Property labelsprop = null;
//  	String label;
//  	ListIterator li;

//  	ConfigChunk ch = Core.findPrefixMatchChunk (name);
//  	if (ch != null) {
//  	    System.out.println ("woo-hoo I found a chunk");
//  	    labelsprop = ch.getProperty ("labels");
//  	}
//  	else
//  	    System.out.println ("foo. no chunk");


//  	if (preskip < 1)
//  	    preskip = 1;

//  	for (i = 0; i < num; i++) {
//  	    sums[i] = 0.0;
//  	    maxvals[i] = 0.0;
//  	    numsamps[i] = 0;
//  	}

//  	// bug - i'm not handling pre/postskip correctly
//  	li = datalines.listIterator();
//  	int numskip = Math.min (preskip, datalines.size());
//  	for (i = 0; i < numskip; i++)
//  	    li.next();
//  	dl = (DataLine)li.next();
//  	while (li.hasNext()) {
//  	    dl2 = li.next();
//  	    if (dl.numlost == 0) {
//  		double t = dl2.vals[0] - dl.vals[0];
//  		if (!Double.isNaN (t)) {
//  		    totalsamps++;
//  		    total += t;
//  		}
//  	    }
//  	    for (i = 0; i < num; i++) {
//  		if (!Double.isNaN(dl.diffs[i])) {
//  		    numsamps[i]++;
//  		    if (dl.diffs[i] > maxvals[i])
//  			maxvals[i] = dl.diffs[i];
//  		    sums[i] += dl.diffs[i];
//  		    //totalsamps++;
//  		    //total += dl.diffs[i];
//  		}
//  	    }
//  	    dl = dl2;
//  	}
//  	String s = name + ":  averages Report per Cycle\n";
//  	double avg;
//  	for (i = 0; i < num; i++) {
//  	    avg = (sums[i]/numsamps[i]);
//  	    if (avg == 0.0)
//  		continue;
//  	    label = "";
//  	    if (labelsprop != null) {
//  		if ( i < labelsprop.getNum())
//  		    label = labelsprop.getValue(i).getString();
//  	    }
//  	    s += "  part " + i + ":  " + padFloat(avg) + " us\t"
//  		+ label + "\n";
//  	}

//  	s += "  Total: " + (total/totalsamps) + " us\n";

//  	double sum, diff;
//  	int numpoints;
	
//  	/* we'll handle each index reading separately */

//  //  	if (doanomoly) {
//  //  	s += "Anomolies report for " + name + "\n";
//  //  	for ( i = 0; i < num; i++) {
	    
//  //  	    avg = sums[i]/numsamps[i];
//  //  	    s += "index " + i + ": avg value is " + avg + "\n";
	    
//  //  	    for (j = preskip; j < datalines.size() - postskip; j++) {
//  //  		dl = (DataLine)datalines.elementAt(j);
//  //  		if (Double.isNaN(dl.diffs[i]))
//  //  		    continue;
//  //  		// what's the best diff function?
//  //  		diff = Math.abs(dl.diffs[i]-avg);
//  //  		if (diff > (avg*cutoff)) {
//  //  		    s += prebuff(dl.diffs[i], 10) + "       at time " + 
//  //  			dl.vals[i]/1000000.0 + " seconds\n";
//  //  		}
//  //  	    }
//  //  	    s += "-------------------------------------------\n";
//  //  	}
//  //  	}
//  	return s;
//      }


  public void read (ConfigStreamTokenizer st) {
    // we'll assume that the initial id stuff has already been read
    // and we can dig straight into the data lines.
    int i, j;
    //int place; // position in current DataLine
    int index;
    double val;
    int lastlost;
    double lastval;


    try {
      for (;;) {
	  st.nextToken();
	  index = Integer.parseInt(st.sval);//(int)st.nval;
	  st.nextToken();
	  //System.out.println ("index is " + index + "\nval is " + st.nval);
	  if (index == -1) {
	      int numlost = Integer.parseInt(st.sval);
	      if (numlost == 0)
		  break; // no data lost, no reason to muck with things
	      for (; place < num; place++)
		  dl.vals[place] = Double.NaN;
	      dl.numlost = numlost;
	      prevplace = -1;
	      addDataLine (dl);
	      dl = new DataLine(num);
	      place = 0;
	      break;
	  }
	  //System.out.println ("place1 is " + place); 
	  val = new Double(st.sval).doubleValue();//(double)st.nval;
	  for (; place != index; place++) {
	      //System.out.println ("place1 is " + place);
	      if (place >= num) {
		  addDataLine (dl);
		  //datalines.addElement(dl);
		  dl = new DataLine (num);
		  place = 0;
	      }
	      dl.vals[place] = 0.0;
	      dl.diffs[place] = 0.0;
	      if (place == index)
		  break; //kludge
	  }
	  place = index;
	  //System.out.println ("place2 is " + place); 
	  dl.vals[place] = val;
	  if (prevplace != -1) {
	      dl.diffs[place] = dl.vals[place] - prevval;
	  }
	  else
	      dl.diffs[place] = Double.NaN;
	  prevplace = place;
	  prevval = val;
	  place = (place+1);
	  if (place >= num) {
	      addDataLine(dl);
	      //datalines.addElement(dl);
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

//      // calculate linetotals
//      for (i = 0; i < datalines.size(); i++) {
//        dl = (DataLine)datalines.elementAt(i);
//        dl.linetotal = 0;
//        for (j = 0; j < num; j++)
//  	if (!Double.isNaN(dl.diffs[j]))
//  	  dl.linetotal += dl.diffs[j];
//      }

  }


};






