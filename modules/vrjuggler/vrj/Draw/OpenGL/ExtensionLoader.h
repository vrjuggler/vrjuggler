/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _VRJ_OPENGL_EXTENSION_LOADER_H_
#define _VRJ_OPENGL_EXTENSION_LOADER_H_


namespace vrj
{

namespace opengl
{

/**
 * Base class for OpenGL extension registration and lookup.
 * This is the base class for a simple helper class for getting at
 * OpenGL/WGL/GLX extensions that we need for VR Juggler.
 *
 * @note This class was renamed from vrj::GlExtensionLoader in VR Juggler
 *       2.3.11.
 */
class ExtensionLoader
{
public:
   /** Void type to use when treating extensions the same. */
   typedef void (*VoidExtFunc)(void);

   ExtensionLoader()
   {;}

   virtual ~ExtensionLoader()
   {;}

   /** Register common extensions that we may need to use. */
   virtual void registerExtensions()
   {;}

   /** Return pointer to extension if it exists. */
   VoidExtFunc getFunctionByName(const char* name);
};

}  // namespace opengl

}  // namespace vrj


#endif
