/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


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
import java.util.Vector;
import VjConfig.*;
import VjGUI.*;



public class NetControl 
    implements Runnable, ChunkDBListener {


    private String                   remote_name;
    private Socket                  sock;
    private ConfigStreamTokenizer   instream;
    private DataOutputStream        out;
    private int                      port;
    private boolean                 connected;
    private Thread                  thread;

    private Vector                  netcontrol_targets;



    public NetControl() {
	remote_name = new String ("localhost");
	port = 4450;
	connected = false;
	netcontrol_targets = new Vector();
	Core.gui_chunkdb.addChunkDBListener (this);
    }



    public String getHost() {
	return remote_name;
    }



    public int getPort() {
	return port;
    }


    public void run() {
	while (connected) {
	    //System.out.println ("a");
	    if (!read())
		break;
	}
    }



    public void setRemoteHost (String _remote_host, int _port) {
	if (!connected) {
	    remote_name = _remote_host;
	    port = _port;
	    NetControlEvent e = new NetControlEvent (this, NetControlEvent.ADDRESS,
						     remote_name, port);
	    notifyNetControlTargets (e);
	}
    }



    public boolean connect () {
	if (connected)
	    return true;
	try {
	    //System.out.println ("Attempting to open socket to " 
	    //			+ remote_name + ":" + port);
	    sock = new Socket (remote_name, port);
	    out = new DataOutputStream (sock.getOutputStream());
	    instream = 
		new ConfigStreamTokenizer (new InputStreamReader(sock.getInputStream()));
	    connected = true;

	    NetControlEvent e = new NetControlEvent (this, NetControlEvent.OPENED, remote_name, port);
	    notifyNetControlTargets (e);
	    thread = new Thread(this);
	    thread.start();
	    Core.consoleInfoMessage ("Net", "Connected to " + remote_name
				     + ":" + port);

	    //Core.perf_collection.removeAllData();

	    getChunkDescs();
	    getChunks();
	    return true;
	}
	catch (UnknownHostException u) {
	    Core.consoleErrorMessage ("Net", "Unknown host - " + remote_name);
	    return false;
	}
	catch (IOException i) {
	    Core.consoleErrorMessage ("Net", "Failure connecting to " 
				      + remote_name + ":" + port + " - "
				      + i.getMessage());
	    //i.printStackTrace();
	    return false;
	}
    }



    public boolean disconnect () {
	if (!connected)
	    return true;
	try {
	    NetControlEvent e = new NetControlEvent (this, NetControlEvent.CLOSED, remote_name, port);
	    notifyNetControlTargets (e);
	    connected = false;
	    //thread.stop(); // deprecated
	    out.close();
	    sock.close();
	    Core.consoleInfoMessage ("Net", "Disconnected from " + remote_name
				     + ":" + port);
	    return true;
	}
	catch (IOException io) {
	    return false;
	}
    }



    protected void reconfigure(ConfigChunk ch) {
	// called whenever vjcontrol_preferences changes
	Property p;

	if (ch == null)
	    return;
	if (ch.getDescToken().equalsIgnoreCase ("vjcontrol")) {
	    try {
		String new_host = ch.getPropertyFromToken("host").getValue(0).getString();
		int new_port = ch.getPropertyFromToken("port").getValue(0).getInt();
		setRemoteHost (new_host, new_port);
	    }
	    catch (Exception e) {
		Core.consoleInfoMessage ("vjControl", "Old vjcontrol preferences file - please check preferences and re-save");
	    }
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
	    Core.consoleTempMessage ("Net","Removing " + db.size() + " chunks");
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
	    Core.consoleTempMessage ("Net", "Removing chunk " + ch.getName());
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



//     public boolean startTimedUpdate (String id, int time) {
// 	/* id is something like "position" */
// 	if (!connected)
// 	    return false;
// 	try {
// 	    out.writeBytes ("update start " + id + " " + time + " all\n");
// 	    out.flush();
// 	    return true;
// 	}
// 	catch (IOException io) {
// 	    return false;
// 	}
//     }



//     public boolean stopTimedUpdate (String id) {
// 	/* id is something like "position" */
// 	if (!connected)
// 	    return false;
// 	try {
// 	    out.writeBytes ("update stop " + id + " all\n");
// 	    out.flush();
// 	    return true;
// 	}
// 	catch (IOException io) {
// 	    return false;
// 	}
//     }



    public boolean getChunkDescs () {
	/* the idea behind this function is to send a msg to the server
	 * requesting all ChunkDescs, which we'll put into the DB.
	 */
	if (!connected)
	    return false;
	try {
	    Core.consoleTempMessage ("Net", "Requesting Descriptions");
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
	    Core.consoleTempMessage ("Net", "Sending Descriptions");
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
	    Core.consoleTempMessage ("Net", "Removing Descriptions");
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
		//System.out.println ("reading chunks");
		instream.nextToken();
		if (instream.sval.equalsIgnoreCase("all")) {
		    /* remove old chunks */
		    Core.active_chunkdb.removeAll();
		}
		else
		    instream.pushBack();
		
		// now we read in the new descriptions
		Core.consoleTempMessage ("Net","Reading ConfigChunks");
		//System.err.println(" about to read chunks");
		
		
		for (;;) {
		    c = Core.active_chunkdb.readAChunk(instream);
		    if (c != null) {
			//System.out.println ("read chunk: \n" + c.toString());
			Core.active_chunkdb.addElement (c);
		    }
		    else
			break;
		}
		Core.consoleTempMessage ("Net","Reading ConfigChunks -- Finished");

		return true;
	    }
	    else if (instream.sval.equalsIgnoreCase ("PerfData1")) {
		instream.pushBack();
		Core.perf_collection.read(instream, false);
		return true;
	    }
	    else if (instream.sval.equalsIgnoreCase ("refresh")) {
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
	    else {
	        Core.consoleErrorMessage ("Net", "Unknown command: " + instream.sval);
		return false;
	    }
	    
	}
	catch (IOException io) {
	    // if connected is true, then the IOE is an error; otherwise,
	    // it's probably just that disconnect() shut down the socket
	    // to take us out of our read loop.
	    if (connected) {
		Core.consoleErrorMessage ("Net", "IOException in NetControl.read(): " +io);
		disconnect();
	    }
	    return false;
	}
    }



    /******************** NetControl Target Stuff *********************/

    public synchronized void addNetControlListener (NetControlListener l) {
	synchronized (netcontrol_targets) {
	    netcontrol_targets.addElement (l);
	}
    }


    public void removeNetControlListener (NetControlListener l) {
	synchronized (netcontrol_targets) {
	    netcontrol_targets.removeElement (l);
	}
    }


    protected void notifyNetControlTargets (NetControlEvent e) {
	Vector l;
	synchronized (netcontrol_targets) {
	    l = (Vector) netcontrol_targets.clone();
	}
	for (int i = 0; i < l.size(); i++) {
	    NetControlListener lis = (NetControlListener)l.elementAt (i);
	    switch (e.event_type) {
	    case e.OPENED:
		lis.openedConnection (e);
		break;
	    case e.CLOSED:
		lis.closedConnection (e);
		break;
	    case e.ADDRESS:
		lis.addressChanged (e);
		break;
	    }
	}
    }



    /****************** ChunkDBListener Stuff **********************/
    //: Core only listens to ChunkDB events from the GUI chunkdb
    public void addChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeChunk (ChunkDBEvent e) {;}
    public void replaceChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeAllChunks (ChunkDBEvent e) {;}


}



