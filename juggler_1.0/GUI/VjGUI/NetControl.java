/*
 * NetControl.java
 *
 * Wrapper around the networking of vjController.  Responsible for
 * taking care of sockets and connections.  Adds a thread to deal
 * with reading from the socket.
 *
 * Author: Christopher Just
 *
 */

package VjGUI;

import java.io.*;
import java.net.*;
import VjConfig.*;
import VjGUI.*;

public class NetControl implements Runnable {

    private String                   remote_name;
    private Socket                  sock;
    private ConfigStreamTokenizer   instream;
    private DataOutputStream        out;
    private int                      port;
    private boolean                 connected;
    private Thread                  thread;



    public NetControl() {
	remote_name = new String ("localhost");
	port = 4450;
	connected = false;
    }



    public void setHost(String s) {
	remote_name = s;
    }


    public void setPort (int i) {
	port = i;
    }


    public String getHost() {
	return remote_name;
    }

    public int getPort() {
	return port;
    }


    public void run() {
	for (;;) {
	    System.out.println ("a");
	    if (!read())
		break;
	}
    }


    public boolean connect () {
	return connect (remote_name, port);
    }

    public boolean connect (String connectsource) {
	return connect (connectsource, port);
    }



    public boolean connect (String connectsource, int portnum) {
	if (connected)
	    return true;
	port = portnum;
	try {
	    remote_name = new String (connectsource);
	    System.out.println ("Attempting to open socket to " 
				+ remote_name + ":" + port);
	    sock = new Socket (remote_name, port);
	    out = new DataOutputStream (sock.getOutputStream());
	    instream = 
		new ConfigStreamTokenizer (new InputStreamReader(sock.getInputStream()));
	    connected = true;
	    Core.enableActiveDB();
	    thread = new Thread(this);
	    thread.start();
	    Core.consoleInfoMessage ("Net", "Connected to " + remote_name);
	    getChunkDescs();
	    getChunks();
	    return true;
	}
	catch (UnknownHostException u) {
	    Core.consoleErrorMessage ("Net", "Unknown host - " + remote_name);
	    return false;
	}
	catch (IOException i) {
	    Core.consoleErrorMessage ("Net", i.getMessage());
	    //i.printStackTrace();
	    return false;
	}
    }



    public boolean disconnect () {
	if (!connected)
	    return true;
	try {
	    Core.disableActiveDB();
	    thread.stop();
	    out.close();
	    sock.close();
	    connected = false;
	    Core.consoleInfoMessage ("Net", "Disconnected from " + remote_name
				     + ":" + port);
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }



    public boolean getChunks () {
	/* requests the server's complete chunkdb */
	if (!connected)
	    return false;
	try {
	    //out.println ("get chunks all");
	    out.writeBytes ("get chunks all\n");
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }



    public boolean sendChunk (ConfigChunk ch) {
	if (!connected)
	    return false;
	try {
	    out.writeBytes ("chunks\n" + ch.toString() + "End\n");
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }


    public boolean sendChunks (ConfigChunkDB db) {
	if (!connected)
	    return false;
	try {
	    out.writeBytes ("chunks\n" + db.fileRep());
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}

    }

    public boolean removeChunks (ConfigChunkDB db) {
	if (!connected)
	    return false;
	try {
	    System.out.println ("remove chunks\n" + db.fileRep());
	    out.writeBytes ("remove chunks\n" + db.fileRep());
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }


    public boolean removeChunk (ConfigChunk ch) {
	if (!connected)
	    return false;
	try {
	    System.out.println ("remove chunks\n" + ch.toString() + "End\n");
	    out.writeBytes ("remove chunks\n" + ch.toString() + "End\n");
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }


    public boolean isConnected() {
	return connected;
    }



    public boolean startTimedUpdate (String id, int time) {
	/* id is something like "position" */
	if (!connected)
	    return false;
	try {
	    out.writeBytes ("update start " + id + " " + time + " all\n");
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }



    public boolean stopTimedUpdate (String id) {
	/* id is something like "position" */
	if (!connected)
	    return false;
	try {
	    out.writeBytes ("update stop " + id + " all\n");
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }



    public boolean getChunkDescs () {
	/* the idea behind this function is to send a msg to the server
	 * requesting all ChunkDescs, which we'll put into the DB.
	 */
	if (!connected)
	    return false;
	try {
	    //out.println ("get descriptions all");
	    out.writeBytes ("get descriptions all\n");
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }



    public boolean sendChunkDesc (ChunkDesc d) {
	/* sends a ChunkDesc to the server. */
	if (!connected)
	    return false;
	try {
	    out.writeBytes ("descriptions\n");
	    // write out d;
	    out.writeBytes (d.toString());
	    out.writeBytes ("end\n");
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }



    public boolean removeChunkDescs (String s) {
	if (!connected)
	    return false;
	try {
	    out.writeBytes ("remove descriptions " + s + "\n");
	    out.flush();
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }



    /* Attempts to read a command from the network.  If no data is ready
     * to be read, returns immediately (it may still block if it starts
     * reading something).
     */
    public boolean read () {
	ConfigChunk c;
	
	if (!connected)
	    return false;
	try {
	    /* it's at this point that the commands get difficult to interpret */
	    instream.nextToken();
	    if (instream.sval.equalsIgnoreCase ("descriptions")) {
		instream.nextToken();
		if (instream.sval.equalsIgnoreCase("all")) {
		    /* remove old descriptions */
		    Core.descdb.removeAll();
		}
		else
		    instream.pushBack();
		
		// now we read in the new descriptions
		if (! Core.descdb.read (instream))
		    return false;
		return true;
	    }
	    else if (instream.sval.equalsIgnoreCase ("chunks")) {
		instream.nextToken();
		if (instream.sval.equalsIgnoreCase("all")) {
		    /* remove old chunks */
		  Core.active_treemodel.chunkdb.removeAll();
		  Core.active_treemodel.buildTree();
		}
		else
		    instream.pushBack();
		
		// now we read in the new descriptions
		System.err.println(" about to read chunks");
		
		
		for (;;) {
		    c = Core.active_treemodel.chunkdb.readAChunk(instream);
		    if (c != null) {
			//System.out.println ("read chunk: \n" + c.toString());
			Core.active_treemodel.insertNode (c);
		    }
		    else
			break;
		}
		System.out.println (" done reading chunks");

		return true;
	    }
	    else if (instream.sval.equalsIgnoreCase ("PerfData1")) {
		instream.pushBack();
		Core.perf_collection.read(instream);
		return true;
	    }
	    else if (instream.sval.equalsIgnoreCase ("refresh")) {
		// juggler is telling the GUI it needs to refresh itself.
		getChunks();
		return true;
	    }
	    else {
		System.err.println ("Unknown command: " + instream.sval);
		return false;
	    }
	    
	    //return true;
	}
	catch (IOException io) {
	    System.err.println ("IOException in NetControl.read(): " +io);
	    disconnect();
	    return false;
	}
    }



}



