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


package VjComponents.Network;

import java.io.IOException;
import java.io.InputStream;

import VjComponents.Network.NetworkModule;
import VjControl.DefaultVjComponent;

/** Base interface for all NetworkModule communications modules.
 *  NetworkModule is used to communicate with VR Juggler applications.
 *  Communications modules can be added to NetworkModule by creating
 *  new objects which implement the NetCommunicator interface.
 */
public abstract class DefaultNetCommunicator 
    extends DefaultVjComponent
    implements NetCommunicator {


    /** Sets the instance of NetworkModule to talk to.
     *  When the Communicator needs to send data, it should
     *  request access to the NetworkModule's write stream.
     */
    public void setNetworkModule (NetworkModule nc) {
        ;
    }


    /** Sends initial messages for a new connection.
     *  Useful for any initial data queries that need to be sent
     *  when a new connection is opened.  Frex, ConfigCommunicator
     *  requests ChunkDesc and ChunkDBs from Juggler.
     */
    public void initConnection () {
        ;
    }


    /** Called when a connection is broken or shut down.
     *  Useful for doing things like removing the Active chunkdb/descdb
     *  of the Config Module.
     */
    public void shutdownConnection () {
        ;
    }


    /** True if the identifier represents a stream we can read.
     *  The various protocols in a VjControl are identified by
     *  string IDs such as "PerfData" or "CfgCommand"
     */
    public boolean acceptsStreamIdentifier (String id) {
        return false;
    }


    /** Read data from a communications stream.
     *  NetworkModule calls this function when it needs the Communicator 
     *  to read part of its input stream.  The Communicator should 
     *  read data until it reaches the end of the stream (how to 
     *  determine this is protocol dependant; for example, VR Juggler
     *  config streams end with the text token "end".
     *  <p>
     *  readStream (and anything called by it) can safely throw
     *  IOExceptions, and in case of EOF etc. this is encouraged.
     */
    public boolean readStream (InputStream instream, String id)
        throws IOException {
        return false;
    }


    public abstract boolean requestUpdate ();
}
