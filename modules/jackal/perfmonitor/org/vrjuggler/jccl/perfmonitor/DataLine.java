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

public class DataLine {
    public double vals[];
    public double diffs[]; // diffs[i] is val[i]-val[i-1]
    public int  numlost; // # points missing after last point 
    public double linetotal; // time from end of last dataline toend
                            // of this one...

    public DataLine(int num) {
        vals = new double[num];
        diffs = new double[num];
	numlost = 0;
    }

    public int getNum () {
	return vals.length;
    }

    public String toString () {
	String s = "DataLine:\n  diffs: ";
	for (int i = 0; i < diffs.length; i++)
	    s += diffs[i] + ", ";
	s += "\n  vals: ";
	for (int i = 0; i < vals.length; i++)
	    s += vals[i] + ", ";
	s += "\n";
	return s;
    }
};
