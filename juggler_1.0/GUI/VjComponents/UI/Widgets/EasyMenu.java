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
import java.util.Vector;

public class EasyMenu {

    protected class EasyMenuItem {
        public String name;
        public JMenuItem menu_item;
        public EasyMenu submenu;   // menu_item or submenu will always be null.

        public EasyMenuItem (String _name, JMenuItem _menu_item) {
            name = _name;
            menu_item = _menu_item;
            submenu = null;
        }

        public EasyMenuItem (String _name, EasyMenu _submenu) {
            name = _name;
            menu_item = null;
            submenu = _submenu;
        }

    }
        


    JMenu menu;
    String name;      // used for quick searching
    Vector items;     // vector of EasyMenuItems
    int max_elems;


    public EasyMenu (String _name, int _max_elems) {
        menu = new JMenu (_name);
        name = _name;
        max_elems = _max_elems;
        items = new Vector();
    }


    public JMenu getJMenu () {
        return menu;
    }


    public String getName () {
        return name;
    }


    public EasyMenuItem getMenuItem (String item_name) {
        EasyMenuItem mi = null;

        for (int i = 0; i < items.size(); i++) {
            mi = (EasyMenuItem)items.elementAt(i);
            if (mi.name.equals(item_name))
                return mi;
        }
        return null;
    }


    public JMenuItem addMenuItem (String path) {
        EasyMenuItem mi;

        // if it won't fit expand the menu and descend
        if (items.size() >= max_elems) {
            mi = getMenuItem ("more...");
            if (mi == null) {
                mi = new EasyMenuItem ("more...", new EasyMenu ("more...", max_elems));
                menu.add (mi.submenu.getJMenu());
                items.addElement (mi);
            }
            if (mi.submenu == null) {
                System.err.println ("ERROR: tried to add a submenu to a menuitem");
                return null;
            }
            return mi.submenu.addMenuItem (path);
        }

        // otherwise we'll handle it here...
        int i = path.indexOf ("/");
        if (i == -1) {
            // it's a leaf
            mi = new EasyMenuItem (path, new JMenuItem (path));
            menu.add (mi.menu_item);
            items.addElement (mi);
            return mi.menu_item;
        }
        else {
            // it's on a submenu
            String submenu_name = path.substring (0, i);
            path = path.substring (i+1);
            mi = getMenuItem (submenu_name);
            if (mi == null) {
                mi = new EasyMenuItem (submenu_name, new EasyMenu (submenu_name, max_elems));
                menu.add (mi.submenu.getJMenu());
                items.addElement (mi);
            }
            if (mi.submenu == null) {
                System.out.println ("error adding '" + path + "': path contains a menuitem!");
            }
            return mi.submenu.addMenuItem (path);
        }
    }


    public void removeMenuItem (String path) {
        System.err.println ("Not implemented!!!");
    }


}
