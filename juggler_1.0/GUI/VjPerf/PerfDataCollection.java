
package VjPerf;

import VjPerf.PerfDataCollector;
import java.io.*;
import java.util.Vector;
import VjConfig.ConfigStreamTokenizer;

public class PerfDataCollection {

    public Vector collectors;  // v of PerfDataCollector


    public PerfDataCollection () {
	collectors = new Vector();
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
	PerfDataCollector p = new PerfDataCollector (_name, _num);
	collectors.addElement (p);
	return p;
    }


    public void read (ConfigStreamTokenizer st) {
	String perfdatatype, name;
	int num;
	PerfDataCollector p;

	try {
	    st.nextToken();
	    if (st.ttype == ConfigStreamTokenizer.TT_WORD) {
		System.out.println ("token is " + st.ttype + " " + st.sval);
		perfdatatype = st.sval;
		if (!st.sval.equalsIgnoreCase ("PerfData1")) {
		    System.out.println ("this isn't something i'm prepared to deal with");
		}
		st.nextToken();
		//System.out.println ("token is " + st.ttype + " " + st.sval);
		name = st.sval;
		st.nextToken();
		//System.out.println ("token is " + st.ttype + " " + st.sval);
		num = Integer.parseInt(st.sval);
		//System.out.println ("read name is " + name + "\nnum is " + num);
		
		System.out.println ("reading perf info for " + name);
		
		p = getCollector (name);
		if (p == null)
		    p = addCollector (name, num);
		
		p.read (st);
	    }
	}
	catch (IOException e) {
	    System.out.println ("shit");
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
