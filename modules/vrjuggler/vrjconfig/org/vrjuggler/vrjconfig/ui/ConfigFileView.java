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
