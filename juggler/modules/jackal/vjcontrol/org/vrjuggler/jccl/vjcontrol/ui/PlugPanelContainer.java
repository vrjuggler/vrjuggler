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


package VjComponents.UI;

import VjControl.VjComponent;


/** A component that attaches to ControlUI Modules and displays PlugPanels.
 *  There's nothing more to it than that.  This object doesn't even own the
 *  PlugPanels - ControlUI Module does.  we just provide a component that
 *  displays them, either thru a tabbed pane, a menu system, or some other 
 *  means.
 *  <p>
 *  In addition to implementing the PlugPanelContainer interface, any 
 *  object fulfilling this role must inherit from javax.swing.JComponent 
 *  (directly or indirectly).
 */
public interface PlugPanelContainer
    extends VjComponent {

    public void insertPanel (PlugPanel p);

    public void removePanel (PlugPanel p);

    public void rebuildDisplays ();

}
