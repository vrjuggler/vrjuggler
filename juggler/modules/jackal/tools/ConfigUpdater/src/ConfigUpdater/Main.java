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

package ConfigUpdater;

import VjConfig.*;
import java.io.*;
import java.net.URL;
import java.util.*;

/** Main for ConfigUpdater utility.  uses the old dual-file-format support
 *  from vjcontrol to load & save a file, converting it to the XML format
 *  along the way.<br>
 *  Arguments to this utility are either file or directory names.  If a 
 *  directory, it will (recursively) consider all files therein.<br>
 *  Rules for figuring out what a file is:
 *  If the name ends in .config or .cfg it's treated as a jackal config
 *  file.
 *  If the name ends in .desc or .dsc it's treated as a chunkdesc file.
 *  If it ends in anything else, it's ignored.
 */
public class Main {

    public static void main (String[] args) {
        if (args.length == 0 ||
            args[0].equalsIgnoreCase ("-h") ||
            args[0].equalsIgnoreCase ("-help") ||
            args[0].equalsIgnoreCase ("--help")) {
            printHelpText();
            return;
        }

        ChunkDescDB global_descdb = new ChunkDescDB();
        ChunkFactory.setChunkDescDB (global_descdb);


        // load the chunkdesc file that we use for processing things...
	URL descurl = ClassLoader.getSystemResource ("ConfigUpdater.desc");
        ConfigIOStatus iostatus;
        try {
            iostatus = ConfigIO.readChunkDescDB (descurl.openStream(), global_descdb, ConfigIO.GUESS);
            if (iostatus.getStatus() == iostatus.FAILURE) {
                System.out.println ("Couldn't load " + descurl
                                    + "- jar archive may be corrupt");
                System.exit(1);
            }
        }
        catch (IOException e) {
                System.out.println ("Couldn't load " + descurl
                                    + "- jar archive may be corrupt");
                System.exit(1);
        }


        for (int i = 0; i < args.length; i++) {
            File f = new File (args[i]);
            if (f.isDirectory())
                processDirectory (f);
            else 
                processFile (f);
        }
    }

    protected static void printHelpText () {
        System.out.println ("Usage: ConfigUpdater [args]");
        System.out.println ("  args can be:");
        System.out.println ("    config files (ending in .config or .cfg)");
        System.out.println ("    chunkdesc files (ending in .desc or .dsc)");
        System.out.println ("    directory names (recursively search dirs");
        System.out.println ("                     looking for files)");
    }

    protected static void processDirectory (File dir) {
        System.out.print ("Processing directory '" + dir);
        File[] files = dir.listFiles();
        if (files == null) {
            System.out.println ("' - Couldn't get file listing!");
            return;
        }
        System.out.println ("' - " + files.length + " files.");

        // it'd be nice to alphabetize the listing here.

        for (int i = 0; i < files.length; i++) {
            if (files[i].isDirectory())
                processDirectory (files[i]);
            else
                processFile (files[i]);
        }
    }

    protected static void processFile (File f) {
        System.out.print ("Processing file '" + f + "'");
        if (!f.exists()) {
            System.out.println (" (doesn't exist (!))");
            return;
        }
        String fname = f.toString().toLowerCase();
        if (fname.endsWith (".config") || fname.endsWith (".cfg")) {
            System.out.println (" (VR Juggler configuration file)");
            processConfigFile (f);
        }
        else if (fname.endsWith (".desc") || fname.endsWith (".dsc")) {
            System.out.println (" (VR Juggler chunkdescs file)");
            processDescFile (f);
        }
        else {
            System.out.println (" (unidentified - ignoring)");
        }
    }

