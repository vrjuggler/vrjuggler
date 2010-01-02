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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.jccl.config.io;

import org.jdom.Namespace;

/**
 * Constants for IO regarding configuration elements and definitions.
 */
public interface ConfigIOConstants
{
   /** The current version of the configuration file format. */
   public static final String CFG_VERSION = "3.0";

   /** The current version of the definition file format. */
   public static final String DEF_VERSION = "3.1";

   /** The string representation of the configuration namespace. */
   public static final String CFG_NS_str = "http://www.vrjuggler.org/jccl/xsd/"+CFG_VERSION+"/configuration";

   /** The string representation of the definition namespace. */
   public static final String DEF_NS_str = "http://www.vrjuggler.org/jccl/xsd/"+DEF_VERSION+"/definition";

   /** The JDOM configuration namespace object. */
   public static final Namespace CFG_NS = Namespace.getNamespace(CFG_NS_str);

   /** The JDOM definition namespace object. */
   public static final Namespace DEF_NS = Namespace.getNamespace(DEF_NS_str);

   /** The location of the configuration schema. */
   public static final String CFG_SCHEMA = CFG_NS_str + ".xsd";

   /** The location of the config definition schema. */
   public static final String DEF_SCHEMA = DEF_NS_str + ".xsd";
}
