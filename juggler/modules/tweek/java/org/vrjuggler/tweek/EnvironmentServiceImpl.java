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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

package org.vrjuggler.tweek;

import java.io.File;
import java.util.ArrayList;
import org.vrjuggler.tweek.beans.BeanAttributes;
import org.vrjuggler.tweek.beans.ServiceBean;
import org.vrjuggler.tweek.services.EnvironmentService;

/**
 * The Environment Service Bean. This service will expand environment variables
 * in a string.
 *
 * @since 1.0
 */
class EnvironmentServiceImpl
   extends ServiceBean
   implements java.io.Serializable, EnvironmentService
{
   static
   {
      if ( System.getProperty("mrj.version") != null )
      {
         os = EnvironmentService.MacOS;
      }
      else if ( System.getProperty("os.name").indexOf("Windows") != -1 )
      {
         os = EnvironmentService.Windows;
      }
      else
      {
         os = EnvironmentService.UNIX;
      }
   }

   public EnvironmentServiceImpl(BeanAttributes attrs)
   {
      super(attrs);

      String data_dir_name;

      // Mac OS X preferences location.
      if ( getOS() == MacOS )
      {
         data_dir_name = getUserHome() + File.separator + "Library";
      }
      else if ( getOS() == Windows )
      {
         data_dir_name = getUserHome() + File.separator + "Application Data";
      }
      // UNIX.
      else
      {
         data_dir_name = getUserHome();
      }

      appDataDir = (new File(data_dir_name)).getAbsolutePath();
   }

   /**
    * Stores the command-line arguments passed to the application when it was
    * started.
    */
   public void setCommandLineArgs(String[] args)
   {
      commandLineArgs = args;
   }

   /**
    * Returns the array of command-line arguments.
    */
   public String[] getCommandLineArgs()
   {
      return commandLineArgs;
   }

   /**
    * Expands environment variables in the input string. Environment variables
    * may be specified using either $(variable_name) or ${variable_name}
    * everything between the parentheses or braces will be considered the name
    * of the variables.
    *
    * @param inputStr  the string in which to look for and expand environment
    *                  variables.
    *
    * @return  the input string with the variables replaced by their values.
    */
   public String expandEnvVars(String inputStr)
   {
      String new_str = inputStr;
      int index, end_index;
      String env_var, value;

      while ( (index = new_str.indexOf("$")) != -1 )
      {
         if ( inputStr.substring(index, index + 2).equals("$(") )
         {
            end_index = inputStr.indexOf(")");
         }
         else if ( inputStr.substring(index, index + 2).equals("${") )
         {
            end_index = inputStr.indexOf("}");
         }
         else
         {
            continue;
         }

         env_var  = inputStr.substring(index + 2, end_index);

         // Treat $HOME as a special case since we can actually get its value
         // from the JVM without any aggrivating hacks.
         if ( env_var.equals("HOME") )
         {
            value = getUserHome();
         }
         else
         {
            value = System.getProperty(env_var);
         }

         if ( value != null )
         {
            new_str = inputStr.substring(0, index) + value +
                      inputStr.substring(end_index + 1);
         }
         else
         {
            System.err.println("WARNING: Environment variable " + env_var +
                               " has no value");
            new_str = inputStr.substring(0, index) +
                      inputStr.substring(end_index + 1);
         }
      }

      return new_str;
   }

   /**
    * Gets the user's HOME environment variable using the JVM instead of the
    * environment variable.
    *
    * @return  the path to the user's home directory
    */
   public String getUserHome()
   {
      return System.getProperty("user.home");
   }

   /**
    * Returns the absolute path to the platfom-specific directory where
    * preferences files and other application data should be stored.
    */
   public String getAppDataDir()
   {
      return appDataDir;
   }

   public int getOS()
   {
      return os;
   }

   private String[] commandLineArgs = null;
   private String   appDataDir      = null;

   private static int os;
}
