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

import javax.swing.ImageIcon;
import java.util.HashMap;
import java.net.URL;


/** Supplies ImageIcons for use in VjControl GUI components.
 *  This provides a common source for image icons, that keeps components
 *  from loading icon files multiple times etc.
 *  <p>
 *  Icons are available in 2 sizes, small and large (0 and 1 for the size
 *  argument).
 *
 *  @author Christopher Just
 *  @version $Revision$
 */
public class IconFactory {


    protected class MapEntry {
        public ImageIcon[] icons;

        public MapEntry() {
            icons = new ImageIcon[2];
        }
    }


    protected HashMap map;


    public IconFactory () {
        map = new HashMap();
    }


    public ImageIcon getIcon (String icon_name, int size) {
        MapEntry me = (MapEntry)map.get (icon_name);
        if (me != null)
            return me.icons[size];
        else
            return null;
    }


    public void registerIcon (String icon_name, int size,
                              URL icon_file_url) {
        ImageIcon icon = new ImageIcon (icon_file_url);
        if (icon == null)
            return;
        MapEntry me = (MapEntry)map.get (icon_name);
        if (me != null) {
            me.icons[size] = icon;
        }
        else {
            me = new MapEntry();
            me.icons[size] = icon;
            map.put (icon_name, me);
        }
    }

}
