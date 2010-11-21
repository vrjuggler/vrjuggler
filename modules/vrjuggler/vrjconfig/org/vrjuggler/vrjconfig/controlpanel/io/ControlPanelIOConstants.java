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

package org.vrjuggler.vrjconfig.controlpanel.io;

import org.jdom.Namespace;

/**
 * Constants for IO regarding control panels.
 */
public interface ControlPanelIOConstants
{
   /** The current version of the control panel file format. */
   public static final String CP_VERSION = "1.0";

   /** The string representation of the control panel namespace. */
   public static final String CP_NS_str = "http://www.vrjuggler.org/vrjuggler/xsd/"+CP_VERSION+"/controlpanel";

   /** The JDOM control panel namespace object. */
   public static final Namespace CP_NS = Namespace.getNamespace(CP_NS_str);

   /** The location of the control panel schema. */
   public static final String CP_SCHEMA = CP_NS_str + ".xsd";
}
