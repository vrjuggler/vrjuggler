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

import VjConfig.ConfigStreamTokenizer;
import VjConfig.ConfigChunk;
import VjControl.*;


/** Core Module for storing VR Juggler performance data
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class PerformanceModule extends DefaultCoreModule {


    public Vector collectors;  // v of PerfDataCollector
    public Vector listeners; // actionListeners
    public int max_samples;
    public File file;


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


//      public boolean addConfig (ConfigChunk ch) {
//          return false;
//      }


//      public boolean removeConfig (String name) {
//          return false;
//      }


//      public void destroy () {
//          ;
//      }


    public void write (DataOutputStream out) throws IOException {
	for (int i = 0; i < collectors.size(); i++) {
	    PerfDataCollector col = (PerfDataCollector)collectors.elementAt(i);
	    col.write (out);
	}
    }


    public PerformanceModule () {
        component_name = "Performance Module";
	collectors = new Vector();
	listeners = new Vector();
	max_samples = 500;
	file = new File(System.getProperty("user.dir",""), "perfdata");
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


    /** Reads a single perfcollection stream 
     *  The stream begins with "PerfData1 <name> <num>" and ends with
     *  "-1 <numlost>" 
     */
    public void readStream (ConfigStreamTokenizer st, String id) 
        throws IOException {
        
	String name;
	int num;
	PerfDataCollector p;

        if (id.equalsIgnoreCase ("PerfData1")) {
            st.nextToken();
            name = st.sval;
            st.nextToken();
            num = Integer.parseInt(st.sval);

            //System.out.println ("read perf info for " + name + "\nnum is " + num);
            p = getCollector (name);
            if (p == null)
                p = addCollector (name, num);
            p.read (st);
	    fireActionPerformed (new ActionEvent (this, 5, "update"));
        }
        else {
            //Core.consoleErrorMessage ("Perf", "Major parsing failure in PerfDataCollection");
            System.out.println ("Major parsing failure in PerfDataCollection");
        }
    }


    /** Read multiple PerfData streams, as from a log file 
     *  The only reason to have this in addition to readStream is that
     *  it only sends out one update message at the end of the file.
     */
    public void readFile (ConfigStreamTokenizer st) {
	String perfdatatype, name;
	int num;
	PerfDataCollector p;

	try {
            for (;;) {
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
            }

            //System.out.println ("firing perfdatacollection update");
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
	    ConfigStreamTokenizer st = 
                new ConfigStreamTokenizer(new FileReader (f));
	    st.eolIsSignificant(true);

	    do {
		st.nextToken();
		//System.out.println ("foo" + st.ttype);
	    } while (st.ttype != '\n');

	    st.eolIsSignificant(false);
	    st.quoteChar('"');

            // apply to self...
	    removeAllData();
	    readFile (st);
	    file = f;

            //System.out.println ("firing perfdatacollection update");
            fireActionPerformed (new ActionEvent (this, 5, "update"));

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

}
