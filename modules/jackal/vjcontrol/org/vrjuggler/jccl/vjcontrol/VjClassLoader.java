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


package VjControl;

import java.net.URLClassLoader;
import java.net.URL;
import java.net.URLStreamHandlerFactory;

/** URL class loader used for _all_ VjControl extension components.
 *  This is an extensible version of URLClassLoader (ie the addURL method
 *  is public).  This is used by the ComponentFactory as the ClassLoader
 *  for loading every class that occurs in VjControl extension .jars.
 *  Since all extension .jars share the same class loader, any extension
 *  .jar can access any class in any other extension .jar.
 *  <p>
 *  It is important to note that the classes in the main VjControl.jar itself
 *  are loaded by the system class loader, and therefore cannot access 
 *  classes in extension .jars unless they use the VjClassLoader directly
 *  (as is done in the ComponentFactory).
 *  
 *  @bug There is no current way to determine loader dependencies between
 *  .jar files.  For example, the VjComponents.UI classes have to be loaded
 *  before or simultaneously with any classes that use or inherit from them.
 */
public class VjClassLoader extends URLClassLoader {
    
    public VjClassLoader(URL[] urls) {
        super (urls);
    }
    
    public VjClassLoader(URL[] urls, ClassLoader parent) {
        super (urls, parent);
    }
    
    public VjClassLoader(URL[] urls, ClassLoader parent,
                          URLStreamHandlerFactory factory) {
        super (urls, parent, factory);
    }

    public void addURL (URL url) {
        super.addURL (url);
    }

}
