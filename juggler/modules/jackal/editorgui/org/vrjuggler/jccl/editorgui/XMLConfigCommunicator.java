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
import javax.xml.parsers.*;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.w3c.dom.*;

import VjConfig.*;
import VjComponents.Network.DefaultNetCommunicator;
import VjControl.*;
import VjComponents.ConfigEditor.ConfigModule;
import VjComponents.Network.NetworkModule;


/** NetworkModule Communicator for VR Juggler configuration commands.
 *  This Communicator reads and writes the standard configuration
 *  requests - commands to send and receive ChunkDescs and
 *  ConfigChunks.
 *  <p>
 *  It is illegal to call any of the reading or message sending 
 *  functions until the XMLConfigCommunicator has been assigned a
 *  NetworkModule object to talk to.
 */
public class XMLConfigCommunicator
    extends DefaultNetCommunicator
    implements ConfigCommunicator {


    protected NetworkModule nc;
    protected DataOutputStream outstream;
    protected ConfigModule config_module;
    protected ConfigChunkDB active_chunkdb;
    protected ChunkDescDB active_descdb;
    protected boolean connected;
    protected XMLConfigIOHandler config_handler;

    public XMLConfigCommunicator () {
        super();
        component_name = "Unconfigured XMLConfigCommunicator";
        nc = null;
        outstream = null;
        config_module = null;
        active_chunkdb = null;
        active_descdb = null;
        connected = false;
        config_handler = null;
    }



    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
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

        config_handler = (XMLConfigIOHandler)ConfigIO.getHandler (ConfigIO.XML);
        if (config_handler == null)
            throw new VjComponentException (component_name + ": Couldn't get XML IO Handler.");
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
        Document doc;
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();

	// build the XML stream into a DOM tree
        try {
            DocumentBuilder builder = factory.newDocumentBuilder();
            //System.out.println ("xml parse begin");
            doc = builder.parse (instream);
            //System.out.println ("xml parse end");
            parseCommands (doc.getDocumentElement());
            //System.out.println ("xml parse interpret end");
        }
        catch (Exception e) {
            IOException e1 = new IOException ("XML Config protocol error: " + e.getMessage());
            //e1.initCause (e);
            throw e1;
        }
    }



    protected void parseCommands (Element node) throws Exception {
        Node child;
        String name = node.getTagName ();

        System.out.println ("reading command: '" + name + "'");
        if (name.equalsIgnoreCase ("apply_chunks")) {
            boolean all = node.getAttribute("all").equalsIgnoreCase("true");
            Core.consoleTempMessage (component_name, "Reading ConfigChunks");

            ConfigChunkDB db = new ConfigChunkDB();
            // parse the db itself
            child = node.getFirstChild();
            while (child != null) {
                switch (child.getNodeType()) {
                case Node.ELEMENT_NODE:
                    config_handler.buildChunkDB (db, child);
                    break;
                case Node.COMMENT_NODE:
                case Node.NOTATION_NODE:
                case Node.PROCESSING_INSTRUCTION_NODE:
                case Node.TEXT_NODE: // whitespace
                    break;
                default:
                    System.out.println ("Unexpected node type...");
                }
                child = child.getNextSibling();
            }
            //System.out.println ("read ConfigChunkDB:\n" + db.xmlRep());
            if (all)
                active_chunkdb.clear();
            active_chunkdb.addAll(db);
            Core.consoleTempMessage (component_name, "Reading ConfigChunks -- Finished");
//             return true;
        }
        else if (name.equalsIgnoreCase ("apply_descs")) {
            boolean all = node.getAttribute("all").equalsIgnoreCase("true");
            // don't do anything for all.

            Core.consoleTempMessage (component_name, "Reading ChunkDescs");

            ChunkDescDB db = new ChunkDescDB();
            // parse the db itself
            child = node.getFirstChild();
            while (child != null) {
                switch (child.getNodeType()) {
                case Node.ELEMENT_NODE:
                    config_handler.buildChunkDescDB (db, child);
                    break;
                case Node.COMMENT_NODE:
                case Node.NOTATION_NODE:
                case Node.PROCESSING_INSTRUCTION_NODE:
                case Node.TEXT_NODE: // whitespace
                    break;
                default:
                    System.out.println ("Unexpected node type...");
                }

                child = child.getNextSibling();
            }

            active_descdb.addAll (db);
            // we're not going thru the ConfigModule to add values to this
            // db, so we have to inform the ChunkFactory ourselves that these
            // new ChunkDescs are available.
            ChunkFactory.addChunkDescDB (db);
            Core.consoleTempMessage (component_name, "Reading ChunkDescs -- Finished");
//             return true;
        }
        else if (name.equalsIgnoreCase ("refresh_all")) {
            getChunks();
//             return true;
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
//             return true;
        }
        else if (name.equalsIgnoreCase ("remove_descs")) {
//             return true; // do nothing
        }
        else {
            throw new Exception ("Unrecognized command: " + name);
//             System.out.println ("Unrecognized command: " + name);
//             return false;
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
                outstream.writeBytes ("<protocol handler=\"xml_config\">\n" +
                                      "<apply_chunks>\n" +
                                      ch.xmlRep ("  ") +
                                      "</apply_chunks>\n" +
                                      "</protocol>\n");
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
                outstream.writeBytes ("<protocol handler=\"xml_config\">\n" +
                                      "<apply_chunks>\n" +
                                      db.xmlRep() +
                                      "</apply_chunks>\n" +
                                      "</protocol>\n");
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
                outstream.writeBytes (db.xmlRep());
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
                outstream.writeBytes ("<ConfigChunkDB>\n" + ch.xmlRep() + "</ConfigChunkDB>\n");
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
                                      "<apply_descs>\n<ChunkDescDB>\n" +
                                      d.xmlRep("  ") +
                                      "</ChunkDescDB>\n</apply_descs>\n" +
                                      "</protocol>\n");
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
