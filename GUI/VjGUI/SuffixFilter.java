/*
 *  File:	    $Name$
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
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



package VjGUI;

import javax.swing.filechooser.*;
import java.io.*;

public class SuffixFilter extends javax.swing.filechooser.FileFilter {

    String suffix;
    String name;

    SuffixFilter (String _name, String _suffix) {
	suffix = _suffix;
	name = _name;
    }

    public boolean accept (File f) {
	if (f.isDirectory() || f.getName().endsWith(suffix))
	    return true;
	else 
	    return false;
    }

    public String getDescription () {
	return name;
    }
}
