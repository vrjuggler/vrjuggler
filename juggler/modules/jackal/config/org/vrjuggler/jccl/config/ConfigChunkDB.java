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



package VjConfig;

import java.util.Vector;
import java.io.*;
import VjConfig.*;

/** Representation of a VR Juggler style configuration file.
 */
public class ConfigChunkDB {

    /** Vector of ConfigChunks. */
    private Vector chunks;

    /** Targets for ChunkDBEvents. */
    private Vector targets;

    /** Identifier for the ConfigChunkDB.
     *  Note: It's generally the case that name will be the last component
     *  of file, but this isn't always true.  VjControl will append tags
     *  to the name in order to distinguish files with the same name
     *  (e.g. "config [1]" and "config [2]").
     */
    public String name;

    /** File that the ChunkDB was loaded from.
     *  This is used for loading & saving the ChunkDB, and is guaranteed
     *  not to be mangled like name could be.
     *  <p>
     *  Note: file defaults to "Untitled", but is never null unless 
     *  explicitly set.
     */
    public File file;


    /** Flag to determine if file has been modified since last saved.
     */
    public boolean need_to_save;


    /** Constructor. */
    public ConfigChunkDB () {
	super();
	chunks = new Vector();
	targets = new Vector();
	name = "Untitled";
	file = new File("Untitled");
        need_to_save = false;
    }


    /** Sets the file to use for loading/saving */
    public final void setFile (File f) {
        file = f;
    }

    /** Sets the identifier string for this DB */
    public final void setName (String _name) {
	name = _name;
    }

    /** Returns the identifier string for this DB */
    public final String getName () {
	return name;
    }


    /** Returns the difference between self and db.
     *  Build a new ConfigChunkDB that's a "difference" of its arguments -
     *  the returned db contains every chunk in db that isn't in self,
     *  or that differs from the same-named chunk in self.
     */
    public ConfigChunkDB diff (ConfigChunkDB d) {
	ConfigChunk c1, c2;
	ConfigChunkDB newdb = new ConfigChunkDB();
	
	for (int i = 0; i < d.size(); i++) {
	    c1 = d.elementAt(i);
	    c2 = get (c1.name);
	    if ((c2 == null) || (!c1.equals(c2)))
		newdb.insertOrdered(c1);
	}
	return newdb;
    }


    /** Returns size of DB. */
    public final int size() {
	return chunks.size();
    }


    /** Gets the ith element of self. */
    public ConfigChunk elementAt (int i) throws ArrayIndexOutOfBoundsException {
	return (ConfigChunk)chunks.elementAt(i);
    }


    /** Removed all ConfigChunks from self and notifies ChunkDBListeners. */
    public void removeAll () {
	chunks.removeAllElements();
	notifyChunkDBTargets (new ChunkDBEvent (this, ChunkDBEvent.REMOVEALL, null, null));
    }


    /** Removes the ConfigChunk at index i and notifies ChunkDBListeners. */
    public void removeChunkAt (int i) {
	ChunkDBEvent e = new ChunkDBEvent (this, ChunkDBEvent.REMOVE, (ConfigChunk)chunks.elementAt(i), null);
	chunks.removeElementAt(i);
	notifyChunkDBTargets (e);
    }


    /** Removes the ConfigChunk in self whose name == name. 
     *  @return True if a matching ConfigChunk was found and removed, 
     *          else false.
     */
    public boolean remove (String name) {
	for (int i = 0; i < size(); i++) {
	    if (elementAt(i).getName().equalsIgnoreCase(name)) {
		ChunkDBEvent e = new ChunkDBEvent (this, ChunkDBEvent.REMOVE, (ConfigChunk)chunks.elementAt(i), null);
		removeChunkAt(i);
		notifyChunkDBTargets (e);
		return true;
	    }
	}
	return false;
    }



    /** Removes a ConfigChunk with the same name as ch.
     *  @return True if a matching ConfigChunk was found and removed,
     *          else false.
     */
    public final boolean remove (ConfigChunk ch) {
        return remove (ch.getName());
    }



    /** Removes any ConfigChunks in self with the same name as any Chunk
     *  in db.
     *  @return True if any ConfigChunks were removed, else false.
     */
    public boolean removeAll (ConfigChunkDB db) {
        boolean retval = false;
        ConfigChunk ch;
        for (int i = 0; i < db.size(); i++) {
            ch = db.elementAt (i);
            retval |= remove (ch);
        }
        return retval;
    }



    /** Find the named ConfigChunk.
     * @return A ConfigChunk in self whose name matches the parameter, or
     *         null if none is found.
     */
    public ConfigChunk get (String name) {
	ConfigChunk ch;
	for (int i = 0; i < size(); i++) {
	    ch = elementAt(i);
	    if (ch.getName().equalsIgnoreCase(name)) {
		return ch;
	    }
	}
	return null;
    }



