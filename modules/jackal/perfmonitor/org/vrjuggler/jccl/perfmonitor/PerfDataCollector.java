/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
package org.vrjuggler.jccl.perfmonitor;

import java.awt.event.*;
import java.io.*;
import java.util.*;
import org.vrjuggler.jccl.config.*;

/** Class for storing performance data from a single source.
 *  A source is, for example, a jccl::PerfDataBuffer.
 *
 *  PerfDataCollector is a source of ActionEvents, with the following
 *  ActionCommands:
 *      "Update" - when data is appended to the Collector.
 *      "Clear" - when the Collector clears out all its data.
 */
public interface PerfDataCollector {

    public void write (DataOutputStream out) throws IOException;


    /** Set the maximum number of samples to store.
     *  When new samples are added, self will remove the oldest samples
     *  to make room.
     *  @param n Maximum number of samples to allow.  If n = -1, no
     *           purging will be done and the collector will add new
     *           samples so long as memory is available.
     */
    public void setMaxSamples (int n);

//     public int getNumPhases();


    /** Returns the name of this collector.
     *  The name is generally the same as the name used for the 
     *  jccl::PerfDataBuffer it was created to read data from.
     */
    public String getName();


    public void refreshMaxValues ();

//      public double getMaxValue ();

    public String dumpData();

    public void generateAverages (int preskip, int postskip);

    public String dumpAverages (int preskip, int postskip, boolean doanomaly, double cutoff);


    public void addActionListener (ActionListener l);

    public void removeActionListener (ActionListener l);



};

