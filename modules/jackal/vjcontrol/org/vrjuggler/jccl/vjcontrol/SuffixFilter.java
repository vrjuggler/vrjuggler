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



package VjControl;

import javax.swing.filechooser.*;
import java.io.File;
import java.io.FilenameFilter;
import java.util.Vector;



/** A convenient FileFilter subclass used by the EasyFileDialog.
 *  SuffixFilter filters files based on a set of suffixes, typically
 *  the file extension, tho it wouldn't necessarily have to be.
 *  This implements both the swing FileFilter interface and the
 *  java.io.FilenameFilter interface.  Now if someone could just tell
 *  me why there are two different almost identical interfaces in the 
 *  Core APIs...
 *
 *  @author Christopher Just
 *  @version $Revision$
 */
public class SuffixFilter 
    extends javax.swing.filechooser.FileFilter
    implements java.io.FilenameFilter {

    String name;
    Vector suffixes;


    public SuffixFilter (String _name, String _suffix) {
        suffixes = new Vector();
        suffixes.addElement(_suffix);
	name = _name;
    }


    public void addSuffix (String _suffix) {
        suffixes.addElement(_suffix);
    }


    /* for FileFilter */
    // used by file dialog, so it should accept directories
    public boolean accept (File f) {
	if (f.isDirectory())
            return true;
        for (int i = 0; i < suffixes.size(); i++) {
            String s = (String)suffixes.elementAt(i);
            if (f.getName().endsWith(s))
                return true;
        }
        return false;
    }


    /* for FileNameFilter */
    // shouldn't accept directories
    public boolean accept (File f, String name) {
        for (int i = 0; i < suffixes.size(); i++) {
            String s = (String)suffixes.elementAt(i);
            if (name.endsWith(s))
                return true;
        }
        return false;
    }


    public String getDescription () {
	return name;
    }
}
