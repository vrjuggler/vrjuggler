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
import VjComponents.PerfMonitor.PerfDataCollector;
import java.io.*;
import java.util.*;

import javax.xml.parsers.*;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.w3c.dom.*;

import VjConfig.ConfigStreamTokenizer;
import VjConfig.ConfigChunk;
import VjControl.*;


/** Core Module for storing VR Juggler performance data.
 * 
 *  @version $Revision$
 */
public class PerformanceModule extends DefaultCoreModule {


    public List collectors;  // v of PerfDataCollector
    public int max_samples;
    public File file;
    public List performance_module_listeners;


    public PerformanceModule () {
        component_name = "Performance Module";
	collectors = new ArrayList();
	max_samples = 500;
	file = new File(System.getProperty("user.dir",""), "perfdata");
        performance_module_listeners = new ArrayList();
    }



    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        component_name = ch.getName();
        component_chunk = ch;
    }


    public void initialize () throws VjComponentException {
        int i;
        List perfdatanames = new ArrayList();
        String lastfname;

        // check command-line arguments stored in Core
        String[] args = Core.getCommandLine();
	for (i = 0; i < args.length; i++) {
	    if (args[i].startsWith ("-p")) {
                if (args[i].length() == 2)
                    perfdatanames.add (args[++i]);
                else
                    perfdatanames.add (args[i].substring(2));
	    }
        }

	for (i = 0; i < perfdatanames.size(); i++) {
	    lastfname = (String)perfdatanames.get(i);
	    loadNewPerfDataFile (lastfname);
	}
    }


    public File getFile () {
        return file;
    }


    public synchronized void write (DataOutputStream out) throws IOException {
	for (int i = 0; i < collectors.size(); i++) {
	    PerfDataCollector col = (PerfDataCollector)collectors.get(i);
	    col.write (out);
	}
    }


    public void removeAllData () {
        synchronized (collectors) {
            collectors.clear();
        }
        notifyPerformanceModuleListenersRemoveAll();
    }


    public void setMaxSamples (int n) {
	max_samples = n;
	for (int i = 0; i < collectors.size(); i++)
	    ((PerfDataCollector)collectors.get(i)).setMaxSamples(n);
    }



    public int getSize() {
	return collectors.size();
    }


    public PerfDataCollector getCollector (int i) throws ArrayIndexOutOfBoundsException {
        synchronized (collectors) {
            return (PerfDataCollector)collectors.get(i);
        }
    }



    public PerfDataCollector getCollector (String _name) {
        synchronized (collectors) {
            int i;
            PerfDataCollector p;
            for (i = 0; i < collectors.size(); i++) {
                p = (PerfDataCollector) collectors.get (i);
                if (p.getName().equalsIgnoreCase (_name))
                    return p;
            }
            return null;
        }
    }


    public PerfDataCollector addCollector (PerfDataCollector pdc) {
	System.out.println ("Added collector: " + pdc.getName());
        synchronized (collectors) {
            collectors.add (pdc);
        }
        notifyPerformanceModuleListenersAdd (pdc);
	return pdc;
    }


    /** Read an xml-formatted stream of performance data. */
    public void readXMLStream (InputStream instream) {
	// new xml format for labeled buffers.
	// the xml dom may include the jccl protocol element...
	// if so just descend down into its tree.
	Document doc;
	DocumentBuilderFactory factory = 
	    DocumentBuilderFactory.newInstance();
	System.out.println ("read xml stream");
	// build the XML stream into a DOM tree
	try {
	    DocumentBuilder builder = factory.newDocumentBuilder();
	    doc = builder.parse (instream);
	    interpretXMLCommands (doc);
	}
//  	catch (javax.xml.parsers.ParserConfigurationException e1) {
//  	    //iostatus.addFailure (e1);
//  	}
//  	catch (org.xml.sax.SAXException e2) {
//  	    //iostatus.addFailure (e2);
//  	}
//  	catch (IOException e3) {
//  	    //iostatus.addFailure (e3);
//  	}
	catch (Exception ex) {
	    ex.printStackTrace();
	}
    }


    /** Utility for readXMLStream. */
    protected void interpretXMLCommands (Node doc) {
        String name = doc.getNodeName();
        String value = doc.getNodeValue();
        NamedNodeMap attributes;
        int attrcount;
        int i;
        boolean retval = true;
        Node child;

	//System.out.println ("interpretXMLCommands");

        switch (doc.getNodeType()) {
        case Node.DOCUMENT_NODE:
        case Node.DOCUMENT_FRAGMENT_NODE:
            child = doc.getFirstChild();
            while (child != null) {
                interpretXMLCommands (child);
                child = child.getNextSibling();
            }
            break;
        case Node.ELEMENT_NODE:
	    if (name.equalsIgnoreCase ("jcclstream")) {
		child = doc.getFirstChild();
		while (child != null) {
		    interpretXMLCommands (child);
		    child = child.getNextSibling();
		}
	    }
            else if (name.equalsIgnoreCase ("protocol")) {
		// should check to verify it's jccl_protocol...
		child = doc.getFirstChild();
		while (child != null) {
		    interpretXMLCommands (child);
		    child = child.getNextSibling();
		}
	    }
	    else if (name.equals ("labeledbuffer")) {
                attributes = doc.getAttributes();
                attrcount = attributes.getLength();
		String buffername = null;
                for (i = 0; i < attrcount; i++) {
                    child = attributes.item(i);
                    if (child.getNodeName().equals ("name")) {
			buffername = child.getNodeValue();
		    }
                }
		if (buffername != null) {
		    PerfDataCollector pdc = getCollector (buffername);
		    if (pdc == null) {
			pdc = new LabeledPerfDataCollector (buffername, 
							    max_samples);
			addCollector (pdc);
		    }
		    if (pdc instanceof LabeledPerfDataCollector) {
			((LabeledPerfDataCollector)pdc).interpretXMLData (doc);
		    }
		    else {
			System.out.println ("perfdatacollector type mismatch");
		    }
		}
		else {
		    System.out.println ("buffername wasn't specified...");
		}
		
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


    /** Read a stream/file using the "vjc_performance" protocol
     *  used by juggler 1.0.x (and currentl for output from
     *  jccl::PerfDataBuffer).
     */
    public void readVjcPerformanceStream (ConfigStreamTokenizer st) {
	String perfdatatype, name;
	int num;
	PerfDataCollector p;

	try {
            for (;;) {

                perfdatatype = "";
                for (;;) {
                    st.nextToken();
                    if (st.ttype == st.TT_EOF)
                        break;
                    if (st.ttype == st.TT_WORD)
                        if (st.sval.equalsIgnoreCase ("PerfData1"))
                            break;
                }

                if (st.ttype != st.TT_EOF) {

                    st.nextToken();
                    name = st.sval;
                    st.nextToken();
                    num = Integer.parseInt(st.sval);
                
                    //System.out.println ("read perf info for " + name + "\nnum is " + num);
                
                    p = getCollector (name);
                    if (p == null) {
			p = new NumberedPerfDataCollector (name, num, max_samples);
			addCollector (p);
		    }
                    if (p instanceof NumberedPerfDataCollector) {
                        ((NumberedPerfDataCollector)p).read (st);
                    }
                    else {
                        System.out.println ("perfdatacollector type mismatch");
                    }
                }
            }

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

        synchronized (collectors) {
            for (i = 0; i < collectors.size(); i++) {
                p = (PerfDataCollector) collectors.get (i);
                s += p.dumpAverages (preskip, postskip, doanomaly, anomalycutoff);
                s += "--------------------------------------\n";
            }
        }
        return s;
    }



    /************************ Perf File Methods ***************************/

    public String loadNewPerfDataFile (String name) {
        name = Core.file.mangleFileName (name);
        File f = new File(name);
        return loadNewPerfDataFile (f);
    }

    public String loadNewPerfDataFile (File f) {

        if (f == null)
            return null;

	Core.consoleInfoMessage (component_name, 
				 "Loading Timing Data file: " + f);

	try {

            FileInputStream temp_in = new FileInputStream (f);
            int ch = temp_in.read();
            if (ch != '<') {
		// old format
		ConfigStreamTokenizer st = 
		    new ConfigStreamTokenizer(new FileReader (f));

		// read that first info line
		st.eolIsSignificant(true);
		do {
		    st.nextToken();
		} while (st.ttype != '\n');
		st.eolIsSignificant(false);
		st.quoteChar('"');

		removeAllData();
		readVjcPerformanceStream (st);
            }
	    else {
		// new format
		removeAllData();
		readXMLStream (new FileInputStream(f));
	    }

	    file = f;

	    return f.toString();
	}
	catch (FileNotFoundException e) {
	    Core.consoleErrorMessage (component_name, "File not found: " + e);
	    return null;
	}
	catch (IOException e) {
	    Core.consoleErrorMessage (component_name, "Loading error: " + e);
	    return null;
	}
    }



    public String savePerfDataFile (File f) {
	if (f == null)
	    return "";

	try {
	    DataOutputStream out = new DataOutputStream(new FileOutputStream(f));
	    out.writeBytes ("VR Juggler FileConnect output gui_save\n");
	    write(out);
	    Core.consoleInfoMessage (component_name, "Saved PerfData file: " + f);
	    file = f;
	    return f.getName();
	}
	catch (IOException e) {
	    Core.consoleErrorMessage (component_name, "IOerror saving file " + f);
	    return "";
	}
    }

    //------------------ PerformanceModuleEvent Stuff ------------------------

    public void addPerformanceModuleListener (PerformanceModuleListener l) {
	synchronized (performance_module_listeners) {
	    performance_module_listeners.add (l);
	}
    }

    public void removePerformanceModuleListener (PerformanceModuleListener l) {
	synchronized (performance_module_listeners) {
	    performance_module_listeners.remove (l);
	}
    }


    private void notifyPerformanceModuleListenersRemoveAll () {
        PerformanceModuleEvent e = new PerformanceModuleEvent (this, PerformanceModuleEvent.REMOVE_ALL, null);
        PerformanceModuleListener l;
        int i, n;
        synchronized (performance_module_listeners) {
            n = performance_module_listeners.size();
            for (i = 0; i < n; i++) {
                l = (PerformanceModuleListener)performance_module_listeners.get(i);
                l.perfDataCollectorsCleared (e);
            }
        }
    }

    private void notifyPerformanceModuleListenersAdd (PerfDataCollector p) {
        PerformanceModuleEvent e = new PerformanceModuleEvent (this, PerformanceModuleEvent.ADD_COLLECTOR, p);
        PerformanceModuleListener l;
        int i, n;
        synchronized (performance_module_listeners) {
            n = performance_module_listeners.size();
            for (i = 0; i < n; i++) {
                l = (PerformanceModuleListener)performance_module_listeners.get(i);
                l.perfDataCollectorAdded (e);
            }
        }
    }

    private void notifyPerformanceModuleListenersRemove (PerfDataCollector p) {
        PerformanceModuleEvent e = new PerformanceModuleEvent (this, PerformanceModuleEvent.REMOVE_COLLECTOR, p);
        PerformanceModuleListener l;
        int i, n;
        synchronized (performance_module_listeners) {
            n = performance_module_listeners.size();
            for (i = 0; i < n; i++) {
                l = (PerformanceModuleListener)performance_module_listeners.get(i);
                l.perfDataCollectorRemoved (e);
            }
        }
    }


}
