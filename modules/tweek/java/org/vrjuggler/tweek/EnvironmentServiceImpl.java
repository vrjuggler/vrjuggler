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
 * @since 0.15.0
 */
class EnvironmentServiceImpl
   extends ServiceBean
   implements java.io.Serializable, EnvironmentService
{
   static
   {
      mHaveTweekJni = false;

      String jvm_ver_str = System.getProperty("java.version").substring(0, 3);
      boolean jvm_has_getenv = false;

      try
      {
         // JDK 1.5.0 has restored the usability of System.getenv().
         float jvm_ver = Float.parseFloat(jvm_ver_str);
         jvm_has_getenv = (jvm_ver >= 1.5);
      }
      catch(NumberFormatException ex)
      {
         // jvm_has_getenv remains false.
      }

      mJvmHasGetenv = jvm_has_getenv;

      // If the JVM does not have System.getenv(), then we use our workaround
      // in the tweek_jni library.
      if ( ! jvm_has_getenv )
      {
         try
         {
            System.loadLibrary("tweek_jni");
            mHaveTweekJni = true;
         }
         catch(UnsatisfiedLinkError ex)
         {
            System.err.println("WARNING: Failed to load tweek_jni extension: " +
                               ex.getMessage());
            // mHaveTweekJni remains false.
         }
      }

      String os_name = System.getProperty("os.name");

      if ( os_name.toLowerCase().startsWith("mac os x") )
      {
         os = EnvironmentService.MacOS;
      }
      else if ( os_name.indexOf("WindowsCE") != -1 )
      {
         os = EnvironmentService.WindowsCE;
      }
      else if ( os_name.indexOf("Windows") != -1 )
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
      // Windows preferences location.
      else if ( getOS() == Windows )
      {
         data_dir_name = getUserHome() + File.separator + "Application Data";
      }
      // WindowsCE/PocketPC preferences location.  At the moment, this is
      // set to the user's home directory because PocketPC does not seem to
      // use "Application Data" as the application-specific data directory.
      else if ( getOS() == WindowsCE )
      {
         data_dir_name = getUserHome();
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
         if ( new_str.substring(index, index + 2).equals("$(") )
         {
            end_index = new_str.indexOf(")");
         }
         else if ( new_str.substring(index, index + 2).equals("${") )
         {
            end_index = new_str.indexOf("}");
         }
         else
         {
            continue;
         }

         env_var  = new_str.substring(index + 2, end_index);

         // Treat $HOME as a special case since we can actually get its value
         // from the JVM without any aggrivating hacks.
         if ( env_var.equals("HOME") )
         {
            value = getUserHome();
         }
         else
         {
            value = getenv(env_var);
         }

         if ( value != null )
         {
            new_str = new_str.substring(0, index) + value +
                      new_str.substring(end_index + 1);
         }
         else
         {
            System.err.println("WARNING: Environment variable " + env_var +
                               " has no value");
            new_str = new_str.substring(0, index) +
                      new_str.substring(end_index + 1);
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

   /**
    * Returns an identifier for the host operationg system.  The value will be
    * one of EnvironmentService.UNIX, EnvironmentService.MacOS, or
    * EnvironmentService.Windows.
    */
   public int getOS()
   {
      return os;
   }

   /**
    * Returns the value of the named environment variable or null if the
    * variable is not set in the user's environment.
    *
    * @param envVarName The name of the variable to look up in the user's
    *                   environment.
    *
    * @return A String object containing the value of the named environment
    *         variable if the variable exists in the user's environment.  If
    *         it does not exist, null is returned.
    */
   public String getenv(String envVarName)
   {
      return getenv(envVarName, null);
   }

   /**
    * Returns the value of the named environment variable or null if the
    * variable is not set in the user's environment.  With versions of the
    * JDK prior to 1.5, this is implemented using a native method call
    * because System.getenv() is not available.  If the native method call is
    * not available, this method always returns null.  With JDK 1.5,
    * System.getenv() is usable again, so it is used directly rather than
    * using the native method call.
    *
    * @param envVarName The name of the variable to look up in the user's
    *                   environment.
    *
    * @return A String object containing the value of the named environment
    *         variable if the variable exists in the user's environment.  If
    *         it does not exist, null is returned.  If the native method call
    *         that is used to implement this method is not available, null is
    *         always returned.
    */
   public String getenv(String envVarName, String defaultValue)
   {
      String value = null;

      // Prefer the JVM System.getenv() over the native method whenever
      // possible.
      if ( mJvmHasGetenv )
      {
         value = System.getenv(envVarName);
      }
      // Fall back on the native method if we have it.
      else if ( mHaveTweekJni )
      {
         value = tweekJniGetenv(envVarName);
      }

      // As a last resort, try System.getProperty().
      if ( null == value )
      {
         // If value is still null, then return defaultValue.
         value = System.getProperty(envVarName, defaultValue);
      }

      return value;
   }

   private native String tweekJniGetenv(String envVarName);

   private String[] commandLineArgs = null;
   private String   appDataDir      = null;

   private static int os;
   private static boolean mHaveTweekJni;
   private static boolean mJvmHasGetenv;
}
