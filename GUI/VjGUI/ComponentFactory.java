

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


