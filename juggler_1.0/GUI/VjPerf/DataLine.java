/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



package VjPerf;

public class DataLine {
    public double vals[];
    public double diffs[]; // diffs[i] is val[i]-val[i-1]
    //public boolean valid[]; /* used to mark missing datapoints */
    public int  numlost; // # points missing after last point 
    public double linetotal; // time from end of last dataline toend
                            // of this one...
// i'll assume for now there's a nan value that i can use.

    public DataLine(int num) {
        vals = new double[num];
        diffs = new double[num];
        //valid = new boolean[num];
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
