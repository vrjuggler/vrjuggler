/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

import java.util.*;
import java.io.*;
import VjConfig.*;

/** Representation of a VR Juggler-style configuration file.
 *  Essentially, a ConfigChunkDB is a container for ConfigChunks. It
 *  guarantees that all the ConfigChunks it contains have unique names
 *  and provides a variety of search and access functions, as well as
 *  utilities for checking dependencies between ConfigChunks.
 *
 *  @see ConfigChunk
 *  @see ChunkDescDB
 *  @see ConfigIO
 *  @author Christopher Just
 *  @version $Revision$
 */
public class ConfigChunkDB implements Cloneable {

    /** Vector of ConfigChunks. */
    private List chunks;

    /** Targets for ChunkDBEvents. */
    private ArrayList targets;

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
	targets = new ArrayList();
	name = "Untitled";
	file = new File("Untitled");
        need_to_save = false;
    }


    /** Deep copy. 
     *  Use this with caution, as any listeners on self will automatically
     *  become listeners on the clone.
     */
    public Object clone () {
        try {
            ConfigChunkDB db = (ConfigChunkDB)super.clone();
            db.targets = (ArrayList)targets.clone();
            ConfigChunk ch;
            int i, n = chunks.size();
            db.chunks = new Vector(n);
            for (i = 0; i < n; i++) {
                ch = (ConfigChunk)chunks.get(i);
                db.chunks.add (new ConfigChunk (ch));
            }
            return db;
        }
        catch (CloneNotSupportedException e) {
            return null; // not gonna happen
        }
    }


    /** Sets the file to use for loading/saving */
    public final void setFile (File f) {
        file = f;
    }

    /** Retruns the file used for loading/saving */
    public final File getFile () {
        return file;
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
	    c1 = (ConfigChunk)d.chunks.get(i);
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
    public ConfigChunk get (int i) throws IndexOutOfBoundsException {
	return (ConfigChunk)chunks.get(i);
    }


    /** Removed all ConfigChunks from self and notifies ChunkDBListeners. */
    public void clear () {
	chunks.clear();
	notifyChunkDBTargets (new ChunkDBEvent (this, ChunkDBEvent.REMOVEALL, null, null));
    }


    /** Removes the ConfigChunk at index i and notifies ChunkDBListeners.
     *  @return The object that was removed
     */
    public ConfigChunk remove (int i) throws IndexOutOfBoundsException {
        ConfigChunk ch = (ConfigChunk)chunks.get(i);
	ChunkDBEvent e = new ChunkDBEvent (this, ChunkDBEvent.REMOVE, 
                                           ch, null);
	chunks.remove(i);
	notifyChunkDBTargets (e);
        return ch;
    }


    /** Removes the ConfigChunk in self whose name == name. 
     *  @return The object that was removed, or null if it wasn't found.
     */
    public ConfigChunk remove (String name) {
        ConfigChunk ch;
        ListIterator i = chunks.listIterator();
        while (i.hasNext()) {
            ch = (ConfigChunk)i.next();
	    if (ch.getName().equalsIgnoreCase(name)) {
		ChunkDBEvent e = new ChunkDBEvent (this, ChunkDBEvent.REMOVE, 
                                                   ch, null);
                i.remove();
		notifyChunkDBTargets (e);
		return ch;
	    }
	}
	return null;
    }



    /** Removes a ConfigChunk with the same name as ch.
     *  @return The object that was removed, or null if it wasn't found.
     */
    public final ConfigChunk remove (ConfigChunk ch) {
        return remove (ch.getName());
    }



    /** Removes any ConfigChunks in self with the same name as any Chunk
     *  in db.
     *  @return True if any ConfigChunks were removed, else false.
     */
    public boolean removeAll (ConfigChunkDB db) {
        boolean retval = false;
        ConfigChunk ch;
        ListIterator i = db.chunks.listIterator();
        while (i.hasNext()) {
            ch = (ConfigChunk)i.next();
            retval |= (null != remove (ch));
        }
        return retval;
    }



    /** Find the named ConfigChunk.
     * @return A ConfigChunk in self whose name matches the parameter, or
     *         null if none is found.
     */
    public ConfigChunk get (String name) {
	ConfigChunk ch;
        Iterator i = chunks.iterator();
        while (i.hasNext()) {
            ch = (ConfigChunk)i.next();
	    if (ch.getName().equalsIgnoreCase(name)) {
		return ch;
	    }
	}
	return null;
    }



    /** Gets all ConfigChunks using a particular ChunkDesc.
     *  @param typename A ChunkDesc name.
     *  @return A list of all ConfigChunks in self using the named ChunkDesc.
     *          List may be empty, but is never null.  The list is not
     *          synchronized.
     */
    public List getOfDescName (String typename) {
	List v = new ArrayList();
        ConfigChunk ch;
        Iterator i = chunks.iterator();
        while (i.hasNext()) {
            ch = (ConfigChunk)i.next();
            if (ch.getDescName().equalsIgnoreCase(typename))
                v.add (ch);
//  	for (int i = 0; i < size(); i++) {
//  	    if (elementAt(i).desc.name.equalsIgnoreCase(typename))
//  		v.add(elementAt(i));
	}
	return v;
    }
    


    /** Gets all ConfigChunks using a particular ChunkDesc.
     *  @param typename A ChunkDesc name.
     *  @return A vector of all ConfigChunks in self using the named ChunkDesc.
     *          This may be an empty Vector, but is never null.
     */
    public List getOfDescToken (String typename) {
	List v = new ArrayList();
        ConfigChunk ch;
        Iterator i = chunks.iterator();
        while (i.hasNext()) {
            ch = (ConfigChunk)i.next();
            if (ch.getDescToken().equalsIgnoreCase(typename))
                v.add (ch);
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
        Iterator i = db.chunks.iterator();
        while (i.hasNext())
            insertOrdered ((ConfigChunk)i.next());
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
            chunks.add (0, c);
        }
        else if (c.getDescToken().equalsIgnoreCase("vjIncludeFile")) {
	    for (i = 0; i < chunks.size(); i++) {
		t = (ConfigChunk)chunks.get(i);
		if (!t.getDescToken().equalsIgnoreCase("vjIncludeDescFile")
                    && !t.getDescToken().equalsIgnoreCase("vjIncludeFile")) 
                    break;                          
	    }
            chunks.add (i, c);
        }
        else
            chunks.add (c);

	ChunkDBEvent e = new ChunkDBEvent (this, ChunkDBEvent.INSERT, null, c);
	notifyChunkDBTargets (e);

    }



    /** Generates a name that won't conflict with anything in self.
     *  This generates a new ConfigChunk name, prefixed with root,
     *  that won't conflict with the names of any Chunks already in self.
     *  It does this by appending an integer to the end of the name.
     */
    public String getNewName (String root) {
	String name = root;
	int num = 2;
	while (get(name) != null) {
	    name = root + Integer.toString(num++);
	}
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
	    if (c != null) {
                c.validateEmbeddedChunkNames(); // needed when loading old files.
		insertOrdered(c);
            }
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
			c.setName( getNewName (c.desc.name));    
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
	for (int i = 0; i < chunks.size(); i++) {
	    t = (ConfigChunk)chunks.get(i);
	    if (t.getName().equals(o.getName())) {
		chunks.set (i,n);
		ChunkDBEvent e = new ChunkDBEvent (this, ChunkDBEvent.REPLACE, o, n);
		notifyChunkDBTargets (e);
		return;
	    }
	}
    }


    
    /** Utility for getDependencies. */
    protected void addDependsForChunk (ConfigChunk ch, List v, String nameprefix) {
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
	    p = (Property)ch.props.get(j);
	    if (p.getValType() == ValType.CHUNK) {
		for (k = 0; k < p.getNum(); k++) {
		    s = p.getValue(k).getString();
		    if (s.equals (""))
			continue;
		    ch2 = get(s);
		    if (ch2 == null) {
			pde = new PropDependEntry();
			pde.propertyname = nameprefix + p.getName();
			if (p.vals.size() > 1)
			    pde.propertyname += "[" + k + "]";
			pde.dependency_name = s;
			v.add (pde);
		    }
		}
	    }
	    else if (p.getValType() == ValType.EMBEDDEDCHUNK) {
		for (k = 0; k < p.getNum(); k++) {
		    ch2 = p.getValue(k).getEmbeddedChunk();
		    String prefix = nameprefix + p.getName();
		    if (p.vals.size() > 1)
			prefix += "[" + k + "].";
		    else
			prefix += ".";
		    addDependsForChunk (ch2, v, nameprefix + prefix);
		}
	    }
	}
    }



    /** Gets dependencies of all ConfigChunks in self.
     *  Includes only dependencies that aren't satisfied by self.
     *  @return a List of ChunkDependEntries.
     */
    public List getDependencies () {
	// gets the dependencies of _this_chunkdb_only_
	int i;
	ConfigChunk ch;
	ChunkDependEntry cde;

	List dep = new ArrayList();
	
	for (i = 0; i < chunks.size(); i++) {
	    cde = null;
	    ch = (ConfigChunk)chunks.get(i);
	    cde = new ChunkDependEntry();
	    cde.chunk = ch;
	    addDependsForChunk (ch, cde.propdepends, "");
	    if (cde.propdepends.size() > 0)
		dep.add (cde);
	}
	return dep;
    }



    /** Searches self for Chunks that satisfy dependencies.
     *  @param v A vector of ChunkDependEntries, assumably generated by
     *           getDependencies called on another ChunkDB.  searchDependencies
     *           modifies the DependEntries with new information.
     */
    public void searchDependencies (List v) {
	ChunkDependEntry cde;
	PropDependEntry pde;
	ConfigChunk ch;
	int i, j;

	for (i = 0; i < v.size(); i++) {
	    cde = (ChunkDependEntry)v.get(i);
	    for (j = 0; j < cde.propdepends.size(); j++) {
		pde = (PropDependEntry)cde.propdepends.get(j);
		//ch = get (pde.dependency_name);
                ch = searchDependency (pde.dependency_name);
		if (ch != null)
		    pde.other_files.add (getName());
	    }
	}
    }

    private ConfigChunk searchDependency (String dep_name) {
        if (!ConfigChunk.hasSeparator(dep_name))
            return get (dep_name);
        else {
            // embedded chunk dependency
            String first = ConfigChunk.getFirstNameComponent (dep_name);
            ConfigChunk ch = get (first);
            if (ch == null)
                return null;
            else
                return ch.getEmbeddedChunk (dep_name);
        }
    }



    /** Builds a string representation of self.
     *  This is a holdover from the times when ConfigChunkDB was a child
     *  of Vector, and Vector.toString() was final.
     */
    public String fileRep() {
        String s = "";
	for (int i = 0; i < size(); i++)
	    s = s + ((ConfigChunk)chunks.get(i)).toString();
	s = s + "End\n";
	return s;
    }


    public String xmlRep () {
        StringBuffer s = new StringBuffer(512);
        s.append("<" + ConfigTokens.chunk_db_TOKEN + ">\n");
        int n = size();
        for (int i = 0; i < size(); i++) {
            s.append (((ConfigChunk)chunks.get(i)).xmlRep("  "));
        }
        s.append ("</" + ConfigTokens.chunk_db_TOKEN + ">\n");
        return s.toString();
    }


    /******************** ChunkDB Target Stuff *********************/

    public synchronized void addChunkDBListener (ChunkDBListener l) {
	synchronized (targets) {
	    targets.add (l);
	}
    }

    public void removeChunkDBListener (ChunkDBListener l) {
	synchronized (targets) {
	    targets.remove (targets.indexOf(l));
	}
    }

    protected void notifyChunkDBTargets (ChunkDBEvent e) {
	ChunkDBListener[] l = new ChunkDBListener[20];
        need_to_save = true;
	synchronized (targets) {
	    l = (ChunkDBListener[]) targets.toArray(l);
	}
        int a = e.getAction();
	for (int i = 0; (i < l.length) && (l[i] != null); i++) {
	    if (a == e.INSERT)
		l[i].configChunkAdded (e);
	    else if (a == e.REMOVE)
		l[i].configChunkRemoved (e);
	    else if (a == e.REPLACE)
		l[i].configChunkReplaced (e);
	    else if (a == e.REMOVEALL)
		l[i].configChunksCleared (e);
	}
    }


}


