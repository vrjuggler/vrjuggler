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

import java.util.List;
import VjConfig.ConfigChunkDB;
import VjConfig.ChunkDescDB;
import VjConfig.ChunkDesc;
import VjConfig.ConfigChunk;

public class ChunkFactory {

    static private ChunkDescDB descdb = null;
    static private ConfigChunkDB default_chunkdb = null;

    static public void setChunkDescDB (ChunkDescDB _db) {
	descdb = _db;
    }

    static public void addChunkDescDB (ChunkDescDB _db) {
        descdb.addAll (_db);
    }

    static public void setDefaultChunkDB (ConfigChunkDB _db) {
	default_chunkdb = _db;
    }

    static public ChunkDesc getChunkDescByToken (String s) {
	return descdb.getByToken(s);
    }

    static public ConfigChunk createChunkWithDescName (String s) {
        return createChunkWithDescName (s, true);
    }

    static public ConfigChunk createChunkWithDescName (String s, boolean use_defaults) {
	ConfigChunk newchunk = null;

	if (default_chunkdb != null) {
	    List v = default_chunkdb.getOfDescName (s);
	    if (!v.isEmpty()) {
		//System.out.println ("creating chunk from default");
		return new ConfigChunk ((ConfigChunk)v.get(0));
	    }
	}
	if (descdb != null) {
	    ChunkDesc cd = descdb.getByName(s);
	    if (cd != null)
		return new ConfigChunk (cd, use_defaults);
	}
	return null;
    }


    static public ConfigChunk createChunkWithDescToken (String s) {
        return createChunkWithDescToken (s, true);
    }

    static public ConfigChunk createChunkWithDescToken (String s, boolean use_defaults) {
	ConfigChunk newchunk = null;

	if (default_chunkdb != null) {
	    List v = default_chunkdb.getOfDescToken (s);
	    if (!v.isEmpty()) {
		//System.out.println ("creating chunk from default");
		return new ConfigChunk ((ConfigChunk)v.get(0));
	    }
	}
	if (descdb != null) {
	    ChunkDesc cd = descdb.getByToken(s);
	    if (cd != null)
		return new ConfigChunk (cd, use_defaults);
	}
	return null;
    }


    static public ConfigChunk createChunk (ChunkDesc cd) {
	return new ConfigChunk (cd);
    }

    // Utility functions dealing with the global DescDB 
    /** Returns the names of all ChunkDescs defined in any of our DBs.
     *  This is useful for GUI components that want to provide a list
     *  or menu of desc names.
     */
    static public String[] getDescNames () {
        int n = descdb.size();
        String[] names = new String[n];
        for (int i = 0; i < n; i++)
            names[i] = ((ChunkDesc)descdb.get(i)).getName();
        return names;
    }

    static public String getNameFromToken (String tok) {
        return descdb.getNameFromToken (tok);
    }

}
