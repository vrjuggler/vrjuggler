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

import VjConfig.ConfigChunk;

/** Base interface for all VjComponents.
 *  VjComponents are dynamically-loadable modules which can be added to
 *  VjControl.  They can provide extensions, such as new databases or
 *  communications methods, or even new GUI components.
 *  <p>
 *  The main vjcontrol classes (ie those in vjcontrol.jar) understand
 *  a certain set of interfaces, all defined in the VjGUI.interfaces
 *  package.  In addition, extension components can declare their
 *  own interfaces for extensions that they support (but this runs
 *  into classloader problems in the current implementation; see
 *  the VjGUI.ComponentFactory for details).
 *  <p>
 *  In addition to implementing VjComponent (or a child thereof), the
 *  extension component must have a constructor that takes no arguments.
 *  The individual interfaces may declare additional requirements.  For
 *  example, several of the GUI components require anything implementing
 *  their interfaces to also inherit from javax.swing.JComponent.
 *
 *  @see VjControl.ComponentFactory
 *  @author Christopher Just
 *  @version $Revision$
 */
public interface VjComponent {

    /** Returns the unique name of an instance of this component. */
    public String getComponentName ();


    /** Configures the component.
     *  The ConfigChunk will usually contain information like the 
     *  instance's proper name, and names of components that it will
     *  need to access.  Generally, once a component has been configured
     *  it's live - for example, configuring the ControlUIModule causes
     *  its window to be displayed; configuring the NetworkModule causes
     *  it to start accepting connections, etc.
     */
    public boolean configure (ConfigChunk ch);


    /** Returns the ConfigChunk used to configure this component. */
    public ConfigChunk getConfiguration ();


    /** add a component that expects to be attached to this chunk.
     *  @returns True if the component was succesfully created and added.
     */
    public boolean addConfig (ConfigChunk ch);


    /** Remove a component that expects to be attached to this chunk.
     *  @param name Name of the component to remove (should be the
     *              same as the name of the ConfigChunk used to create
     *              it).
     *  @returns True if the component was found and removed.  False
     *           indicates that the removeConfig couldn't find any
     *           such component to remove.
     */
    public boolean removeConfig (String name);


    /** Called when a component is about to be removed.
     *  Whenever vjcontrol is about to remove a component, it calls
     *  this method, which allows the component to remove references
     *  to itself (mainly, to stop listening for events).
     *  <p>
     *  In general, VjControl's component configuration system will
     *  remove supcomponents first; a recursive destroy like this is
     *  usually only triggered when someone exits the application.
     */
    public void destroy ();


}
