/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
import java.util.Vector;


/**
 * @since 1.0
 */
public class ExtensionFileFilter extends FileFilter
{
   public ExtensionFileFilter (String desc)
   {
      description = desc;
   }

   /**
    * Adds the given extension (without ".") to this object's known file
    * extensions.
    */
   public void addExtension (String ext)
   {
      m_exts.add(ext);
   }

   public boolean accept (java.io.File f)
   {
      boolean status = false;

      // Always accept directories so that they can be navigated.
      if ( f.isDirectory() )
      {
         status = true;
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
    * Gets the extension of the given file.
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

   private String description = null;
   private Vector m_exts      = new Vector();
}
