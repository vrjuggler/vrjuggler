/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.ui;

import java.io.File;
import javax.swing.*;
import javax.swing.filechooser.*;

/**
 * Used to set the description and Icon shown for files in a JFileChooser.
 */
public class ConfigFileView extends FileView
{
   /**
    * Load the icon once to be shown.
    */
   public ConfigFileView()
   {
      ClassLoader loader = getClass().getClassLoader();
      mConfigFileIcon = new
         ImageIcon(loader.getResource("org/vrjuggler/jccl/editors/images/config_element.gif"));
   }

   /**
    * Let the LnF FileView figure this out.
    */
   public String getName(File f)
   {
      return null; 
   }

   /**
    * Let the LnF FileView figure this out.
    */
   public String getDescription(File f)
   {
      return null; 
   }

   /**
    * Let the LnF FileView figure this out.
    */
   public Boolean isTraversable(File f)
   {
      return null; 
   }

   /**
    * Get the description for the given File.
    */
   public String getTypeDescription(File f)
   {
      String ext = null;
      String s = f.getName();
      int i = s.lastIndexOf('.');

      if (i > 0 &&  i < s.length() - 1) 
      {
         ext = s.substring(i+1).toLowerCase();
         if(ext.equals("jconf"))
         {
            return "VR Juggler Configuration File";
         }
      }                 
      return null;
   }
   
   /**
    * Get the Icon to be displayed for the given File.
    */
   public Icon getIcon(File f)
   {
      String ext = null;
      String s = f.getName();
      int i = s.lastIndexOf('.');

      if (i > 0 &&  i < s.length() - 1) 
      {
         ext = s.substring(i+1).toLowerCase();
         if(ext.equals("jconf"))
         {
            return mConfigFileIcon;
         }
      }                 
      return null;
   }
  
   /** The Icon to be displayed for all configuration files. */
   private Icon mConfigFileIcon;
}
