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
 *  The core VjControl classes understand a small set of interfaces -
 *  VjComponent, CoreModule, etc.
 *  In addition, extension components can declare their
 *  own interfaces for extensions that they support.  For example,
 *  the VjComponents.UI package includes the ControlUIModule (the
 *  main VjControl GUI window) as well as the PlugPanel interface,
 *  which defines components that can be added to the UI window.
 *  <p>
 *  VjComponents are JavaBeans; this means that in addition to
 *  implementing a VjComponent interface, they must have a 0-argument
 *  constructor (JavaBeans should support serialization, tho VjControl
 *  does not require this).
 *  <p>
 *  Every component instance has a unique string name, and a parent
 *  component.  The parent is responsible for instantiating and 
 *  destroying its children.
 *  <p>
 *  VjControl uses Jackal-style configuration files to determine
 *  which instances of components to create.  The ConfigChunk for a
 *  component instance specifies its name, parent, and any other
 *  components it needs to perform its duties.
 *  <p>
 *  Note that, while components are usually created when VjControl
 *  processes their ConfigChunks, this is not always the case.
 *  The parent component may instead create its children on demand.
 *  The primary example of this behavior is with ConfigChunkPanel
 *  components.  These are created on-demand by the ConfigUIHelper
 *  module whenever the user wants to edit a ConfigChunk.
 *  <p>
 *  Creation of a VjComponent is a three-step process.  First, the
 *  constructor is called.  The the component is configured.  This
 *  is usually done with the VjComponent.configure() method, although
 *  the component may also provide get/set methods to configure itself
 *  (see VjComponents.ConfigEditor.ConfigurePane's usage of
 *  ChunkDBPanel for an example of where this is useful).  Finally,
 *  initialize() is called.  At this point the component should be
 *  considered "live".
 *
 *  @see VjControl.ComponentFactory
 *  @author Christopher Just
 *  @version $Revision$
 */
public interface VjComponent {

    /** Sets the unique name of an instance of this component. */
    public void setComponentName (String _name);

    /** Returns the unique name of an instance of this component. */
    public String getComponentName ();


    /** Configures the component.
     *  The ConfigChunk will usually contain information like the 
     *  instance's proper name, the component responsible for
     *  instantiating it, and names of components that it will
     *  need to access.
     *  @throws VjComponentException, if the component can't process the
     *          chunk.
     */
    public void setConfiguration (ConfigChunk ch) throws VjComponentException;


    /** Makes a component active.
     *  This method performs whatever tasks are necessary for the 
     *  component to begin performing its duties.  For example,
     *  initialize() causes the ControlUIModule to display its
     *  window and the NetworkModule to start accepting
     *  connections.  Other typical actions in initialize include
     *  registering for event messages.
     *  <p>
     *  The component must be configured (e.g. by configure()) 
     *  before calling initialize().
     *
     *  @throws VjComoponentException if initialization fails.
     */
    public void initialize () throws VjComponentException;


    /** Returns the ConfigChunk used to configure this component. */
    public ConfigChunk getConfiguration ();


    /** Creates and adds a child component.
     *  Self tries to create a new child component using the data in ch.
     *  Ch is then passed to the new child via its configure() method.
     *  Note that some parent components will simply store ch in order
     *  to create components on-demand - the parent component should
     *  specify what its behavior is.
     *  @param  A non-null ConfigChunk.  Typically this will use the
     *          vjc_genericinstance ChunkDesc, but the only absolute 
     *          requirements are that it have a "ClassName" field
     *          with the name of the child component's class, and that
     *          the child component can configure() itself with the 
     *          chunk.  See VjFiles/vjcontrol.dsc for examples.
     *  @return True if the component was succesfully created and added.
     *          False typically means that the parent doesn't know
     *          what to do with this chunk, and is a configuration error.
     */
    public boolean addConfig (ConfigChunk ch);


    /** Remove a component that expects to be attached to this chunk.
     *  @param name Name of the component to remove (should be the
     *              same as the name of the ConfigChunk used to create
     *              it).
     *  @return True if the component was found and removed.  False
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
