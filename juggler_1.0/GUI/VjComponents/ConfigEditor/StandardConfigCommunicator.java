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
import VjComponents.ConfigEditor.ConfigCommunicator;
import VjComponents.Network.NetworkModule;


/** NetworkModule Communicator for VR Juggler configuration commands.
 *  This Communicator reads and writes the standard configuration
 *  requests - commands to send and receive ChunkDescs and
 *  ConfigChunks.
 *  <p>
 *  This is a slightly pathalogical case wrt the number of stream
 *  identifiers accepted, because the original implementation
 *  expected configuration commands to be the "default" communications
 *  mode (of which there's no longer any such thing).
 *  <p>
 *  It is illegal to call any of the reading or message sending 
 *  functions until the StandardConfigCommunicator has been assigned a
 *  NetworkModule object to talk to.
 */
public class StandardConfigCommunicator extends DefaultNetCommunicator
    implements ConfigCommunicator {


    protected NetworkModule nc;
    protected DataOutputStream outstream;
    protected ConfigModule config_module;
    protected ConfigChunkDB active_chunkdb;
    protected ChunkDescDB active_descdb;
    protected boolean connected;

    public StandardConfigCommunicator () {
        super();
        component_name = "Unconfigured StandardConfigCommunicator";
        nc = null;
        outstream = null;
        config_module = null;
        active_chunkdb = null;
        active_descdb = null;
        connected = false;
    }



    public boolean configure (ConfigChunk ch) {
        component_name = ch.getName();

        // get pointers to the modules we need.
        Property p = ch.getPropertyFromToken ("Dependencies");
        if (p != null) {
            int i;
            int n = p.getNum();
            String s;
            VjComponent c;
            for (i = 0; i < n; i++) {
                s = p.getValue(i).toString();
                c = Core.getComponentFromRegistry(s);
                if (c != null) {
                    if (c instanceof ConfigModule)
                        config_module = (ConfigModule)c;
                }
            }
        }
        if (config_module == null)
            config_module = (ConfigModule)Core.getComponentFromRegistry ("Config Module");
        if (config_module == null) {
            Core.consoleErrorMessage (component_name, "Instantiated with unmet VjComponent Dependencies. Fatal Configuration Error!");
            return false;
        }

        return true;
    }


//      public boolean addConfig (ConfigChunk ch) {
//          return false;
//      }


//      public boolean removeConfig (String name) {
//          return false;
//      }


    public void destroy () {
        if (connected)
            shutdownConnection ();
    }


    public void setNetworkModule (NetworkModule _nc) {
        nc = _nc;
        outstream = nc.getOutputStream();

        if (nc.isConnected())
            initConnection ();
    }


    /** For a new connection, requests chunkdesc and chunkdbs */
    public void initConnection () {
        outstream = nc.getOutputStream();
        connected = true;

        active_chunkdb = new ConfigChunkDB();
        active_chunkdb.setName ("Active Configuration");
        active_descdb = new ChunkDescDB();
        active_descdb.setName ("Active Descriptions");
        config_module.initConnection (this, active_descdb, active_chunkdb);

        getChunkDescs();
        getChunks();
    }


    /** Called when a connection is broken or close. */
    public void shutdownConnection () {
        connected = false;
        config_module.shutdownConnection (this);
    }


    /** Returns true for all stream identifiers we think we understand. */
    public boolean acceptsStreamIdentifier (String id) {
        if (id.equalsIgnoreCase ("descriptions"))
            return true;
        if (id.equalsIgnoreCase ("chunks"))
            return true;
        if (id.equalsIgnoreCase ("refresh"))
            return true;
        return false;
    }


    /** Reads a command stream from the network.  
     *  Returns control when it reaches the end of a single command.
     */
    public boolean readStream (InputStream _instream, String id) 
    throws IOException {
        if (!connected) {
            Core.consoleErrorMessage ("Net", "StandardConfigCommunicator.readStream called without connection.");
            return false;
        }

        ConfigStreamTokenizer instream = new ConfigStreamTokenizer (new InputStreamReader (_instream));

	ConfigChunk c;
	
        /* it's at this point that the commands get difficult to interpret */
        if (id.equalsIgnoreCase ("descriptions")) {
            instream.nextToken();
            if (instream.sval.equalsIgnoreCase("all")) {
                /* remove old descriptions */
                active_descdb.clear();
            }
            else
                instream.pushBack();
            
            // now we read in the new descriptions
            if (! active_descdb.read (instream))
                return false;
            return true;
        }
        else if (id.equalsIgnoreCase ("chunks")) {
            //System.out.println ("reading chunks");
            instream.nextToken();
            if (instream.sval.equalsIgnoreCase("all")) {
                /* remove old chunks */
                active_chunkdb.clear();
            }
            else
                instream.pushBack();
            
            // now we read in the new descriptions
            Core.consoleTempMessage ("Net","Reading ConfigChunks");
            //System.err.println(" about to read chunks");
		
		
            for (;;) {
                c = active_chunkdb.readAChunk(instream);
                if (c != null) {
                    //System.out.println ("read chunk: \n" + c.toString());
                    active_chunkdb.add (c);
                }
                else
                    break;
            }
            Core.consoleTempMessage ("Net","Reading ConfigChunks -- Finished");
            
            return true;
        }
        else { //id.equalsIgnoreCase ("refresh")
            // juggler is telling the GUI it needs to refresh itself.
            // for the moment, no args is the same as 'all'
            instream.nextToken();
            if (instream.sval.equalsIgnoreCase ("all"))
                getChunks();
            else {
                instream.pushBack();
                getChunks();
            }
            return true;
        }
    }



    //----------------------- Send Commands --------------------------

    public boolean getChunks () {
	/* requests the server's complete chunkdb */
	if (!connected) {
            Core.consoleErrorMessage ("Net", "StandardConfigCommunicator send command requested, but no connection.");
	    return false;
        }
        synchronized (outstream) {
            try {
                //out.println ("get chunks all");
                outstream.writeBytes ("get chunks all\n");
                outstream.flush();
                return true;
            }
            catch (IOException io) {
                return false;
            }
        }
    }



    public boolean sendChunk (ConfigChunk ch) {
	if (!connected) {
            Core.consoleErrorMessage ("Net", "StandardConfigCommunicator send command requested, but no connection.");
	    return false;
        }
        synchronized (outstream) {
            try {
                outstream.writeBytes ("chunks\n" + ch.toString() + "End\n");
                outstream.flush();
                return true;
            }
            catch (IOException io) {
                return false;
            }
        }
    }


    public boolean sendChunks (ConfigChunkDB db) {
        if (!connected) {
            Core.consoleErrorMessage ("Net", "StandardConfigCommunicator send command requested, but no connection.");
	    return false;
        }
        synchronized (outstream) {
            try {
                outstream.writeBytes ("chunks\n" + db.fileRep());
                outstream.flush();
                return true;
            }
            catch (IOException io) {
                return false;
            }
        }
    }



    public boolean removeChunks (ConfigChunkDB db) {
        if (!connected) {
            Core.consoleErrorMessage ("Net", "StandardConfigCommunicator send command requested, but no connection.");
            return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net","Removing " + db.size() + " chunks");
                outstream.writeBytes ("remove chunks\n" + db.fileRep());
                outstream.flush();
                return true;
            }
            catch (IOException io) {
                return false;
            }
        }
    }



    public boolean removeChunk (ConfigChunk ch) {
        if (!connected) {
            Core.consoleErrorMessage ("Net", "StandardConfigCommunicator send command requested, but no connection.");
	    return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net", "Removing chunk " + ch.getName());
                outstream.writeBytes ("remove chunks\n" + ch.toString() + "End\n");
                outstream.flush();
                return true;
            }
            catch (IOException io) {
                return false;
            }
        }
    }


    public boolean getChunkDescs () {
	/* the idea behind this function is to send a msg to the server
	 * requesting all ChunkDescs, which we'll put into the DB.
	 */
        if (!connected) {
            Core.consoleErrorMessage ("Net", "StandardConfigCommunicator send command requested, but no connection.");
            return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net", "Requesting Descriptions");
                outstream.writeBytes ("get descriptions all\n");
                outstream.flush();
                return true;
            }
            catch (IOException io) {
                return false;
            }
        }
    }



    public boolean sendChunkDesc (ChunkDesc d) {
	/* sends a ChunkDesc to the server. */
        if (!connected) {
            Core.consoleErrorMessage ("Net", "StandardConfigCommunicator send command requested, but no connection.");
            return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net", "Sending Descriptions");
                outstream.writeBytes ("descriptions\n");
                // write out d;
                outstream.writeBytes (d.toString());
                outstream.writeBytes ("end\n");
                outstream.flush();
                return true;
            }
            catch (IOException io) {
                return false;
            }
        }
    }



    public boolean removeChunkDescs (String s) {
        if (!connected) {
            Core.consoleErrorMessage ("Net", "StandardConfigCommunicator send command requested, but no connection.");
            return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net", "Removing Descriptions");
                outstream.writeBytes ("remove descriptions " + s + "\n");
                outstream.flush();
                return true;
            }
            catch (IOException io) {
                return false;
            }
        }
    }


    public boolean requestUpdate () {
        return getChunks();
    }

}
