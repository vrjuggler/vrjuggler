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

/** Event spawned by ConfigModule for add/remove databases.
 *  By becoming a ConfigModuleListener, an object can receive these
 *  events, which give information about addition or removal of
 *  ConfigChunkDBs and ChunkDescDBs.
 */
package VjComponents.ConfigEditor;

import java.util.EventObject;

import VjConfig.ChunkDescDB;
import VjConfig.ConfigChunkDB;


public class ConfigModuleEvent extends EventObject {
    private ConfigChunkDB chunkdb;
    private ChunkDescDB descdb;


    public ConfigModuleEvent (Object _source, ConfigChunkDB _chunkdb, ChunkDescDB _descdb) {
	super(_source);
	descdb = _descdb;
	chunkdb = _chunkdb;
    }

    public ConfigChunkDB getChunkDB () {
	return chunkdb;
    }

    public ChunkDescDB getDescDB () {
	return descdb;
    }

}