    protected static int processDescFile (File original_name) {
        File backup_name = new File (original_name.toString() + ".bak");
        ChunkDescDB db = new ChunkDescDB();
        ConfigIOStatus iostatus;

        iostatus = ConfigIO.readChunkDescDB (original_name, db, ConfigIO.GUESS);
        // write out errors to UI
        for (int i = 0; i < iostatus.size(); i++)
            System.out.println (iostatus.get(i).toString());
        if (iostatus.getStatus() >= iostatus.ERRORS) {
            System.out.println (iostatus.getSummary());
            System.out.println ("Skipping file...");
            return 1;
        }

        if (!original_name.renameTo (backup_name)) {
            System.out.println ("Error: Failed to move file (!)");
            return 1;
        }

        try {
            ConfigIO.writeChunkDescDB (original_name, db, ConfigIO.XML);
            return 0;
        }
        catch (IOException e) {
            System.out.println ("Error: write failed: " + e.toString());
            return 1;
        }
    }

    protected static int processConfigFile (File original_name) {
        File backup_name = new File (original_name.toString() + ".bak");
        if (!original_name.renameTo (backup_name)) {
            System.out.println ("Error: Failed to move file (!)");
            return 1;
        }
        ConfigChunkDB db = new ConfigChunkDB();
        ConfigIOStatus iostatus;

        iostatus = ConfigIO.readConfigChunkDB (backup_name, db, ConfigIO.GUESS);
        // write out errors to UI
        for (int i = 0; i < iostatus.size(); i++)
            System.out.println (iostatus.get(i).toString());
        if (iostatus.getStatus() >= iostatus.ERRORS) {
            System.out.println (iostatus.getSummary());
            System.out.println ("Skipping file...");
            return 1;
        }

        // do some processing on the file
        updateChunkDB (db);

        try {
            ConfigIO.writeConfigChunkDB (original_name, db, ConfigIO.XML);
            return 0;
        }
        catch (IOException e) {
            System.out.println ("Error: write failed: " + e.toString());
            return 1;
        }
    }


