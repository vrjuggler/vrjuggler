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

#ifndef _VJ_CONFIG_TOKENS_H_
#define _VJ_CONFIG_TOKENS_H_

namespace jccl {
   
const char end_TOKEN[] = "end";

const char vj_valuelabels_TOKEN[] = "vj_valuelabels";
const char type_TOKEN[] = "type";
const char lbrace_TOKEN[] = "{";
const char rbrace_TOKEN[] = "}";

const char true_TOKEN[] = "true";
const char false_TOKEN[] = "false";

const char vj_enumeration_TOKEN[] = "vj_enumeration";

// socket communication tokens
const char get_TOKEN[] = "get";
const char descriptions_TOKEN[] = "descriptions";
const char all_TOKEN[] = "all";
const char remove_TOKEN[] = "remove";
const char chunks_TOKEN[] = "chunks";

// Types
const char int_TOKEN[] = "int";
const char integer_TOKEN[] = "integer";
const char float_TOKEN[] = "float";
const char bool_TOKEN[] = "bool";
const char boolean_TOKEN[] = "boolean";
const char string_TOKEN[] = "string";
const char distance_TOKEN[] = "distance";
const char chunk_TOKEN[] = "chunk";
const char embeddedchunk_TOKEN[] = "embeddedchunk";

};
#endif
