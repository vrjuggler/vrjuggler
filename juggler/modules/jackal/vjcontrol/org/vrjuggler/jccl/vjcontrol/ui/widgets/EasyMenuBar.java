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


package VjComponents.UI.Widgets;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JMenuBar;
import java.util.Vector;


public class EasyMenuBar {

    Vector menus;
    JMenuBar menubar;
    int max_elems;

    public EasyMenuBar () {
        menus = new Vector();
        menubar = new JMenuBar();
        max_elems = 20;
    }

    public JMenuBar getJMenuBar () {
        return menubar;
    }


    private EasyMenu getEasyMenu (String item_name) {
        EasyMenu mi = null;

        for (int i = 0; i < menus.size(); i++) {
            mi = (EasyMenu)menus.elementAt(i);
            if (mi.getName().equals(item_name))
                return mi;
        }
        return null;
    }

    public JMenuItem addMenuItem (String path) {
        String submenu_name;
        int i = path.indexOf ("/");
        if (i == -1) {
            submenu_name = path;
            path = null;
        }
        else {
            submenu_name = path.substring (0, i);
            path = path.substring (i+1);
        }
        EasyMenu mi = getEasyMenu (submenu_name);
        if (mi == null) {
            mi = new EasyMenu (submenu_name, max_elems);
            menubar.add (mi.getJMenu());
            menus.addElement (mi);
        }
        if (path == null)
            return null;
        else
            return mi.addMenuItem (path);
    }

    public void removeMenuItem (String path) {
        System.err.println ("EasyMenuBar.removeMenuItem not implemented!");
    }

}

