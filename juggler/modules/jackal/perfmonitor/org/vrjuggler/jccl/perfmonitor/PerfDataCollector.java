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

import java.awt.event.*;
import java.io.*;
import java.util.*;

import VjComponents.PerfMonitor.DataLine;
import VjConfig.*;
import VjControl.Core;
import VjConfig.ConfigStreamTokenizer;

/** Class for storing performance data from a single source.
 *  A source is, for example, a jccl::PerfDataBuffer.
 *
 *  PerfDataCollector is a source for ActionEvents, with the following
 *  ActionCommands:
 *      "Update" - when data is appended to the Collector.
 *      "Clear" - when the Collector clears out all its data.
 */
public interface PerfDataCollector {

    public void write (DataOutputStream out) throws IOException;


    public void setMaxSamples (int n);

//     public int getNumPhases();

    public String getName();


    public void refreshMaxValues ();

    public double getMaxValue ();

//     public double getMaxValueForPhase (int phase);

//     public double getAverageForPhase (int _phase);

    public String dumpData();

    public void generateAverages (int preskip, int postskip);

    public String dumpAverages (int preskip, int postskip, boolean doanomaly, double cutoff);


//     public void read (ConfigStreamTokenizer st);


    public void addActionListener (ActionListener l);

    public void removeActionListener (ActionListener l);



};

