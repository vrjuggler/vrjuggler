/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _JCCL_CONFIG_TOKENS_H_
#define _JCCL_CONFIG_TOKENS_H_

namespace jccl
{

/**********************************************************
typedef enum { T_INT, T_FLOAT, T_BOOL, T_STRING, T_DISTANCE,
           T_CHUNK, T_EMBEDDEDCHUNK, T_INVALID } VarType;

Had to rename T_INVALID due to a conflict of names with some
Mac OS X variables.  This could be avoided, but just changing
it here is much much easier.  Places which used T_INVALID have
also been changed to VJ_T_INVALID.
************************************************************/
typedef enum
{
   T_INT,
   T_FLOAT,
   T_BOOL,
   T_STRING,
   T_DISTANCE,
   T_CHUNK,
   T_EMBEDDEDCHUNK,
   VJ_T_INVALID
} VarType;

typedef enum
{
   U_Feet,
   U_Inches,
   U_Meters,
   U_Centimeters,
   U_BadUnit
}
CfgUnit;

// ---------- TOKENS ---------------- //
/* Old unused tokens

const char end_TOKEN[] = "end";
const char vj_valuelabels_TOKEN[] = "vj_valuelabels";
const std::string true_TOKEN("true");
const std::string false_TOKEN("false");
const char vj_enumeration_TOKEN[] = "vj_enumeration";

// socket communication tokens
const char get_TOKEN[] = "get";
const char descriptions_TOKEN[] = "descriptions";
const char all_TOKEN[] = "all";
const char remove_TOKEN[] = "remove";
const char chunks_TOKEN[] = "chunks";
*/

// Types
const std::string int_TOKEN("int");
const std::string float_TOKEN("float");
const std::string bool_TOKEN("bool");
const std::string string_TOKEN("string");
const std::string distance_TOKEN("distance");
const std::string chunk_TOKEN("chunk");
const std::string embeddedchunk_TOKEN("embeddedchunk");

// XML format tokens
const std::string chunk_desc_TOKEN("ChunkDesc");
const std::string token_TOKEN("token");
const std::string name_TOKEN("name");
const std::string property_desc_TOKEN("PropertyDesc");
const std::string num_TOKEN("num");
const std::string type_TOKEN("type");
const std::string help_TOKEN("help");
const std::string item_TOKEN("item");
const std::string default_value_TOKEN("defaultvalue");

const std::string chunk_desc_db_TOKEN("ChunkDescDB");
const std::string chunk_db_TOKEN("ConfigChunkDB");

const std::string vj_include_desc_file_TOKEN("vjIncludeDescFile");
const std::string vj_include_file_TOKEN("vjIncludeFile");

// Processing instruction tokens
const std::string include_desc_INSTRUCTION("org-vrjuggler-jccl-includedesc");
const std::string include_INSTRUCTION("org-vrjuggler-jccl-include");
const std::string file_TOKEN("file");

const std::string EMBEDDED_SEPARATOR("/");


} // End of jccl namespace


#endif