    /** Looks for chunks to be modified/replaced.  There's a lot of
     *  pretty specific info in this.
     */
    protected static void updateChunkDB (ConfigChunkDB db) {
        List chunks;
        ConfigChunk ch1, ch2, ch3;
        Property p;
        int i;
        int val;
        VarValue v;

        // simdisplays need to be replaced w/ displaywindows...
        chunks = db.getOfDescToken ("simDisplay");
        for (i = 0; i < chunks.size(); i++) {
            ch1 = (ConfigChunk)chunks.get(i);
            db.remove (ch1.getName());

            ch2 = ChunkFactory.createChunkWithDescToken ("displayWindow");
            ch2.setName (ch1.getName());

            p = ch2.getPropertyFromToken ("origin");
            p.setValue (ch1.getValueFromToken ("origin", 0), 0);
            p.setValue (ch1.getValueFromToken ("origin", 1), 1);

            p = ch2.getPropertyFromToken ("size");
            p.setValue (ch1.getValueFromToken ("size", 0), 0);
            p.setValue (ch1.getValueFromToken ("size", 1), 1);

            p = ch2.getPropertyFromToken ("pipe");
            p.setValue (ch1.getValueFromToken ("pipe", 0), 0);

            // 'view' prop has value 3 for stereo...
            p = ch2.getPropertyFromToken ("stereo");
            p.setValue (ch1.getValueFromToken ("view", 0).getInt() == 3, 0 );

            p = ch2.getPropertyFromToken ("border");
            p.setValue (ch1.getValueFromToken ("border",0), 0);

            p = ch2.getPropertyFromToken ("active");
            p.setValue (ch1.getValueFromToken ("active",0), 0);

            // and then create a single sim viewport.  yeesh.
            ch3 = ChunkFactory.createChunkWithDescToken ("simViewport");
            ch3.setName (ch2.getName() + " viewport");

            v = new VarValue (ValType.FLOAT);

            p = ch3.getPropertyFromToken ("origin");
            v.set (0.0f);
            p.setValue (v, 0);
            p.setValue (v, 1);

            p = ch3.getPropertyFromToken ("size");
            v.set (1.0f);
            p.setValue (v, 0);
            p.setValue (v, 1);

            p = ch3.getPropertyFromToken ("view");
            p.setValue (ch1.getValueFromToken ("view",0), 0);

            p = ch3.getPropertyFromToken ("cameraPos");
            p.setValue (ch1.getValueFromToken ("cameraPos",0), 0);

            p = ch3.getPropertyFromToken ("user");
            p.setValue (ch1.getValueFromToken ("user",0), 0);

            p = ch3.getPropertyFromToken ("wandPos");
            p.setValue (ch1.getValueFromToken ("wandPos",0), 0);

            p = ch3.getPropertyFromToken ("active");
            p.setValue (ch1.getValueFromToken ("active",0), 0);

            p = ch2.getPropertyFromToken ("sim_viewports");
            v = new VarValue (ValType.EMBEDDEDCHUNK);
            v.set (ch3);
            p.setValue (v, 0);

            db.add (ch2);
        }


        // surfacedisplays need to be replaced w/ displaywindows...
        chunks = db.getOfDescToken ("surfaceDisplay");
        for (i = 0; i < chunks.size(); i++) {
            ch1 = (ConfigChunk)chunks.get(i);
            db.remove (ch1.getName());

            ch2 = ChunkFactory.createChunkWithDescToken ("displayWindow");
            ch2.setName (ch1.getName());

            p = ch2.getPropertyFromToken ("origin");
            p.setValue (ch1.getValueFromToken ("origin", 0), 0);
            p.setValue (ch1.getValueFromToken ("origin", 1), 1);

            p = ch2.getPropertyFromToken ("size");
            p.setValue (ch1.getValueFromToken ("size", 0), 0);
            p.setValue (ch1.getValueFromToken ("size", 1), 1);

            p = ch2.getPropertyFromToken ("pipe");
            p.setValue (ch1.getValueFromToken ("pipe", 0), 0);

            // 'view' prop has value 3 for stereo...
            p = ch2.getPropertyFromToken ("stereo");
            p.setValue (ch1.getValueFromToken ("view", 0).getInt() == 3, 0 );

            p = ch2.getPropertyFromToken ("border");
            p.setValue (ch1.getValueFromToken ("border",0), 0);

            p = ch2.getPropertyFromToken ("active");
            p.setValue (ch1.getValueFromToken ("active",0), 0);

            // and then create a single sim viewport.  yeesh.
            ch3 = ChunkFactory.createChunkWithDescToken ("surfaceViewport");
            ch3.setName (ch2.getName() + " viewport");

            v = new VarValue (ValType.FLOAT);

            p = ch3.getPropertyFromToken ("origin");
            v.set (0.0f);
            p.setValue (v, 0);
            p.setValue (v, 1);

            p = ch3.getPropertyFromToken ("size");
            v.set (1.0f);
            p.setValue (v, 0);
            p.setValue (v, 1);

            p = ch3.getPropertyFromToken ("view");
            p.setValue (ch1.getValueFromToken ("view",0), 0);

            p = ch3.getPropertyFromToken ("corners");
            p.setValue (ch1.getValueFromToken ("corners",0), 0);
            p.setValue (ch1.getValueFromToken ("corners",1), 1);
            p.setValue (ch1.getValueFromToken ("corners",2), 2);
            p.setValue (ch1.getValueFromToken ("corners",3), 3);

            p = ch3.getPropertyFromToken ("user");
            p.setValue (ch1.getValueFromToken ("user",0), 0);

            p = ch3.getPropertyFromToken ("active");
            p.setValue (ch1.getValueFromToken ("active",0), 0);

            p = ch3.getPropertyFromToken ("tracked");
            p.setValue (ch1.getValueFromToken ("tracked",0), 0);

            p = ch3.getPropertyFromToken ("trackerproxy");
            p.setValue (ch1.getValueFromToken ("trackerproxy",0), 0);


            p = ch2.getPropertyFromToken ("surface_viewports");
            v = new VarValue (ValType.EMBEDDEDCHUNK);
            v.set (ch3);
            p.setValue (v, 0);

            db.add (ch2);
        }





    }

}
