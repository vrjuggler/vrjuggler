/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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



package VjComponents.PerfMonitor;

import java.awt.event.*;
import java.io.*;
import java.util.*;

//import javax.xml.parsers.*;
//import org.xml.sax.SAXException;
//import org.xml.sax.SAXParseException;
import org.w3c.dom.*;

import VjConfig.*;
import VjControl.Core;
import VjConfig.ConfigStreamTokenizer;

/** Class for storing performance data from a single source.
 *  A source is, for example, a jccl::PerfDataBuffer.
 *
 *  PerfDataCollector is a source for ActionEvents, with the following
 *  ActionCommands:
 *      "Update" - when data is appended to the Collector.
 *      "Clear" - when the Collector clears out all its data.
 */
public class LabeledPerfDataCollector implements PerfDataCollector {

    protected class DataElem {
	public double stamp;
	public String category;
	public String index;
    
	public DataElem (String _category, String _index, double _stamp) {
	    category = _category;
	    index = _index;
	    stamp = _stamp;
	}
    }

    // stores a single "line" - execution loop - of data...
    protected class DataLine {
	List samples;  // list of DataElems
	boolean cutoff;     // true if there was lost data at end of line.

	public DataLine () {
	    samples = new ArrayList();
	}

	public void addSample (String category, String index, double stamp) {
	    samples.add (new DataElem (category, index, stamp));
	}

	public void setCutoff (boolean _cutoff) {
	    cutoff = _cutoff;
	}

	public Iterator iterator () {
	    return samples.iterator();
	}
    }

    protected class IndexInfo {
	// summary of info for a given index string
	int num_samples;
	double total_times;

	public IndexInfo () {
	    num_samples = 0;
	    total_times = 0.0;
	}

	public void addSample (double sample_time) {
	    num_samples++;
	    total_times = total_times + sample_time;
	}

	public void removeSample (double sample_time) {
	    num_samples--;
	    total_times = total_times - sample_time;
	}

	public double getAverage () {
	    return total_times/num_samples;
	}
    }

    // stores all performance data coming from a particular buffer
    public String name;
    public List datalines; // list of DataLines
    public int num;        // # of unique indices - useful for graphing

    HashMap index_info;          // for each unique index string, store:
                                 // total # of values, total of values
                                 // so that we can create averages, etc.

//  public int numsamps[];
    public double totalsum;
    public int totalsamps;
    DataLine current_dl = null;
    int maxdatalines;
    protected List action_listeners;

    public LabeledPerfDataCollector(String _name, int maxsamples) {
        datalines = new LinkedList();
        action_listeners = new ArrayList();
	index_info = new HashMap();
	name = _name;
	num = 0;
	totalsum = 0.0;
	totalsamps = 0;
	current_dl = new DataLine ();
//  	place = 0;
	maxdatalines = maxsamples;
	System.out.println ("creating PerfDataCollector " + _name + ".");


    }


      public void write (DataOutputStream out) throws IOException {
//  	out.writeBytes ("PerfData1 \"" + name + "\" " + num + "\n");
//  	ListIterator li = datalines.listIterator(0);
//  	while (li.hasNext()) {
//  	    DataLine dl = (DataLine)li.next();
//  	    int i = 0;
//  	    while ((i < num) && Double.isNaN(dl.vals[i]))
//  		i++;
//  	    while ((i < num) && !Double.isNaN(dl.vals[i])) {
//  		if (dl.vals[i] > 0.0f)
//  		    out.writeBytes (" " + i + "  " + dl.vals[i] + "\n");
//  		i++;
//  	    }
//  	    if (i != num) {
//  		out.writeBytes ("-1 " + dl.numlost + "\n");
//  		out.writeBytes ("PerfData1 \"" + name + "\" " + num + "\n");
//  	    }
//  	}
//  	out.writeBytes ("-1 0\n");
      }


    private void addDataLine (DataLine new_dl) {
	Iterator i;

	while (datalines.size() > maxdatalines) {
	    DataLine dl = (DataLine)datalines.remove(0);
	    i = dl.iterator();
	    while (i.hasNext()) {
		DataElem de = (DataElem)i.next();
		IndexInfo ii = (IndexInfo)index_info.get(de.index);
		ii.removeSample (de.stamp);
	    }
	}

	datalines.add(new_dl);
	i = new_dl.iterator();
	while (i.hasNext()) {
	    DataElem de = (DataElem)i.next();
// 	    IndexInfo ii = (IndexInfo)index_info.get(de.index);
// 	    if (ii == null) {
// 		ii = new IndexInfo();
// 		index_info.put (de.index, ii);
// 		num++;
// 	    }
// 	    ii.addSample (de.stamp);
	}
    }



    public void setMaxSamples (int n) {
	maxdatalines = n;
	System.out.println ("collector: max lines set to " + maxdatalines);
    }


    public int getNumIndices() {
	return num;
    }

    public String getName() {
	return name;
    }

    protected void addDataElem (String label, String category, double time) {

    }


