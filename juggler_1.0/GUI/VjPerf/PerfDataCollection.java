
package VjPerf;

import java.awt.event.*;
import VjPerf.PerfDataCollector;
import java.io.*;
import java.util.Vector;
import VjConfig.ConfigStreamTokenizer;

public class PerfDataCollection {

    public Vector collectors;  // v of PerfDataCollector
    public Vector listeners; // actionListeners
    public int max_samples;

    public PerfDataCollection () {
	collectors = new Vector();
	listeners = new Vector();
	max_samples = 500;
    }



    public void removeAllData () {
	collectors.removeAllElements();
	fireActionPerformed (new ActionEvent (this, 1, "removealldata"));
    }


    public void setMaxSamples (int n) {
	max_samples = n;
	for (int i = 0; i < collectors.size(); i++)
	    ((PerfDataCollector)collectors.elementAt(i)).setMaxSamples(n);
    }


    public void addActionListener (ActionListener l) {
	if (!listeners.contains (l))
	    listeners.addElement (l);
    }
    public void removeActionListener (ActionListener l) {
	listeners.removeElement (l);
    }

    public void fireActionPerformed (ActionEvent e) {
	int i;
	ActionListener l;
	for (i = 0; i < listeners.size(); i++) {
	    l = (ActionListener)listeners.elementAt(i);
	    l.actionPerformed (e);
	}
    }

    public int getSize() {
	return collectors.size();
    }

    public PerfDataCollector getCollector (int i) {
	try {
	    return (PerfDataCollector)collectors.elementAt(i);
	}
	catch (ArrayIndexOutOfBoundsException e) {
	    return null;
	}
    }

    public PerfDataCollector getCollector (String _name) {
	int i;
	PerfDataCollector p;
	for (i = 0; i < collectors.size(); i++) {
	    p = (PerfDataCollector) collectors.elementAt (i);
	    if (p.name.equalsIgnoreCase (_name))
		return p;
	}
	return null;
    }

    
    public PerfDataCollector addCollector (String _name, int _num) {
	PerfDataCollector p = new PerfDataCollector (_name, _num, max_samples);
	collectors.addElement (p);
	return p;
    }


    public void read (ConfigStreamTokenizer st, boolean multiple) {
	String perfdatatype, name;
	int num;
	PerfDataCollector p;

	try {
	    do {
		//System.out.println ("ttype is " + st.ttype);
		st.nextToken();
		//if (st.ttype == st.TT_EOF)
		//  break;
		if (st.ttype != ConfigStreamTokenizer.TT_WORD) {
		    st.pushBack();
		    break;
		}
		perfdatatype = st.sval;
		if (!st.sval.equalsIgnoreCase ("PerfData1")) {
		    st.pushBack();
		    break;
		}
		st.nextToken();
		name = st.sval;
		st.nextToken();
		num = Integer.parseInt(st.sval);

		//System.out.println ("read perf info for " + name + "\nnum is " + num);

		p = getCollector (name);
		if (p == null)
		    p = addCollector (name, num);
		
		p.read (st);

	    } while (multiple);
	    System.out.println ("firing perfdatacollection update");
	    fireActionPerformed (new ActionEvent (this, 5, "update"));

	}
	catch (IOException e) {
	    // I always seem to hit an exception at eof of a data file...
	    // so for now I'm just gonna ignore it.  this isn't good.
	    //System.out.println ("ERROR - " + e);
	    //System.out.println ("at line " + st.lineno());
	    //e.printStackTrace();
	}
    }


    public String dumpAverages (int preskip, int postskip, boolean doanomaly, float anomalycutoff) {
	PerfDataCollector p;
	int i;
	String s = "";

	for (i = 0; i < collectors.size(); i++) {
	    p = (PerfDataCollector) collectors.elementAt (i);
	    s += p.dumpAverages (preskip, postskip, doanomaly, anomalycutoff);
	    s += "--------------------------------------\n";
	}
	return s;
    }

}
