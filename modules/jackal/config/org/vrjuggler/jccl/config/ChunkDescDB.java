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


/* corresponds to ChunkDescDB.C/.h */

package VjConfig;

import java.util.Vector;
import java.util.ArrayList;
import java.util.List;
import java.io.*;

import VjConfig.DescDBEvent;
import VjConfig.DescDBListener;

public class ChunkDescDB {

    public String name;
    public File file;

    private ArrayList targets;
    private ArrayList descs;

    public boolean need_to_save;

    public ChunkDescDB () {
	name = "Unnamed";
	file = new File ("");
	targets = new ArrayList();
	descs = new ArrayList();
        need_to_save = false;
    }


    public final void setName(String _name) {
	name = _name;
    }


    public final String getName () {
	return name;
    }


    public final void setFile (File _file) {
	file = _file;
    }


    public final File getFile () {
        return file;
    }


    public final synchronized int size () {
	return descs.size();
    }


    public final synchronized ChunkDesc get (int i) {
	return (ChunkDesc)descs.get(i);
    }


//      public synchronized ChunkDescDB diff (ChunkDescDB d) {
//  	/* builds a new ChunkDescDB that's sort of the difference of its
//  	 * arguments - the returned db contains every chunk in d that isn't
//  	 * in self or differs from the same-named chunk in self 
//  	 */
//  	ChunkDesc c1, c2;
//  	ChunkDescDB newdb = new ChunkDescDB();
//          synchronized (d) {
//              int n = d.descs.size();
//              for (int i = 0; i < n; i++) {
//                  c1 = (ChunkDesc)d.descs.get(i);
//                  c2 = getByToken (c1.token);
//                  if ((c2 == null) || (!c1.equals(c2)))
//                      newdb.descs.add(c1);
//              }
//          }
//  	return newdb;
//      }



    public void replace (ChunkDesc oldc, ChunkDesc newc) {
        DescDBEvent e = null;
        synchronized (this) {
            int i = descs.indexOf (oldc);
            if (i >= 0) {
                e = new DescDBEvent (this, DescDBEvent.REPLACE, oldc, newc);
                descs.set (i, newc);
            }
        }
        if (e == null)
	    add (newc);
        else
            notifyDescDBTargets (e);
    }
    


    public void clear () {
        synchronized (this) {
            descs.clear();
        }
	notifyDescDBTargets (new DescDBEvent (this, DescDBEvent.REMOVEALL, null, null));
    }



    public ChunkDesc remove (ChunkDesc d) {
        return removeByToken (d.getToken());
    }


    public ChunkDesc removeByToken (String tok) {
        ChunkDesc d, retval = null;
        synchronized (this) {
            int n = descs.size();
            for (int i = 0; i < n; i++) {
                d = (ChunkDesc)descs.get(i);
                if (d.token.equals(tok)) {
                    retval = d;
                    descs.remove(i);
                    break;
                }
            }
        }
        if (retval != null) {
            DescDBEvent e = new DescDBEvent (this, DescDBEvent.REMOVE, 
                                             retval, null);
            notifyDescDBTargets(e);
        }
        return retval;
    }



    public ChunkDesc removeByName(String tok) {
        ChunkDesc d, retval = null;
        synchronized (this) {
            int n = descs.size();
            for (int i = 0; i < n; i++) {
                d = (ChunkDesc)descs.get(i);
                if (d.name.equalsIgnoreCase(tok)) {
                    retval = d;
                    descs.remove(i);
                    break;
                }
            }
        }
        if (retval != null) {
            DescDBEvent e = new DescDBEvent (this, DescDBEvent.REMOVE, 
                                             retval, null);
            notifyDescDBTargets (e);
        }
        return retval;
    }
    


    public List getTokenBegins (String tok) {
	ChunkDesc d;
	ArrayList v = new ArrayList();
        ChunkDesc[] da;
        synchronized (this) {
            da = new ChunkDesc[descs.size()];
            da = (ChunkDesc[])descs.toArray(da);
        }
        for (int i = 0; i < da.length; i++) {
            //d = (ChunkDesc)descs.get(i);
            if (da[i].token.startsWith (tok))
                v.add(da[i]);
        }
	return v;
    }



    public final ChunkDesc getByToken (String tok) {
        synchronized (this) {
            int n = descs.size();
            for (int i = 0; i < n; i++) {
                if (((ChunkDesc)descs.get(i)).token.equalsIgnoreCase(tok)) {
                    return (ChunkDesc)descs.get(i);
                }
            }
        }
	return null;
    }
    


