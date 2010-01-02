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

#ifndef _JCCL_CONFIG_TOKENS_H_
#define _JCCL_CONFIG_TOKENS_H_

#include <jccl/jcclConfig.h>

#include <string>

namespace jccl
{
   typedef enum
   {
      T_INT,
      T_FLOAT,
      T_BOOL,
      T_STRING,
      T_ELEMENT_PTR,
      T_CHILD_ELEMENT,
      VJ_T_INVALID
   } VarType;

   const std::string EMBEDDED_SEPARATOR("/");

   namespace types_tokens
   {
      const std::string BOOLEAN("boolean");
      const std::string INTEGER("integer");
      const std::string FLOAT("float");
      const std::string STRING("string");
      const std::string CONFIGELEMENTPOINTER("configelementpointer");
      const std::string CONFIGELEMENT("configelement");
   }

   namespace definition_tokens
   {
      /** The current version of the configuration definition file format. */
      const std::string DEF_VERSION("3.1");

      // Processing instructions
      const std::string SETTINGS_INSTRUCTION("org-vrjuggler-jccl-settings");
      const std::string DEF_VERSION_ATTR("definition.version");
      const std::string DESC_DB_VERSION_ATTR("desc.db.version");

      /** The XML namespace for configuration file content. */
      const std::string DEF_NS_str("http://www.vrjuggler.org/jccl/xsd/" +
                                   DEF_VERSION + "/definition");

      /** The name of the XML Schema file for the definition file format. */
      const std::string DEF_SCHEMA(DEF_NS_str + ".xsd");

      // XML Format tokens
      // XML Format tokens
      const std::string DEFAULTVALUE("defaultvalue");
      const std::string DEFINITION("definition");
      const std::string DEFINITION_VERSION("definition_version");
      const std::string PARENT("parent");
      const std::string ENUMERATION("enumeration");
      const std::string ENUM_VALUE("enum");
      const std::string HELP("help");
      const std::string LABEL("label");
      const std::string NAME("name");
      const std::string PROPERTY("property");
      const std::string VALUE("value");
      const std::string VALUETYPE("valuetype");
      const std::string VERSION("version");
   }

   namespace configuration_tokens
   {
      /** The current version of the configuration file format. */
      const std::string CFG_VERSION("3.0");

      // Processing instructions
      const std::string SETTINGS_INSTRUCTION("org-vrjuggler-jccl-settings");
      const std::string CFG_VERSION_ATTR("configuration.version");
      const std::string CONFIG_DB_VERSION_ATTR("config.db.version");

      /** The XML namespace for configuration file content. */
      const std::string CFG_NS_str("http://www.vrjuggler.org/jccl/xsd/" +
                                   CFG_VERSION + "/configuration");

      /** The name of the XML Schema file for the configuration file format. */
      const std::string CFG_SCHEMA(CFG_NS_str + ".xsd");

      // XML Format tokens
      const std::string CONFIGURATION("configuration");
      const std::string DEFINITION_PATH("definition_path");
      const std::string DIR("dir");
      const std::string ELEMENTS("elements");
      const std::string INCLUDE("include");
      const std::string NAME("name");
      const std::string VERSION("version");
   }

} // End of jccl namespace


#endif
