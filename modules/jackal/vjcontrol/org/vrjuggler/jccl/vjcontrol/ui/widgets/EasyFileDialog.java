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


package VjComponents.UI.Widgets;

import java.awt.Component;
import java.io.IOException;
import java.io.File;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import java.util.Vector;
import java.util.HashMap;

import VjControl.SuffixFilter;

/** Utility functions for managing FileDialogs.
 * 
 *  @author Christopher Just
 *  @version $Revision$
 */
public class EasyFileDialog {

    /* Why do we have a separate vector for filters if they're all in
     * the hashmap anyway? because this way we have a specific ordering
     * for how the filters should appear in the file dialog's pulldown.
     * This is useful for components that add several filters, so that
     * they at least appear adjacent.
     */

    protected Vector filters;
    protected HashMap filter_lookup;


    /** Constructor. */
    public EasyFileDialog () {
        filters = new Vector();
        filter_lookup = new HashMap();
    }


    /** Returns a FileFilter with the given id (or null). */
    public FileFilter getFilter (String id) {
        return (FileFilter)filter_lookup.get(id);
    }


    /** Add a filter to self.
     *  The id is used as a universal name for this filter, so that e.g.
     *  several components can try to add a filter for "ConfigChunkDB" files.
     *  Only the first actually gets added; so make sure you only use the
     *  Filter that gets returned from the add.
     *  @param f A FileFilter (usually a VjControl.SuffixFilter).
     *  @param id A universal name for this filter.
     *  @return f, or a previously added filter with the same id name.
     */
    public FileFilter addFilter (FileFilter f, String id) {
        FileFilter oldfilter = getFilter(id);
        if (oldfilter == null) {
            filters.add (f);
            filter_lookup.put (id, f);
            return f;
        }
        else {
            return oldfilter;
        }
    }



    public void removeFilter (String id) {
        /* CJ: not implemented yet; could use ref counting of filters. sigh */
    }



    /** Opens a file dialog frame and gets a File from the user.
     *  @param dir Initial directory to view.
     *  @param parent UI component to use as the Frame's parent.
     *  @param id Name of a FileFilter that had been added with addFilter().
     *            This can be null or empty string, in which case there is
     *            no filtering of directory names by default.
     *  @return The selected File, or null (e.g. if the user hit "cancel").
     */
    public File requestOpenFile (File dir, Component parent, String id) {
        return requestOpenFile (dir, parent, getFilter(id));
    }



    /** Opens a file dialog frame and gets a File from the user.
     *  @param dir Initial directory to view.
     *  @param parent UI component to use as the Frame's parent.
     *  @param default_filter FileFilter that has been added with addFilter().
     *                        This can be null, in which case there is
     *                        no filtering of directory names by default.
     *  @return The selected File, or null (e.g. if the user hit "cancel").
     */
    public File requestOpenFile(File dir, Component parent, FileFilter default_filter) {

        if (dir == null)
            dir = new File (System.getProperty ("user.dir"));
  	/* opens up a file requester... */
 	JFileChooser chooser = new JFileChooser(dir);

        for (int i = 0; i < filters.size(); i++)
            chooser.addChoosableFileFilter ((FileFilter)filters.get(i));
        if (default_filter != null)
            chooser.setFileFilter (default_filter);
 	chooser.setFileHidingEnabled (false);
 	chooser.setDialogType(JFileChooser.OPEN_DIALOG);
 	int returnVal = chooser.showOpenDialog(parent); 
 	if(returnVal == JFileChooser.APPROVE_OPTION)
 	    return chooser.getSelectedFile();
 	else
            return null;
     }



     public File requestSaveFile (File file, Component parent, FileFilter default_filter) {

         try {
             JFileChooser chooser;
             if (file != null)
                 file = new File(file.getCanonicalPath());
             else
                 file = new File(System.getProperty("user.dir"));
            
             /* opens up a file requester... */
             if (file.isDirectory())
                 chooser = new JFileChooser(file);
             else {
                 //System.out.println ("File is " + file + "\n and parent is " + file.getParent());
                 String dir = file.getParent();
                 if (dir == null) 
                     dir = "";
                 chooser = new JFileChooser (new File(dir));
                 //chooser.setSelectedFile (file);
             }
             chooser.setDialogType(JFileChooser.SAVE_DIALOG);

             for (int i = 0; i < filters.size(); i++)
                 chooser.addChoosableFileFilter ((FileFilter)filters.get(i));
             if (default_filter != null)
                 chooser.setFileFilter (default_filter);
            
             chooser.setFileHidingEnabled (false);
             chooser.setApproveButtonText("Save");
             chooser.setDialogTitle("Save File...");
            
             if ((file != null) && (!file.isDirectory()))
                 chooser.setSelectedFile (file);
            
             int returnVal = chooser.showSaveDialog(parent); 
             if(returnVal == JFileChooser.APPROVE_OPTION) 
                 return chooser.getSelectedFile();
             else
                 return null;
         }
         catch (IOException e) {
             return null;
         }
     }


}

