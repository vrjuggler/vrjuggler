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

import java.io.*;

import VjConfig.ConfigIOHandler;
import VjConfig.ConfigChunkDB;
import VjConfig.ConfigChunk;
import VjConfig.ChunkFactory;
import VjConfig.ConfigParserException;

public class StandardConfigIOHandler implements ConfigIOHandler {

    //----------------- ConfigChunkDB Methods ------------------------

    public void readConfigChunkDB (File file, ConfigChunkDB db) throws IOException, ConfigParserException {
        FileReader r = new FileReader (file);
        ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
        if (!db.read (st))
            throw new ConfigParserException ("Standard Parser failed.");
    }
               

    public void readConfigChunkDB (InputStream in, ConfigChunkDB db) throws IOException, ConfigParserException {
        InputStreamReader r = new InputStreamReader (in);
        ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
        if (!db.read (st))
            throw new ConfigParserException ("Standard Parser failed.");
    }

    

    public boolean writeConfigChunkDB (DataOutputStream out, ConfigChunkDB db) {
        try {
            out.writeBytes (db.fileRep());
            return true;
        }
        catch (Exception e) {
            return false;
        }
    }

    public boolean writeConfigChunkDB (File file, ConfigChunkDB db) {
        try {
            DataOutputStream out = new DataOutputStream (new FileOutputStream (file));
            out.writeBytes (db.fileRep());
            return true;
        }
        catch (Exception e) {
            return false;
        }
    }


    //----------------- ChunkDescDB Methods ----------------------------

    public void readChunkDescDB (File file, ChunkDescDB db) throws IOException, ConfigParserException {
        FileReader r = new FileReader (file);
        ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
        if (!db.read (st))
            throw new ConfigParserException ("Standard Parser failed.");
    }
               

    public void readChunkDescDB (InputStream in, ChunkDescDB db) throws IOException, ConfigParserException {
        InputStreamReader r = new InputStreamReader (in);
        ConfigStreamTokenizer st = new ConfigStreamTokenizer(r);
        if (!db.read (st))
            throw new ConfigParserException ("Standard Parser failed.");
    }

    

    public boolean writeChunkDescDB (DataOutputStream out, ChunkDescDB db) {
        try {
            out.writeBytes (db.fileRep());
            return true;
        }
        catch (Exception e) {
            return false;
        }
    }

    public boolean writeChunkDescDB (File file, ChunkDescDB db) {
        try {
            DataOutputStream out = new DataOutputStream (new FileOutputStream (file));
            out.writeBytes (db.fileRep());
            return true;
        }
        catch (Exception e) {
            return false;
        }
    }


}
