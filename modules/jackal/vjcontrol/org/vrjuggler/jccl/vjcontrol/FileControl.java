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

import java.io.*;
import java.util.*;


/** File utility functions.
 *  Utility functions for managing file names, doing environment variable
 *  replacement, and loading VjControl extension modules.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class FileControl {

    Map env_vars; 

    public FileControl () {
        env_vars = Collections.synchronizedMap(new HashMap());
        String homedir = System.getProperty ("user.home");
        if (homedir == null)
            homedir = ".";
        env_vars.put ("HOME", homedir);

    }


    String getEnvVar (String name) {
        String val = (String)env_vars.get (name);
        if (val == null) {
            val = System.getProperty (name);
            if (val == null)
                val = "";
            env_vars.put (name, val);
        }
        return val;
    }


    /** Performs substitution on file name strings.
     *  This tries to do environment variable replacement.  
     *  Variables should be of the form ${NAME} or $NAME (the former
     *  is preferred).  Note that it is difficult to access arbitrary
     *  environment variables in java without using deprecated
     *  methods, but the vjcontrol startup script tries to guarantee
     *  that at least ${HOME} and ${VJ_BASE_DIR} and ${JCCL_BASE_DIR}
     *  will work.
     */
    public String mangleFileName (String s) {

	if (s == null)
	    return "";

        if (s.startsWith ("${")) {
            int i = s.indexOf ("}");
            if (i != -1) {
                String var_name = s.substring (2, i);
                s = getEnvVar (var_name) + s.substring (i+1);
            }
        }
        else if (s.startsWith ("$")) {
            // variable of form $VARNAME/whatever
            int i = s.indexOf (File.separatorChar);
            if (i != -1) {
                String var_name = s.substring (1, i);
                s = getEnvVar (var_name) + s.substring (i);
            }
            else
                s = getEnvVar (s.substring(1));
        }

	return s;
    }



    public String[] getFileNames (String dir, String suffix) {

        dir = mangleFileName (dir);
        File d = new File(dir);
        if (d.isDirectory()) {
            return (d.list (new SuffixFilter ("jar", suffix)));
        }
        else if (dir.endsWith (suffix)) {
            String[] retval = new String[1];
            retval[0] = dir;
            return retval;
        }
        return null;
    }



    /********************* Extension Loader Methods *********************/

    public void loadExtensions (File f) {
        if (f == null)
            return;
        String ext_dir = f.getAbsolutePath();
        String[] ext_jars = getFileNames (ext_dir, "jar");
        
        if (ext_jars != null) {
            for (int j = 0; j < ext_jars.length; j++) {
                Core.component_factory.registerJarURL ("file:" + ext_jars[j]);
            }
        }
    }


}