    /** doc should be the element node for this labeledbuffer... we just
     *  extract all the subelements from it...
     */
    public void interpretXMLData (Node doc) {
        String name = doc.getNodeName();
        String value = doc.getNodeValue();
        NamedNodeMap attributes;
        int attrcount;
        int i;
        boolean retval = true;
        Node child;

        switch (doc.getNodeType()) {
        case Node.DOCUMENT_NODE:
        case Node.DOCUMENT_FRAGMENT_NODE:
            child = doc.getFirstChild();
            while (child != null) {
                interpretXMLData (child);
                child = child.getNextSibling();
            }
            break;
        case Node.ELEMENT_NODE:
            if (name.equalsIgnoreCase ("stamp")) {
                attributes = doc.getAttributes();
                attrcount = attributes.getLength();
		String label = "";
                String category = "";
                double time = 0.0;
                for (i = 0; i < attrcount; i++) {
                    child = attributes.item(i);
                    if (child.getNodeName().equals ("label")) {
			label = child.getNodeValue();
		    }
                    else if (child.getNodeName().equals ("category")) {
                        category = child.getNodeValue();
                    }
                    else if (child.getNodeName().equals ("time")) {
                        time = Double.parseDouble (child.getNodeValue());
                    }
                }
                addDataElem (name, category, time);

            }
            else {
                // shouldn't be anything else, eh?
            }
        case Node.COMMENT_NODE:
        case Node.NOTATION_NODE:
        case Node.PROCESSING_INSTRUCTION_NODE:
        case Node.TEXT_NODE:
            break;
        default:
	    System.out.println ("unexpected dom node...");
            //iostatus.addWarning ("Unexpected DOM node type...");
	}
    }

      public void refreshMaxValues () {
//  	DataLine dl;
//  	int i;
//  	for (i = 0; i < num; i++)
//  	    maxvals[i] = 0.0;
//  	maxlinetotal = 0.0;
//  	ListIterator li = datalines.listIterator(0);
//  	while (li.hasNext()) {
//  	    dl = (DataLine)li.next();
//  	    for (i = 0; i < num; i++)
//  		if (!Double.isNaN(dl.diffs[i]))
//  		    maxvals[i] = Math.max(maxvals[i], dl.diffs[i]);
//  	    if (!Double.isNaN(dl.linetotal))
//  		maxlinetotal = Math.max (maxlinetotal, dl.linetotal);
//  	}
//  //  	for (i = 0; i < num; i++) 
//  //  	    System.out.println ("max " + i + " is " + maxvals[i]);
      }

    public double getMaxValue () {
//  	double maxval = 0.0;
//  	for (int i = 0; i < num; i++)
//  	    maxval += maxvals[i];
//  	System.out.println ("alt generation method show " + maxval);
//  	return maxlinetotal;
	  return 0.0;
    }

//      public double getMaxValueForPhase (int phase) {
//  	    return maxvals[phase];
//      }


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

//  	DataLine d;
//  	ListIterator li;
//  	int i,j;
//  	for (j = 0; j < num; j++)
//  	    s = s + prebuff (j, (j==0)?9:18);
//  	s += "               Total\n";
//  	//for (i = 0; i < datalines.size(); i++) {
//  	li = datalines.listIterator(0);
//  	while (li.hasNext()) {
//  	    d = (DataLine)li.next();
//  	    //d = (DataLine)datalines.elementAt(i);
//  	    for (j = 0; j < num; j++) {
//  		s = s + prebuff(d.diffs[j],(j==0)?9:18);
//  	    }
//  	    if (d.numlost > 0)
//  		s = s + prebuff(d.linetotal,18) + "\t(lost " + d.numlost + ")\n";
//  	    else
//  		s += "\n";
//  	}

  	return s;
    }


    private String padFloat (double f) {
	// reformats f to a string w/ 7 spaces before decimal, and
	// 3 after...
	String s = Double.toString(f);
	int i = s.lastIndexOf('.');
	if ((i >= 0) && (i +3 < s.length()))
	    s = s.substring (0, i + 3);
	while (s.length() < 11)
	    s = ' ' + s;
	return s;
    }


    public void generateAverages (int preskip, int postskip) {

    }

    public String dumpAverages (int preskip, int postskip, boolean doanomaly, double cutoff) {
	return "";  
//  	int i;
//    	Property labelsprop = null;
//    	String label;

//          // kludgey - make sure we have latest perfdata labels chunk
//          List infochunks = Core.vjcontrol_chunkdb.getOfDescToken ("PerfData");
//          ConfigChunk ch;
//          for (int j = 0; j < infochunks.size(); j++) {
//              ch = (ConfigChunk)infochunks.get(j);
//              if (name.startsWith (ch.getName())) {
//                  infochunk = ch;
//                  break;
//              }
//          }

//          if (infochunk != null)
//              labelsprop = infochunk.getProperty ("labels");

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

//  	s += "  Total: " + (totalsum/totalsamps) + " us\n";

	//return s;
    }



    //--------------------- ActionEvent Stuff ------------------------

    public void addActionListener (ActionListener l) {
	synchronized (action_listeners) {
	    action_listeners.add (l);
	}
    }

    public void removeActionListener (ActionListener l) {
	synchronized (action_listeners) {
	    action_listeners.remove (l);
	}
    }


    private void notifyActionListenersUpdate () {
        ActionEvent e = new ActionEvent (this, ActionEvent.ACTION_PERFORMED,
                                         "Update");
        notifyActionListeners (e);
    }

    private void notifyActionListenersClear () {
        ActionEvent e = new ActionEvent (this, ActionEvent.ACTION_PERFORMED,
                                         "Clear");
        notifyActionListeners (e);
    }

    private void notifyActionListeners (ActionEvent e) {
        ActionListener l;
        int i, n;
        synchronized (action_listeners) {
            n = action_listeners.size();
            for (i = 0; i < n; i++) {
                l = (ActionListener)action_listeners.get(i);
                l.actionPerformed (e);
            }
        }
    }




};






