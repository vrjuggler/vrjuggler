/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


package VjGUI;

import VjConfig.*;

public class ChunkTreeNodeInfo {

    public ConfigChunkDB db;
    public ConfigChunk ch;
    public String s;
    public boolean isdesc;
    public int childchunks;


    public ChunkTreeNodeInfo (ConfigChunkDB _db, ConfigChunk _ch) {
	db = _db;
	ch = _ch;
	s = null;
	isdesc = false;
	childchunks = 1;
    }

    public ChunkTreeNodeInfo (ConfigChunkDB _db) {
	db = _db;
	ch = null;
	s = null;
	isdesc = false;
	childchunks = 0;
    }

    public ChunkTreeNodeInfo (ConfigChunkDB _db, String _s) {
	this(_db, _s, false);
	childchunks = 0;
    }

    public ChunkTreeNodeInfo (ConfigChunkDB _db, String _s, boolean _isdesc) {
	/* for orgtree label nodes and chunkdescs(maybe) */
	db = _db;
	ch = null;
	s = _s;
	isdesc = _isdesc;
	childchunks = 0;
    }


    public boolean isDescNode() {
	return isdesc;
    }
    public boolean isChunkNode() {
	return (ch != null);
    }


    public String toString () {
	if (s != null)
	    return s;
	else if (ch != null)
	    return ch.getName();
	else if (db != null)
	    return db.name;
	else
	    return "empty";
    }
    
}

