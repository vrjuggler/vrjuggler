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



package VjComponents.ConfigEditor;

import java.awt.event.*;
import java.io.*;
import java.util.*;
import VjConfig.ConfigStreamTokenizer;
import VjConfig.ConfigChunk;
import VjConfig.VarValue;
import VjControl.DefaultCoreModule;
import VjComponents.ConfigEditor.ChunkOrgTree;
import VjControl.FileControl;
import VjControl.Core;


/** Core Module for ChunkOrgTrees
 *  This module provides ChunkOrgTrees, which several UI components use 
 *  for displaying ConfigChunks in a categorized tree.
 *  <p>
 *  This module is separate from the ConfigModule because only certain
 *  UI components need it.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class ChunkOrgTreeModule extends DefaultCoreModule {

    protected ChunkOrgTree active_tree;


    public ChunkOrgTreeModule () {
        super();
        component_name = "Unconfigured ChunkOrgTreeModule";
        active_tree = new ChunkOrgTree();
    }


//      public String getComponentName () {
//          return component_name;
//      }


    public boolean configure (ConfigChunk ch) {
        int i;
        boolean autoload = true;
        String orgtreename = null;

        component_name = ch.getName();

        // check command-line arguments stored in Core
        String[] args = Core.getCommandLine();
	for (i = 0; i < args.length; i++) {
	    if (args[i].startsWith ("-o")) {
                if (args[i].length() == 2)
                    orgtreename = args[++i];
                else
                    orgtreename = args[i].substring(2);
	    }
	    else if (args[i].equalsIgnoreCase ("-noautoload")) {
		autoload = false;
	    }
        }

        // if autoloading is ok, and we haven't been superceded by the
        // command line anyway, check the global prefs...
        if (autoload && (orgtreename == null)) {
            List v = Core.vjcontrol_chunkdb.getOfDescToken ("vjcontrol");
            for (i = 0; i < v.size(); i++) {
                ConfigChunk chunk = (ConfigChunk)v.get(i);
                VarValue val = chunk.getValueFromToken ("orgtreefile", 0);
                if (val != null)
                    orgtreename = val.getString();
            }
        }

	if (orgtreename == null && autoload) {
	    loadMainChunkOrgTree();
	}
	else if (orgtreename != null) {
            loadChunkOrgTree (active_tree, orgtreename);
	}

        return true;
    }



    public ChunkOrgTree getOrgTree () {
        return active_tree;
    }

    public void setOrgTree (ChunkOrgTree ot) {
        active_tree = ot;
    }


    /************************* OrgTree Methods **************************/



    public void loadMainChunkOrgTree () {
	FileReader r;
	ConfigStreamTokenizer st;
	File f1;
	String homedir;

        try {
            f1 = new File (Core.file.mangleFileName ("$HOME/.vjconfig/orgtree.org"));
            r = new FileReader (f1);
        }
        catch (FileNotFoundException e) {
            try {
                f1 = new File (Core.file.mangleFileName ("$VJ_SHARE_DIR/Data/orgtree.org"));
                r = new FileReader(f1);
            }
            catch (FileNotFoundException e2) {
                Core.consoleErrorMessage (component_name, "Couldn't find default orgtree $VJ_SHARE_DIR/Data/orgtree.org");
                return;
            }
        }
            
        Core.consoleInfoMessage (component_name, "Loading organization tree: " + f1);
        st = new ConfigStreamTokenizer (r);
        active_tree.setName (f1.getName());
        active_tree.setFile (f1);
        active_tree.read(st);
        //--- BUG ---
        //Core.rebuildAllTrees();
    }


    public boolean loadChunkOrgTree (ChunkOrgTree orgtree, String name) {
        name = Core.file.mangleFileName (name);
        File f = new File (name);
        return loadChunkOrgTree (orgtree, f);
    }


    public boolean loadChunkOrgTree (ChunkOrgTree orgtree, File f) {
	FileReader r;
	ConfigStreamTokenizer st;
            
        try {
            r = new FileReader (f);
            Core.consoleInfoMessage (component_name, "Loading organization tree: " + f);
                st = new ConfigStreamTokenizer (r);
                orgtree.setName (f.getName());
                orgtree.setFile (f);
                orgtree.read(st);
                //Core.rebuildAllTrees();
                return true;
        }
        catch (FileNotFoundException e) {
            Core.consoleErrorMessage (component_name, "File not found: " + f);
            return false;
        }
    }



    public String saveChunkOrgTree (ChunkOrgTree ot, File f) {
	if (f == null)
	    return "";

	try {
	    DataOutputStream out = new DataOutputStream(new FileOutputStream(f));
	    out.writeBytes(ot.toString());
	    Core.consoleInfoMessage (component_name, "Saved ChunkOrgTree file: " + f);

	    /* do some fixing up if the name changed */
	    ot.setFile (f);
	    ot.setName (f.getName());
	    return f.getName();
	}
	catch (IOException e) {
	    Core.consoleErrorMessage (component_name, "IOerror saving file " + f);
	    return "";
	}
    }


}
