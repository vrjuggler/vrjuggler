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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.jccl.config;


/**
 * This class contains "constants" that represent tokens that may appear in
 * chunk description files and in chunk files.
 *
 * @note It may be worthwhile standardize the naming convention used for these
 *       constants.
 */
public interface ConfigTokens
{
   public static final String SETTINGS_INSTRUCTION  = "org-vrjuggler-jccl-settings";
   public static final String INCLUDE_INSTRUCTION   = "org-vrjuggler-jccl-include";
   public static final String INCLUDE_DESC_INSTRUCTION   = "org-vrjuggler-jccl-includedesc";

   public static final String int_TOKEN           = "int";
   public static final String float_TOKEN         = "float";
   public static final String bool_TOKEN          = "bool";
   public static final String string_TOKEN        = "string";
   public static final String chunk_TOKEN         = "chunk";
   public static final String embeddedchunk_TOKEN = "embeddedchunk";
   public static final String invalid_TOKEN       = "invalid";

   public static final String version_TOKEN       = "version";

   public static final String true_TOKEN          = "true";
   public static final String false_TOKEN         = "false";

   public static final String chunk_desc_TOKEN    = "ChunkDesc";
   public static final String name_TOKEN          = "name";
   public static final String token_TOKEN         = "token";
   public static final String property_desc_TOKEN = "PropertyDesc";
   public static final String num_TOKEN           = "num";
   public static final String type_TOKEN          = "type";
   public static final String help_TOKEN          = "help";
   public static final String category_TOKEN      = "category";
   public static final String allowed_type_TOKEN  = "allowedtype";

   public static final String chunk_desc_db_TOKEN = "ChunkDescDB";
   public static final String chunk_db_TOKEN      = "ConfigChunkDB";

   public static final String item_TOKEN          = "item";
   public static final String item_label_TOKEN    = "label";
   public static final String default_value_TOKEN = "defaultvalue";
   public static final String user_level_TOKEN    = "level";
   public static final String var_num_TOKEN       = "variable";
   public static final String prop_enum_TOKEN     = "enumeration";

   public static final String beginner_TOKEN      = "beginner";
   public static final String expert_TOKEN        = "expert";

   public static final String vj_include_desc_file_TOKEN = "vjIncludeDescFile";
   public static final String vj_include_file_TOKEN      = "vjIncludeFile";
}
