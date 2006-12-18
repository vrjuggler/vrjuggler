/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <vrj/Draw/DirectX/Config.h>

#include <vrj/Draw/DirectX/D3dDrawManager.h>
#include <vrj/Draw/DirectX/D3dApp.h>


namespace vrj
{

// NOTE: These member functions are implemented here rather than being
// inlined within D3dApp.h so that vrj::D3dApp is properly compiled into the
// VR Juggler DirectX Draw Manager library.  Sometimes, full inlining is a
// bad thing.

D3dApp::D3dApp(Kernel* kern) : App(kern)
{
   /* Do nothing. */ ;
}


D3dApp::~D3dApp()
{
   /* Do nothing. */ ;
}

DrawManager* D3dApp::getDrawManager()
{
   return D3dDrawManager::instance();
}

}
