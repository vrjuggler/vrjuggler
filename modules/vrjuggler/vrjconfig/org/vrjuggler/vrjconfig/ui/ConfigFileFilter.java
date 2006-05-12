/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

package org.vrjuggler.vrjconfig.ui;

import java.io.File;
import javax.swing.filechooser.*;

/**
 * FileFilter for VR Juggler configuration files.
 */
public class ConfigFileFilter extends FileFilter
{
   /**
    * Verify that the given file is a configuration file.
    */
   public boolean accept(File f)
   {
      if(f.isDirectory())
      {
         return true;
      }
      String ext = null;
      String s = f.getName();
      int i = s.lastIndexOf('.');

      if (i > 0 &&  i < s.length() - 1) 
      {
         ext = s.substring(i+1).toLowerCase();
         if(ext.equals("jconf"))
         {
            return true;
         }
      }                 
      return false;
   }

   /**
    * Get a description of this filter to display in JFileChooser.
    */
   public String getDescription()
   {
      return("VR Juggler Configuration File (*.jconf)");
   }
}
