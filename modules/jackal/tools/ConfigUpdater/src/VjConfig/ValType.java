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

/** Identification of the different types of values that VarValues can have.
 *  Each possible ValType is represented by a static public member of this
 *  class.  Because these instances are final and no other ValTypes can be
 *  created, it is safe to do direct object comparisons.  ie vt1 == vt2 can
 *  be treated as equivalent to vt1.equals(vt2).
 */
public final class ValType
{
   private final static int t_string        = 1;
   private final static int t_float         = 2;
   private final static int t_int           = 3;
   private final static int t_bool          = 4;
   private final static int t_chunk         = 5;
   private final static int t_embeddedchunk = 6;
   private final static int t_invalid       = 7;

   public final static ValType STRING           = new ValType(t_string);
   public final static ValType FLOAT            = new ValType(t_float);
   public final static ValType INT              = new ValType(t_int);
   public final static ValType BOOL             = new ValType(t_bool);
   public final static ValType CHUNK            = new ValType(t_chunk);
   public final static ValType EMBEDDEDCHUNK    = new ValType(t_embeddedchunk);
   public final static ValType INVALID          = new ValType(t_invalid);

   private int val; // type identifier for this instance.

   /** Constructor.  Private so that no other ValTypes can be created. */
   private ValType(int i)
   {
      val = i;
   }

   /** Returns the ValType associated with a String value.
    *  This can be used in various parsing routines.
    */
   public static ValType getValType(String s)
   {
      String s2 = s.toLowerCase().trim();
      if (s2.equals(ConfigTokens.string_TOKEN))
      {
         return STRING;
      }
      else if (s2.equals(ConfigTokens.float_TOKEN))
      {
         return FLOAT;
      }
      else if (s2.equals(ConfigTokens.int_TOKEN))
      {
         return INT;
      }
      else if (s2.equals(ConfigTokens.bool_TOKEN))
      {
         return BOOL;
      }
      else if (s2.equals(ConfigTokens.chunk_TOKEN))
      {
         return CHUNK;
      }
      else if (s2.equals(ConfigTokens.embeddedchunk_TOKEN))
      {
         return EMBEDDEDCHUNK;
      }
      else
      {
         return INVALID;
      }
   }

   /** Converts the ValType to a String.
    *  The string literals used are the same as those recognized by
    *  ValTpe.getValType(String s).
    */
   public String toString()
   {
      switch (val)
      {
      case t_string:
         return ConfigTokens.string_TOKEN;
      case t_float:
         return ConfigTokens.float_TOKEN;
      case t_int:
         return ConfigTokens.int_TOKEN;
      case t_bool:
         return ConfigTokens.bool_TOKEN;
      case t_chunk:
         return ConfigTokens.chunk_TOKEN;
      case t_embeddedchunk:
         return ConfigTokens.embeddedchunk_TOKEN;
      default:
         return ConfigTokens.invalid_TOKEN;
      }
   }

   public boolean equals(ValType v)
   {
      if (v == null)
      {
         return false;
      }
      return (val == v.val);
   }
}
