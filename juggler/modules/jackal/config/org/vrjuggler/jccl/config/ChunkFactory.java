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



package VjConfig;

import java.util.Vector;
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

    static public void setDefaultChunkDB (ConfigChunkDB _db) {
	default_chunkdb = _db;
    }

    static public ChunkDesc getChunkDescByToken (String s) {
	return descdb.getByToken(s);
    }

    static public ConfigChunk createChunkWithDescName (String s) {
	ConfigChunk newchunk = null;

	if (default_chunkdb != null) {
	    Vector v = default_chunkdb.getOfDescName (s);
	    if (!v.isEmpty()) {
		//System.out.println ("creating chunk from default");
		return new ConfigChunk ((ConfigChunk)v.elementAt(0));
	    }
	}
	if (descdb != null) {
	    ChunkDesc cd = descdb.getByName(s);
	    if (cd != null)
		return new ConfigChunk (cd);
	}
	return null;
    }


    static public ConfigChunk createChunkWithDescToken (String s) {
	ConfigChunk newchunk = null;

	if (default_chunkdb != null) {
	    Vector v = default_chunkdb.getOfDescToken (s);
	    if (!v.isEmpty()) {
		//System.out.println ("creating chunk from default");
		return new ConfigChunk ((ConfigChunk)v.elementAt(0));
	    }
	}
	if (descdb != null) {
	    ChunkDesc cd = descdb.getByToken(s);
	    if (cd != null)
		return new ConfigChunk (cd);
	}
	return null;
    }


    static public ConfigChunk createChunk (ChunkDesc cd) {
	return new ConfigChunk (cd);
    }

    static public ConfigChunk createChunk (ChunkDesc cd, String name) {
	return new ConfigChunk (cd, name);
    }
}
