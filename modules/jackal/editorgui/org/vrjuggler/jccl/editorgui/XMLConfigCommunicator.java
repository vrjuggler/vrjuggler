/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
package org.vrjuggler.jccl.editorgui;

import java.io.*;
import java.util.Iterator;
import java.util.List;
import javax.xml.parsers.*;
import org.jdom.*;
import org.jdom.input.SAXBuilder;

import org.vrjuggler.jccl.config.*;
import org.vrjuggler.jccl.net.DefaultNetCommunicator;
import org.vrjuggler.jccl.net.NetworkModule;
import org.vrjuggler.jccl.vjcontrol.*;

/** NetworkModule Communicator for VR Juggler configuration commands.
 *  This Communicator reads and writes the standard configuration
 *  requests - commands to send and receive ChunkDescs and
 *  ConfigChunks.
 *  <p>
 *  It is illegal to call any of the reading or message sending
 *  functions until the XMLConfigCommunicator has been assigned a
 *  NetworkModule object to talk to.
 *
 *  @version $Revision$
 */
public class XMLConfigCommunicator
    extends DefaultNetCommunicator
    implements ConfigCommunicator
{

    protected NetworkModule nc;
    protected DataOutputStream outstream;
    protected ConfigModule config_module;
    protected ConfigChunkDB active_chunkdb;
    protected ChunkDescDB active_descdb;
    protected boolean connected;

    public XMLConfigCommunicator () {
        super();
        component_name = "Unconfigured XMLConfigCommunicator";
        nc = null;
        outstream = null;
        config_module = null;
        active_chunkdb = null;
        active_descdb = null;
        connected = false;
    }

    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        component_name = ch.getName();

        // get pointers to the modules we need.
        VarValue prop_val = ch.getProperty(VjComponentTokens.DEPENDENCIES);

        if ( prop_val != null )
        {
            int n = ch.getPropertyCount(VjComponentTokens.DEPENDENCIES);
            String s;
            VjComponent c;

            for ( int i = 0; i < n; ++i )
            {
                s = ch.getProperty(VjComponentTokens.DEPENDENCIES, i).toString();
                c = Core.getVjComponent (s);
                if (c != null) {
                    if (c instanceof ConfigModule)
                        config_module = (ConfigModule)c;
                }
            }
        }
    }

    public void initialize () throws VjComponentException {
        if (config_module == null)
            throw new VjComponentException (component_name + ": Initialized with unmet dependencies.");
    }

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
        active_chunkdb.setName ("Active (" + nc.getHost() + ":" +
                                nc.getPort() + ")" );
        active_descdb = new ChunkDescDB();
        active_descdb.setName ("Active (" + nc.getHost() + ":" +
                               nc.getPort() + ")" );
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
        return id.equalsIgnoreCase ("xml_config");
    }

    /** Reads a command stream from the network.
     *  Returns control when it reaches the end of a single command.
     */
    public void readStream (InputStream instream, String id)
    throws IOException {

   ConfigChunk c;

   // build the XML stream into a DOM tree
        try
        {
            SAXBuilder builder = new SAXBuilder();
            Document doc = builder.build(instream);
            parseCommands (doc.getRootElement());
        }
        catch (Exception e) {
            IOException e1 = new IOException ("XML Config protocol error: " + e.getMessage());
            //e1.initCause (e);
            throw e1;
        }
    }

    protected void parseCommands (Element node) throws Exception {
        String name = node.getName();
        ConfigIOStatus iostatus = new ConfigIOStatus();

        System.out.println ("reading command: '" + name + "'");
        if (name.equalsIgnoreCase ("apply_chunks")) {
            boolean all = node.getAttribute("all").getBooleanValue();
            Core.consoleTempMessage (component_name, "Reading ConfigChunks");

            ConfigChunkDB db = new ConfigChunkDB();
            // parse the db itself
            List children = node.getChildren();
            Iterator j    = children.iterator();

            while ( j.hasNext() )
            {
               db.build((Element) j.next());
            }
            //System.out.println ("read ConfigChunkDB:\n" + db.xmlRep());
            if (iostatus.getStatus() != iostatus.FAILURE) {
                if (all)
                    active_chunkdb.clear();
                active_chunkdb.addAll(db);
            }

            // write out errors to UI
            for (int i = 0; i < iostatus.size(); i++)
                Core.consoleInfoMessage (component_name, iostatus.get(i).toString());
            if (iostatus.getStatus() >= iostatus.ERRORS)
                Core.consoleErrorMessage (component_name, iostatus.getSummary());
            else
                Core.consoleInfoMessage (component_name, iostatus.getSummary());

            //Core.consoleTempMessage (component_name, "Reading ConfigChunks -- Finished");
        }
        else if (name.equalsIgnoreCase ("apply_descs")) {
            boolean all = node.getAttribute("all").getBooleanValue();
            // don't do anything for all.

            Core.consoleTempMessage (component_name, "Reading ChunkDescs");

            ChunkDescDB db = new ChunkDescDB();
            // parse the db itself
            List children = node.getChildren();
            Iterator j    = children.iterator();

            while ( j.hasNext() )
            {
               db.build((Element) j.next());
            }

            if (iostatus.getStatus() != iostatus.FAILURE) {
                active_descdb.addAll (db);
                // we're not going thru the ConfigModule to add values to this
                // db, so we have to inform the ChunkFactory ourselves that
                // new ChunkDescs are available.
                ChunkFactory.addChunkDescDB (db);
            }

            // write out errors to UI
            for (int i = 0; i < iostatus.size(); i++)
                Core.consoleInfoMessage (component_name, iostatus.get(i).toString());
            if (iostatus.getStatus() >= iostatus.ERRORS)
                Core.consoleErrorMessage (component_name, iostatus.getSummary());
            else
                Core.consoleInfoMessage (component_name, iostatus.getSummary());

            //Core.consoleTempMessage (component_name, "Reading ChunkDescs -- Finished");
        }
        else if (name.equalsIgnoreCase ("refresh_all")) {
            getChunks();
        }
        else if (name.equalsIgnoreCase ("remove_chunks")) {
//             boolean all = node.getAttribute("all").equalsIgnoreCase("true");
//             if (all)
//                 active_chunkdb.removeAll();
            Core.consoleTempMessage (component_name, "Removing ConfigChunks");

            ConfigChunkDB db = new ConfigChunkDB();
            // parse the db itself

            active_chunkdb.removeAll (db);
            Core.consoleTempMessage (component_name, "Removing ConfigChunks -- Finished");
        }
        else if (name.equalsIgnoreCase ("remove_descs")) {
            // do nothing
        }
        else {
            throw new Exception ("Unrecognized command: " + name);
        }
    }

    //----------------------- Send Commands --------------------------

    public boolean getChunks () {
   /* requests the server's complete chunkdb */
   if (!connected) {
            Core.consoleErrorMessage ("Net", "XMLConfigCommunicator send command requested, but no connection.");
       return false;
        }
        synchronized (outstream) {
            try {
                //out.println ("get chunks all");
                outstream.writeBytes ("<protocol handler=\"xml_config\">\n" +
                                      "<request_current_chunks/>\n" +
                                      "</protocol>\n");
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
            Core.consoleErrorMessage ("Net", "XMLConfigCommunicator send command requested, but no connection.");
            return false;
        }
        synchronized (outstream) {
            try {
                // XXX: This should be fixed so that it does not write out XML
                // tags this way.
                // (PH 5/9/2002)
                outstream.writeBytes ("<protocol handler=\"xml_config\">\n" +
                                      "<apply_chunks>\n<" +
                                      ConfigTokens.chunk_db_TOKEN +
                                      ">\n" + ch + "</" +
                                      ConfigTokens.chunk_db_TOKEN +
                                      "></apply_chunks>\n</protocol>\n");
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
            Core.consoleErrorMessage ("Net", "XMLConfigCommunicator send command requested, but no connection.");
       return false;
        }
        synchronized (outstream) {
            try {
                outstream.writeBytes("<protocol handler=\"xml_config\">\n" +
                                     "<apply_chunks>\n");
                db.write(outstream);
                outstream.writeBytes("</apply_chunks>\n</protocol>\n");
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
            Core.consoleErrorMessage ("Net", "XMLConfigCommunicator send command requested, but no connection.");
            return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net","Removing " + db.size() + " chunks");
                //outstream.writeBytes ("remove chunks\n" + db.fileRep());
                outstream.writeBytes ("<protocol handler=\"xml_config\">\n" +
                                      "<remove_chunks>\n");
                db.write(outstream);
//                 for (int i = 0; i < db.size(); i++) {
//                     ConfigChunk ch = db.elementAt (i);
//                     outstream.writeBytes ("  <name>\"" + ch.getName() + "\"</name>\n");
//                 }
                outstream.writeBytes ("</remove_chunks>\n" +
                                      "</protocol>\n");
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
            Core.consoleErrorMessage ("Net", "XMLConfigCommunicator send command requested, but no connection.");
       return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net", "Removing chunk " + ch.getName());
                //                outstream.writeBytes ("remove chunks\n" + ch.toString() + "End\n");
                outstream.writeBytes ("<protocol handler=\"xml_config\">\n" +
                                      "<remove_chunks>\n");
//                 outstream.writeBytes ("  <name>\"" + ch.getName() + "\"</name>\n");
                outstream.writeBytes ("<" + ConfigTokens.chunk_db_TOKEN +
                                      ">\n" + ch + "</" +
                                      ConfigTokens.chunk_db_TOKEN + ">\n");
                outstream.writeBytes ("</remove_chunks>\n" +
                                      "</protocol>\n");

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
            Core.consoleErrorMessage ("Net", "XMLConfigCommunicator send command requested, but no connection.");
            return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net", "Requesting Descriptions");
                outstream.writeBytes ("<protocol handler=\"xml_config\">\n" +
                                      "<request_current_descs/>\n" +
                                      "</protocol>\n");
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
            Core.consoleErrorMessage ("Net", "XMLConfigCommunicator send command requested, but no connection.");
            return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net", "Sending Descriptions");
                outstream.writeBytes ("<protocol handler=\"xml_config\">\n" +
                                      "<apply_descs>\n<" +
                                      ConfigTokens.chunk_desc_db_TOKEN +
                                      ">\n" + d +
                                      "</" + ConfigTokens.chunk_desc_db_TOKEN +
                                      ">\n</apply_descs>\n</protocol>\n");
                //outstream.writeBytes ("descriptions\n");
                // write out d;
                //outstream.writeBytes (d.toString());
                //outstream.writeBytes ("end\n");
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
            Core.consoleErrorMessage ("Net", "XMLConfigCommunicator send command requested, but no connection.");
            return false;
        }
        synchronized (outstream) {
            try {
                Core.consoleTempMessage ("Net", "Removing Descriptions");
                outstream.writeBytes ("<protocol handler=\"xml_config\">\n" +
                                      "<remove_descs>\n");
                //for (int i = 0; i < db.size(); i++) {
                //ConfigChunk ch = db.elementAt (i);
                outstream.writeBytes ("  <name>\"" + s + "\"</name>\n");
                //}
                outstream.writeBytes ("</remove_descs>\n" +
                                      "</protocol>\n");
                //outstream.writeBytes ("remove descriptions " + s + "\n");
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
