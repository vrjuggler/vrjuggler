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

import java.io.*;
import java.net.*;
import java.util.*;
import VjConfig.*;
import VjControl.*;
import VjComponents.Network.NetCommunicator;

/** Network connection manager.
 *  Responsible for taking care of sockets and connections.  Adds a thread
 *  to deal with reading from the socket.
 *  <p>
 *  NetControl knows nothing about the protocols being exchanged on the
 *  connection; it relies on a set of external NetCommunicator objects to
 *  parse and interpret the data streams.
 *  <p>
 *  The current implementation can only handle a single connection at a
 *  time.
 *  
 *  @bug This shouldn't really depend on ConfigStreamTokenizer; we should
 *       tweak this IO setup to be more generic if at all possible.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class NetworkModule
    extends DefaultCoreModule
    implements Runnable, 
               ChunkDBListener {

    private String                   remote_name;
    private Socket                   sock;
    private InputStream              instream;
    private DataOutputStream         outstream;
    private int                      port;
    private volatile boolean         connected;
    private Thread                   thread;

    private List                     netcontrol_targets;
    private List                     communicators;


    public NetworkModule() {
        super();
        component_name = "Unconfigured NetworkModule";
	remote_name = new String ("localhost");
	port = 4450;
	connected = false;
	netcontrol_targets = new ArrayList();
        communicators = new ArrayList();
	Core.vjcontrol_chunkdb.addChunkDBListener (this);
    }


    //---------------- VjComponent CoreModule Methods ------------------------

    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        component_name = ch.getName();
        component_chunk = ch;
    }

    
    public void initialize () throws VjComponentException {
        boolean hostset = false;
        String hostname = "", argument;
        int portnum = 0;

        String[] args = Core.getCommandLine();
	for (int i = 0; i < args.length; i++) {
	    if (args[i].startsWith ("-n") && (!args[i].equalsIgnoreCase ("-noautoload"))) {
                if (args[i].length() == 2)
                    argument = args[++i];
                else 
                    argument = args[i].substring(2);
                hostset = true;
                int index = argument.indexOf (':');
                if (index == -1) {
                    hostname = argument;
                    portnum = port;
                }
                else {
                    hostname = argument.substring(0, index);
                    portnum = Integer.parseInt(argument.substring(index+1));
                }
	    }
        }

        if (hostset) {
            setRemoteHost (hostname, portnum);
            connect();
        }
    }



    public VjComponent addConfig (ConfigChunk ch) throws VjComponentException {

        String classname = ch.getValueFromToken ("classname", 0).getString();
        if (classname == null)
            throw new VjComponentException (component_name + ": Not a component instance chunk: " + ch.getName());
        if (Core.component_factory.classSupportsInterface (classname, VjComponents.Network.NetCommunicator.class)) {
            NetCommunicator nc = (NetCommunicator) Core.component_factory.createComponent(classname);
            nc.setConfiguration (ch);
            nc.initialize ();
            addCommunicator (nc);
            return nc;
        }
        else 
            throw new VjComponentException (component_name + ": Unrecognized component: " + classname);
    }


    public boolean removeConfig (String name) {
        return false;
    }


    public void destroy () {
        Core.vjcontrol_chunkdb.removeChunkDBListener (this);
    }


    //---------------------- NetworkModule services -------------------------

    public String getHost() {
	return remote_name;
    }



    public int getPort() {
	return port;
    }



    public DataOutputStream getOutputStream () {
        return outstream;
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



    /** Attempts to open a connection
     *  The host & port must already be set.  If the connection is 
     *  opened successfully, NetControl will create a read
     *  thread to monitor the connection. It will then call initConnection 
     *  for all registered NetCommunicators.
     */
    public boolean connect () {
	if (connected)
	    return true;
	try {
	    //System.out.println ("Attempting to open socket to " 
	    //			+ remote_name + ":" + port);
	    sock = new Socket (remote_name, port);
	    outstream = new DataOutputStream (sock.getOutputStream());
	    instream = sock.getInputStream();
	    connected = true;

	    NetControlEvent e = new NetControlEvent (this, NetControlEvent.OPENED, remote_name, port);
	    notifyNetControlTargets (e);
	    thread = new Thread(this);
	    thread.start();
	    Core.consoleInfoMessage (component_name, "Connected to " 
                                     + remote_name + ":" + port);

            // check communicators
            NetCommunicator comm;
            synchronized (communicators) {
                int i, n = communicators.size();
                for (i = 0; i < n; i++) {
                    comm = (NetCommunicator)communicators.get(i);
                    comm.initConnection ();
                }
            }

	    return true;
	}
	catch (UnknownHostException u) {
	    Core.consoleErrorMessage (component_name, "Unknown host - " 
                                      + remote_name);
	    return false;
	}
	catch (IOException i) {
	    Core.consoleErrorMessage (component_name, "Failure connecting to " 
				      + remote_name + ":" + port + " - "
				      + i.getMessage());
	    //i.printStackTrace();
	    return false;
	}
    }



    public boolean disconnect () {
        boolean retval = true;
	if (!connected)
	    return true;
        Core.consoleInfoMessage (component_name, "Disconnected from " 
                                 + remote_name + ":" + port);
        NetControlEvent e = new NetControlEvent (this, NetControlEvent.CLOSED, remote_name, port);
        notifyNetControlTargets (e);
        connected = false;
        thread.interrupt();
	try {
	    outstream.close();
	    sock.close();
	    retval = true;
	}
	catch (IOException io) {
            Core.consoleErrorMessage (component_name, "Exception during disconnect: " + io);
            io.printStackTrace();
	    retval = false;
	}
        // check communicators
        NetCommunicator comm;
        synchronized (communicators) {
            int i, n = communicators.size();
            for (i = 0; i < n; i++) {
                comm = (NetCommunicator)communicators.get(i);
                comm.shutdownConnection ();
            }
        }
        return retval;
    }



    /** Updates config based on VjControl global preferences. */
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
		Core.consoleInfoMessage (component_name, "Old vjcontrol preferences file - please check preferences and re-save");
	    }
	}
    }



    public boolean isConnected() {
	return connected;
    }


    /** Tells each communicator to try to update its current information
     *  from the network.  For example, the ConfigCommunicator's requestUpdate
     *  requests the complete ChunkDB from the Juggler app on the other side
     *  of the connection.
     */
    public boolean requestUpdates() {
        synchronized (communicators) {
            int i, n = communicators.size();
            NetCommunicator nc;
            for (i = 0; i < n; i++) {
                nc = (NetCommunicator)communicators.get(i);
                nc.requestUpdate();
            }
        }
        return true;
    }


    public NetCommunicator getCommunicator (String name) {
        synchronized (communicators) {
            NetCommunicator nc;
            int i, n = communicators.size();
            for (i = 0; i < n; i++) {
                nc = (NetCommunicator)communicators.get(i);
                if (nc.getComponentName().equalsIgnoreCase (name))
                    return nc;
            }
            return null;
        }
    }


    protected void addCommunicator (NetCommunicator n) {
        synchronized (communicators) {
            n.setNetworkModule (this);
            communicators.add (n);
        }
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




    /** Socket read thread main loop. */
    public void run() {
        try {
            while (connected) {
                read();
            }
        }
        catch (EOFException e) {
            disconnect();
        }
        System.out.println ("quitting network listen thread");
    }


    /** Utility for read. */
    protected String readLine (InputStream instream) throws IOException {
        StringBuffer s = new StringBuffer(64);
        char ch;
        int i;
        for (;;) {
            i = instream.read();
            if (i == -1)
                throw new EOFException();
            ch = (char)i;
            if (ch == '\n')
                break;
            s.append (ch);
        }
        return new String(s);
    }


    /** Attempts to read a command from the network.
     *  Throws EOFException when the stream ends, or if we are not
     *  currently connected.
     */
    protected void read () throws EOFException {
	ConfigChunk c;
        String id = null;
	NetCommunicator comm = null;
        boolean accepted = false;
        boolean retval = false;
        String s;
        int j, k;

	if (!connected)
	    throw new EOFException();

        try {
            // need to parse the <protocol handler="foo"> line
            do {
                s = readLine (instream);
                System.out.println ("read stream begin: '" + s + "'");
                j = s.indexOf ("<protocol handler=\"");
                k = s.lastIndexOf ('"');
                if ((j != -1) && (k >= j + 19))
                    id = s.substring (j+19,k);
                
            } while (id == null);
            System.out.println ("protocol id name is '" + id + "'");
        }
        catch (IOException e) {
            if (e instanceof EOFException)
                // Throw the EOFException here to terminate the read thread.
                throw (EOFException)e;
            else {
                Core.consoleInfoMessage (component_name, e.toString());
                return;
            }
        }
            
        // check communicators
        synchronized (communicators) {
            int i, n = communicators.size();
            for (i = 0; i < n; i++) {
                comm = (NetCommunicator)communicators.get(i);
                if (comm.acceptsStreamIdentifier (id)) {
                    accepted = true;
                    break;
                }
            }
        }
        ProtocolInputStream in = new ProtocolInputStream (instream, "</protocol>");
        try {
            if (accepted) {
                comm.readStream (in, id);
                in.windToEnd();
            }
            else {
                Core.consoleErrorMessage (component_name, "Unknown protocol handler: '" + s + "'");
                in.windToEnd();
            }
        }
        catch (IOException e) {
            // We ignore EOFExceptions thrown by readStream because that 
            // would only signal the end of the ProtocolInputStream, and
            // not its parent input stream.
            if (e instanceof EOFException)
                return;
            else
                Core.consoleInfoMessage (component_name, e.toString());
        }
    }



    //------------------------ NetControl Target Stuff ----------------------

    public synchronized void addNetControlListener (NetControlListener l) {
	synchronized (netcontrol_targets) {
	    netcontrol_targets.add (l);
	}
    }


    public void removeNetControlListener (NetControlListener l) {
	synchronized (netcontrol_targets) {
	    netcontrol_targets.remove (l);
	}
    }


    protected void notifyNetControlTargets (NetControlEvent e) {
        NetControlListener[] l;
	synchronized (netcontrol_targets) {
            l = new NetControlListener[netcontrol_targets.size()];
	    l = (NetControlListener[]) netcontrol_targets.toArray(l);
	}
	for (int i = 0; i < l.length; i++) {
	    int a = e.event_type;
	    if (a == e.OPENED)
		l[i].openedConnection (e);
	    if (a == e.CLOSED)
		l[i].closedConnection (e);
	    if (a == e.ADDRESS)
		l[i].addressChanged (e);
	}
    }



    //----------------- ChunkDBListener Stuff -------------------------------

    /** Listen to changes in VjControl global preferences. */
    public void addChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeChunk (ChunkDBEvent e) {;}
    public void replaceChunk (ChunkDBEvent e) {
	reconfigure (e.getNewChunk());
    }
    public void removeAllChunks (ChunkDBEvent e) {;}


}



