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

package org.vrjuggler.jccl.vjcontrol;


/**
 * This class exists as a container for tokens that are needed by the
 * VjComponent system.  It is needed only for that code.  The tokens defined
 * here match elements in the configuration chunks used by the VjComponent
 * system.
 */
public class VjComponentTokens
{
   /** Do not allow objects of this type to be created. */
   private VjComponentTokens ()
   {
   }

   public static final String CATEGORY      = "Category";
   public static final String CHUNK_FILES   = "chunkfiles";
   public static final String CLASS_NAME    = "ClassName";
   public static final String DEFAULT_PANEL = "default_panel";
   public static final String DESC_FILES    = "descfiles";
   public static final String DEPENDENCIES  = "Dependencies";
   public static final String FONT_NAME     = "fontname";
   public static final String FONT_SIZE     = "fontsize";
   public static final String HOST          = "host";
   public static final String LOOKNFEEL     = "looknfeel";
   public static final String PARENT_COMP   = "ParentComp";
   public static final String PORT          = "port";
   public static final String USED_FOR      = "UsedFor";
   public static final String USER_LEVEL    = "user_level";
   public static final String USER_TYPE     = "user_type";
   public static final String WINDOW_SIZE   = "windowsize";
}
