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


package VjComponents.PerfMonitor;

import java.io.*;
import VjConfig.*;
import VjComponents.Network.DefaultNetCommunicator;
import VjControl.*;
import VjComponents.PerfMonitor.PerformanceModule;
import VjComponents.Network.NetworkModule;

/** NetControl Communicator for VR Juggler performance data.
 *  This Communicator reads performance data written about by
 *  VR Juggler's PerfDataBuffers.  Data is stored in a 
 *  PerformanceModule object.
 *  <p>
 *  It is illegal to call any of the reading or message sending 
 *  functions until the ConfigCommunicator has been assigned a
 *  NetworkModule object to talk to.
 */
public class PerformanceCommunicator 
    extends DefaultNetCommunicator {


    private NetworkModule nc;
    DataOutputStream outstream;
    PerformanceModule perf_module;


    public PerformanceCommunicator () {
        super();
        component_name = "Unconfigured PerformanceCommunicator";
        nc = null;
        outstream = null;
        perf_module = null;
    }



    public void setConfiguration (ConfigChunk ch) throws VjComponentException {
        component_chunk = ch;
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
                    if (c instanceof PerformanceModule)
                        perf_module = (PerformanceModule)c;
                }
            }
        }
    }


    public void initialize () throws VjComponentException {
        if (perf_module == null)
            throw new VjComponentException (component_name + ": Initialized with unmet dependencies.");
     }


//      public boolean addConfig (ConfigChunk ch) {
//          return false;
//      }


//      public boolean removeConfig (String name) {
//          return false;
//      }


    public void destroy () {
        ;
    }


    public void setNetworkModule (NetworkModule _nc) {
        nc = _nc;
        outstream = nc.getOutputStream();
    }


    public void initConnection () {
        outstream = nc.getOutputStream();
        perf_module.removeAllData();
    }


    public void shutdownConnection () {
        ;
    }


    /** Returns true for all stream identifiers we think we understand. */
    public boolean acceptsStreamIdentifier (String id) {
        if (id.equalsIgnoreCase ("PerfData1"))
            return true;
        return false;
    }


    /** Reads a command stream from the network.  
     *  Returns control when it reaches the end of a single command.
     */
    public void readStream (InputStream instream, String id) 
    throws IOException {
        ConfigStreamTokenizer in = new ConfigStreamTokenizer (new InputStreamReader (instream));
        perf_module.readStream (in, id);
    }


    public boolean requestUpdate () {
        return true;
    }

}
