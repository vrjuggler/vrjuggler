/*
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
 */

package VjGUI;

import VjGUI.util.*;
import java.util.Vector;
import java.net.URL;
import java.net.URLClassLoader;

public class ComponentFactory {

    public class CEntry {
        public String class_name;
        public Class class_obj;

        public CEntry (String _class_name, Class _class) {
            class_name = _class_name;
            class_obj = _class;
        }
    }

    public Vector registered_classes;
    //public URLClassLoader urlcl;

    public ComponentFactory () {
        registered_classes = new Vector();
        //urlcl = new URLClassLoader (null);
    }

    public void registerPanel (String class_name, String jar_file_url) {
        ClassLoader cl;
        Class c = null;

        try {
            if (jar_file_url != null && !jar_file_url.equals ("")) {
                //urlcl.addURL (new URL(jar_file_url));
                URL[] u = new URL[1];
                u[0] = new URL(jar_file_url);
                cl = new URLClassLoader (u);
            }
            else
                cl = ClassLoader.getSystemClassLoader();
            c = cl.loadClass (class_name);
        }
        catch (Exception e) {
            ;
        }
        if (c != null)
            registered_classes.addElement (new CEntry (class_name, c));
    }

    public PlugPanel createPlugPanel (String class_name) {
        PlugPanel p = null;
        try {
            Class c = ClassLoader.getSystemClassLoader().loadClass(class_name);
            p = (PlugPanel)c.newInstance();
        }
        catch (Exception e) {
            System.out.println ("error in ComponentFactory.createPlugPanel");
        }
        return p;
    }

}


