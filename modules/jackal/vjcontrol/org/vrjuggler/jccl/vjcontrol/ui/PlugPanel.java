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

import javax.swing.ImageIcon;
import javax.swing.JComponent;

import VjControl.VjComponent;


/** Interface for PlugPanel components.
 *  PlugPanels are GUI extensions that can be loaded into ControlUI's
 *  tabbed pane (or whatever interface replaces said tabbed pane).
 *  <p>
 *  PlugPanels can inherit from JComponent, or they can contain 
 *  a JComponent that is returned via getUIComponent().
 *
 *  @author Christopher Just
 *  @version $Revision$
 */
public interface PlugPanel
    extends VjComponent {


    /** Returns the UI component associated with this panel.
     *  Many PlugPanels simply inherit from JComponent and return
     *  themselves.
     *  Note that this method may be called before initUIComponent,
     *  for example when initializing a JTabbedPane as a container
     *  of plugpanels.  Simply return the empty panel; 
     *  initUIComponent will be called when the panel is actually
     *  displayed to the user.
     */
    public JComponent getUIComponent();


    /** Initializes the PlugPanel's graphical elements.
     *  This function is called immediately before the panel is displayed
     *  to the user.  Creation and layout of a PlugPanel's graphical
     *  elements should be placed in this function instead of the
     *  constructor or configure methods, to ensure the fastest
     *  possible VjControl startup time.
     *  Note that initUIComponent may be called repeatedly for a single
     *  PlugPanel; be prepared to ignore redundant calls.
     */
    public boolean initUIComponent();


    /** Rebuild display in case of emergency.
     *  This function provides a workaround for various graphical 
     *  glitches that infest certain JDK implementations. ControlUI
     *  can call this method on its various panels to force a refresh.
     *
     *  @deprecated 
     */
    public void rebuildDisplay ();


    /** An icon to use to represent an instance of this component.
     *  May return null.
     */
    public ImageIcon getComponentIcon ();

}

