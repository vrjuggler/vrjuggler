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

import java.io.InputStream;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.File;

import VjConfig.ConfigChunkDB;
import VjConfig.ChunkDescDB;
//import VjConfig.ConfigChunk;
import VjConfig.ChunkFactory;

public interface ConfigIOHandler {

    public boolean readConfigChunkDB (File file, ConfigChunkDB db) throws FileNotFoundException;


    public boolean readConfigChunkDB (InputStream in, ConfigChunkDB db);
    

    public boolean writeConfigChunkDB (DataOutputStream out, ConfigChunkDB db);

    public boolean writeConfigChunkDB (File file, ConfigChunkDB db);


    public boolean readChunkDescDB (File file, ChunkDescDB db) throws FileNotFoundException;


    public boolean readChunkDescDB (InputStream in, ChunkDescDB db);
    

    public boolean writeChunkDescDB (DataOutputStream out, ChunkDescDB db);

    public boolean writeChunkDescDB (File file, ChunkDescDB db);


}
