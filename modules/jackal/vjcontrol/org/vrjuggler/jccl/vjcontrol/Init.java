/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
package org.vrjuggler.jccl.vjcontrol;

import java.io.*;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.vjcontrol.*;

/** Main executable for VjControl.
 *  This class includes main() for VjControl, and is responsible for initial
 *  startup, configuration, and loading initial components .jars.
 *
 *  @version $Revision$
 */
public class Init {


    /** Main() routine for VjControl. */
    public static void main (String args[]) {

        Core.initInstance();
        Core.setCommandLine (args);

        /* do config stuff... */
        loadVjControlConfig();
        configurationSanityCheck();

        /* load extensions... */
        ArrayList extension_dirs = new ArrayList();
        //extension_dirs.add ("$JCCL_BASE_DIR/bin/VjControl.jar");

        /* we'll look thru the command line for extension .jars */
        for (int i = 0; i < args.length; i++) {
            if (args[i].equalsIgnoreCase ("-j")) {
                extension_dirs.add (args[++i]);
            }
            else if (args[i].endsWith (".jar") || args[i].endsWith(".JAR")) {
                extension_dirs.add (args[i]);
            }
        }

        Core.component_factory.registerBaseJar();

        loadExtensions (extension_dirs);

        Core.consoleInfoMessage("VjControl 1.1",
                                "VR Juggler Control and Configuration Program");
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
            ch = ChunkFactory.createChunkWithDescToken("vjcontrol");
            ch.setName ("VjControl global preferences");
            ch.setProperty(VjComponentTokens.FONT_NAME, 0, new VarValue("Courier"));
            ch.setProperty(VjComponentTokens.FONT_SIZE, 0, new VarValue(12));
            ch.setProperty(VjComponentTokens.LOOKNFEEL, 0, new VarValue("Java"));
            ch.setProperty(VjComponentTokens.HOST, 0, new VarValue("localhost"));
            ch.setProperty(VjComponentTokens.PORT, 0, new VarValue (4450));
            Core.vjcontrol_chunkdb.add(ch);
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
            Core.descdb.build(descurl.openStream());
        }
        catch (IOException e) {
            // from descurl.openStream()...
            Core.consoleErrorMessage ("Init", "Couldn't load " + descurl
                                      + "- vjcontrol.jar may be corrupt");
            System.exit(1);
        }


        f1 = new File (Core.file.mangleFileName ("$HOME/.vjconfig/vjcontrol.cfg"));
        Core.vjcontrol_chunkdb.setName(f1.getName());

        try
        {
            Core.vjcontrol_chunkdb.build(f1);
        }
        catch (IOException io_ex)
        {
            try
            {
                f2 = new File(Core.file.mangleFileName("$VJ_SHARE_DIR/data/vjcontrol.cfg"));
                Core.vjcontrol_chunkdb.build(f2);
            }
            catch (IOException io_ex2)
            {
                 Core.consoleErrorMessage("Init",
                                          "Couldn't load VjControl prefs '" +
                                          f1 + "' or '" + f2 + "'.");
            }
        }
    }
}