    /** Gets all ConfigChunks using a particular ChunkDesc.
     *  @param typename A ChunkDesc name.
     *  @return A vector of all ConfigChunks in self using the named ChunkDesc.
     *          This may be an empty Vector, but is never null.
     */
    public Vector getOfDescName (String typename) {
	Vector v = new Vector();
	for (int i = 0; i < size(); i++) {
	    if (elementAt(i).desc.name.equalsIgnoreCase(typename))
		v.addElement(elementAt(i));
	}
	return v;
    }
    


    /** Gets all ConfigChunks using a particular ChunkDesc.
     *  @param typename A ChunkDesc name.
     *  @return A vector of all ConfigChunks in self using the named ChunkDesc.
     *          This may be an empty Vector, but is never null.
     */
    public Vector getOfDescToken (String typename) {
	Vector v = new Vector();
	for (int i = 0; i < size(); i++) {
	    if (((ConfigChunk)elementAt(i)).desc.token.equalsIgnoreCase(typename))
		v.addElement((ConfigChunk)elementAt(i));
	}
	return v;
    }



    /** Inserts ch into self.
     *  Any same-named Chunk already in self is removed.
     */
    public final void add (ConfigChunk ch) {
        insertOrdered (ch);
    }



    /** Inserts all Chunks in db into self.
     *  Any same-named Chunks already in self are removed.
     */
    public void addAll (ConfigChunkDB db) {
        int i;
        int n = db.size();
        for (i = 0; i < n; i++)
            insertOrdered (db.elementAt(i));
    }



    /** Inserts c into self.
     *  Adds a Chunk into self, with the constraint that it's stored with
     *  adjacent to all other Chunks using the same ChunkDesc.
     */
    public void insertOrdered (ConfigChunk c) {
	ConfigChunk t;
	int i = 0;

	remove (c.name);

        if (c.getDescToken().equalsIgnoreCase("vjIncludeDescFile")) {
            chunks.insertElementAt(c, 0);
        }
        else if (c.getDescToken().equalsIgnoreCase("vjIncludeFile")) {
	    for (i = 0; i < chunks.size(); i++) {
		t = (ConfigChunk)chunks.elementAt(i);
		if (!t.getDescToken().equalsIgnoreCase("vjIncludeDescFile")
                    && !t.getDescToken().equalsIgnoreCase("vjIncludeFile")) 
                    break;                          
	    }
            chunks.insertElementAt(c,i);
        }
        else
            chunks.addElement (c);

	ChunkDBEvent e = new ChunkDBEvent (this, ChunkDBEvent.INSERT, null, c);
	notifyChunkDBTargets (e);

    }



    /** Generates a name that won't conflict with anything in self.
     *  This generates a new ConfigChunk name, prefixed with root,
     *  that won't conflict with the names of any Chunks already in self.
     *  It does this by appending an integer to the end of the name.
     */
    public String getNewName (String root) {
	String name;
	int num = 1;
	do {
	    name = root + Integer.toString(num++);
	} while (get(name) != null);
	return name;
    }



    /** Reads a series of ConfigChunks from st.
     *  Terminates when it encounters the end of the stream or the token
     *  "End".
     */
    public boolean read (ConfigStreamTokenizer st) {
	ConfigChunk c;

	for (;;) {
	    c = readAChunk(st);
	    if (c != null)
		insertOrdered(c);
	    else
		break;
	}
	return true;
    }



    /** Auxiliary for read. */
    public ConfigChunk readAChunk (ConfigStreamTokenizer st) {
	String s;
	ConfigChunk c = null;
	ChunkDesc d;

	try {
	    while (c == null) {
		st.nextToken();
		if ((st.ttype == ConfigStreamTokenizer.TT_EOF) ||
		    ((st.ttype == ConfigStreamTokenizer.TT_WORD) &&
		     st.sval.equalsIgnoreCase ("end")))
		    return null;
		c = ChunkFactory.createChunkWithDescToken(st.sval);
		if (c != null) {
		    c.read(st);
		    if ((c.name == null) || (c.name.equals(""))) {
			c.name = getNewName (c.desc.name);    
		    }
		} 
		else {
		    System.err.println ("Error reading ConfigChunks - no such chunk type " + st.sval);
		    for(;;) {
			st.nextToken();
			if ((st.ttype == ConfigStreamTokenizer.TT_WORD &&
			     st.sval.equalsIgnoreCase("end")) ||
			    (st.ttype == ConfigStreamTokenizer.TT_EOF))
			    break;
		    }    
		}
	    }
	    return c;   
	}
	catch (IOException io) {
	    System.err.println ("IO Error in ConfigChunkDB.read()");
	    return null;
	}
    }