    public final synchronized ChunkDesc getByName (String tok) {
        int n = descs.size();
	for (int i = 0; i < n; i++)
	    if (((ChunkDesc)descs.get(i)).name.equalsIgnoreCase(tok))
		return (ChunkDesc)descs.get(i);
	return null;
    }



    public final synchronized String getTokenFromName (String _name) {
        int n = descs.size();
	for (int i = 0; i < n; i++)
	    if (((ChunkDesc)descs.get(i)).name.equalsIgnoreCase(_name))
		return ((ChunkDesc)descs.get(i)).token;
	return null;
    }



    public final synchronized String getNameFromToken (String tok) {
        int n = descs.size();
	for (int i = 0; i < n; i++)
	    if (((ChunkDesc)descs.get(i)).token.equalsIgnoreCase(tok))
		return ((ChunkDesc)descs.get(i)).name;
	return null;
    }



    /** Adds d to self.
     *  Note: addElement may alter the name of d in order to avoid 
     *  conflicts with ChunkDescs already in self.
     */
    public void add (ChunkDesc d) {
        // remove any desc with the same token
        removeByToken (d.token);
        synchronized (this) {

            // make sure the name will be unique
            if (getByName (d.getName()) != null) {
                // this is ugly & clunky, but it's a rarely occurring case,
                // and java (yay!) will handle the memory for us.  If we're
                // changing the name on insert, we need to allocate a new
                // chunkdesc with the new name, so that other chunkdescdbs
                // using this same chunk keep the original name.  since
                // we always replace instead of modifying chunkdescs, this
                // lets the same desc appear in 2 descdbs simultaneously.
                d = (ChunkDesc)d.clone();
                d.setName (generateUniqueName (d.getName()));
            }

            // do the deed
            descs.add(d);
        }
	DescDBEvent e = new DescDBEvent (this, DescDBEvent.INSERT, null, d);
	notifyDescDBTargets (e);
    }



    protected String generateUniqueName (String name) {
        String n;
        int i = 1;
        do {
            n = name + " <" + (i++) + ">";
        } while (getByName (n) != null);
        return n;
    }



    public void addAll (List v) {
        int n = v.size();
	for (int i = 0; i < n; i++)
	    add ((ChunkDesc)v.get(i));
    }



    public final void addAll (ChunkDescDB db) {
        synchronized (db) {
            addAll (db.descs);
        }
    }



    public boolean read (ConfigStreamTokenizer st) {
	String name;
	ChunkDesc c;

	try {
	    while (true) {
		st.nextToken();
		if (st.sval.equalsIgnoreCase ("end"))
		    return true;
		if (!st.sval.equalsIgnoreCase("chunk"))
		    throw new IOException();
		st.nextToken();
		name = st.sval;
		//System.err.println ("Reading chunkDesc: " + name);
		c = new ChunkDesc (name);
		c.read(st);
		add(c);
	    }

	}
	catch (IOException io) {
	    System.err.println ("IO Error in ChunkDescDB.read()");
	    return false;
	}
    }



    public String fileRep () {
	String s = "";
	ChunkDesc d;
        synchronized (this) {
            int n = descs.size();
            for (int i = 0; i < n; i++) {
                d = (ChunkDesc)descs.get(i);
                s += d.toString();
            }
        }
	s += "End\n";
	return s;
    }


    public String xmlRep () {
        StringBuffer s = new StringBuffer(512);
        s.append("<ChunkDescDB>\n");
        synchronized (this) {
            int n = size();
            for (int i = 0; i < descs.size(); i++) {
                s.append (((ChunkDesc)descs.get(i)).xmlRep("  "));
            }
        }
        s.append ("</ChunkDescDB>\n");
        return s.toString();
    }



    /******************** DescDB Target Stuff *********************/

    public synchronized void addDescDBListener (DescDBListener l) {
	synchronized (targets) {
	    targets.add (l);
	}
    }


    public void removeDescDBListener (DescDBListener l) {
	synchronized (targets) {
	    targets.remove (targets.indexOf(l));
	}
    }


    protected void notifyDescDBTargets (DescDBEvent e) {
	DescDBListener[] l = new DescDBListener[20];
        need_to_save = true;
	synchronized (targets) {
	    l = (DescDBListener[]) targets.toArray(l);
	}
        int a = e.getAction();
	for (int i = 0; (i < l.length) && (l[i] != null); i++) {
	    if (a == e.INSERT)
		l[i].addDesc (e);
	    else if (a == e.REMOVE)
		l[i].removeDesc (e);
	    else if (a == e.REPLACE)
		l[i].replaceDesc (e);
	    else if (a == e.REMOVEALL)
		l[i].removeAllDescs (e);
	}
    }

}




