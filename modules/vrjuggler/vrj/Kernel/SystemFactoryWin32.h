/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
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
