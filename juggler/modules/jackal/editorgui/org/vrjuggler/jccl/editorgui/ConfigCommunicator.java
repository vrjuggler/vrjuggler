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


package VjComponents.ConfigEditor;

import java.io.*;

import VjConfig.*;
import VjComponents.Network.DefaultNetCommunicator;
import VjControl.Core;
import VjControl.VjComponent;
import VjComponents.ConfigEditor.ConfigModule;
import VjComponents.Network.NetworkModule;


/** NetworkModule Communicator interface for VR Juggler configuration
 *  commands.  We've got two communications protocols (the old standard
 *  one and the new XML-based one) that share this common API.
 *  <p>
 *  It is illegal to call any of the reading or message sending 
 *  functions until the ConfigCommunicator has been assigned a
 *  NetworkModule object to talk to.  This should be done in InitConnection.
 */
public interface ConfigCommunicator {

    public abstract boolean getChunks ();

    public abstract boolean sendChunk (ConfigChunk ch);

    public abstract boolean sendChunks (ConfigChunkDB db);

    public abstract boolean removeChunks (ConfigChunkDB db);

    public abstract boolean removeChunk (ConfigChunk ch);

    public abstract boolean getChunkDescs ();

    public abstract boolean sendChunkDesc (ChunkDesc d);

    public abstract boolean removeChunkDescs (String s);

}
