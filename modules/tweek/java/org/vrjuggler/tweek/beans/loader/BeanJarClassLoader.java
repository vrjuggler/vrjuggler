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

package org.vrjuggler.tweek.beans.loader;

import java.io.File;
import java.io.InputStream;
import java.io.IOException;
import java.net.URL;
import java.util.*;
import java.util.jar.*;
import java.util.zip.InflaterInputStream;


/**
 * An extension to the basic ClassLoader class that knows how to find classes
 * within dynamically loaded JAR files and how to find dependencies of loaded
 * classes.  This class exists as a singleton so that there is only one class
 * loader used for instantiating all Beans and dependencies thereof.
 */
public class BeanJarClassLoader extends ClassLoader
{
   /**
    * Adds the given JarFile object to the collection of known JAR files.
    */
   public synchronized void addJarFile (JarFile file)
   {
      mJars.add(file);
   }

   /**
    * Extends the loader's "class path" to include the named directory or list
    * of directories.  These directories will be searched to find dependencies
    * of loaded classes.  The given path must be a semicolon-separated list of
    * directories.
    */
   public void extendClassPath (String path)
   {
      if ( path != null && path.length() > 0 )
      {
         int begin_index = 0;
         int end_index   = path.indexOf(';');

         while ( end_index != -1 || begin_index < path.length() )
         {
            if ( end_index == -1 )
            {
               end_index = path.length();
            }

            String cur_dir = path.substring(begin_index, end_index);
            mDirs.add(cur_dir);
            begin_index = end_index + 1;
            end_index   = path.indexOf(';', begin_index);
         }
      }
   }

   /**
    * Adds the given list of dependencies (String objects naming the JAR files
    * that must be loaded).
    *
    * Pre-condition: The class path has been extended via extendClassPath().
    *
    * Post condition: Those dependency JAR files that are found are added to
    * the vector of known JAR files.
    *
    * @return A list of String objects containing any dependencies that were
    *         not resolved is returned to the caller.
    */
   public List resolveDependencies (List deps)
   {
      List unresolved_deps = new ArrayList();

      for ( Iterator i = deps.iterator(); i.hasNext(); )
      {
         String name = (String) i.next();

         // If our set of found classes already contains the name of the item
         // being looked up, don't bother searching for it again.
         if ( ! mFoundClasses.contains(name) )
         {
            File jar_file = null;

            for ( int j = 0; j < mDirs.size(); j++ )
            {
               File temp = new File(((String) mDirs.elementAt(j)) +
                                    File.separator + name);

               if ( temp.exists() )
               {
                  jar_file = temp;
                  mFoundClasses.add(name);
                  break;
               }
            }

            // If the named file was found in mDirs, we use it to create a
            // JarFile object that is then added to mJars.
            if ( jar_file != null )
            {
               try
               {
                  synchronized (mJars)
                  {
                     mJars.add(new JarFile(jar_file));
                  }
               }
               catch (IOException e)
               {
                  e.printStackTrace();
               }
            }
            // If the named file was not found, we add it to unresolved_deps
            // and leave further handling up to the caller.
            else
            {
               unresolved_deps.add(name);
            }
         }
      }

      return unresolved_deps;
   }

   public Class findClass (String classname) throws ClassNotFoundException
   {
      Class loaded_class = null;

      try
      {
         byte[] b = loadClassData(classname);
         loaded_class = defineClass(classname, b, 0, b.length);
      }
      catch (IOException e)
      {
         System.err.println("ERROR: Could not load class data for " + classname);
      }

      return loaded_class;
   }

   public URL findResource (String name)
   {
      URL path = null;

      for ( int i = 0; i < mJars.size(); i++ )
      {
         JarFile jar_file = (JarFile) mJars.elementAt(i);
         JarEntry entry   = jar_file.getJarEntry(name);

         if ( entry != null )
         {
            try
            {
               path = new URL("jar:file:" + jar_file.getName() + "!/" + name);
               break;
            }
            catch (java.net.MalformedURLException url_ex)
            {
               System.err.println("WARNING: Invalid URL constructed when " +
                                  "trying to find resource " + name + " in " +
                                  jar_file.getName());
            }
         }
      }

      return path;
   }

   public URL getResource(String name)
   {
      URL url = super.getResource(name);

      if (url == null)
      {
         url = this.findResource(name);
      }

      return url;
   }

   /**
    * Returns a reference to the singleton BeanJarClassLoader object.
    */
   public static BeanJarClassLoader instance ()
   {
      if ( mInstance == null )
      {
         mInstance = new BeanJarClassLoader();
      }

      return mInstance;
   }

   // ========================================================================
   // Protected methods.
   // ========================================================================

   /**
    * Default constructor.  This is protected so that this class cannot be
    * instantiated by outside parties.
    */
   protected BeanJarClassLoader ()
   {
      /* Do nothing. */ ;
   }

   /**
    * Loads the actual data bytes for the named class and returns those bytes
    * to the caller.
    *
    * Pre-condition: The class path and dependency list have been extended to
    * include all dependencies needed by the class being instantiated.
    */
   protected byte[] loadClassData (String classname)
      throws IOException, ClassNotFoundException
   {
      InputStream stream = null;
      int size = 0;

      // Search the collection of known JAR files for the named class.
      synchronized (mJars)
      {
         for ( int i = 0; i < mJars.size(); i++ )
         {
            // First look up the class as a .class entry in the JAR file.
            String entry_name = classname.replace('.', '/') + ".class";
            JarFile jar_file  = (JarFile) mJars.elementAt(i);
            JarEntry entry    = jar_file.getJarEntry(entry_name);

            // If no such entry was found, try looking it up as a .ser entry.
            if ( entry == null )
            {
               entry_name = classname.replace('.', '/') + ".ser";
               entry      = jar_file.getJarEntry(entry_name);
            }

            // If an entry for the class was found, get an input stream for it
            // so that we can read the bytes and return them to the caller.
            if ( entry != null )
            {
               stream = jar_file.getInputStream(entry);
               size   = (int) entry.getSize();

               break;
            }
         }
      }

      // If no data stream was found for the entry, we cannot load the class
      // data.
      if ( stream == null )
      {
         throw new ClassNotFoundException("Could not find class " + classname);
      }

      byte[] class_bytes = new byte[size];

      try
      {
         int bytes = 0, count;

         // Read in those bytes!
         while ( bytes < size )
         {
            count = stream.read(class_bytes, bytes, size - bytes);

            if ( count == -1 )
            {
               break;
            }
            else
            {
               bytes += count;
            }
         }
      }
      catch (IOException e)
      {
         e.printStackTrace();
         class_bytes = null;
      }

      return class_bytes;
   }

   protected static BeanJarClassLoader mInstance = null;

   protected Vector mJars         = new Vector();
   protected Vector mDirs         = new Vector();
   protected Set    mFoundClasses = new HashSet();
}
