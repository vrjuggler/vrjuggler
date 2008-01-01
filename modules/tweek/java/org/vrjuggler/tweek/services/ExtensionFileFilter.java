/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.services;

import javax.swing.filechooser.*;
import java.io.FileFilter;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;


/**
 * A filter for file dialogs and directories that will accept only files that
 * have extensions the filter supports. This filter works with both the Swing
 * FileChooser FileFilter and the standard java.io.FileFilter interface.
 *
 * @see java.io.FileFilter
 * @see javax.swing.filechooser.FileFilter
 *
 * @since 0.0.3
 */
public class ExtensionFileFilter
   extends javax.swing.filechooser.FileFilter
   implements java.io.FileFilter
{
   /**
    * Creates a new extension file filter with an empty description and no
    * supported extensions that will accept directories.
    */
   public ExtensionFileFilter ()
   {
      this("");
   }

   /**
    * Creates a new extension file filter with the given description and no
    * supported extensions that will accept directories.
    *
    * @param desc    a short description of this filter
    */
   public ExtensionFileFilter (String desc)
   {
      this( desc, true );
   }

   /**
    * Creates a new extension file filter with an empty description and an
    * initial set of supported extensions that will accept directories.
    *
    * @param extensions    a list of the extensions without the leading '.'
    */
   public ExtensionFileFilter (List extensions)
   {
      this( "", extensions );
   }

   /**
    * Creates a new extension file filter with the given description and an
    * initial set of supported extensions that will accept directories.
    *
    * @param desc          a short description of this filter
    * @param extensions    a list of the extensions without the leading '.'
    */
   public ExtensionFileFilter (String desc, List extensions )
   {
      this( desc, extensions, true );
   }

   /**
    * Creates a new extension file filter with the given description and no
    * supported extensions that will accept directories.
    *
    * @param desc          a short description of this filter
    * @param acceptDirs    true to accept directories as well, false otherwise
    */
   public ExtensionFileFilter (String desc, boolean acceptDirs )
   {
      this( desc, new ArrayList(), acceptDirs );
   }

   /**
    * Creates a new extension file filter with the given description and no
    * supported extensions that will accept directories.
    *
    * @param extensions    a list of the extensions without the leading '.'
    * @param acceptDirs    true to accept directories as well, false otherwise
    */
   public ExtensionFileFilter ( List extensions, boolean acceptDirs )
   {
      this( "", extensions, acceptDirs );
   }


   /**
    * Creates a new extension file filter with the given description and an
    * initial set of supported extensions. If acceptDirs is true, this filter
    * will also accept directories.
    *
    * @param desc          a short description of this filter
    * @param extensions    a list of the extensions without the leading '.'
    * @param acceptDirs    true to accept directories as well, false otherwise
    */
   public ExtensionFileFilter (String desc, List extensions, boolean acceptDirs)
   {
      description = desc;
      acceptDirectories = acceptDirs;
      for ( Iterator itr=extensions.iterator(); itr.hasNext(); ) {
         addExtension( (String)itr.next() );
      }
   }

   /**
    * Adds the given extension (without ".") to this object's known file
    * extensions.
    *
    * @param ext  the extension to add
    */
   public void addExtension (String ext)
   {
      m_exts.add(ext);
   }

   /**
    * Tests whether the given file is accepted by this filter.
    *
    * @return  true if the file has an extensions supported by this filter;
    *          false otherwise
    */
   public boolean accept (java.io.File f)
   {
      boolean status = false;

      // Only accept directories if acceptDirectories is set
      if ( f.isDirectory() )
      {
         if ( acceptDirectories )
         {
            status = true;
         }
      }
      else
      {
         String ext = getExtension(f);

         // Not all files have extensions, so we have to be careful.
         if ( ext != null )
         {
            for ( int i = 0; i < m_exts.size(); i++ )
            {
               if ( ext.equals(m_exts.elementAt(i)) )
               {
                  status = true;
                  break;
               }
            }
         }
      }

      return status;
   }

   /**
    * Gets the textual description of this filter.
    */
   public String getDescription ()
   {
      String full_desc = description;

      if ( m_exts.size() > 0 )
      {
         full_desc += " (";

         for ( int i = 0; i < m_exts.size(); i++ )
         {
            String ext = (String) m_exts.elementAt(i);
            full_desc += "*." + ext;

            // Only print the comma if this is not the last element in the
            // vector.
            if ( i + 1 < m_exts.size() )
            {
               full_desc += ", ";
            }
         }

         full_desc += ")";
      }

      return full_desc;
   }

   /**
    * Gets the extension of the given file without the leading '.'.
    *
    * @param f    the file from which to get the extension
    *
    * @return  the extension of the file without the leading '.'
    */
   private String getExtension (java.io.File f)
   {
      String ext = null;
      String s   = f.getName();
      int i      = s.lastIndexOf('.');

      if ( i > 0 && i < s.length() - 1 )
      {
         ext = s.substring(i + 1).toLowerCase();
      }

      return ext;
   }

   /**
    * The description of this filter.
    */
   private String description = null;

   /**
    * A list of all the extensions supported by this filter.
    */
   private Vector m_exts      = new Vector();

   /**
    * Flag to test whether or not directories will be accepted by this filter.
    */
   private boolean acceptDirectories;
}
