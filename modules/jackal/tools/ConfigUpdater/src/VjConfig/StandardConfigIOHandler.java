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

import java.io.*;

import VjConfig.ConfigIOHandler;
import VjConfig.ConfigChunkDB;
import VjConfig.ConfigChunk;
import VjConfig.ChunkFactory;

public class StandardConfigIOHandler implements ConfigIOHandler {

    //----------------- ConfigChunkDB Methods ------------------------

    public void readConfigChunkDB (File file, ConfigChunkDB db,
                                   ConfigIOStatus iostatus) {
        try {
            FileReader r = new FileReader (file);
            
            ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
            if (!db.read (st))
                iostatus.addFailure ("Standard Parser failed.");
        }
        catch (FileNotFoundException e) {
            iostatus.addFailure (e);
        }
    }
               

    public void readConfigChunkDB (InputStream in, ConfigChunkDB db,
                                   ConfigIOStatus iostatus) {
        InputStreamReader r = new InputStreamReader (in);
        ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
        if (!db.read (st))
            iostatus.addFailure ("Standard Parser failed.");
    }

    

    public void writeConfigChunkDB (DataOutputStream out, ConfigChunkDB db) throws IOException {
        out.writeBytes (db.fileRep());
    }


    public void writeConfigChunkDB (File file, ConfigChunkDB db) throws IOException {
        DataOutputStream out = new DataOutputStream (new FileOutputStream (file));
        out.writeBytes (db.fileRep());
    }


    //----------------- ChunkDescDB Methods ----------------------------

    public void readChunkDescDB (File file, ChunkDescDB db, 
                                 ConfigIOStatus iostatus) {
        try {
            FileReader r = new FileReader (file);
            ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
            if (!db.read (st))
                iostatus.addFailure ("Standard Parser failed.");
        }
        catch (FileNotFoundException e) {
            iostatus.addFailure (e);
        }
    }
               

    public void readChunkDescDB (InputStream in, ChunkDescDB db,
                                 ConfigIOStatus iostatus) {
        InputStreamReader r = new InputStreamReader (in);
        ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
        if (!db.read (st))
            iostatus.addFailure ("Standard Parser failed.");
    }

    

    public void writeChunkDescDB (DataOutputStream out, ChunkDescDB db) throws IOException {

            out.writeBytes (db.fileRep());
    }


    public void writeChunkDescDB (File file, ChunkDescDB db) throws IOException {

            DataOutputStream out = new DataOutputStream (new FileOutputStream (file));
            out.writeBytes (db.fileRep());
    }


}