    /** Removes o and inserts n. */
    public void replace(ConfigChunk o, ConfigChunk n) {
	/* removes old from the db and inserts new in old's place */
	ConfigChunk t;
	for (int i = 0; i < size(); i++) {
	    t = (ConfigChunk)elementAt(i);
	    if (t.getName().equals(o.getName())) {
		chunks.removeElementAt(i);
		chunks.insertElementAt(n,i);
		ChunkDBEvent e = new ChunkDBEvent (this, ChunkDBEvent.REPLACE, o, n);
		notifyChunkDBTargets (e);
		return;
	    }
	}
    }


    
    /** Utility for getDependencies. */
    protected Vector addDependsForChunk (ConfigChunk ch, Vector v, String nameprefix) {
	// returns vector of pde's
	// finds dependencies in ch and adds 'em to v.
	int i, j, k;
	ConfigChunk ch2;
	Property p;
	String s;
	VarValue val;
	ChunkDependEntry cde;
	PropDependEntry pde;

	for (j = 0; j < ch.props.size(); j++) {
	    p = (Property)ch.props.elementAt(j);
	    if (p.valtype.equals (ValType.t_chunk)) {
		for (k = 0; k < p.vals.size(); k++) {
		    s = ((VarValue)p.vals.elementAt(k)).getString();
		    if (s.equals (""))
			continue;
		    ch2 = get(s);
		    if (ch2 == null) {
			pde = new PropDependEntry();
			pde.propertyname = nameprefix + p.getName();
			if (p.vals.size() > 1)
			    pde.propertyname += "[" + k + "]";
			pde.dependency_name = s;
			v.addElement (pde);
		    }
		}
	    }
	    else if (p.valtype.equals (ValType.t_embeddedchunk)) {
		for (k = 0; k < p.vals.size(); k++) {
		    ch2 = ((VarValue)p.vals.elementAt(k)).getEmbeddedChunk();
		    String prefix = nameprefix + p.getName();
		    if (p.vals.size() > 1)
			prefix += "[" + k + "].";
		    else
			prefix += ".";
		    addDependsForChunk (ch2, v, nameprefix + prefix);
		}
	    }
	}
	return v;
    }



    /** Gets dependencies of all ConfigChunks in self.
     *  Includes only dependencies that aren't satisfied by self.
     *  @return a Vector of ChunkDependEntries.
     */
    public Vector getDependencies () {
	// gets the dependencies of _this_chunkdb_only_
	int i;
	ConfigChunk ch;
	ChunkDependEntry cde;

	Vector dep = new Vector();
	
	for (i = 0; i < size(); i++) {
	    cde = null;
	    ch = (ConfigChunk)elementAt(i);
	    cde = new ChunkDependEntry();
	    cde.chunk = ch;
	    addDependsForChunk (ch, cde.propdepends, "");
	    if (cde.propdepends.size() > 0)
		dep.addElement (cde);
	}
	return dep;
    }



    /** Searches self for Chunks that satisfy dependencies.
     *  @param v A vector of ChunkDependEntries, assumably generated by
     *           getDependencies called on another ChunkDB.  searchDependencies
     *           modifies the DependEntries with new information.
     */
    public void searchDependencies (Vector v) {
	ChunkDependEntry cde;
	PropDependEntry pde;
	ConfigChunk ch;
	int i, j;

	for (i = 0; i < v.size(); i++) {
	    cde = (ChunkDependEntry)v.elementAt(i);
	    for (j = 0; j < cde.propdepends.size(); j++) {
		pde = (PropDependEntry)cde.propdepends.elementAt(j);
		ch = get (pde.dependency_name);
		if (ch != null)
		    pde.other_files.addElement (getName());
	    }
	}
    }



    /** Builds a string representation of self.
     *  This is a holdover from the times when ConfigChunkDB was a child
     *  of Vector, and Vector.toString() was final.
     */
    public String fileRep() {
        String s = "";
	for (int i = 0; i < size(); i++)
	    s = s + ((ConfigChunk)elementAt(i)).toString();
	s = s + "End\n";
	return s;
    }



    /******************** ChunkDB Target Stuff *********************/

    public synchronized void addChunkDBListener (ChunkDBListener l) {
	synchronized (targets) {
	    targets.addElement (l);
	}
    }

    public void removeChunkDBListener (ChunkDBListener l) {
	synchronized (targets) {
	    targets.removeElement (l);
	}
    }

    protected void notifyChunkDBTargets (ChunkDBEvent e) {
	Vector l;
        need_to_save = true;
	synchronized (targets) {
	    l = (Vector) targets.clone();
	}
	for (int i = 0; i < l.size(); i++) {
	    ChunkDBListener lis = (ChunkDBListener)l.elementAt (i);
            int a = e.getAction();
	    if (a == e.INSERT)
		lis.addChunk (e);
	    else if (a == e.REMOVE)
		lis.removeChunk (e);
            else if (a ==  e.REPLACE)
		lis.replaceChunk (e);
            else if (a ==  e.REMOVEALL)
		lis.removeAllChunks (e);
	}
    }

}


