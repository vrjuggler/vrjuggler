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


/** Provides default implementation of VjComponent interface.
 *  A component can choose to subclass from this class in order to get
 *  definitions of core VjComponent functions.
 *
 *  @see VjControl.ComponentFactory
 *  @see VjControl.VjComponent
 *  @author Christopher Just
 *  @version $Revision$
 */
public abstract class DefaultVjComponent
    implements VjComponent {

    /** Unique identifier for an instance of this component. */
    protected String component_name;

    /** ConfigChunk used to configure this component. */
    protected ConfigChunk component_chunk;


    /** Constructor. */
    public DefaultVjComponent () {
        super();
        component_chunk = null;
        component_name = "Unconfigured Component";
    }


    public void setComponentName (String _name) {
        component_name = _name;
    }


    /** Returns the unique name of this instance. */
    public String getComponentName () {
        return component_name;
    }


    /** Configure the component.  Returns success (or not). */
    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        component_chunk = ch;
        component_name = ch.getName();
    }



    /** Returns the ConfigChunk used to configure this component. */
    public ConfigChunk getConfiguration () {
        return component_chunk;
    }


    public void initialize () throws VjComponentException {
    }


    /** Adds a component.
     *  @return Nothing (this implementation always throws).
     *  @throws The default implementation always throws a
     *          VjComponentException.
     */
    public VjComponent addConfig (ConfigChunk ch) throws VjComponentException {
        throw new VjComponentException (component_name + " does not support child component: " + ch.getName());
    }



    /** Remove a component that expects to be attached to this chunk.
     *  @param name Name of the component to remove (should be the
     *              same as the name of the ConfigChunk used to create
     *              it).
     *  @return True if the component was found and removed.  False
     *          indicates that the removeConfig couldn't find any
     *          such component to remove.
     */
    public boolean removeConfig (String name) {
        return false;
    }


    /** Called when a component is about to be removed.
     *  Whenever vjcontrol is about to remove a component, it calls
     *  this method, which allows the component to remove references
     *  to itself (mainly, to stop listening for events).
     *  <p>
     *  This most commonly occurs with GUI components, such as
     *  ConfigChunkPanels.
     */
    public void destroy () {
        ;
    }

}
