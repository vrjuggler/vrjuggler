
package ConfigUpdater;

import VjConfig.*;
import java.io.*;
import java.net.URL;

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


    protected static void updateChunkDB (ConfigChunkDB db) {
    }

}
