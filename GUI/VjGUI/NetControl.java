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
import VjGUI.ClientGlobals;
import VjConfig.ConfigStreamTokenizer;
import VjConfig.ChunkDesc;
import VjConfig.ConfigChunk;


public class NetControl implements Runnable {

    public String                   remoteName;
    private Socket                  sock;
    private ConfigStreamTokenizer   instream;
    private DataOutputStream        out;
    public int                      port;
    ClientGlobals                   core;
    private boolean                 connected;
    private Thread                  thread;



    public NetControl(ClientGlobals c) {
	remoteName = new String ("localhost");
	core = c;
	port = 4450;
	connected = false;
    }



    public void run() {
	for (;;) {
	    System.out.println ("a");
	    if (!core.net.read())
		break;
	}
    }



    public boolean connect (String connectsource) {
	return connect (connectsource, port);
    }



    public boolean connect (String connectsource, int portnum) {
	if (connected)
	    return true;
	port = portnum;
	try {
	    remoteName = new String (connectsource);
	    System.out.println ("Attempting to open socket to " 
				+ remoteName + ":" + port);
	    sock = new Socket (remoteName, port);
	    out = new DataOutputStream (sock.getOutputStream());
	    instream = 
		/*		new ConfigStreamTokenizer(new BufferedReader 
					  (new InputStreamReader 
					   (sock.getInputStream()))
					  );
		*/
		new ConfigStreamTokenizer (new InputStreamReader(sock.getInputStream()));
	    connected = true;
	    thread = new Thread(this);
	    thread.start();
	    return true;
	}
	catch (UnknownHostException u) {
	    System.err.println ("Unknown host - " + remoteName);
	    return false;
	}
	catch (IOException i) {
	    System.err.println ("IO error in socket: " + i.getMessage());
	    i.printStackTrace();
	    return false;
	}
    }



  public boolean disconnect () {
    if (!connected)
      return true;
    try {
      thread.stop();
      out.close();
      sock.close();
      connected = false;
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
	System.out.println ("not tested");
	return false;
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
	
	if (!connected)
	    return false;
	try {
	    /* it's at this point that the commands get difficult to interpret */
	    instream.nextToken();
	    if (instream.sval.equalsIgnoreCase ("descriptions")) {
		instream.nextToken();
		if (instream.sval.equalsIgnoreCase("all")) {
		    /* remove old descriptions */
		    core.descs.removeAll();
		}
		else
		    instream.pushBack();
		
		// now we read in the new descriptions
		if (! core.descs.read (instream))
		    return false;
		core.ui.update();
		return true;
	    }
	    else if (instream.sval.equalsIgnoreCase ("chunks")) {
		instream.nextToken();
		if (instream.sval.equalsIgnoreCase("all")) {
		    /* remove old chunks */
		    core.chunks.removeAll();
		}
		else
		    instream.pushBack();
		
		// now we read in the new descriptions
		System.err.println(" about to read chunks");
		if (! core.chunks.read (instream))
		    return false;
		core.ui.update();
		return true;
	    }
	    else {
		System.err.println ("Unknown command: " + instream.sval);
		return false;
	    }
	    
	    //return true;
	}
	catch (IOException io) {
	    System.err.println ("IOException in NetControl.read()");
	    return false;
	}
    }



}
