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


package VjControl;

import java.util.List;
import java.util.ArrayList;
import java.io.*;
import java.net.URL;
import VjControl.*;
import VjConfig.*;


/** Main executable for VjControl.
 *  This class includes main() for VjControl, and is responsible for initial
 *  startup, configuration, and loading initial components .jars.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class Init {


    /** Main() routine for VjControl. */
    public static void main (String args[]) {

	Core.initialize();
        Core.setCommandLine (args);

	/* do config stuff... 	 */
	loadVjControlConfig();
	configurationSanityCheck();

        /* load extensions... */
        ArrayList extension_dirs = new ArrayList();
        extension_dirs.add ("$VJ_BASE_DIR/bin/VjControl.jar");

        /* we'll look thru the command line for extension .jars */
 	for (int i = 0; i < args.length; i++) {
            if (args[i].equalsIgnoreCase ("-j")) {
                extension_dirs.add (args[++i]);
            }
        }

        loadExtensions (extension_dirs);
     	
	Core.consoleInfoMessage ("VjControl 1.0", "VR Juggler Control and Configuration Program");
	
    }



    /** Performs minimal sanity checking on the VjControl config.
     *  This method ensures that we have a vjcontrol config ConfigChunk
     *  and that it has at least basic values.
     *  Most of this function will actually go away once we have default
     *  values in ChunkDescs.
     */
    protected static void configurationSanityCheck() {
	ConfigChunk ch;
	List v = Core.vjcontrol_chunkdb.getOfDescToken ("vjcontrol");
	if (v.size() == 0) {
	    System.err.println ("didn't get chunk");
	    ch = ChunkFactory.createChunkWithDescToken ("vjcontrol");
            ch.setName ("VjControl global preferences");
	    ch.setPropertyFromToken ("fontname", new VarValue ("Courier"), 0);
	    ch.setPropertyFromToken ("fontsize", new VarValue (12), 0);
	    ch.setPropertyFromToken ("looknfeel", new VarValue ("Java"), 0);
	    ch.setPropertyFromToken ("name", new VarValue ("VjControl Config"), 0);
	    ch.setPropertyFromToken ("host", new VarValue ("localhost"), 0);
	    ch.setPropertyFromToken ("port", new VarValue (4450), 0);
	    Core.vjcontrol_chunkdb.add (ch);
	}
    }



    /** Loads an initial set of VjControl extensions listed on command line.
     *  This also ensures that contents.config for VjControl.jar itself
     *  is parsed.
     */
    protected static void loadExtensions (ArrayList extension_dirs) {
        // loads whatever .jar file extensions it can find

        int n = extension_dirs.size();
        for (int i = 0; i < n; i++) {
            String ext_dir = (String)extension_dirs.get(i);
            String[] ext_jars = Core.file.getFileNames (ext_dir, "jar");
            
            if (ext_jars != null) {
                for (int j = 0; j < ext_jars.length; j++) {
                    Core.component_factory.registerJarURL ("file:" + ext_jars[j]);
                }
            }
        }
    }



    public static void loadVjControlConfig() {
	ConfigStreamTokenizer st;
	File f1 = null;
        File f2 = null;
	FileReader r;
	InputStreamReader inr;
	String homedir;

	// 1st - load the chunkdesc into memory...

	URL descurl = ClassLoader.getSystemResource ("VjFiles/vjcontrol.dsc");
	try {
            ConfigIO.readChunkDescDB (descurl.openStream(), Core.descdb, ConfigIO.GUESS);
	}
	catch (IOException e2) {
            Core.consoleErrorMessage (e2.toString());
	    Core.consoleErrorMessage ("Init", "Couldn't load " + descurl
				      + "- vjcontrol.jar may be corrupt");
	}
		
        f1 = new File (Core.file.mangleFileName ("$HOME/.vjconfig/vjcontrol.cfg"));
        Core.vjcontrol_chunkdb.setName (f1.getName());
        Core.vjcontrol_chunkdb.setFile (f1);

	try {
            ConfigIO.readConfigChunkDB (f1, Core.vjcontrol_chunkdb, ConfigIO.GUESS);
	}
	catch (FileNotFoundException e2) {
	    try {
		f2 = new File (Core.file.mangleFileName ("$VJ_SHARE_DIR/Data/vjcontrol.cfg"));
                ConfigIO.readConfigChunkDB (f2, Core.vjcontrol_chunkdb, ConfigIO.GUESS);
	    }
	    catch (FileNotFoundException e3) {
		Core.consoleErrorMessage ("Init","Couldn't load VjControl prefs '" + f1 + "' or '" + f2 + "'.");
		return;
	    }
            catch (IOException e4) {
                Core.consoleErrorMessage ("Init", e4.toString());
            }
	}
        catch (IOException e5) {
            Core.consoleErrorMessage ("Init", e5.toString());
        }
    }


}
 


