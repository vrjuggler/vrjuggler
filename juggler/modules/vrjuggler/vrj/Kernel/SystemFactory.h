#ifndef _VJ_SYSTEM_FACTORY_
#define _VJ_SYSTEM_FACTORY_

#include <config.h>
#include <Kernel/vjDisplayManager.h>
class vjDisplayManager;
#include <Kernel/vjDisplay.h>
class vjDisplay;
class vjGlWindow;

//------------------------------------------------------------
//: Abstract base class for system specific Factory.
//
//	In order to create System specific Factories, Concrete
//  derived classes must be created. 
//
// @author Allen Bierbaum
//  Date: 9-7-97
//
class vjSystemFactory
{
public: 
	//: Get system specific OpenGL window
	//! POST: Returns an OpenGL window for the current system
    virtual vjGlWindow* getGLWindow() = 0;
};

#endif
