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

package VjConfig;


public class ConfigTokens
{
   public static final String int_TOKEN           = "int";
   public static final String float_TOKEN         = "float";
   public static final String bool_TOKEN          = "bool";
   public static final String string_TOKEN        = "string";
   public static final String chunk_TOKEN         = "chunk";
   public static final String embeddedchunk_TOKEN = "embeddedchunk";
   public static final String invalid_TOKEN       = "invalid";

   public static final String chunk_desc_TOKEN    = "ChunkDesc";
   public static final String name_TOKEN          = "name";
   public static final String property_desc_TOKEN = "PropertyDesc";
   public static final String num_TOKEN           = "num";
   public static final String type_TOKEN          = "type";
   public static final String help_TOKEN          = "help";

   public static final String chunk_desc_db_TOKEN = "ChunkDescDB";
   public static final String chunk_db_TOKEN      = "ConfigChunkDB";
}
