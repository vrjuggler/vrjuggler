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
