/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_WIN32_SYSTEM_FACTORY_H_
#define _VJ_WIN32_SYSTEM_FACTORY_H_

#include <vjConfig.h>
#include <Kernel/vjSystemFactory.h>

class vjDisplayManager;

//---------------------------------------------------
//: Concrete class for SGI System specific Factory.
//
//	Concrete class to create SGI system specific objects.
//
// @author Allen Bierbaum
//  Date: 9-9-97
//----------------------------------------------------
class vjWin32SystemFactory : public vjSystemFactory
{
public:

   /**
	 * POST: Returns an SGI OpenGL window
    */
    virtual vjGlWindow* getGLWindow();


      // --- Singleton stuff --- //
protected:
   /// Constructor:  Hidden, so no instantiation is allowed
   vjWin32SystemFactory()   // Do nothing, but hide
   {;}

public:
   /// Get instance of singleton object
   static vjWin32SystemFactory* instance()
   {
      if (_instance == NULL)
         _instance = new vjWin32SystemFactory;
      return _instance;
   }

private:
   /// The instance
   static vjWin32SystemFactory* _instance;
};

#endif
