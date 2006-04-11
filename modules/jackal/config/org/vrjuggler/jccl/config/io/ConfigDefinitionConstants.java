/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

package org.vrjuggler.jccl.config.io;

import org.jdom.Namespace;


public interface ConfigDefinitionConstants
{
   public static final Namespace XSI_NS =
      Namespace.getNamespace("xsi",
                             "http://www.w3.org/2001/XMLSchema-instance");

   public static final String ABSTRACT               = "abstract";
   public static final String ALLOWED_TYPE           = "allowed_type";
   public static final String CATEGORY               = "category";
   public static final String DEFAULTVALUE           = "defaultvalue";
   public static final String DEFINITION             = "definition";
   public static final String DEFINITION_VERSION     = "definition_version";
   public static final String ENUMERATION            = "enumeration";
   public static final String EDITABLE               = "editable";
   public static final String ENUM_VALUE             = "enum";
   public static final String HELP                   = "help";
   public static final String LABEL                  = "label";
   public static final String NAME                   = "name";
   public static final String ICON_PATH              = "icon_path";
   public static final String PARENT                 = "parent";
   public static final String PROPERTY               = "property";
   public static final String UPGRADE_TRANSFORM      = "upgrade_transform";
   public static final String VALUE                  = "value";
   public static final String VALUETYPE              = "valuetype";
   public static final String VARIABLE               = "variable";
   public static final String VERSION                = "version";

   // Types
   public static final String BOOLEAN                = "boolean";
   public static final String INTEGER                = "integer";
   public static final String FLOAT                  = "float";
   public static final String STRING                 = "string";
   public static final String CONFIG_ELEMENT         = "configelement";
   public static final String CONFIG_ELEMENT_POINTER = "configelementpointer";

   public static final String SETTINGS_INSTRUCTION   = "org-vrjuggler-jccl-settings";
   public static final String DEF_VERSION_ATTR       = "definition.version";
   public static final String SCHEMA_LOCATION        = "schemaLocation";
}
